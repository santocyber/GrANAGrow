<?php
// Verificar se o formulário foi submetido
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Conectar ao banco de dados
    $host = 'santocyber.helioho.st';
    $dbname = 'santocyber_telemetria';
    $username_db = 'santocyber_telemetria';
    $password_db = 'telemetria';

    try {
        $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username_db, $password_db);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        // Preparar a declaração SQL para inserir os dados
        $stmt = $pdo->prepare("INSERT INTO usuario (nome, usuario, senha, email, telefone, rede_social, email_verificado) VALUES (:nome, :usuario, :senha, :email, :telefone, :rede_social, 0)");

        // Limpar e validar os dados do formulário
        $nome = htmlspecialchars($_POST['nome']);
        $usuario = htmlspecialchars($_POST['usuario']);
        $senha = password_hash($_POST['senha'], PASSWORD_DEFAULT); // Hash da senha
        $email = filter_var($_POST['email'], FILTER_SANITIZE_EMAIL);
        $telefone = isset($_POST['telefone']) ? htmlspecialchars($_POST['telefone']) : null;
        $rede_social = htmlspecialchars($_POST['rede_social']);

        // Vincular os parâmetros e executar a consulta
        $stmt->bindParam(':nome', $nome);
        $stmt->bindParam(':usuario', $usuario);
        $stmt->bindParam(':senha', $senha);
        $stmt->bindParam(':email', $email);
        $stmt->bindParam(':telefone', $telefone);
        $stmt->bindParam(':rede_social', $rede_social);
        $stmt->execute();

        // Gerar um token de verificação de e-mail
        $token = bin2hex(random_bytes(32)); // Gera um token hexadecimal aleatório de 32 bytes

        // Atualizar o token de verificação de e-mail no banco de dados
        $stmt = $pdo->prepare("UPDATE usuario SET token_verificacao = :token WHERE email = :email");
        $stmt->bindParam(':token', $token);
        $stmt->bindParam(':email', $email);
        $stmt->execute();

        // Enviar e-mail de verificação
        $assunto = 'Verifique seu endereço de e-mail';
        $mensagem = "Clique no link abaixo para verificar seu e-mail: \n";
        $mensagem .= "verificar_email.php?token=$token";
        mail($email, $assunto, $mensagem);

        // Redirecionar para o índice com mensagem de sucesso
        header("Location: index.php?mensagem=sucesso");
        exit();
    } catch(PDOException $e) {
        // Em caso de erro de conexão com o banco de dados, exibir mensagem de erro
        echo "Erro de conexão: " . $e->getMessage();
    }
}
?>
