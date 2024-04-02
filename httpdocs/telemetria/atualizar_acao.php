<?php
// Verifica se foi enviado um pedido POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Verifica se o campo 'mac' está presente no pedido POST
    if (isset($_POST['mac'])) {
        // Recupera o valor do campo 'mac'
        $mac = $_POST['mac'];
        
        // Verifica se o campo 'estado' está presente no pedido POST
        if (isset($_POST['estado'])) {
            // Recupera o valor do campo 'estado'
            $estado = $_POST['estado'];
            
            // Configurações de conexão com o banco de dados
            $dsn = 'mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria';
            $username = 'santocyber_telemetria';
            $password = 'telemetria';
            
            try {
                // Conecta ao banco de dados
                $pdo = new PDO($dsn, $username, $password);
                
                // Prepara a declaração SQL para atualizar o estado do dispositivo na tabela 'telemetria'
                $stmt = $pdo->prepare("UPDATE telemetria SET estado = :estado WHERE mac = :mac");
                
                // Executa a declaração SQL com os parâmetros fornecidos
                $stmt->execute(['estado' => $estado, 'mac' => $mac]);
                
                // Exibe uma mensagem de sucesso
                echo "Estado do dispositivo atualizado com sucesso para: $estado";
				 header("Location: " . $_SERVER['HTTP_REFERER']);            exit();

            } catch (PDOException $e) {
                // Exibe uma mensagem de erro caso ocorra uma exceção ao acessar o banco de dados
                echo "Erro ao acessar o banco de dados: " . $e->getMessage();
            }
        } else {
            // Se o campo 'estado' não estiver presente no pedido POST
            echo "Erro: Campo 'estado' não encontrado no pedido POST.";
        }
    } else {
        // Se o campo 'mac' não estiver presente no pedido POST
        echo "Erro: Campo 'mac' não encontrado no pedido POST.";
    }
} else {
    // Se o pedido não for do tipo POST
    echo "Erro: Este script espera um pedido POST.";
}
?>
