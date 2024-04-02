<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Bem-Vindo ao Mundo da Diversão!</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            color: #333;
            margin: 0;
            padding: 0;
        }

        .container {
            max-width: 800px;
            margin: 50px auto;
            padding: 20px;
            background-color: #fff;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }

        h1, h2 {
            text-align: center;
            color: #333;
        }

        p {
            margin-bottom: 20px;
            line-height: 1.6;
        }

        .cta-btn {
            display: block;
            width: 200px;
            margin: 20px auto;
            padding: 10px 20px;
            background-color: #007bff;
            color: #fff;
            text-align: center;
            text-decoration: none;
            border-radius: 5px;
            transition: background-color 0.3s;
        }

        .cta-btn:hover {
            background-color: #0056b3;
        }

        .btn-container {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 20px;
        }

        .btn {
            padding: 10px 20px;
            background-color: #007bff;
            color: #fff;
            text-decoration: none;
            border-radius: 5px;
            transition: background-color 0.3s;
        }

        .btn:hover {
            background-color: #0056b3;
        }

        .warning {
            color: red;
        }

        .ranking {
            position: fixed;
            top: 20px;
            left: 20px;
            background-color: rgba(255, 255, 255, 0.8);
            padding: 10px;
            border-radius: 5px;
        }
		
		

    </style>
</head>
<body>
    <div class="ranking">
        <h2>Top 20</h2>
        <ol>
            <?php
            // Conectar ao banco de dados
            $host = 'santocyber.helioho.st';
            $dbname = 'santocyber_telemetria';
            $username_db = 'santocyber_telemetria';
            $password_db = 'telemetria';

            try {
                $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username_db, $password_db);
                $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

                // Obtém os top 20 usuários por score
                $stmt = $pdo->query("SELECT usuario, score FROM usuario ORDER BY score DESC LIMIT 20");
                $rank = 1;
                while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
                    echo "<li>#$rank: " . htmlspecialchars($row['usuario']) . " - Score: " . htmlspecialchars($row['score']) . "</li>";
                    $rank++;
                }
            } catch(PDOException $e) {
                // Em caso de erro de conexão com o banco de dados, exibe mensagem de erro
                echo "Erro de conexão: " . $e->getMessage();
            }
            ?>
        </ol>
    </div>

    <div class="container">
        <h1>Bem-vindo
            <?php
			session_start();

// Verifica se o usuário está logado
if(!isset($_SESSION['username'])) {
    // Redireciona para a página de login, ou exibe uma mensagem de erro, dependendo do seu caso
    echo "Erro: Você não está logado.";
	echo '<a href="https://santocyber.helioho.st">Ir para a página inicial</a>';

    exit;
}
			
            if(isset($_SESSION['username'])) {
                echo htmlspecialchars($_SESSION['username']);
            }
            ?> ao Mundo da Diversão!</h1>
        <div class="score">
            <?php
            try {
                // Obtém o score do usuário
                $username = $_SESSION['username']; // Obtém o nome de usuário da sessão
                $stmt = $pdo->prepare("SELECT score FROM usuario WHERE usuario = :username");
                $stmt->bindParam(':username', $username);
                $stmt->execute();
                $user = $stmt->fetch(PDO::FETCH_ASSOC);

                // Exibe o score
                echo "Seu SCORE: " . htmlspecialchars($user['score']);
            } catch(PDOException $e) {
                // Em caso de erro de conexão com o banco de dados, exibe mensagem de erro
                echo "Erro de conexão: " . $e->getMessage();
            }
            ?>
        </div>
        <p>Explore este mundo emocionante onde a diversão é a principal atração. Aqui, você pode encontrar uma variedade de jogos e atividades para desfrutar e relaxar. Lembre-se de que o entretenimento saudável pode trazer muitos benefícios, incluindo alívio do estresse, melhoria do humor e fortalecimento das relações sociais.</p>
        <p><span class="warning">*</span> Aproveite com responsabilidade. O vício em jogos pode ser prejudicial.</p>
        <div class="btn-container">
			   <button class="btn" id="backButton" onclick="goBack()">Voltar</button>
	
            <a href="pix/" class="btn">COLOCAR CRÉDITO</a>
        </div>
        <a href="tamagrana/" class="cta-btn">TamaGreen</a>
        <a href="slot.php" class="cta-btn">Slot Machine</a>
        <a href="riverraid.php" class="cta-btn">RiverRaid</a>
        <a href="tetris.php" class="cta-btn">Tetris</a>
        <a href="asteroids/" class="cta-btn">Asteroids</a>
        <a href="lunar/" class="cta-btn">Pouso Lunar</a>
        <a href="pacman/" class="cta-btn">PacMan</a>
    </div>
	
	<script>
			      // Definir a função goBack fora do escopo DOMContentLoaded
        function goBack() {
            window.history.back();
        }
		</script>
</body>
</html>