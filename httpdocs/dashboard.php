<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dashboard</title>
    <style>
        /* Estilos CSS aqui */
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            transition: background-color 0.5s ease; /* Adiciona transição de cor de fundo para o modo escuro */
        }

        .container {
            max-width: 1200px;
            margin: 20px auto;
            padding: 20px;
            background-color: #333; /* Alterado para um cinza escuro no modo escuro */
            color: #fff; /* Alterado para branco no modo escuro */
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            position: relative;
            transition: background-color 0.5s ease; /* Adiciona transição de cor de fundo para o modo escuro */
        }

        .user-info {
            font-size: 24px;
            font-weight: bold;
            margin-bottom: 20px;
        }

        .avatar {
            width: 100px;
            height: 100px;
            border-radius: 50%;
            overflow: hidden;
            margin-right: 20px;
        }

        .avatar img {
            width: 100%;
            height: auto;
        }

        .score {
            font-size: 36px;
            font-weight: bold;
            margin-bottom: 30px;
        }

        .button {
            display: inline-block;
            padding: 20px 40px;
            font-size: 20px;
            font-weight: bold;
            text-decoration: none;
            color: #fff;
            background-color: #4CAF50;
            border-radius: 10px;
            margin-right: 20px;
        }

        .logout-button {

			position: absolute;
            top: 30px;
            right: 10px;
			  color: #fff;

        }     
		
		.settings-button {
            background-color: #4CAF50;
            color: #fff;
            font-size: 24px;
            font-weight: bold;
            text-align: center;
            width: 40px;
            height: 40px;
            border: none;
            border-radius: 50%;
            cursor: pointer;
            position: absolute;
            top: 20px;
            right: 50px;
            transition: background-color 0.3s ease;
        }

        .settings-button:hover {
            background-color: #45a049;
        }

        .settings-card {
            background-color: #c3e6cb; /* Verde claro */
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            padding: 20px;
            margin-top: 20px;
            position: absolute;
            top: 100%;
            right: 20px;
            z-index: 1;
            display: none;
            transition: background-color 0.5s ease; /* Adiciona transição de cor de fundo para o modo escuro */
        }

        .dark-mode-switch {
            margin-bottom: 20px;
        }

        .dark-mode-switch label {
            margin-right: 10px;
        }

        .upload-form input[type="file"] {
            margin-bottom: 10px;
        }

        .upload-form button {
            display: block;
            padding: 10px 20px;
            font-size: 16px;
            font-weight: bold;
            text-decoration: none;
            color: #fff;
            background-color: #4CAF50;
            border: none;
            border-radius: 5px;
        }

        .dark-mode {
            background-color: #222; /* Alterado para um cinza mais escuro */
            color: #fff; /* Alterado para branco no modo escuro */
        }
		.popup {
    display: none; /* Adiciona para ocultar o popup inicialmente */
}
		#changePasswordButton {
    padding: 10px 20px; /* Reduzindo o padding para diminuir o tamanho do botão */
    font-size: 16px; /* Reduzindo o tamanho da fonte para diminuir o tamanho do botão */
}
    </style>
</head>
<body>
	
	                <?php
                session_start();

// Verifica se o usuário está logado
if(!isset($_SESSION['username'])) {
    // Redireciona para a página de login, ou exibe uma mensagem de erro, dependendo do seu caso
   // echo "Erro: Você não está logado.";
	//echo '<a href="https://santocyber.helioho.st">Ir para a página inicial</a>';
    header("Location: index.php");

    exit;
}
?>
	
    <div class="container">
        <div class="user-info">
            <div class="avatar">

				
				<?php				
				
				
				
				
				
                if(isset($_COOKIE['user_avatar'])) {
                    echo '<img src="' . $_COOKIE['user_avatar'] . '" alt="Avatar">';
                } else {
                    // Se a foto estiver no banco de dados, exibe a foto do banco de dados
                    $host = 'santocyber.helioho.st';
                    $dbname = 'santocyber_telemetria';
                    $username_db = 'santocyber_telemetria';
                    $password_db = 'telemetria';

                    try {
                        $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username_db, $password_db);
                        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

                        $username = $_SESSION['username'];
                        $stmt = $pdo->prepare("SELECT foto FROM usuario WHERE usuario = :username");
                        $stmt->bindParam(':username', $username);
                        $stmt->execute();
                        $user = $stmt->fetch(PDO::FETCH_ASSOC);

                        if ($user && $user['foto']) {
                            $base64_image = base64_encode($user['foto']);
                            echo '<img src="data:image/jpeg;base64,' . $base64_image . '" alt="Avatar">';
                        } else {
                            echo '<img src="avatar.jpg" alt="Avatar">'; // Use um avatar padrão caso não tenha imagem
                        }
                    } catch(PDOException $e) {
                        echo '<img src="avatar.jpg" alt="Avatar">'; // Use um avatar padrão em caso de erro
                    }
                }
                ?>
            </div>
            <?php
            if(isset($_SESSION['username'])) {
                echo htmlspecialchars($_SESSION['username']);
            }
            ?>
        </div>
        <div class="score">
            <?php
            // Conectar ao banco de dados
            $host = 'santocyber.helioho.st';
            $dbname = 'santocyber_telemetria';
            $username_db = 'santocyber_telemetria';
            $password_db = 'telemetria';

            try {
                $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username_db, $password_db);
                $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

                // Obtém o score do usuário
                $username = $_SESSION['username']; // Obtém o nome de usuário da sessão
                $stmt = $pdo->prepare("SELECT score FROM usuario WHERE usuario = :username");
                $stmt->bindParam(':username', $username);
                $stmt->execute();
                $user = $stmt->fetch(PDO::FETCH_ASSOC);

                // Exibe o score
                echo "Score: " . htmlspecialchars($user['score']);
            } catch(PDOException $e) {
                // Em caso de erro de conexão com o banco de dados, exibe mensagem de erro
                echo "Erro de conexão: " . $e->getMessage();
            }
            ?>
        </div>
        <a href="/games" class="button">GAMES</a>
        <a href="/granagw/" class="button">MONITOR</a>
        <a href="/telemetria/" class="button">TELEMETRIA</a>
        <a href="/chat/" class="button">BATEPAPO</a>
        <a href="/chat/forum.php" class="button">DUVIDAS?</a>
        
        <button class="settings-button" id="toggleSettings">&#8722;</button>
		  <!-- Botão para logout -->
    <form action="logout.php" method="POST">
<button type="submit" class="logout-button" title="LOGOUT: Quero sair!">&#128721;</button>    </form>
   

        <div class="settings-card" id="settingsCard">
            <h2>Configurações</h2>
            <div class="dark-mode-switch">
                <input type="checkbox" id="darkModeToggle">
                <label for="darkModeToggle">Modo Escuro</label>
            </div>
            <div class="upload-form">
                <form action="upload.php" method="post" enctype="multipart/form-data">
                    <input type="file" name="image" accept="image/*">
                    <button type="submit">Enviar Foto</button>
                </form>
            </div>
			    <button class="button" id="changePasswordButton">Alterar Senha</button>
			<!-- Popup para alteração de senha -->
<div class="popup" id="changePasswordPopup">
    <div class="popup-content">
        <span class="close" id="closePopup">&times;</span>
        <h2>Alterar Senha</h2>
        <form id="changePasswordForm">
            <input type="password" id="oldPassword" placeholder="Senha Antiga" required><br>
            <input type="password" id="newPassword" placeholder="Nova Senha" required><br>
            <input type="password" id="confirmNewPassword" placeholder="Confirmar Nova Senha" required><br>
            <button type="submit">Confirmar</button>
        </form>
		  
</div>
</div>
        </div>
    </div>
	



    <script>
        // Script para alternar entre o modo claro e escuro
        const darkModeToggle = document.getElementById('darkModeToggle');
        darkModeToggle.addEventListener('change', () => {
            document.body.classList.toggle('dark-mode', darkModeToggle.checked);
            document.querySelector('.settings-card').classList.toggle('dark-mode', darkModeToggle.checked);
        });

        // Script para esconder/mostrar o card de configurações
        const toggleSettingsButton = document.getElementById('toggleSettings');
        const settingsCard = document.getElementById('settingsCard');
        toggleSettingsButton.addEventListener('click', () => {
            settingsCard.style.display = (settingsCard.style.display === 'none') ? 'block' : 'none';
        });
		

		 // Script para abrir o popup de alteração de senha quando o botão "Alterar Senha" for clicado
    const changePasswordButton = document.getElementById('changePasswordButton');
    const changePasswordPopup = document.getElementById('changePasswordPopup');
    const closePopupButton = document.getElementById('closePopup');

    changePasswordButton.addEventListener('click', () => {
        changePasswordPopup.style.display = 'block';
    });

    closePopupButton.addEventListener('click', () => {
        changePasswordPopup.style.display = 'none';
    });

    // Script para enviar as informações de alteração de senha
    const changePasswordForm = document.getElementById('changePasswordForm');
    changePasswordForm.addEventListener('submit', (event) => {
        event.preventDefault(); // Impede o envio do formulário por padrão

        // Obtenha os valores dos campos
        const oldPassword = document.getElementById('oldPassword').value;
        const newPassword = document.getElementById('newPassword').value;
        const confirmNewPassword = document.getElementById('confirmNewPassword').value;

        // Verifique se as senhas nova e confirmada correspondem
        if (newPassword !== confirmNewPassword) {
            alert('As senhas nova e confirmada não correspondem.');
            return;
        }

        // Envie as informações de alteração de senha (implemente a lógica apropriada aqui)
        // Por exemplo, você pode enviar uma solicitação AJAX para o servidor
        // com os dados para alteração de senha e manipular a resposta do servidor
        // para exibir uma mensagem de sucesso ou erro ao usuário.
        
        // Aqui, por exemplo, estamos apenas exibindo um alerta para simular o envio bem-sucedido.
        alert('Senha alterada com sucesso!');
        // Feche o popup
        changePasswordPopup.style.display = 'none';
    });
		

    </script>
</body>
</html>