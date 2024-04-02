<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Slot Machine Game</title>
<style>
body {
    font-family: Arial, sans-serif;
    background-color: yellow; /* Fundo amarelo */
    color: red; /* Texto vermelho */
    padding: 20px; /* Adicionando padding para espaçamento interno */
}

.slot-machine {
    text-align: center;
    margin-top: 50px;
}

#reels {
    display: flex;
    justify-content: center;
    margin-bottom: 20px;
}

.reel {
    width: 140px;
    height: 140px;
    background-color: red;
    margin: 0 10px;
    display: inline-block;
    font-size: 100px; /* Aumentar o tamanho do emoji */
    line-height: 120px; /* Ajustar a altura do emoji */
    text-align: center;
    border-radius: 40px; /* Arredondar as bordas */
}

#spinButton {
    padding: 10px 20px;
    font-size: 18px;
    cursor: pointer;
}

p {
    font-size: 24px; /* Aumentar o tamanho do texto */
}

 /* Centralizar mensagem de boas-vindas */
        h1 {
            text-align: center;
        }

        /* Estilo para o botão de voltar */
        #backButton {
            display: block;
            margin: 20px auto;
            padding: 10px 20px;
            font-size: 18px;
            cursor: pointer;
        }
	
	
	




</style>
</head>
<body>
	 <h1>Bem-Vindo
            <?php
            session_start();
            if(isset($_SESSION['username'])) {
                echo htmlspecialchars($_SESSION['username']);
            }
            ?> ao Mundo da Diversão!</h1>
<?php
session_start();

// Verifica se o usuário está logado
if(!isset($_SESSION['username'])) {
    // Redireciona para a página de login, ou exibe uma mensagem de erro, dependendo do seu caso
    echo "Erro: Você não está logado.";
    exit;
}

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

    // Verifica se o score foi encontrado
    if ($user && isset($user['score'])) {
        // Exibe o score
        echo "Seu SCORE GERAL: " . htmlspecialchars($user['score']);
    } else {
        echo "Erro: Score não encontrado para este usuário.";
    }
} catch(PDOException $e) {
    // Em caso de erro de conexão com o banco de dados, exibe mensagem de erro
    echo "Erro de conexão: " . $e->getMessage();
}
?>


    <div class="slot-machine">
        <div id="reels">
            <div class="reel" id="reel1"></div>
            <div class="reel" id="reel2"></div>
            <div class="reel" id="reel3"></div>
        </div>
        <button id="spinButton">GIRE</button>
        <p>Score: <span id="score">0</span></p>
    </div>
    <script>
		
		      // Definir a função goBack fora do escopo DOMContentLoaded
        function goBack() {
            window.history.back();
        }
		    let spinning = false; // Variável para controlar se os rolos estão girando

		
	document.addEventListener('DOMContentLoaded', function() {
    const reels = document.querySelectorAll('.reel');
    const spinButton = document.getElementById('spinButton');
    const scoreDisplay = document.getElementById('score');
    let score = 0;

    const fruits = ['🍎', '🍌', '🍇', '🍊', '🍓', '🍉'];

function spin() {
    if (spinning) return;
    spinning = true;
    reels.forEach(reel => {
        let i = 0;
        const interval = setInterval(() => {
            reel.textContent = fruits[i];
            i++;
            if (i === fruits.length) i = 0;
        }, 100);
        setTimeout(() => {
            clearInterval(interval);
            const randomIndex = Math.floor(Math.random() * fruits.length);
            reel.textContent = fruits[randomIndex];
            if (reels[0].textContent === reels[1].textContent && reels[1].textContent === reels[2].textContent) {
                score++;
                scoreDisplay.textContent = score;
                // Envie o score para o servidor
                updateScore(score);
            }
            spinning = false;
            spinButton.disabled = false;
        }, 2000);
    });
    spinButton.disabled = true;
}

   function updateScore(score) {
        // Enviar o score para o servidor via AJAX
        const xhr = new XMLHttpRequest();
        xhr.open('POST', 'atualizar_score.php');
        xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
        xhr.onload = function() {
            // Verifique se a solicitação foi bem-sucedida
            if (xhr.status === 200) {
                // Faça algo após a atualização do score
            } else {
                console.error('Erro ao atualizar score: ' + xhr.statusText);
            }
        };
        xhr.send('score=' + score); // Corrigi o nome do parâmetro enviado
    }

    spinButton.addEventListener('click', spin);
});
		
		
		

</script>
	
	    <button id="backButton" onclick="goBack()">Voltar</button>

</body>
</html>

