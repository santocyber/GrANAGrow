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

// Função para validar e sanitizar os dados
function sanitizeInput($input) {
    return htmlspecialchars(stripslashes(trim($input)));
}

// Verificar se a solicitação é um POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Capturar e sanitizar os dados da URL
    $granaentrada = sanitizeInput($_POST['granaentrada']);
    $objsaida = sanitizeInput($_POST['objsaida']);
    $ip = sanitizeInput($_POST['iplocal']);
    $horalocal = sanitizeInput($_POST['horalocal']);
    $datalocal = sanitizeInput($_POST['datalocal']);
    $wifilocal = sanitizeInput($_POST['wifilocal']);
    $mac_value = sanitizeInput($_POST['mac_value']);
	$nomedobot = sanitizeInput($_POST['nomedobot']);
	$geo = sanitizeInput($_POST['geo']);
    $last_update = date('Y-m-d H:i:s');

    // Verificar se a linha já existe na tabela com base no endereço MAC
    $checkSql = "SELECT * FROM telemetria WHERE mac = '$mac_value'";
    $result = $conn->query($checkSql);

    if ($result->num_rows > 0) {
        // A linha já existe, então vamos atualizá-la
        $updateSql =  "UPDATE telemetria 
                      SET ip = '$ip', objsaida = '$objsaida', granaentrada = '$granaentrada', wifilocal = '$wifilocal', nome = '$nomedobot', geolocalizacao = '$geo',datalocal = '$datalocal', horalocal = '$horalocal' WHERE mac = '$mac_value'";

        if ($conn->query($updateSql) === TRUE) {
            echo "Dados atualizados com sucesso na tabela";
        } else {
            echo "Erro ao atualizar dados na tabela: " . $conn->error . "<br>";
        }
    } else {
        // A linha não existe, então vamos inseri-la
        $insertSql = "INSERT INTO telemetria (mac, ip , objsaida, granaentrada, wifilocal, horalocal, datalocal , nome, geolocalizacao)
                      VALUES ('$mac_value', '$ip', '$objsaida', '$granaentrada', '$wifilocal', '$horalocal','$datalocal', '$nomedobot', '$geo')";

        if ($conn->query($insertSql) === TRUE) {
            echo "Dados inseridos com sucesso na tabela";
        } else {
            echo "Erro ao inserir dados na tabela: " . $conn->error . "<br>";
        }
    }
} else {
    echo "Apenas solicitações POST são aceitas";
}

// Fechar a conexão
$conn->close();
?>
