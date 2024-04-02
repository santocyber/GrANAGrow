<?php
// Verifica se foi enviado um pedido POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Verifica se o parâmetro 'mac' foi enviado
    if (isset($_POST['mac'])) {
        // Captura o MAC recebido do formulário
        $mac = $_POST['mac'];

        try {
            // Conecta-se ao banco de dados usando PDO
            $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria', 'santocyber_telemetria', 'telemetria');
            $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            // Verifica se há dados a serem excluídos
            $stmt_count = $pdo->prepare("SELECT COUNT(*) FROM dados_serializados WHERE mac = :mac");
            $stmt_count->execute(array(':mac' => $mac));
            $count = $stmt_count->fetchColumn();

            if ($count > 0) {
                // Prepara a consulta SQL para apagar os dados associados ao MAC fornecido
                $stmt_delete = $pdo->prepare("DELETE FROM dados_serializados WHERE mac = :mac");

                // Executa a declaração
                $stmt_delete->execute(array(':mac' => $mac));

                echo "Dados apagados com sucesso.";
            } else {
                echo "Nenhum dado encontrado para o MAC fornecido.";
            }
        } catch (PDOException $e) {
            echo "Erro ao apagar os dados: " . $e->getMessage();
        }
    } else {
        echo "Erro: Parâmetro 'mac' não foi recebido.";
    }
} else {
    echo "Erro: Este script aceita apenas pedidos POST.";
}
?>
