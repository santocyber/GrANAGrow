<?php
// Se a solicitação for POST, receber os dados enviados pelo ESP32
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $dados = $_POST['dados'];
    // Adicionar os dados recebidos ao histórico
    file_put_contents("historico_dados.txt", $dados . PHP_EOL, FILE_APPEND);
    exit();
}

// Se a solicitação for GET e houver um parâmetro "limpar", limpar o histórico
if ($_SERVER['REQUEST_METHOD'] === 'GET' && isset($_GET['limpar'])) {
    // Limpar o histórico de mensagens
    if(file_put_contents("historico_dados.txt", "") !== false) {
        echo 'success'; // Envie uma mensagem de sucesso se o histórico foi limpo com sucesso
    } else {
        echo 'error'; // Envie uma mensagem de erro se ocorrer um problema ao limpar o histórico
    }
    exit();
}

// Retornar os dados atuais para atualização em tempo real
$dados_atuais = file_get_contents("historico_dados.txt");
echo $dados_atuais;
?>
