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
    if (!empty($_POST['mac']) && !empty($_POST['timerfoto'])) {
        $mac = $_POST['mac'];
        $timerfoto = $_POST['timerfoto'];

        try {
            $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria', 'santocyber_telemetria', 'telemetria');
            $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

			
			
			  // Verificar o estado atual do timer automático no banco de dados
            $stmt = $pdo->prepare("SELECT timerfotostatus FROM telemetria WHERE mac = :mac");
            $stmt->bindParam(':mac', $_POST['mac']);
            $stmt->execute();
            $result = $stmt->fetch(PDO::FETCH_ASSOC);
            $timerfotostatus = $result['timerfotostatus'];

            // Inverter o estado do timer automático
            $timerfotostatus = $timerfotostatus == 1 ? 0 : 1;
			
			
			
			
			
            $stmt = $pdo->prepare("UPDATE telemetria SET timerfoto = :timerfoto , timerfotostatus = :timerfotostatus WHERE mac = :mac");
            $stmt->bindParam(':timerfoto', $timerfoto);
		    $stmt->bindParam(':timerfotostatus', $timerfotostatus);

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

