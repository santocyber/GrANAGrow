<?php
header('Content-Type: application/json');

try {
    $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria', 'santocyber_telemetria', 'telemetria');
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // Verifique se o parâmetro 'mac' está presente na solicitação
    if (isset($_GET['mac'])) {
        $mac = $_GET['mac'];

        // Use uma instrução SQL SELECT para obter o valor atual da coluna 'credito' antes de atualizá-la
        $stmt = $pdo->prepare("SELECT credito FROM telemetria WHERE mac = :mac");
        $stmt->bindParam(':mac', $mac, PDO::PARAM_STR);
        $stmt->execute();

        // Verifique se há resultados
        $row = $stmt->fetch(PDO::FETCH_ASSOC);

        if ($row !== false) {
            $credito = $row['credito'];

            // Atualize a coluna 'credito' para NULL
            $stmt = $pdo->prepare("UPDATE telemetria SET credito = NULL WHERE mac = :mac");
            $stmt->bindParam(':mac', $mac, PDO::PARAM_STR);
            $stmt->execute();

            // Agora, atualize a coluna 'acoes' para 'ligado'
            $stmt = $pdo->prepare("UPDATE telemetria SET acoes = 'ligado' WHERE mac = :mac");
            $stmt->bindParam(':mac', $mac, PDO::PARAM_STR);
            $stmt->execute();

            // Responda com o valor original da coluna "credito" em formato JSON
            echo json_encode(['message' => 'Valor da coluna "credito" para o mac ' . $mac . ' foi obtido e definido como NULL', 'credito' => $credito]);
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

