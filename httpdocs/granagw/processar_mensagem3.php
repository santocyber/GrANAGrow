<?php
session_start();

// Verifica se o usuário está logado
if (!isset($_SESSION['username'])) {
    // Redireciona para a página de login, ou exibe uma mensagem de erro, dependendo do seu caso
    echo "Erro: Você não está logado.";
    exit;
}

// Armazena o nome de usuário atual em uma variável
$username = $_SESSION['username'];


if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (!empty($_POST['mac'])) {
        $mac = $_POST['mac'];

        try {
            $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria', 'santocyber_telemetria', 'telemetria');
            $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            // Inverte o estado do timer automático
            $stmt = $pdo->prepare("SELECT mensagemstatus FROM telemetria WHERE mac = :mac");
            $stmt->bindParam(':mac', $_POST['mac']);
            $stmt->execute();
            $result = $stmt->fetch(PDO::FETCH_ASSOC);
            $mensagemstatus = $result['mensagemstatus'];

                       $mensagemstatus = $mensagemstatus == 3 ? 0 : 3;

            $stmt = $pdo->prepare("UPDATE telemetria SET mensagemstatus = :mensagemstatus WHERE mac = :mac");
            $stmt->bindParam(':mensagemstatus', $mensagemstatus);
            $stmt->bindParam(':mac', $mac);
            $stmt->execute();

            // Redireciona de volta para a página anterior após o envio bem-sucedido
            header("Location: {$_SERVER['HTTP_REFERER']}");
            exit();
        } catch (PDOException $e) {
            echo "Erro ao salvar os dados: " . $e->getMessage();
        }
    } else {
        echo "Erro: Todos os campos são obrigatórios.";
    }
} else {
    echo "Erro: Método de requisição inválido.";
}
?>
