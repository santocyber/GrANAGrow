<?php
session_start();

// Verifica se o usuário está logado
if (!isset($_SESSION['username'])) {
    // Se não estiver logado, redireciona para a página de login
    header("Location: login.php");
    exit;
}

// Verifica se o ID do tópico foi fornecido via GET
if (!isset($_GET['id'])) {
    echo "ID do tópico não especificado.";
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

    // Obtém o ID do tópico a ser excluído
    $topic_id = $_GET['id'];

    // Exclui todas as postagens associadas ao tópico
    $stmt = $pdo->prepare("DELETE FROM posts WHERE topic_id = ?");
    $stmt->execute([$topic_id]);

    // Exclui o tópico do banco de dados
    $stmt = $pdo->prepare("DELETE FROM topics WHERE id = ?");
    $stmt->execute([$topic_id]);

    // Redireciona de volta para a página do fórum após a exclusão do tópico
    header("Location: forum.php");
    exit;
} catch (PDOException $e) {
    echo "Erro de conexão: " . $e->getMessage();
}
?>
