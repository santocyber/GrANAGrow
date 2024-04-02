<?php
// Verificar se o token está presente na URL
if(isset($_GET['token'])) {
    $token = $_GET['token'];

    // Conectar ao banco de dados
    $host = 'santocyber.helioho.st';
    $dbname = 'santocyber_telemetria';
    $username_db = 'santocyber_telemetria';
    $password_db = 'telemetria';

    try {
        $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username_db, $password_db);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        // Verificar se o token está presente na tabela de usuários
        $stmt = $pdo->prepare("SELECT * FROM usuario WHERE token_verificacao = ?");
        $stmt->execute([$token]);
        $user = $stmt->fetch();

        if($user) {
            // Atualizar o status de verificação de e-mail para 1 (verificado)
            $stmt = $pdo->prepare("UPDATE usuario SET email_verificado = 1 WHERE token_verificacao = ?");
            $stmt->execute([$token]);

            echo "Seu e-mail foi verificado com sucesso!";
        } else {
            echo "Token inválido ou expirado.";
        }
    } catch(PDOException $e) {
        // Em caso de erro de conexão com o banco de dados, exibir mensagem de erro
        echo "Erro de conexão: " . $e->getMessage();
    }
} else {
    echo "Token não fornecido na URL.";
}
?>
