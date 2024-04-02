<?php
// Verifica se foi enviado um pedido POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Verifica se o parâmetro 'identificador' foi enviado
    if (isset($_POST['identificador'])) {
        // Captura o identificador recebido do formulário
        $identificador = $_POST['identificador'];

        try {
            // Conecta-se ao banco de dados usando PDO
            $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria', 'santocyber_telemetria', 'telemetria');
            $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            // Prepara e executa a declaração DELETE
            $stmt_delete = $pdo->prepare("DELETE FROM telemetria WHERE identificador = :identificador");
            $stmt_delete->execute(array(':identificador' => $identificador));

            // Verifica se algum registro foi deletado
            if ($stmt_delete->rowCount() > 0) {
                echo "Dados apagados com sucesso.";
            } else {
                echo "Nenhum dado encontrado para o identificador fornecido.";
            }
        } catch (PDOException $e) {
            echo "Erro ao apagar os dados: " . $e->getMessage();
        }
    } else {
        echo "Erro: Parâmetro 'identificador' não foi recebido.";
    }
} else {
    echo "Erro: Este script aceita apenas pedidos POST.";
}
?>
