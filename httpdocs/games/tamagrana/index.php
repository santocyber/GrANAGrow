<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Planta Tamagotchi</title>
    <link rel="stylesheet" href="styles.css">
</head>
<body>

    <h1>Bem-Vindo 
        <?php
        session_start();
        if(isset($_SESSION['username'])) {
            echo htmlspecialchars($_SESSION['username']);
        }
        ?>
        ao TamaGreen!</h1>

    <?php
    session_start();

    // Verifica se o usuário está logado
    if(!isset($_SESSION['username'])) {
        // Redireciona para a página de login, ou exibe uma mensagem de erro, dependendo do seu caso
        echo "Erro: Você não está logado.";
        echo '<a href="https://santocyber.helioho.st">Ir para a página inicial</a>';
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

        // Obtém o nome de usuário da sessão
        $username = $_SESSION['username'];

        // Busca os dados do jogo na tabela tamagreen
        $stmt = $pdo->prepare("SELECT * FROM tamagreen WHERE usuario = :username");
        $stmt->bindParam(':username', $username);
        $stmt->execute();
        $tamagreen_data = $stmt->fetch(PDO::FETCH_ASSOC);

        if(!$tamagreen_data) {
            // Se o usuário não tiver dados do jogo, cria uma nova entrada com valores padrão
            $stmt = $pdo->prepare("INSERT INTO tamagreen (usuario, tamanhoPlanta, diasPassados, phSolo, umidadeAmbiente, temperaturaAmbiente, luzLigada, exaustorLigado, desumidificadorLigado, pontosColheita, pontosJogo) VALUES (:username, 0, 0, 7, 50, 25, 0, 0, 0, 0, 0)");
            $stmt->bindParam(':username', $username);
            $stmt->execute();

            // Busca os dados novamente após a inserção
            $stmt = $pdo->prepare("SELECT * FROM tamagreen WHERE usuario = :username");
            $stmt->bindParam(':username', $username);
            $stmt->execute();
            $tamagreen_data = $stmt->fetch(PDO::FETCH_ASSOC);
        }

		
		   // Busca os dados do usuário na tabela usuario
    $stmt = $pdo->prepare("SELECT score FROM usuario WHERE usuario = :username");
    $stmt->bindParam(':username', $username);
    $stmt->execute();
    $usuario_data = $stmt->fetch(PDO::FETCH_ASSOC);

    if (!$tamagreen_data || !$usuario_data) {
        // Se o usuário não tiver dados do jogo ou do usuário, exibe uma mensagem de erro
        echo "Erro: Não foi possível encontrar dados do jogo ou do usuário.";
        exit;
    }

    // Exibir o score na tabela de status
    $score = $usuario_data['score'];
		
		
		
		
				      // Agora, vamos definir uma variável JavaScript com o tamanho da planta
        echo "<script>";
        echo "var tamanhoPlanta = " . $tamagreen_data['tamanhoPlanta'] . ";";
        echo "</script>";
  


    } catch(PDOException $e) {
        // Em caso de erro de conexão com o banco de dados, exibe mensagem de erro
        echo "Erro de conexão: " . $e->getMessage();
    }
    ?>

    <form id="gameForm" method="post" action="salvar_jogo.php">
        <!-- Input hidden para cada dado do jogo -->
        <input type="hidden" id="tamanhoPlantaInput" name="tamanhoPlanta">
        <input type="hidden" id="diasPassadosInput" name="diasPassados">
        <input type="hidden" id="phSoloInput" name="phSolo">
        <input type="hidden" id="umidadeAmbienteInput" name="umidadeAmbiente">
        <input type="hidden" id="temperaturaAmbienteInput" name="temperaturaAmbiente">
        <input type="hidden" id="luzLigadaInput" name="luzLigada">
        <input type="hidden" id="exaustorLigadoInput" name="exaustorLigado">
        <input type="hidden" id="desumidificadorLigadoInput" name="desumidificadorLigado">
        <input type="hidden" id="pontosColheitaInput" name="pontosColheita">
        <input type="hidden" id="pontosJogoInput" name="pontosJogo">
    </form>

    <div class="planta" id="planta">
        <!-- Emoji da planta será atualizado dinamicamente -->
    </div>

    <div class="actions">
        <button onclick="regarPlanta()">Regar</button>
        <button onclick="controlarPhAgua('aumentar')">Subir pH</button>
        <button onclick="controlarPhAgua('diminuir')">Baixar pH</button>
        <button onclick="ligarExaustor()">Ligar Exaustor</button>
        <button onclick="toggleLuz()">Ligar Luz</button>
        <button onclick="toggleDesumidificador()">Desumidificar</button> <!-- Alterado -->
    </div>
<div class="status">
    <h2>Status da Planta:</h2>
    <table>
        <tr>
            <th>Fator</th>
            <th>Valor</th>
        </tr>
        <tr>
            <td>Tamanho:</td>
            <td id="tamanhoPlantaServer"><?php echo $tamagreen_data['tamanhoPlanta']; ?></td>
        </tr>
        <tr>
            <td>Última Rega:</td>
            <td><?php echo $tamagreen_data['ultimaRega']; ?></td>
        </tr>
        <tr>
            <td>Estoque da Colheita:</td>
            <td><?php echo $tamagreen_data['pontosColheita']; ?></td>
        </tr>
        <tr>
            <td>Umidade Ambiente:</td>
            <td id="umidadeAmbiente">--</td>
        </tr>
        <tr>
            <td>pH do Solo:</td>
            <td id="phSolo">--</td>
        </tr>
        <tr>
            <td>Temperatura Ambiente:</td>
            <td id="temperatura">--</td>
        </tr>
        <tr>
            <td>Luz:</td>
            <td id="statusLuz">Apagada</td>
        </tr>
        <tr>
            <td>Exaustor:</td>
            <td id="statusExaustor">Desligado</td>
        </tr>
        <tr>
            <td>Desumidificador:</td>
            <td id="statusDesumidificador">Desligado</td>
        </tr>
        <tr>
            <td> Previsao de Colheita:</td>
            <td id="valorColheita">--</td>
        </tr>
		 <tr>
            <td>Score Geral:</td>
            <td id="score"><?php echo $score; ?></td>
        </tr>
    </table>

    <button onclick="prepararEnvio()">Manutenção Concluída</button>
	
</div>
	    <script src="script.js"></script>
	

 <script>
        // Chama a função iniciarJogo() automaticamente quando a página for carregada
        iniciarJogo();
    </script>
</body>
</html>