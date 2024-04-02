<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Visualização de Fotos</title>
	    <link rel="stylesheet" href="style.css">

    <style>
        /* Estilos CSS para a organização dos elementos */
        body {
            margin: 0;
            padding: 0;
            font-family: Arial, sans-serif;
        }
        


        .fullscreen {
            display: none;
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background-color: rgba(0, 0, 0, 0.8);
            z-index: 9999;
            align-items: center;
            justify-content: center;
        }
        
        .fullscreen img {
            max-width: 90%;
            max-height: 90%;
            margin: auto;
            display: block;
        }
        
        .image-info {
            color: #fff;
            text-align: center;
            margin-bottom: 20px;
        }

        .button-wrapper {
			 position: fixed;
            top: 60px; /* Posiciona o grupo de botões no topo */
            left: 20px;   
            display: flex;
            flex-direction: column;
            z-index: 9999;
			max-height: 200px; /* Defina uma altura máxima para a div */
  

        }

        .button-wrapper button {
        margin-bottom: 10px; /* Adicione uma margem inferior para separar os botões */
        }

        .close-button {
            background-color: #f44336;
            color: white;
            border: none;
            border-radius: 50%;
            padding: 5px 10px;
            font-size: 16px;
            cursor: pointer;
            position: absolute;
            top: 5px;
            right: 5px;
        }
    </style>
</head>
<body class="dark-mode"> <!-- Aplicando o tema dark mode -->

<div class="fullscreen" onclick="closeFullscreen()">
    <div class="image-info" id="imageInfo"></div>
    <button class="close-button" onclick="closeFullscreen()">X</button>
    <img id="fullscreenImg" src="" alt="Imagem em Tela Cheia">
</div>

<div class="container">
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

    // Definindo o número de fotos por página
    $limit = 60;

    // Definindo a página atual
    $page = isset($_GET['page']) ? $_GET['page'] : 1;
    $offset = ($page - 1) * $limit;

    // Verificar se o parâmetro MAC foi enviado via GET
    if(isset($_GET['mac'])) {
        $mac = $_GET['mac'];

        // Consulta para selecionar as fotos para o MAC específico, com paginação
        $sql = "SELECT foto, timestamp FROM fotos_serializadas WHERE mac = '$mac' ORDER BY timestamp ASC LIMIT $limit OFFSET $offset";
        $result = $conn->query($sql);

        if ($result->num_rows > 0) {
            // Início do loop para processar os resultados da consulta
            $timestamps = []; // Array para armazenar os timestamps das fotos
            while ($row = $result->fetch_assoc()) {
                // Exibir a thumbnail da foto com a função de expansão para tela cheia
                $fotoData = base64_encode($row['foto']);
                $timestamp = $row['timestamp'];
                $timestamps[] = $timestamp; // Adicionar o timestamp ao array
                echo "<img src='data:image/jpeg;base64,{$fotoData}' class='thumbnail' onclick=\"openFullscreen('data:image/jpeg;base64,{$fotoData}', '$timestamp', '$mac')\" />";
            }
            // Enviar os timestamps para a página delete_current_group_photos.php
            echo "<form id='deleteGroupForm' action='delete_current_group_photos.php' method='post' style='display: none;'>";
            foreach ($timestamps as $timestamp) {
                echo "<input type='hidden' name='timestamps[]' value='$timestamp'>";
            }
            echo "<input type='hidden' name='mac' value='$mac'>";
            echo "</form>";
        } else {
            echo "Nenhuma foto encontrada.";
        }
       

    } else {
        echo "MAC não especificado.";
    }
    ?>


</div>

<!-- Botões para operações -->
<div class="button-wrapper">
	 <a href="index.php"> <button>Voltar para a Página Inicial</button></a>
    <button onclick="deletePhoto()">Excluir Foto</button>
	<?php if ($page > 1) {
            $prevPage = $page - 1;
            echo "<a href=\"verfotos.php?mac=$mac&page=1\"><button class='first-photos-button'>Voltar às Primeiras 60 Fotos</button></a>";}?>
    <button class="delete-group-button" onclick="deleteCurrentGroup()">Excluir Fotos deste Grupo</button>
    <button onclick="deleteAllPhotos()">Excluir Todas as Fotos</button>
    <button onclick="gifAllPhotos()">Salva GIF</button>
	<button onclick="startSlideshow()">Começar Slideshow</button>
	<?php
	  // Botão para ver mais 60 fotos antigas
        $nextPage = $page + 1;
        echo "<button class='navigation-button' style='left: 20px;'><a href=\"verfotos.php?mac=$mac&page=$nextPage\">Mostrar Mais 60 Fotos</a></button>";?>
</div>
	
	
	
	 <form id="deleteAllForm" action="delete_all_photos.php" method="get">
        <input type="hidden" name="mac" value="<?php echo $_GET['mac']; ?>">
    </form>	 
	<form id="gifAllForm" action="get_images.php" method="get">
        <input type="hidden" name="mac" value="<?php echo $_GET['mac']; ?>">
    </form>

	
    <form id='deleteGroupForm' action='delete_current_group_photos.php' method='post' style='display: none;'>
    <!-- Incluir aqui os campos ocultos com os timestamps -->
</form>

<script>
    var slideshowInterval; // Variável global para armazenar o intervalo do slideshow

    // Função para abrir a imagem em tela cheia
    function openFullscreen(imgSrc, timestamp, mac) {
        var fullscreenImg = document.getElementById('fullscreenImg');
        var imageInfo = document.getElementById('imageInfo');
        fullscreenImg.src = imgSrc; // Define a imagem em tela cheia com o src da miniatura clicada
        imageInfo.innerHTML = "MAC: " + mac + "<br>" + "Timestamp: " + timestamp;
        document.querySelector('.fullscreen').style.display = 'flex'; // Exibe o elemento de tela cheia
    }
	
	  var slideshowInterval; // Variável global para armazenar o intervalo do slideshow

    // Função para fechar a imagem em tela cheia
    function closeFullscreen() {
        document.querySelector('.fullscreen').style.display = 'none'; // Oculta o elemento de tela cheia
        clearInterval(slideshowInterval); // Limpa o intervalo do slideshow ao fechar a tela cheia
    }

    // Função para excluir a foto
    function deletePhoto() {
        if (confirm("Tem certeza que deseja excluir esta foto?")) {
            // Coloque aqui a lógica para excluir a foto
        }
    }

    // Função para excluir todas as fotos do grupo atual
    function deleteCurrentGroup() {
        if (confirm("Tem certeza que deseja excluir todas as fotos deste grupo?")) {
            document.getElementById('deleteGroupForm').submit(); // Enviar o formulário com os timestamps
        }
    }

    // Função para excluir todas as fotos
    function deleteAllPhotos() {
        if (confirm("Tem certeza que deseja excluir todas as fotos?")) {
            document.getElementById('deleteAllForm').submit(); // Enviar o formulário para excluir todas as fotos
        }
    } 
	
	// Função para excluir todas as fotos
    function gifAllPhotos() {
        if (confirm("Tem certeza que deseja criar o gif, santocyber vai fazer a magica?")) {
            document.getElementById('gifAllForm').submit(); // Enviar o formulário para excluir todas as fotos
        }
    }

    // Função para iniciar o slideshow das 60 fotos
    function startSlideshow() {
        // Coloque aqui a lógica para começar o slideshow
		 // Inicia o slideshow automaticamente após abrir a imagem em tela cheia
        slideshowInterval = setInterval(nextSlide, 100); // Avança para a próxima foto a cada 3 segundos
    }
	
	
	    // Função para pausar o slideshow
    function pauseSlideshow() {
        clearInterval(slideshowInterval); // Limpa o intervalo do slideshow
    }

    // Função para avançar para a próxima foto no slideshow
    function nextSlide() {
        var thumbnails = document.querySelectorAll('.thumbnail'); // Seleciona todas as miniaturas
        var currentSlide = document.querySelector('.fullscreen img').src; // Obtém a fonte da imagem atual

        // Encontra o índice da miniatura que corresponde à imagem atual
        var currentIndex;
        thumbnails.forEach(function(thumbnail, index) {
            if (thumbnail.src === currentSlide) {
                currentIndex = index;
            }
        });

        // Avança para a próxima miniatura (ou volta ao início se for a última)
        var nextIndex = (currentIndex + 1) % thumbnails.length;
        thumbnails[nextIndex].click(); // Simula o clique na próxima miniatura
    }

   
</script>

</body>
</html>
