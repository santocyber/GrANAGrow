<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Monitor GrANAGrow</title>
    <meta http-equiv="refresh" content="600"> <!-- Adicionando o refresh automático -->
    <link rel="stylesheet" href="style.css">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
	
	
	<script>
		
		

				

		
		
		
		
		
		// Função para salvar o valor selecionado do menu suspenso e o estado do modo escuro no localStorage
        function saveSettings() {
            var numLabelsSelect = document.getElementById('num-labels-settings');
            var selectedValue = numLabelsSelect.value;
            localStorage.setItem('numLabels', selectedValue);

            var darkModeToggle = document.getElementById('dark-mode-toggle');
            var darkModeState = darkModeToggle.checked;
            localStorage.setItem('darkMode', darkModeState);

     
        }
		
		
		


        // Função para alternar o modo escuro e salvar o estado
        function toggleDarkMode() {
            var darkModeToggle = document.getElementById('dark-mode-toggle');
            if (darkModeToggle.checked) {
                document.body.classList.add('dark-mode');
            } else {
                document.body.classList.remove('dark-mode');
            }
            saveSettings();
        }
		
		
		
		// Função para recuperar o valor salvo de numLabels e o estado do modo escuro de localStorage
function retrieveSettings() {
   
    var darkModeState = localStorage.getItem('darkMode');
    if (darkModeState === 'true') {
        document.getElementById('dark-mode-toggle').checked = true;
        toggleDarkMode(); // Chama a função para aplicar o modo escuro se estiver ativado
    }

    // Após recuperar as configurações, gere os gráficos
    // generateCharts();
}

function changeLabels() {
    // Redirecione para a mesma página com o novo valor de numLabels
    window.location.href = '<?php echo $_SERVER['PHP_SELF']; ?>?numLabels=' + document.getElementById('num-labels-settings').value;
}		
		
		
		
		
		
		
		

        // Função para gerar gráficos
        function generateChart(canvasId, data, numLabels) {
            var ctx = document.getElementById(canvasId).getContext('2d');
            var myChart = new Chart(ctx, {
                type: 'line',
                data: {
                    labels: data.labels.map(function(timestamp) {
                        var date = new Date(timestamp * 1000); // Multiplicando por 1000 para converter de segundos para milissegundos
                        var formattedDate = date.toLocaleString('pt-BR', { day: '2-digit', month: 'numeric', year: 'numeric', hour: 'numeric', minute: 'numeric' });
                        return formattedDate;
                    }),
                    datasets: data.datasets.map(function(dataset) {
                        return {
                            label: dataset.label,
                            data: dataset.data,
                            backgroundColor: dataset.backgroundColor,
                            borderColor: dataset.borderColor,
                            borderWidth: dataset.borderWidth
                        };
                    })
                },
                options: {
                    scales: {
                        y: {
                            beginAtZero: true
                        }
                    }
                }
            });
        }
    </script>
</head>
<body onload="retrieveSettings()">
    <div class="container">
        <h1>Monitor GrANAGrow</h1>
	

<?php
try {
    $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria', 'santocyber_telemetria', 'telemetria');
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // Obtendo numLabels do localStorage
       // Defina o valor padrão de numLabels
    $numLabels = 60;

    // Verifique se o valor foi recebido do menu suspenso
    if(isset($_GET['numLabels'])) {
        $numLabels = $_GET['numLabels'];
    }
	
	$query = "SELECT identificador, nome, ip, temperatura, umidade, pressao, mac, datalocal, horalocal, wifilocal, last_update, acoes, geolocalizacao, usuarios FROM telemetria";

    if ($_SERVER['REQUEST_METHOD'] === 'GET') {
        if (isset($_GET['searchTerm'])) {
            $searchTerm = $_GET['searchTerm'];
            $query .= " WHERE mac LIKE :searchTerm OR nome LIKE :searchTerm OR wifilocal LIKE :searchTerm OR identificador LIKE :searchTerm OR acoes LIKE :searchTerm";
        } elseif (isset($_GET['listAll'])) {
            // Se a opção de listar todas for selecionada, não é necessário modificar a consulta
        }
    }

    // Verificar se a cláusula WHERE está presente antes de adicionar a cláusula ORDER BY
    if (strpos($query, 'WHERE') !== false) {
        // Adicionando a cláusula ORDER BY para ordenar os resultados pelo campo identificador
        $query .= " ORDER BY identificador ASC";
    }

    $stmt = $pdo->prepare($query);

    if (isset($searchTerm)) {
        $stmt->bindValue(':searchTerm', '%' . $searchTerm . '%', PDO::PARAM_STR);
    }

    $stmt->execute();

    while ($linha = $stmt->fetch(PDO::FETCH_ASSOC)) {
        echo "<div class='card'>";
        echo "<table>";
        echo "<tr><th>{$linha['nome']}</th></tr>";
        echo "<tr><td><canvas id='grafico_" . $linha['mac'] . "'></canvas></td></tr>";
        echo "<tr><td><a href='verfotos.php?mac=" . $linha['mac'] . "'><button>Ver Fotos</button></a></td></tr>";
        echo "<tr><td>";
        echo "<table border='0'>";
        echo "<tr><td>Identificador</td><td>{$linha['identificador']}</td></tr>";
        echo "<tr><td>IP</td><td>{$linha['ip']}</td></tr>";
        echo "<tr><td>Temperatura</td><td>{$linha['temperatura']}</td></tr>";
        echo "<tr><td>Umidade</td><td>{$linha['umidade']}</td></tr>";
        echo "<tr><td>Pressão</td><td>{$linha['pressao']}</td></tr>";
        echo "<tr><td>MAC</td><td>{$linha['mac']}</td></tr>";
        echo "<tr><td>Data Local</td><td>{$linha['datalocal']}</td></tr>";
        echo "<tr><td>Hora Local</td><td>{$linha['horalocal']}</td></tr>";
        echo "<tr><td>Wi-Fi Local</td><td>{$linha['wifilocal']}</td></tr>";
        echo "<tr><td>Última Atualização</td><td>{$linha['last_update']}</td></tr>";
        echo "<tr><td>Ações</td><td>{$linha['acoes']}</td></tr>";
        echo "<tr><td>Geolocalização</td><td>{$linha['geolocalizacao']}</td></tr>";
        echo "<tr><td>Usuário</td><td>{$linha['usuarios']}</td></tr>";
        echo "</table>";
        echo "</td></tr>";
        echo "</table>";
        echo "</div>";

        // Processamento dos dados para os gráficos
        $stmtDados = $pdo->prepare("SELECT temperatura, umidade, pressao , ph, timestamp FROM dados_serializados WHERE mac = :mac ORDER BY timestamp DESC LIMIT :numLabels");
        $stmtDados->bindValue(':mac', $linha['mac']);
        $stmtDados->bindValue(':numLabels', $numLabels, PDO::PARAM_INT);

        // Execute a consulta SQL
        if ($stmtDados->execute()) {
            $dados_serializados = $stmtDados->fetchAll(PDO::FETCH_ASSOC);

            if (!empty($dados_serializados)) {
                $serializedData = json_encode($dados_serializados);

                $chartData = [
                    'labels' => array_column(json_decode($serializedData, true), 'timestamp'),
                    'datasets' => [
                        [
                            'label' => 'Temperatura (°C)',
                            'data' => array_column(json_decode($serializedData, true), 'temperatura'),
                            'backgroundColor' => 'rgba(255, 99, 132, 0.2)',
                            'borderColor' => 'rgba(255, 99, 132, 1)',
                            'borderWidth' => 1
                        ],
                        [
                            'label' => 'Umidade (%)',
                            'data' => array_column(json_decode($serializedData, true), 'umidade'),
                            'backgroundColor' => 'rgba(54, 162, 235, 0.2)',
                            'borderColor' => 'rgba(54, 162, 235, 1)',
                            'borderWidth' => 1
                        ],
                        [
                            'label' => 'Pressão (hPa)',
                            'data' => array_column(json_decode($serializedData, true), 'pressao'),
                            'backgroundColor' => 'rgba(255, 206, 86, 0.2)',
                            'borderColor' => 'rgba(255, 206, 86, 1)',
                            'borderWidth' => 1
                        ],
                        [
                            'label' => 'pH',
                            'data' => array_column(json_decode($serializedData, true), 'ph'),
                            'backgroundColor' => 'rgba(75, 192, 192, 0.2)',
                            'borderColor' => 'rgba(75, 192, 192, 1)',
                            'borderWidth' => 1
                        ]
                    ]
                ];
                echo "<script>generateChart('grafico_" . $linha['mac'] . "', " . json_encode($chartData) . ");</script>";
            }
        }
    }
  // Card de configurações
    echo "<div class='card'>";
    echo "<h2>Configurações</h2>";
    echo "<div>";
    echo "<label for='num-labels-settings'>Número de Rótulos:</label>";
    echo "<select id='num-labels-settings' onchange='changeLabels()'>";
    echo "<option value='60' " . ($numLabels == 60 ? "selected" : "") . ">1 hora</option>";
    echo "<option value='1440' " . ($numLabels == 1440 ? "selected" : "") . ">1 dia</option>";
    echo "<option value='10080' " . ($numLabels == 10080 ? "selected" : "") . ">1 semana</option>";
    echo "<option value='40320' " . ($numLabels == 40320 ? "selected" : "") . ">1 mes</option>";
    echo "<option value='525600' " . ($numLabels == 525600 ? "selected" : "") . ">1 ano</option>";
    echo "</select>";
    echo "</div>";
	
	
	
	
    // Botão Dark Mode Switch
    echo "<label class='switch' for='dark-mode-toggle'>";
    echo "<span id='dark-mode-label'>Dark Mode</span>";
    echo "<input type='checkbox' id='dark-mode-toggle' onclick='toggleDarkMode()'>";
    echo "<span class='slider round'></span>";
    echo "</label>";
    echo "</div>";

} catch (PDOException $e) {
    echo "Erro: " . $e->getMessage();
}
?>


		
		
		<!-- Formulário de busca -->
        <div class="container">
            <br>
            <div class="search-container">
                <form method="get">
                    <input type="text" id="searchTerm" name="searchTerm" placeholder="Buscar por Identificador, MAC, Nome, Wi-Fi Local, Ações ou Parceiros" required>
                    <button type="submit">Buscar</button>
                </form>
                <form method="get">
                    <input type="hidden" name="listAll" value="1">
                    <button type="submit">Listar Tudo</button>
                </form>
            </div>
        </div>
    </div>
</body>
</html>
