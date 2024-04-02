<?php
session_start();

// Verifica se o usuário está logado
if (!isset($_SESSION['username'])) {
    // Se não estiver logado, redireciona para a página de login
    header("Location: login.php");
    exit;
}

// Verifica se o ID da postagem foi fornecido via GET
if (!isset($_GET['id'])) {
    echo "ID da postagem não especificado.";
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

    // Obtém o ID da postagem a ser editada
    $post_id = $_GET['id'];

    // Verifica se a postagem pertence ao usuário logado (opcional)
    // Aqui você pode adicionar lógica adicional, como verificar se o usuário é o autor da postagem, se necessário.

    // Verifica se o formulário foi enviado
    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        // Verifica se o conteúdo da postagem foi enviado
        if (isset($_POST['content'])) {
            // Atualiza o conteúdo da postagem no banco de dados
            $content = $_POST['content'];
            $stmt = $pdo->prepare("UPDATE posts SET content = ? WHERE id = ?");
            $stmt->execute([$content, $post_id]);

            // Redireciona de volta para a página do tópico após a edição da postagem
            header("Location: topic.php?id={$_GET['topic_id']}");
            exit;
        } else {
            echo "Ocorreu um erro ao processar o formulário.";
        }
    }

    // Caso o formulário não tenha sido enviado, busca o conteúdo atual da postagem para exibição no formulário de edição
    $stmt = $pdo->prepare("SELECT * FROM posts WHERE id = ?");
    $stmt->execute([$post_id]);
    $post = $stmt->fetch(PDO::FETCH_ASSOC);

    if (!$post) {
        echo "Postagem não encontrada.";
        exit;
    }

    // Exibe o formulário de edição da postagem
    ?>
    <!DOCTYPE html>
    <html lang="pt-BR">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Editar Postagem</title>
    </head>
    <body>
        <h1>Editar Postagem</h1>
        <form action="" method="post">
            <textarea name="content" rows="4" cols="50"><?php echo $post['content']; ?></textarea><br>
            <input type="submit" value="Salvar">
        </form>
    </body>
    </html>
    <?php
} catch (PDOException $e) {
    echo "Erro de conexão: " . $e->getMessage();
}
?>
