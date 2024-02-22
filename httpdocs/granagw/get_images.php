<?php
// Verificar se o método de requisição é GET
if ($_SERVER["REQUEST_METHOD"] == "GET") {
    // Verificar se os parâmetros esperados foram recebidos
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

        // Consulta para selecionar as últimas 240 imagens da tabela fotos_serializadas com base no MAC, ordenadas por timestamp de forma descendente
        $sql = "SELECT foto, timestamp FROM fotos_serializadas WHERE mac = '$mac' ORDER BY timestamp DESC LIMIT 50";
        $result = $conn->query($sql);

        $imagens_base64 = array();

        if ($result->num_rows > 0) {
            // Loop através dos resultados da consulta e adiciona as imagens ao array
            while ($row = $result->fetch_assoc()) {
                $imagens_base64[] = array(
                    "imagem" => base64_encode($row["foto"]),
                    "timestamp" => $row["timestamp"]
                );
            }
        }

        // Fechar a conexão
        $conn->close();
    } else {
        echo "Parâmetro 'mac' não especificado.";
        exit(); // Encerra a execução se o parâmetro 'mac' não for especificado
    }
} else {
    echo "Método de requisição inválido.";
    exit(); // Encerra a execução se o método de requisição for inválido
}

// Se houver imagens, vamos criar o GIF
if (!empty($imagens_base64)) {
    // Cria um objeto Imagick
    $imagick = new Imagick();

    // Define o formato do GIF
    $imagick->setFormat("gif");

    // Adiciona cada imagem ao objeto Imagick
    foreach ($imagens_base64 as $imagem_data) {
        // Cria uma nova imagem do blob binário
        $frame = new Imagick();
        $frame->readImageBlob(base64_decode($imagem_data["imagem"]));

        // Adiciona a marca d'água com o timestamp (data e hora)
        $draw = new ImagickDraw();
        $draw->setFillColor('black');
        $draw->setFont('arial.ttf');
        $draw->setFontSize(20);
        $draw->setGravity(Imagick::GRAVITY_SOUTHEAST);
        $draw->annotation(10, 10, $imagem_data["timestamp"]);
        $frame->drawImage($draw);

        // Adiciona o quadro ao GIF
        $imagick->addImage($frame);
    }

    // Define as configurações do GIF (tempo de duração de cada quadro)
    $imagick->setImageDelay(100); // Tempo de 100 milissegundos entre os quadros
    $imagick->setImageDispose(500); // Dispose = 2 para deixar cada quadro na tela até que o próximo seja desenhado

    // Combina todos os quadros em um único GIF animado
    $imagick = $imagick->deconstructImages();

    // Define o cabeçalho Content-Type para GIF
    header('Content-Type: image/gif');

    // Exibe o GIF animado
    echo $imagick->getImagesBlob();

} else {
    echo "Nenhuma imagem encontrada para o MAC especificado.";
}
?>
