<?php
// Connect to the database
include("connection.php");
$db = new dbObj();
$connection = $db->getConnstring();

$config = parse_ini_file("gateway_config.ini", true);
$pix = $config['pix'];
$pix_webhook_url = $pix['wbhook_url'];
$pix_qrcode_duration = $pix['qrcode_duration'];
$pix_user_id = $pix['user_id'];
$pix_external_pos_id = 16263583;
//$pix_external_pos_id = $pix['external_pos_id'];
$pix_token = $config['pix']['token'];

// URL da API
$url = "https://api.mercadopago.com/pos/{$pix_external_pos_id}";

$curl = curl_init($url);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);

// Define o cabeçalho de autorização
$headers = [
    "Authorization: Bearer {$pix_token}"
];
curl_setopt($curl, CURLOPT_HTTPHEADER, $headers);

// Executa a solicitação GET
$response = curl_exec($curl);

// Verifica se ocorreu algum erro na solicitação
if ($response === false) {
    $error_message = curl_error($curl);
    echo "Erro na solicitação cURL: " . $error_message;
} else {
    // Verifica o código de status HTTP
    $http_status = curl_getinfo($curl, CURLINFO_HTTP_CODE);

    if ($http_status === 200) {
        // Decodifica a resposta JSON
        $pos_data = json_decode($response);

        if (isset($pos_data->id)) {
            // Exibe os detalhes do caixa
            echo "ID do Caixa: " . $pos_data->id . "<br>";
            echo "STATUS do Caixa: " . $pos_data->status . "<br>";
            echo "NOME do Caixa: " . $pos_data->name . "<br>";
			echo "Dinheiro fixo do Caixa: " . $pos_data->fixed_amount . "<br>";
            echo "QR Code do Caixa: " . $pos_data->qr_code . "<br>";
            echo "External ID do Caixa: " . $pos_data->external_id . "<br>";
            echo "User ID do Caixa: " . $pos_data->user_id . "<br>";
            echo "Imagem do Caixa: <img src='" . $pos_data->qr->image . "' alt='Imagem do Caixa'><br>";
        } else {
            echo "Não foi possível obter os detalhes do caixa.";
        }
    } else {
        echo "Erro na solicitação da API. Código de Status HTTP: " . $http_status;
    }
}

// Fecha a conexão cURL
curl_close($curl);
?>
