<?php
// Verifica se foi enviado um arquivo
if(isset($_FILES['image']) && $_FILES['image']['error'] === UPLOAD_ERR_OK) {
    // Configurações para conexão com o banco de dados
    $host = 'santocyber.helioho.st';
    $dbname = 'santocyber_telemetria';
    $username_db = 'santocyber_telemetria';
    $password_db = 'telemetria';

    try {
        // Conecta ao banco de dados
        $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username_db, $password_db);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        // Prepara a consulta SQL para atualizar a foto do usuário
        $stmt = $pdo->prepare("UPDATE usuario SET foto = :foto WHERE usuario = :username");

        // Obtém o nome de usuário da sessão
        session_start();
        $username = $_SESSION['username'];

        // Caminho do arquivo temporário
        $temp_file = $_FILES['image']['tmp_name'];

        // Lê o conteúdo do arquivo temporário
        $foto = file_get_contents($temp_file);

        // Executa a consulta SQL para atualizar a foto do usuário
        $stmt->bindParam(':foto', $foto, PDO::PARAM_LOB);
        $stmt->bindParam(':username', $username);
        $stmt->execute();

        // Define um cookie para armazenar a foto do usuário
      //  setcookie('user_avatar', 'data:image/jpeg;base64,' . base64_encode($foto), time() + (86400 * 30), "/"); // 86400 = 1 dia

        // Redireciona de volta para o dashboard
        header("Location: /dashboard.php");
        exit(); // Termina a execução do script após o redirecionamento

    } catch(PDOException $e) {
        // Em caso de erro de conexão com o banco de dados, exibe mensagem de erro
        echo "Erro de conexão: " . $e->getMessage();
    }
} else {
    // Se nenhum arquivo foi enviado ou ocorreu um erro, exibe mensagem de erro
    echo "Erro no upload da imagem.";
}
?>
