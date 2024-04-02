<?php
session_start();

// Verifica se o usuário está logado
if (!isset($_SESSION['username'])) {
    // Se não estiver logado, redireciona para a página de login
    header("Location: login.php");
    exit;
}

// Verifica se o ID da postagem e o ID do tópico foram fornecidos via GET
if (!isset($_GET['id']) || !isset($_GET['topic_id'])) {
    echo "ID da postagem ou ID do tópico não especificados.";
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

    // Obtém o ID da postagem a ser excluída
    $post_id = $_GET['id'];

    // Exclui a postagem do banco de dados
    $stmt = $pdo->prepare("DELETE FROM posts WHERE id = ?");
    $stmt->execute([$post_id]);

    // Redireciona de volta para a página do tópico após a exclusão da postagem
    header("Location: topic.php?id={$_GET['topic_id']}");
    exit;
} catch (PDOException $e) {
    echo "Erro de conexão: " . $e->getMessage();
}
?>
