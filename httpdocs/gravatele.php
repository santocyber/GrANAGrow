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
    $temperatura = sanitizeInput($_POST['temperatura']);
    $umidade = sanitizeInput($_POST['umidade']);
    $pressao = sanitizeInput($_POST['pressao']);
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
                      SET ip = '$ip', umidade = '$umidade', pressao = '$pressao', temperatura = '$temperatura', wifilocal = '$wifilocal', nome = '$nomedobot', geolocalizacao = '$geo',datalocal = '$datalocal', horalocal = '$horalocal' WHERE mac = '$mac_value'";

        if ($conn->query($updateSql) === TRUE) {
            echo "Dados atualizados com sucesso na tabela";
        } else {
            echo "Erro ao atualizar dados na tabela: " . $conn->error . "<br>";
        }
    } else {
        // A linha não existe, então vamos inseri-la
        $insertSql = "INSERT INTO telemetria (mac, ip , umidade, temperatura, pressao, wifilocal, horalocal, datalocal , nome, geolocalizacao)
                      VALUES ('$mac_value', '$ip', '$umidade', '$temperatura', '$pressao', '$wifilocal', '$horalocal','$datalocal', '$nomedobot', '$geo')";

        if ($conn->query($insertSql) === TRUE) {
            echo "Dados inseridos com sucesso na tabela";
        } else {
            echo "Erro ao inserir dados na tabela: " . $conn->error . "<br>";
        }
    }

    // Capturar o timestamp atual
    $timestamp = time();

    // Selecionar os dados serializados existentes da linha
    $selectDadosSerializadosSql = "SELECT dados_serializados FROM telemetria WHERE mac = '$mac_value'";
    $resultDadosSerializados = $conn->query($selectDadosSerializadosSql);
    
    if ($resultDadosSerializados->num_rows > 0) {
        // Recuperar os dados serializados existentes
        $row = $resultDadosSerializados->fetch_assoc();
        $dados_serializados_existente = json_decode($row['dados_serializados'], true);

        // Adicionar novos valores aos dados serializados
        $dados_serializados_existente[] = array(
            'temperatura' => $temperatura,
            'umidade' => $umidade,
            'pressao' => $pressao,
            'timestamp' => $timestamp
        );

        // Serializar os dados atualizados
        $dados_serializados_atualizados = json_encode($dados_serializados_existente);

        // Atualizar os dados serializados na linha
        $updateDadosSerializadosSql = "UPDATE telemetria SET dados_serializados = '$dados_serializados_atualizados' WHERE mac = '$mac_value'";
        if ($conn->query($updateDadosSerializadosSql) === TRUE) {
            echo "Dados serializados atualizados com sucesso na tabela";
        } else {
            echo "Erro ao atualizar dados serializados na tabela: " . $conn->error . "<br>";
        }
    } else {
        // Se não houver dados serializados, criar uma nova série de dados
        $dados_serializados_novos = array();
        $dados_serializados_novos[] = array(
            'temperatura' => $temperatura,
            'umidade' => $umidade,
            'pressao' => $pressao,
            'timestamp' => $timestamp
        );
        $dados_serializados_novos_json = json_encode($dados_serializados_novos);

        // Inserir os dados serializados na linha
        $insertDadosSerializadosSql = "INSERT INTO telemetria (mac, dados_serializados) VALUES ('$mac_value', '$dados_serializados_novos_json')";
        if ($conn->query($insertDadosSerializadosSql) === TRUE) {
            echo "Nova série de dados serializados inserida com sucesso na tabela";
        } else {
            echo "Erro ao inserir nova série de dados serializados na tabela: " . $conn->error . "<br>";
        }
    }
} else {
    echo "Apenas solicitações POST são aceitas";
}

// Fechar a conexão
$conn->close();
?>
