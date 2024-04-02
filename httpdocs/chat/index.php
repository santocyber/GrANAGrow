<?php
session_start();

// Verifica se o usuário não está logado
if (!isset($_SESSION['username'])) {
    // Redireciona o usuário para a página inicial
    header("Location: https://santocyber.helioho.st");
    exit; // Encerra o script para evitar que o restante do código seja executado
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Chat Futurista Cyberpunk</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <!-- Botão para voltar à página inicial -->
    <button id="home-button" onclick="window.location.href='../dashboard.php'">Voltar à Página Inicial</button>

    <div id="chat-container">
        <div id="chat-box"></div>
        <div id="message-input-container">
            <input type="text" id="message-input" placeholder="Digite sua mensagem...">
            <button id="send-button">Enviar</button>
            <button id="delete-messages-button">Apagar Mensagens</button>
        </div>
    </div>

    <script>
        // Passa o nome de usuário para o JavaScript
        const username = "<?php echo isset($_SESSION['username']) ? $_SESSION['username'] : ''; ?>";
    </script>
    <script src="script.js"></script>
</body>
</html>
