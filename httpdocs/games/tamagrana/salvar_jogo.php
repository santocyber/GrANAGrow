<?php
session_start();

// Verifica se o usuário está logado
if (!isset($_SESSION['username'])) {
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

    // Verifica se não há dados na tabela tamagreen para o usuário
    if (!$tamagreen_data) {
        // Redireciona para a página de erro
        echo "Erro: Não foi possível encontrar dados do jogo para o usuário.";
        exit;
    }

    // Verifica se a última rega foi feita no mesmo dia
    if ($tamagreen_data['ultimaRega'] && date("Y-m-d", strtotime($tamagreen_data['ultimaRega'])) == date("Y-m-d")) {
        // Se foi no mesmo dia, exibe mensagem e não faz nenhuma alteração
        echo "Já foi feita a manutenção do dia. Volte amanhã.";
        echo '<a href="index.php">Voltar para o jogo</a>';

        exit;
    }

// Verifica se a última rega foi feita ontem
if ($tamagreen_data['ultimaRega'] && date("Y-m-d", strtotime($tamagreen_data['ultimaRega'])) == date("Y-m-d", strtotime("-1 day"))) {
    // Se foi ontem, soma um no tamanho da planta
    $tamanhoPlanta = intval($tamagreen_data['tamanhoPlanta']) + 1;
} else {
    // Caso contrário, mantém o tamanho da planta
    $tamanhoPlanta = intval($tamagreen_data['tamanhoPlanta']);
}

// Verifica se a última rega foi feita depois de 3 dias
if ($tamagreen_data['ultimaRega'] && date("Y-m-d", strtotime($tamagreen_data['ultimaRega'])) < date("Y-m-d", strtotime("-3 days"))) {
    // Se foi depois de 3 dias, diminui os pontos de colheita em 3
     $tamanhoPlanta -= 3;
}

// Adiciona 1 ponto ao tamanho da planta se pontosColheita for maior que 10
if (intval($_POST['pontosColheita']) > 10) {
    $tamanhoPlanta++;
}



    // Zera os pontos de colheita se o tamanho da planta for maior que 40
    if ($tamanhoPlanta > 42) {
        $pontosColheita = 0;
        $tamanhoPlanta = 0;

        // Adiciona 1 ponto ao score na tabela usuario
        $stmt = $pdo->prepare("UPDATE usuario SET score = score + 1 WHERE usuario = :username");
        $stmt->bindParam(':username', $username);
        $stmt->execute();
    } else {
        // Caso contrário, mantém os pontos de colheita
        $pontosColheita = intval($_POST['pontosColheita']) + intval($tamagreen_data['pontosColheita']);
    }

	
	// Verifica se os pontos de colheita são negativos e diminui um ponto no tamanho da planta
	//	$tamagreen_data['pontosColheita']

if (intval($_POST['pontosColheita']) < 0) {
   // $tamanhoPlanta--;
	    $tamanhoPlanta -= 2;
}
	
	if (intval($tamagreen_data['pontosColheita']) < -50) {
   // $tamanhoPlanta--;
	    $tamanhoPlanta -= 1;
}


	
    // Atualiza os dados do jogo na tabela tamagreen
    $ultimaRega = date("Y-m-d H:i:s"); // Obtém a data e hora atual para a última rega
    $ultimoAcesso = date("Y-m-d H:i:s"); // Obtém a data e hora atual para o último acesso

    $stmt = $pdo->prepare("UPDATE tamagreen SET ultimaRega = :ultimaRega, ultimoAcesso = :ultimoAcesso, tamanhoPlanta = :tamanhoPlanta, pontosColheita = :pontosColheita WHERE usuario = :username");
    $stmt->bindParam(':ultimaRega', $ultimaRega);
    $stmt->bindParam(':ultimoAcesso', $ultimoAcesso);
    $stmt->bindParam(':tamanhoPlanta', $tamanhoPlanta);
    $stmt->bindParam(':pontosColheita', $pontosColheita);
    $stmt->bindParam(':username', $username);
    $stmt->execute();

    // Redireciona de volta para a página do jogo
    header("Location: index.php");
    exit;
} catch (PDOException $e) {
    // Em caso de erro de conexão com o banco de dados, exibe mensagem de erro
    echo "Erro de conexão: " . $e->getMessage();
}
?>
