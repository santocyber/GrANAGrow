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
function checkTelemetriaData($mac, $conn) {
    $checkSql = "SELECT * FROM telemetria WHERE mac = '$mac'";
    $result = $conn->query($checkSql);
    return $result->num_rows > 0;
}

// Função para adicionar dados na tabela dados_serializados
function addSerializedData($mac, $temperatura, $umidade, $pressao, $ph, $timestamp, $conn) {
    $insertSql = "INSERT INTO dados_serializados (mac, temperatura, umidade, pressao, ph, timestamp) 
                  VALUES ('$mac', '$temperatura', '$umidade', '$pressao', '$ph', '$timestamp')";
    
    if ($conn->query($insertSql) === TRUE) {
        echo "Dados inseridos com sucesso na tabela dados_serializados";
    } else {
        echo "Erro ao inserir dados na tabela dados_serializados: " . $conn->error . "<br>";
    }
}

// Verificar se a solicitação é um POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Capturar e sanitizar os dados da URL
    $temperatura = sanitizeInput($_POST['temperatura']);
    $umidade = sanitizeInput($_POST['umidade']);
    $pressao = sanitizeInput($_POST['pressao']);
    $ph = sanitizeInput($_POST['ph']); // Adicionando o campo "ph"
    $ip = sanitizeInput($_POST['iplocal']);
    $horalocal = sanitizeInput($_POST['horalocal']);
    $datalocal = sanitizeInput($_POST['datalocal']);
    $wifilocal = sanitizeInput($_POST['wifilocal']);
    $ping = sanitizeInput($_POST['ping']);
    $mac = sanitizeInput($_POST['mac']);
    $nomedobot = sanitizeInput($_POST['nomedobot']);
    $geo = sanitizeInput($_POST['geo']);
    $usuarios = sanitizeInput($_POST['usuarios']);
    $last_update = date('Y-m-d H:i:s');
    $timestamp = time();

    // Verificar se a linha já existe na tabela com base no endereço MAC
    if (checkTelemetriaData($mac, $conn)) {
        // A linha já existe, então vamos atualizá-la
        $updateSql =  "UPDATE telemetria 
                      SET ip = '$ip', umidade = '$umidade', pressao = '$pressao', temperatura = '$temperatura', ph = '$ph', wifilocal = '$wifilocal', ping = '$ping', nome = '$nomedobot', geolocalizacao = '$geo', usuarios = '$usuarios',datalocal = '$datalocal', horalocal = '$horalocal' WHERE mac = '$mac'";

        if ($conn->query($updateSql) === TRUE) {
            echo "Dados atualizados com sucesso na tabela telemetria";
        } else {
            echo "Erro ao atualizar dados na tabela telemetria: " . $conn->error . "<br>";
        }
    } else {
        // A linha não existe, então vamos inseri-la
        $insertSql = "INSERT INTO telemetria (mac, ip , umidade, temperatura, pressao, ph, wifilocal, ping, horalocal, datalocal , nome, geolocalizacao, usuarios)
                      VALUES ('$mac', '$ip', '$umidade', '$temperatura', '$pressao', '$ph', '$wifilocal', '$ping', '$horalocal','$datalocal', '$nomedobot', '$geo', '$usuarios')";

        if ($conn->query($insertSql) === TRUE) {
            echo "Dados inseridos com sucesso na tabela telemetria";
        } else {
            echo "Erro ao inserir dados na tabela telemetria: " . $conn->error . "<br>";
        }
    }

    // Adicionar os dados à tabela dados_serializados
    addSerializedData($mac, $temperatura, $umidade, $pressao, $ph, $timestamp, $conn);

} else {
    echo "Apenas solicitações POST são aceitas";
}

// Fechar a conexão
$conn->close();
?>
