<?php
header('Content-Type: application/json');

try {
    $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria', 'santocyber_telemetria', 'telemetria');
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // Verifique se o parâmetro 'mac' está presente na solicitação
    if (isset($_GET['mac'])) {
        $mac = $_GET['mac'];

        // Use uma instrução SQL SELECT para obter o valor atual das colunas 'credito' e 'acoes' antes de atualizá-las
        $stmt = $pdo->prepare("SELECT credito, acoes FROM telemetria WHERE mac = :mac");
        $stmt->bindParam(':mac', $mac, PDO::PARAM_STR);
        $stmt->execute();

        // Verifique se há resultados
        $row = $stmt->fetch(PDO::FETCH_ASSOC);

        if ($row !== false) {
            $credito = $row['credito'];
            $acoes = $row['acoes'];

            // Atualize as colunas 'credito' e 'acoes' para NULL
            $stmt = $pdo->prepare("UPDATE telemetria SET credito = NULL, acoes = NULL WHERE mac = :mac");
            $stmt->bindParam(':mac', $mac, PDO::PARAM_STR);
            $stmt->execute();

            // Responda com os valores originais das colunas "credito" e "acoes" em formato JSON
            echo json_encode(['message' => 'Valores das colunas "credito" e "acoes" para o mac ' . $mac . ' foram obtidos e definidos como NULL', 'credito' => $credito, 'acoes' => $acoes]);
        } else {
            echo json_encode(['error' => 'Não foi possível encontrar um registro com o mac especificado']);
        }
    } else {
        echo json_encode(['error' => 'O parâmetro "mac" não foi especificado']);
    }
} catch (PDOException $e) {
    echo json_encode(['error' => 'Erro: ' . $e->getMessage()]);
}
?>
