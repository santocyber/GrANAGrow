<?php
// Informações de conexão com o banco de dados
$hostname = 'santocyber.helioho.st';
$username = 'santocyber_telemetria';
$password = 'telemetria';
$database = 'santocyber_telemetria';

try {
    // Conectar ao banco de dados usando PDO
    $pdo = new PDO("mysql:host=$hostname;dbname=$database", $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // Verificar se 'mac' está definido no POST
    if (isset($_POST['mac'])) {
        $mac = $_POST['mac'];
        
        // Sanitizar o valor do parâmetro 'mac' para evitar SQL injection
        $mac = filter_var($mac, FILTER_SANITIZE_STRING);
        
        // Preparar a consulta SQL usando um placeholder
        $sql = "SELECT acoes, estado, mensagem, mensagemstatus, hrliga, hrdesliga, timerautomatico, timerfoto, timerfotostatus FROM telemetria WHERE mac = :mac";

        // Preparar a declaração SQL
        $stmt = $pdo->prepare($sql);

        // Bind do valor do parâmetro
        $stmt->bindParam(':mac', $mac, PDO::PARAM_STR);

        // Executar a consulta
        $stmt->execute();

        // Obter o resultado da consulta
        $row = $stmt->fetch(PDO::FETCH_ASSOC);

        if ($row) {
            $acoes = $row['acoes'];
            $estado = $row['estado'];
            $mensagem = $row['mensagem'];
			$mensagemstatus = $row['mensagemstatus'];		
            $hrliga = $row['hrliga'];
            $hrdesliga = $row['hrdesliga'];
            $timerautomatico = $row['timerautomatico'];
            $timerfoto = $row['timerfoto'];
            $timerfotostatus = $row['timerfotostatus'];

            // Retornar a ação, estado, hrliga e hrdesliga como resposta JSON
            $response = array(
                'acoes' => $acoes,
                'estado' => $estado,
                'mensagem' => $mensagem,
                'mensagemstatus' => $mensagemstatus,
                'hrliga' => $hrliga,
                'hrdesliga' => $hrdesliga,
                'timerautomatico' => $timerautomatico,
                'timerfoto' => $timerfoto,
                'timerfotostatus' => $timerfotostatus
            );
            echo json_encode($response);
        } else {
            // A ação não foi encontrada
            echo json_encode(array('error' => 'Ação não encontrada'));
        }
    } else {
        // 'mac' não está definido no POST
        echo json_encode(array('error' => 'Parâmetro "mac" não especificado no POST'));
    }
} catch (PDOException $e) {
    // Tratar erros de conexão com o banco de dados
    echo json_encode(array('error' => 'Erro de conexão com o banco de dados: ' . $e->getMessage()));
}
?>
