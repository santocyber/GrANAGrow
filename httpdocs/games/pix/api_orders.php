<?php
// Função para estabelecer a conexão com o banco de dados
function connectToDatabase() {
    $servername = "santocyber.helioho.st";
    $username = "santocyber_telemetria";
    $password = "telemetria";
    $dbname = "santocyber_telemetria";

    $conn = new mysqli($servername, $username, $password, $dbname);

    if ($conn->connect_error) {
        die("Conexão falhou: " . $conn->connect_error);
    }

    return $conn;
}

// Função para validar e sanitizar os dados
function sanitizeInput($input) {
    return htmlspecialchars(stripslashes(trim($input)));
}



function get_order($external_id, $mac) {
    $conn = connectToDatabase();
    $query = "SELECT * FROM order_data WHERE external_id = ? AND mac = ?;";
    $stmt = $conn->prepare($query);
    $stmt->bind_param('is', $external_id, $mac);
    $stmt->execute();
    $result = $stmt->get_result();

    $response = array();
    while ($row = $result->fetch_object()) {
        $response[] = $row;
    }
    $stmt->close();
    $conn->close();

    return $response;
}

function get_order_status($external_id, $mac) {
    $conn = connectToDatabase();
    $query = "SELECT order_status FROM order_data WHERE external_id = ? AND mac = ?;";
    $stmt = $conn->prepare($query);
    $stmt->bind_param('is', $external_id, $mac);
    $stmt->execute();
    $stmt->bind_result($order_status);
    $stmt->fetch();
    $stmt->close();
    $conn->close();

    $response = array(
        "order_status" => $order_status,
        "mac" => $mac,
        "external_id" => $external_id
    );

    $json_response = json_encode($response);

    // Defina os cabeçalhos de resposta
    header("Content-Type: application/json");
    header("Content-Length: " . strlen($json_response));

    // Envie a resposta JSON
    echo $json_response;
}

function get_orders() {
    $conn = connectToDatabase();
    $query = "SELECT * FROM order_data";
    $result = $conn->query($query);

    $response = array();
    while ($row = $result->fetch_object()) {
        $response[] = $row;
    }
    $conn->close();

    return $response;
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

// Função para verificar se uma ordem já existe no banco de dados
function order_exists($external_id, $mac) {
    $conn = connectToDatabase();
    $query = "SELECT COUNT(*) FROM order_data WHERE external_id = ? AND mac = ?";
    $stmt = $conn->prepare($query);
    $stmt->bind_param('is', $external_id, $mac);
    $stmt->execute();
    $stmt->bind_result($count);
    $stmt->fetch();
    $stmt->close();
    $conn->close();

    return $count > 0;
}

// ...

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

    $external_id = $_POST["external_id"];
    $mac = $_POST["mac"];

    if (order_exists($external_id, $mac)) {
        $response = get_order_status($external_id, $mac);
        echo json_encode($response);
    } else {
        http_response_code(409);
        header("Content-Type: text/plain");
        echo "A Ordem {$external_id} já existe";
    }
}

?>

