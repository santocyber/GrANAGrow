<?php
// Verificar se o método de requisição é GET
if ($_SERVER["REQUEST_METHOD"] == "GET") {
    // Verificar se o parâmetro esperado foi recebido
    if (isset($_GET['mac'])) {
        // Recuperar o MAC do parâmetro GET
        $mac = $_GET['mac'];

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

        // Consulta para excluir todas as fotos associadas ao MAC
        $sql = "DELETE FROM fotos_serializadas WHERE mac = '$mac'";
        if ($conn->query($sql) === TRUE) {
            echo "Todas as fotos associadas ao MAC $mac foram excluídas com sucesso. <a href='verfotos.php?mac=$mac'>clique aqui para voltar</a>";
        } else {
            echo "Erro ao excluir fotos: " . $conn->error;
        }

        // Fechar a conexão
        $conn->close();
    } else {
        echo "Parâmetro 'mac' não especificado.";
		
    }
} else {
    echo "Método de requisição inválido.";
}
?>
