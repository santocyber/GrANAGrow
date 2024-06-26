<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>GrANA</title>
    <style>
        /* Estilos para o cabeçalho e o rodapé */
        .header {
            position: relative;
            text-align: center;
            background-color: #4CAF50;
            color: white;
            padding: 20px;
            border-radius: 8px 8px 0 0;
        }

        .footer {
            text-align: center;
            background-color: #4CAF50;
            color: white;
            padding: 10px;
            border-radius: 0 0 8px 8px;
            position: fixed;
            bottom: 0;
            width: 100%;
            margin-top: 70px; /* Espaçamento para evitar sobreposição */
        }

        .login-container {
            width: 300px;
            margin: 0 auto;
            text-align: center;
            margin-top: 50px; /* Adicionado para centralizar o formulário */
        }

        .login-container h2 {
            margin-bottom: 20px; /* Espaçamento abaixo do título */
        }

        .login-container input[type="text"],
        .login-container input[type="password"],
        .login-container button {
            width: 100%;
            padding: 10px;
            margin-bottom: 15px;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
        }

        .login-container button {
            background-color: #4CAF50;
            color: white;
            border: none;
            cursor: pointer;
        }

        .login-container button:hover {
            background-color: #45a049;
        }

        .error-message {
            color: red;
        }

        /* Estilos para o botão de cadastro */
        .register-button {
            position: absolute;
            top: 20px;
            right: 20px;
            background-color: #4CAF50;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            text-decoration: none;
        }

        .register-button:hover {
            background-color: #45a049;
        }
 /* Estilos para o botão de cadastro */
        .game-button {
            position: absolute;
            top: 20px;
            left: 20px;
            background-color: #4CAF50;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            text-decoration: none;
        }

        .game-button:hover {
            background-color: #45a049;
        }
    </style>
</head>
<body>
    <div class="header">
        <h1 class="title">GrANA</h1>
        <a class="register-button" href="cadastro.php">Cadastro</a>
        <a class="game-button" href="http://mirako.org">Sobre</a>
    </div>

    <div class="login-container">
        <h2>Entre na GrANA</h2>
        <form action="login.php" method="POST">
            <input type="text" id="username" name="username" placeholder="Username" required><br><br>
            <input type="password" id="password" name="password" placeholder="Password" required><br><br>
            <button type="submit">Login</button>
        </form>

        <!-- Exibir mensagem de erro abaixo do formulário, se houver -->

		
		<?php
// Iniciar a sessão para verificar se o usuário está logado
session_start();

// Verificar se o usuário já está logado
if (isset($_SESSION['username'])) {
    // Se o usuário já estiver logado, redirecioná-lo para a página dashboard.php
    header("Location: dashboard.php");
    exit(); // Certificar-se de que o script não continua a ser executado após o redirecionamento
}

	
		
		
		
		
		
		
		
		
		
		
        // Verificar se há uma mensagem de erro na URL e exibir apropriadamente
        $error = $_GET['error'] ?? '';
        if ($error === 'user') {
            echo '<p class="error-message">Usuário não encontrado. Tente novamente.</p>';
        } else if ($error === 'password') {
            echo '<p class="error-message">Senha incorreta. Tente novamente.</p>';
        }
		
		if(isset($_GET['mensagem']) && $_GET['mensagem'] == 'sucesso') {
    echo "<p>Cadastro realizado com sucesso!</p>";
}
        ?>
    </div>

    <div class="footer">
        <p>&copy;  GrANA - The Green ANArchy Project. Nenhum direitos reservados. CC0</p>
    </div>
</body>
</html>
