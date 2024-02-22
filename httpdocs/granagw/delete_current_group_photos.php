<?php
// Verificar se o método da requisição é POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Verificar se os dados foram recebidos
    if (isset($_POST['timestamps'], $_POST['mac']) && is_array($_POST['timestamps']) && !empty($_POST['mac'])) {
        // Configurações do banco de dados
        $servername = "santocyber.helioho.st";
        $username = "santocyber_telemetria";
        $password = "telemetria";
        $dbname = "santocyber_telemetria";

        // Conexão com o banco de dados usando a extensão mysqli
        $conn = new mysqli($servername, $username, $password, $dbname);

        // Verificar a conexão
        if ($conn->connect_error) {
            die("Conexão falhou: " . $conn->connect_error);
        }

        // Preparar a declaração SQL para excluir as fotos com os timestamps e o MAC correspondentes
        $sql = "DELETE FROM fotos_serializadas WHERE mac = ? AND timestamp IN (" . implode(",", array_fill(0, count($_POST['timestamps']), '?')) . ")";

        // Preparar a declaração SQL
        $stmt = $conn->prepare($sql);

        // Verificar se a preparação da declaração foi bem-sucedida
        if ($stmt === false) {
            die("Erro na preparação da declaração: " . $conn->error);
        }

        // Vincular os parâmetros e executar a declaração
        $stmt->bind_param(str_repeat("s", count($_POST['timestamps'])) . "s", $_POST['mac'], ...$_POST['timestamps']);
        $stmt->execute();

        // Verificar se a execução da declaração foi bem-sucedida
        if ($stmt->affected_rows > 0) {
            echo "Grupo de fotos excluído com sucesso.";
        } else {
            echo "Nenhuma foto foi excluída.";
        }

        // Fechar a declaração e a conexão
        $stmt->close();
        $conn->close();
    } else {
        echo "Dados insuficientes.";
    }
} else {
    echo "Método de requisição inválido.";
}
?>
