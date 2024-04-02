<html lang="pt-BR">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>PIX</title> 

<h1></h1>


<!DOCTYPE html>

<style>
body {
    font-family: Arial, sans-serif;
    margin: 0;
    padding: 0;
    background-color: #f4f4f4;
}

.container {
    max-width: 600px;
    margin: 50px auto;
    padding: 20px;
    background-color: #fff;
    border-radius: 8px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
}

.form-group {
    margin-bottom: 20px;
}

label {
    display: block;
    margin-bottom: 5px;
}

input[type="text"], input[type="hidden"] {
    width: 100%;
    padding: 8px;
    border: 1px solid #ccc;
    border-radius: 4px;
}

button {
    display: block;
    width: 100%;
    padding: 10px;
    border: none;
    border-radius: 4px;
    background-color: #007bff;
    color: #fff;
    cursor: pointer;
}

button:hover {
    background-color: #0056b3;
}
</style>
</head>
<body>

<div class="container">
    <h2>Bem-vindo
            <?php

		session_start();

// Verifica se o usuário está logado
if(!isset($_SESSION['username'])) {
    // Redireciona para a página de login, ou exibe uma mensagem de erro, dependendo do caso
    echo "Erro: Você não está logado.";
    exit;
}
		
		
		
            if(isset($_SESSION['username'])) {
                echo htmlspecialchars($_SESSION['username']);
            }
            ?> vamos jogar, faz um PIX!</h2>
    <form action="pixapi.php" method="post">
        <div class="form-group">
			<input type="hidden" id="mac" name="mac" value="<?php echo isset($_SESSION['username']) ? htmlspecialchars($_SESSION['username']) : ''; ?>">

        </div>
        <div class="form-group">
            <label for="value">Valor:</label>
            <input type="number" id="value" name="value" required>
        </div>
        <?php
            // Definindo o valor de timelapse fornecido pelo PHP
   // Obtém o tempo atual em segundos
                $timelapse = time();
            echo '<input type="hidden" name="external_id" value="' . $timelapse . '">';
        ?>
        <button type="submit">Enviar</button>
    </form>
</div>
</body>
</html>

