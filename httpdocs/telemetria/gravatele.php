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

// Função para verificar se os dados existem na tabela telemetria
function checkTelemetriaData($mac_value, $conn) {
    $checkSql = "SELECT * FROM telemetria WHERE mac = '$mac_value'";
    $result = $conn->query($checkSql);
    return $result->num_rows > 0;
}

// Verificar se a solicitação é um POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Capturar e sanitizar os dados da URL
    $ip = sanitizeInput($_POST['iplocal']);
    $horalocal = sanitizeInput($_POST['horalocal']);
    $datalocal = sanitizeInput($_POST['datalocal']);
    $wifilocal = sanitizeInput($_POST['wifilocal']);
    $granaentrada = sanitizeInput($_POST['granaentrada']);
    $objsaida = sanitizeInput($_POST['objsaida']);
    $ping = sanitizeInput($_POST['ping']);
    $mac_value = sanitizeInput($_POST['mac_value']);
    $nomedobot = sanitizeInput($_POST['nomedobot']);
    $geo = sanitizeInput($_POST['geo']);
    $usuarios = sanitizeInput($_POST['usuarios']);
    $last_update = date('Y-m-d H:i:s');
    $timestamp = time();

    // Verificar se a linha já existe na tabela com base no endereço MAC
    if (checkTelemetriaData($mac_value, $conn)) {
        // A linha já existe, então vamos atualizá-la
        $updateSql =  "UPDATE telemetria 
              SET ip = '$ip', wifilocal = '$wifilocal', ping = '$ping', nome = '$nomedobot', geolocalizacao = '$geo', usuarios = '$usuarios', datalocal = '$datalocal', horalocal = '$horalocal', granaentrada = '$granaentrada', objsaida = '$objsaida' WHERE mac = '$mac_value'";

        if ($conn->query($updateSql) === TRUE) {
            echo "Dados atualizados com sucesso na tabela telemetria";
        } else {
            echo "Erro ao atualizar dados na tabela telemetria: " . $conn->error . "<br>";
        }
    } else {
        // A linha não existe, então vamos inseri-la
        $insertSql = "INSERT INTO telemetria (mac, ip, wifilocal, ping, horalocal, datalocal , nome, geolocalizacao, usuarios, granaentrada, objsaida)
                      VALUES ('$mac_value', '$ip', '$wifilocal', '$ping', '$horalocal','$datalocal', '$nomedobot', '$geo', '$usuarios', '$granaentrada', '$objsaida')";

        if ($conn->query($insertSql) === TRUE) {
            echo "Dados inseridos com sucesso na tabela telemetria";
        } else {
            echo "Erro ao inserir dados na tabela telemetria: " . $conn->error . "<br>";
        }
    }

  
} else {
    echo "Apenas solicitações POST são aceitas";
}

// Fechar a conexão
$conn->close();
?>


