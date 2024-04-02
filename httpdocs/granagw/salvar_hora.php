<?php
// Verificar se a solicitação é do tipo POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Verificar se os dados necessários foram recebidos
    if (isset($_POST['mac']) && isset($_POST['hrliga']) && isset($_POST['hrdesliga'])) {
        // Conectar ao banco de dados
        try {
            $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria', 'santocyber_telemetria', 'telemetria');
            $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            // Verificar o estado atual do timer automático no banco de dados
            $stmt = $pdo->prepare("SELECT timerautomatico FROM telemetria WHERE mac = :mac");
            $stmt->bindParam(':mac', $_POST['mac']);
            $stmt->execute();
            $result = $stmt->fetch(PDO::FETCH_ASSOC);
            $timerautomatico = $result['timerautomatico'];

            // Inverter o estado do timer automático
            $timerautomatico = $timerautomatico == 1 ? 0 : 1;

            // Atualizar o estado do timer automático na tabela telemetria
            $stmt = $pdo->prepare("UPDATE telemetria SET hrliga = :hrliga, hrdesliga = :hrdesliga, timerautomatico = :timerautomatico WHERE mac = :mac");
            $stmt->bindParam(':hrliga', $_POST['hrliga']);
            $stmt->bindParam(':hrdesliga', $_POST['hrdesliga']);
            $stmt->bindParam(':timerautomatico', $timerautomatico);
            $stmt->bindParam(':mac', $_POST['mac']);
            $stmt->execute();

            // Redirecionar de volta para a página principal ou exibir uma mensagem de sucesso
            header("Location: " . $_SERVER['HTTP_REFERER']);            exit();
        } catch (PDOException $e) {
            // Responder com uma mensagem de erro
            echo "Erro ao salvar os dados: " . $e->getMessage();
        }
    } else {
        // Responder com uma mensagem de erro se os dados estiverem faltando
        echo "Erro: Dados insuficientes.";
    }
} else {
    // Responder com uma mensagem de erro se a solicitação não for POST
    echo "Erro: Método de solicitação inválido.";
}
?>
