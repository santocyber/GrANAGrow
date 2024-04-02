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
    // Verifica se todos os campos foram enviados corretamente
    if (!empty($_POST['mac']) && !empty($_POST['credito'])) {
        // Recupera os valores do formulário
        $mac = $_POST['mac'];
        $credito = $_POST['credito'];

        try {
            // Conecta ao banco de dados
            $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria', 'santocyber_telemetria', 'telemetria');
            $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            // Inicia uma transação para garantir a integridade dos dados
            $pdo->beginTransaction();

            // Insere o crédito no banco de dados
            $stmt = $pdo->prepare("UPDATE telemetria SET credito = :credito, acoes = 'credito' WHERE mac = :mac AND usuarios = :username");
            $stmt->bindParam(':credito', $credito);
            $stmt->bindParam(':mac', $mac);
            $stmt->bindParam(':username', $username);
            $stmt->execute();

            // Commit da transação
            $pdo->commit();

            // Redireciona de volta para a página anterior após o envio bem-sucedido
            header("Location: {$_SERVER['HTTP_REFERER']}");
            exit();
        } catch (PDOException $e) {
            // Em caso de erro, faz rollback da transação e exibe uma mensagem de erro
            $pdo->rollBack();
            echo "Erro: " . $e->getMessage();
        }
    } else {
        // Em caso de campos em falta, exibe uma mensagem de erro
        echo "Erro: Todos os campos são obrigatórios.";
    }
}
?>
