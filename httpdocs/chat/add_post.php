<?php
session_start();

// Verifica se o usuário está logado
if (!isset($_SESSION['username'])) {
    // Se não estiver logado, redireciona para a página de login
    header("Location: login.php");
    exit;
}

// Verifica se os dados do formulário foram enviados
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Conexão com o banco de dados
    $host = 'santocyber.helioho.st';
    $dbname = 'santocyber_telemetria';
    $username_db = 'santocyber_telemetria';
    $password_db = 'telemetria';

    try {
        $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username_db, $password_db);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        // Obtém os dados do formulário
        $topic_id = $_POST['topic_id'];
        $content = $_POST['content'];
        $created_by = $_SESSION['username'];

        // Insere a nova postagem no banco de dados
        $stmt = $pdo->prepare("INSERT INTO posts (topic_id, content, created_by) VALUES (?, ?, ?)");
        $stmt->execute([$topic_id, $content, $created_by]);

        // Redireciona de volta para a página do tópico após a adição da postagem
        header("Location: topic.php?id=$topic_id");
        exit;
    } catch (PDOException $e) {
        echo "Erro de conexão: " . $e->getMessage();
    }
} else {
    echo "Ocorreu um erro ao processar o formulário.";
}
?>
