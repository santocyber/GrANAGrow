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
    // Verifica se o ID do tópico foi fornecido via POST
    if (!isset($_POST['id'])) {
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

        // Obtém os dados do formulário
        $topic_id = $_POST['id'];
        $title = $_POST['title'];
        $content = $_POST['content'];

        // Atualiza o tópico no banco de dados
        $stmt = $pdo->prepare("UPDATE topics SET title = ?, content = ? WHERE id = ?");
        $stmt->execute([$title, $content, $topic_id]);

        // Redireciona de volta para a página do tópico após a edição do tópico
        header("Location: topic.php?id=$topic_id");
        exit;
    } catch (PDOException $e) {
        echo "Erro de conexão: " . $e->getMessage();
    }
} else {
    // Se os dados do formulário não foram enviados, exibe o formulário de edição do tópico
    if (isset($_GET['id'])) {
        $topic_id = $_GET['id'];

        // Conexão com o banco de dados
        $host = 'santocyber.helioho.st';
        $dbname = 'santocyber_telemetria';
        $username_db = 'santocyber_telemetria';
        $password_db = 'telemetria';

        try {
            $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username_db, $password_db);
            $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            // Busca informações do tópico
            $stmt = $pdo->prepare("SELECT * FROM topics WHERE id = ?");
            $stmt->execute([$topic_id]);
            $topic = $stmt->fetch(PDO::FETCH_ASSOC);

            if (!$topic) {
                echo "Tópico não encontrado.";
                exit;
            }
        } catch (PDOException $e) {
            echo "Erro de conexão: " . $e->getMessage();
            exit;
        }
    } else {
        echo "ID do tópico não especificado.";
        exit;
    }
}
?>

<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Editar Tópico</title>
</head>
<body>
    <h1>Editar Tópico</h1>
    <form action="" method="post">
        <input type="hidden" name="id" value="<?php echo $topic['id']; ?>">
        <label for="title">Título:</label><br>
        <input type="text" id="title" name="title" value="<?php echo $topic['title']; ?>"><br>
        <label for="content">Conteúdo:</label><br>
        <textarea id="content" name="content" rows="4" cols="50"><?php echo $topic['content']; ?></textarea><br>
        <input type="submit" value="Salvar">
    </form>
</body>
</html>
