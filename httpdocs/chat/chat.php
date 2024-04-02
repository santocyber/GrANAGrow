<?php
session_start();

// Verifica se o usuário está logado
if (!isset($_SESSION['username'])) {
    // Se não estiver logado, retorna uma resposta de erro
    echo json_encode(array('status' => 'error', 'message' => 'Usuário não está logado'));
    exit;
}

// Conexão com o banco de dados
$host = 'santocyber.helioho.st';
$dbname = 'santocyber_telemetria';
$username_db = 'santocyber_telemetria';
$password_db = 'telemetria';

try {
    $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username_db, $password_db);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // Verifica o método de requisição
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        // Recebe os dados da mensagem do usuário
        $message = filter_var($_POST['message'] ?? '', FILTER_SANITIZE_STRING);

        // Insere a mensagem no banco de dados
        $username = $_SESSION['username'];
        $stmt = $pdo->prepare("INSERT INTO chat (username, message) VALUES (:username, :message)");
        $stmt->bindParam(':username', $username, PDO::PARAM_STR);
        $stmt->bindParam(':message', $message, PDO::PARAM_STR);
        $stmt->execute();

        echo json_encode(array('status' => 'success', 'message' => 'Mensagem enviada com sucesso'));
    } elseif ($_SERVER['REQUEST_METHOD'] === 'GET') {
        // Retorna o histórico de mensagens do chat
        $stmt = $pdo->query("SELECT * FROM chat ORDER BY id DESC LIMIT 10");
        $messages = $stmt->fetchAll(PDO::FETCH_ASSOC);

        echo json_encode(array('status' => 'success', 'messages' => $messages));
    } elseif ($_SERVER['REQUEST_METHOD'] === 'DELETE') {
        // Deleta todas as mensagens do chat
        $stmt = $pdo->query("DELETE FROM chat");
        
        echo json_encode(array('status' => 'success', 'message' => 'Todas as mensagens do chat foram apagadas com sucesso'));
    } else {
        echo json_encode(array('status' => 'error', 'message' => 'Método não permitido'));
    }
} catch (PDOException $e) {
    echo json_encode(array('status' => 'error', 'message' => 'Erro de conexão com o banco de dados: ' . $e->getMessage()));
}
?>
