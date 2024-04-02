<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Fórum</title>





<?php
session_start();

// Verifica se o usuário está logado
if (!isset($_SESSION['username'])) {
    // Se não estiver logado, redireciona para a página de login
    header("Location: ../../index.php");
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
} catch (PDOException $e) {
    echo "Erro de conexão: " . $e->getMessage();
}
?>
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Fórum</title>
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

        .new-topic-button {
            display: block;
            width: 200px;
            margin: 0 auto 20px;
            padding: 10px;
            text-align: center;
            background-color: #007bff;
            color: #fff;
            text-decoration: none;
            border-radius: 5px;
            transition: background-color 0.3s;
        }

        .new-topic-button:hover {
            background-color: #0056b3;
        }

        .topic-list {
            list-style-type: none;
            padding: 0;
        }

        .topic-list li {
            margin-bottom: 10px;
            padding: 10px;
            background-color: #f2f2f2;
            border-radius: 5px;
        }

        .topic-list li a {
            text-decoration: none;
            color: #333;
            font-weight: bold;
        }

        .topic-list li a:hover {
            color: #000;
        }
    </style>

</head>
<body>
    <div class="container">
        <!-- Botão para voltar para a página de dashboard -->
        <a href="../dashboard.php" class="dashboard-button">Voltar para o Dashboard</a>
        <h1>Fórum</h1>
        <a href="new_topic.php" class="new-topic-button">Criar Novo Tópico</a>
        <ul class="topic-list">
            <?php
            try {
                // Query para obter todos os tópicos
                $stmt = $pdo->query("SELECT * FROM topics");
                $topics = $stmt->fetchAll(PDO::FETCH_ASSOC);

                // Exibir os tópicos
                foreach ($topics as $topic) {
                    echo "<li><a href='topic.php?id={$topic['id']}'>{$topic['title']}</a></li>";
                }
            } catch (PDOException $e) {
                echo "Erro de conexão: " . $e->getMessage();
            }
            ?>
        </ul>
    </div>
</body>
</html>
