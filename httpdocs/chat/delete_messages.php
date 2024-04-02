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

    // Executa a query para deletar todas as mensagens
    $stmt = $pdo->prepare("DELETE FROM chat");
    $stmt->execute();

    // Retorna uma resposta de sucesso
    echo json_encode(array('status' => 'success', 'message' => 'Todas as mensagens do chat foram apagadas'));
} catch (PDOException $e) {
    // Em caso de erro, retorna uma resposta de erro
    echo json_encode(array('status' => 'error', 'message' => 'Erro ao apagar as mensagens do chat: ' . $e->getMessage()));
}
?>
