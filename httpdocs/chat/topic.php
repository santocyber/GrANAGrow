<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Tópico</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background-color: #f7f7f7;
        }

        .container {
            max-width: 800px;
            margin: 20px auto;
            padding: 20px;
            background-color: #fff;
            border-radius: 5px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }

        h1 {
            text-align: center;
        }

        .back-button {
            display: inline-block;
            background-color: #007bff;
            color: #fff;
            padding: 10px 20px;
            text-decoration: none;
            border-radius: 5px;
            transition: background-color 0.3s;
            margin-bottom: 20px;
            cursor: pointer;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }

        .back-button:hover {
            background-color: #0056b3;
        }

        .back-button:active {
            transform: translateY(2px);
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }

        .post {
            margin-bottom: 20px;
            padding: 20px;
            background-color: #f2f2f2;
            border-radius: 5px;
        }

        .post-content {
            margin-bottom: 10px;
        }

        .post-timestamp {
            color: #888;
            font-size: 12px;
        }

        .post-author {
            font-weight: bold;
        }

        .actions {
            margin-top: 10px;
        }

        .actions a {
            margin-right: 10px;
            color: #007bff;
            text-decoration: none;
        }

        .actions a:hover {
            text-decoration: underline;
        }

        .new-post-form {
            margin-top: 20px;
        }

        .new-post-form textarea {
            width: 100%;
            padding: 10px;
            margin-bottom: 10px;
            border: 1px solid #ccc;
            border-radius: 5px;
            box-sizing: border-box;
        }

        .new-post-form input[type="submit"] {
            background-color: #007bff;
            color: #fff;
            border: none;
            padding: 10px 20px;
            border-radius: 5px;
            cursor: pointer;
            transition: background-color 0.3s;
        }

        .new-post-form input[type="submit"]:hover {
            background-color: #0056b3;
        }
    </style>
</head>
<body>
    <div class="container">
        <!-- Botão de voltar para o fórum -->
        <a href="forum.php" class="back-button">Voltar para o Fórum</a>
        <h1>Tópico</h1>

   <?php
        session_start();

        // Verifica se o usuário está logado
        if (!isset($_SESSION['username'])) {
            // Se não estiver logado, redireciona para a página de login
            header("Location: login.php");
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

            // Verifica se o parâmetro 'id' está presente na URL
            if (!isset($_GET['id'])) {
                echo "ID do tópico não especificado.";
                exit;
            }

            $topic_id = $_GET['id'];

            // Busca informações do tópico
            $stmt = $pdo->prepare("SELECT * FROM topics WHERE id = ?");
            $stmt->execute([$topic_id]);
            $topic = $stmt->fetch(PDO::FETCH_ASSOC);

            if (!$topic) {
                echo "Tópico não encontrado.";
                exit;
            }

            // Exibe o título do tópico, o autor e o timestamp
            echo "<h1>{$topic['title']}</h1>";
            echo "<p><strong>Autor do Tópico:</strong> {$topic['created_by']}</p>";
            echo "<p><strong>Data de Criação:</strong> {$topic['created_at']}</p>"; // Adicionando o timestamp
            echo "<p>{$topic['content']}</p>";

            // Verifica se o usuário atual é o autor do tópico
            if ($_SESSION['username'] == $topic['created_by']) {
                // Botões para editar e excluir tópico
                echo "<div class='actions'>";
                echo "<a href='edit_topic.php?id={$topic['id']}' style='margin-right: 10px;'>Editar Tópico</a>";
                echo "<a href='delete_topic.php?id={$topic['id']}'>Excluir Tópico</a>";
                echo "</div>";
            }

            // Exibe as postagens neste tópico
            $stmt = $pdo->prepare("SELECT * FROM posts WHERE topic_id = ?");
            $stmt->execute([$topic_id]);
            $posts = $stmt->fetchAll(PDO::FETCH_ASSOC);

            foreach ($posts as $post) {
                echo "<div class='post'>";
                echo "<p class='post-content'>{$post['content']}</p>";
                echo "<p class='post-timestamp'>Postado por <span class='post-author'>{$post['created_by']}</span> em {$post['created_at']}</p>";
                // Verifica se o usuário atual é o autor da postagem
                if ($_SESSION['username'] == $post['created_by']) {
                    // Botões para editar e excluir postagem
                    echo "<div class='actions'>";
                    echo "<a href='edit_post.php?id={$post['id']}&topic_id={$topic_id}' style='margin-right: 10px;'>Editar Postagem</a>";
                    echo "<a href='delete_post.php?id={$post['id']}&topic_id={$topic_id}'>Excluir Postagem</a>";
                    echo "</div>";
                }
                echo "</div>";
            }

            // Formulário para adicionar nova postagem
            echo "
            <h2>Adicionar Nova Postagem</h2>
            <form class='new-post-form' action='add_post.php' method='post'>
                <input type='hidden' name='topic_id' value='$topic_id'>
                <textarea name='content' rows='4' cols='50'></textarea><br>
                <input type='submit' value='Enviar'>
            </form>
            ";
        } catch (PDOException $e) {
            echo "Erro de conexão: " . $e->getMessage();
        }
    ?>
    </div>
</body>
</html>
