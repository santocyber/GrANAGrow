<?php
session_start();

// Verifica se o usuário está logado
if (!isset($_SESSION['username'])) {
    echo json_encode(["error" => "Usuário não está logado"]);
    exit;
}

// Conectar ao banco de dados
$host = 'santocyber.helioho.st';
$dbname = 'santocyber_telemetria';
$username_db = 'santocyber_telemetria';
$password_db = 'telemetria';

try {
    $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username_db, $password_db);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // Obtém o nome de usuário da sessão
    $username = $_SESSION['username'];

    // Consulta os dados do jogo do usuário atual
    $stmt = $pdo->prepare("SELECT * FROM tamagreen WHERE usuario = ?");
    $stmt->execute([$username]);
    $jogo = $stmt->fetch(PDO::FETCH_ASSOC);

    echo json_encode($jogo);
} catch (PDOException $e) {
    echo json_encode(["error" => "Erro de conexão com o banco de dados: " . $e->getMessage()]);
}
?>
