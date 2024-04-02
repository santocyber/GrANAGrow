<?php
session_start();

// Verifica se o usuário está logado
if (!isset($_SESSION['username'])) {
    echo "Erro: Você não está logado.";
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

    // Recupera o score atual do usuário do banco de dados
    $username = $_SESSION['username'];
    $stmt = $pdo->prepare("SELECT score FROM usuario WHERE usuario = :username");
    $stmt->bindParam(':username', $username);
    $stmt->execute();
    $row = $stmt->fetch(PDO::FETCH_ASSOC);
    $currentScore = $row['score'];

    // Adiciona os pontos recebidos ao score atual
    $pointsToAdd = $_POST['score'];
    $newScore = $currentScore + $pointsToAdd;

    // Atualiza o score do usuário no banco de dados
    $stmt = $pdo->prepare("UPDATE usuario SET score = :score WHERE usuario = :username");
    $stmt->bindParam(':score', $newScore);
    $stmt->bindParam(':username', $username);
    $stmt->execute();

    // Resposta de sucesso
    echo "Score atualizado com sucesso. Novo score: $newScore";
} catch(PDOException $e) {
    // Em caso de erro de conexão com o banco de dados, exibe mensagem de erro
    echo "Erro de conexão: " . $e->getMessage();
}
?>
