

<?php

// Verifica se a requisição é do tipo POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Recebe a string enviada pelo ESP32
    $string_from_esp32 = file_get_contents('php://input');

    // Substitua 'SEU_TOKEN' pelo token do seu bot Telegram
    $token = '6976621889:AAEMX1LRx5K8iHcD7l6FredR6PFW4yv9j1s';
    // Substitua 'ID_DO_CHAT' pelo ID do chat que você deseja enviar a mensagem
    $chat_id2 = '1497293517';
	$chat_id = '-1001148024857';

    // URL da API do Telegram para enviar mensagem
    $apiUrl = "https://api.telegram.org/bot$token/sendMessage";

    // Dados para enviar para a API
    $data = array(
        'chat_id' => $chat_id,
        'text' => $string_from_esp32
    );

    // Inicializa a sessão cURL
    $ch = curl_init($apiUrl);
    // Configuração para enviar os dados via POST
    curl_setopt($ch, CURLOPT_POST, true);
    // Converte os dados para o formato JSON
    curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($data));
    // Define o cabeçalho para aceitar JSON
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: application/json'));
    // Para receber a resposta da API
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

    // Executa a requisição
    $response = curl_exec($ch);

    // Verifica se houve algum erro
    if ($response === false) {
        // Exibe o erro, caso ocorra
        echo 'Erro ao enviar mensagem: ' . curl_error($ch);
    } else {
        // Converte a resposta JSON para um array
        $responseData = json_decode($response, true);
        // Verifica se a mensagem foi enviada com sucesso
        if ($responseData['ok']) {
            echo 'Mensagem enviada com sucesso!';
        } else {
            // Exibe o motivo do erro, caso ocorra
            echo 'Erro ao enviar mensagem: ' . $responseData['description'];
        }
    }

    // Fecha a sessão cURL
    curl_close($ch);
} else {
    // Se a requisição não for do tipo POST, retorna uma mensagem de erro
    echo 'Apenas requisições POST são permitidas!';
}
