<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Excluir Foto</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>

<?php
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

// Verificar se o método da solicitação é POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Verificar se os parâmetros necessários foram recebidos
    if(isset($_POST['mac']) && isset($_POST['timestamp'])) {
        $mac = $_POST['mac'];
        $timestamp = $_POST['timestamp'];

        // Consulta para excluir a foto do banco de dados
        $sql = "DELETE FROM fotos_serializadas WHERE mac = '$mac' AND timestamp = '$timestamp'";
        
        if ($conn->query($sql) === TRUE) {
            echo "Foto excluída com sucesso.";
        } else {
            echo "Erro ao excluir foto: " . $conn->error;
        }
    } else {
        echo "Parâmetros insuficientes.";
    }
}

// Fechar a conexão
$conn->close();
?>

<br>
<a href="index.php"><button>Voltar à Página Inicial</button></a>

</body>
</html>
