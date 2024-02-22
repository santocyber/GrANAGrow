<?php
// Verificar se o formulário de login foi enviado
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Conectar ao banco de dados
    $host = 'santocyber.helioho.st';
    $dbname = 'santocyber_telemetria';
    $username_db = 'santocyber_telemetria';
    $password_db = 'telemetria';

    try {
        $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username_db, $password_db);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        // Obter os dados do formulário
        $username = $_POST["username"];
        $password = $_POST["password"];

        // Consultar o banco de dados para verificar as credenciais
        $stmt = $pdo->prepare("SELECT * FROM usuario WHERE usuario = :username");
        $stmt->bindParam(':username', $username);
        $stmt->execute();
        $user = $stmt->fetch(PDO::FETCH_ASSOC);

        if ($user) {
            // Verificar se a senha está correta
            if (password_verify($password, $user['senha'])) {
                // Redirecionar para a página desejada após o login bem-sucedido
                header("Location: /granagw/");
                exit();
            } else {
                // Se a senha estiver incorreta, redirecionar de volta para o index com mensagem de erro
                header("Location: /index.php?error=password");
                exit();
            }
        } else {
            // Se o usuário não existir, redirecionar de volta para o index com mensagem de erro
            header("Location: /index.php?error=user");
            exit();
        }
    } catch(PDOException $e) {
        // Em caso de erro de conexão com o banco de dados, exibir mensagem de erro
        echo "Erro de conexão: " . $e->getMessage();
    }
}
?>
