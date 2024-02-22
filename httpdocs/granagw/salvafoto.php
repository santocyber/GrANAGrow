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

// Verificar se o parâmetro 'mac' está presente na URL
if (isset($_GET['mac'])) {
    // Capturar e sanitizar o MAC address da URL
    $mac = sanitizeInput($_GET['mac']);

    // Verificar se os dados binários da imagem estão presentes no corpo da solicitação POST
    $imageData = file_get_contents('php://input');

    if ($imageData !== false) {
        // Preparar a instrução SQL para inserir os dados na tabela dados_serializados
        $insertSql = "INSERT INTO fotos_serializadas (mac, timestamp, foto) VALUES (?, NOW(), ?)";
        $stmt = $conn->prepare($insertSql);
        
        // Vincular parâmetros e executar a instrução preparada
        $stmt->bind_param("ss", $mac, $imageData);

        if ($stmt->execute()) {
            echo "Imagem adicionada com sucesso na tabela fotos_serializadas para o MAC $mac.";
        } else {
            echo "Erro ao adicionar a imagem na tabela dados_serializados: " . $stmt->error;
        }
    } else {
        echo "Dados binários da imagem não foram fornecidos na solicitação POST.";
    }
} else {
    echo "Parâmetro 'mac' não foi fornecido na URL.";
}

// Fechar a conexão
$conn->close();

function sanitizeInput($input) {
    return htmlspecialchars(stripslashes(trim($input)));
}
?>
