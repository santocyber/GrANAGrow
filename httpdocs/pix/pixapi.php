<?php
$config = parse_ini_file("gateway_config.ini", true);
$pix = $config['pix'];
$pix_webhook_url = $pix['wbhook_url'];
$pix_qrcode_duration = $pix['qrcode_duration'];
$pix_token = $pix['token'];
$pix_user_id = $pix['user_id'];
$pix_external_pos_id = $pix['external_pos_id'];

// Configurações do banco de dados
$servername = "santocyber.helioho.st";
$username = "santocyber_telemetria";
$password = "telemetria";
$dbname = "santocyber_telemetria";

// Conexão com o banco de dados usando a extensão mysqli
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar a conexão
if ($conn->connect_error) {
    die("Conexão falhou: " . $conn->connect_error);
}

// Função para validar e sanitizar os dados
function sanitizeInput($input) {
    return htmlspecialchars(stripslashes(trim($input)));
}

// Função para obter a data de expiração
function get_expiration() {
    global $pix_qrcode_duration;
    $current_time = new DateTime('NOW', new DateTimeZone('America/Sao_Paulo'));
    $current_time->add(new DateInterval('PT' . $pix_qrcode_duration . 'M'));
    return $current_time->format('Y-m-d\TH:i:s.vP');
}

// Função para criar um pedido
function create_order($external_id, $mac, $value = 3.00) {
    global $pix_token, $pix_webhook_url, $pix_user_id, $pix_external_pos_id, $conn;

    $order_data = [
        "external_reference" => $external_id,
        "title" => $mac,
        "description" => $mac,
        "notification_url" => $pix_webhook_url,
        "expiration_date" => get_expiration(),
        "total_amount" => $value,
        "items" => [
            [
                "title" => $mac,
			    "description" => $mac,
                "unit_price" => $value,
                "quantity" => 1,
                "unit_measure" => "unit",
                "total_amount" => $value
            ]
        ]
    ];

    $url = "https://api.mercadopago.com/instore/orders/qr/seller/collectors/{$pix_user_id}/pos/{$pix_external_pos_id}/qrs";

    $headers = [
        "Authorization: Bearer {$pix_token}",
        "Content-Type: application/json"
    ];

    $curl = curl_init($url);
    curl_setopt($curl, CURLOPT_URL, $url);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($curl, CURLOPT_POST, 1);
    curl_setopt($curl, CURLOPT_HTTPHEADER, $headers);
    curl_setopt($curl, CURLOPT_POSTFIELDS, json_encode($order_data));
    $response = curl_exec($curl);
    curl_close($curl);

    $response_object = json_decode($response);

    if (isset($response_object->qr_data)) {
        $qr_data = $response_object->qr_data;
        $size = strlen($qr_data);

        // Inserir $mac e $value na tabela do banco de dados
        $insert_query = "INSERT INTO order_data (external_id, mac, value) VALUES (?, ?, ?)";
        $stmt = $conn->prepare($insert_query);
        $stmt->bind_param('isd', $external_id, $mac, $value);
        $stmt->execute();
        $stmt->close();

        header("Content-Type: text/plain");
        header("Content-length: $size");
        echo $qr_data;
    } else {
        http_response_code(500); // Internal Server Error
        header("Content-Type: text/plain");
        echo "Erro na resposta da API: " . json_encode($response_object);
    }
}

// Função para verificar se um pedido existe
function order_exists($external_id, $mac) {
    global $conn;
    $query = "SELECT * FROM order_data WHERE external_id = ? AND mac = ?";
    $stmt = $conn->prepare($query);
    $stmt->bind_param('is', $external_id, $mac);
    $stmt->execute();
    $result = $stmt->get_result();

    $response = [];
    while ($row = $result->fetch_assoc()) {
        $response[] = $row;
    }

    $stmt->close();

    return count($response) > 0;
}

// Verifique o método da solicitação
$request_method = $_SERVER['REQUEST_METHOD'];
switch ($request_method) {
    case 'POST':
        handle_post_request();
        break;

    default:
        // Método de solicitação inválido
        http_response_code(405);
        header("Content-Type: text/plain");
        echo "Método não permitido";
        break;
}

// Função para lidar com solicitações POST
function handle_post_request() {
    if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
        // Método de solicitação inválido
        http_response_code(405);
        header("Content-Type: text/plain");
        echo "Método não permitido";
        return;
    }

    // Verifique se os dados necessários estão presentes no corpo da solicitação POST
    $required_fields = ["external_id", "mac"];
    foreach ($required_fields as $field) {
        if (empty($_POST[$field])) {
            http_response_code(400);
            header("Content-Type: text/plain");
            echo "É preciso informar o campo $field";
            return;
        }
    }

    //$external_id = sanitizeInput($_POST["external_id"]);
    //$mac = sanitizeInput($_POST["mac"]);
	
	$external_id = $_POST["external_id"];
    $mac = $_POST["mac"];	
    $value = isset($_POST["value"]) ? floatval($_POST["value"]) : 3.00;

    if (!order_exists($external_id, $mac)) {
        create_order($external_id, $mac, $value);
    } else {
        http_response_code(409);
        header("Content-Type: text/plain");
        echo "A Ordem {$external_id} e MAC {$mac} já existe";
    }
}
?>
