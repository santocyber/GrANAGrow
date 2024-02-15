<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>GrANAGrow</title>
    <meta http-equiv="refresh" content="600"> <!-- Adicionando o refresh automático -->
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
	   <!-- JavaScript para gerar os gráficos -->
    <script>
       function generateChart(canvasId, data) {
        var ctx = document.getElementById(canvasId).getContext('2d');
        var myChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: data.labels.map(function(timestamp) {
                    var date = new Date(timestamp * 1000); // Multiplicando por 1000 para converter de segundos para milissegundos
                    var formattedDate = date.toLocaleString('pt-BR', { day: '2-digit', month: 'long', year: 'numeric', hour: 'numeric', minute: 'numeric' });
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

        function toggleDarkMode() {
            var body = document.body;
            var darkModeToggle = document.getElementById("dark-mode-toggle");
            var darkModeLabel = document.getElementById("dark-mode-label");

            if (body.classList.contains("dark-mode")) {
                body.classList.remove("dark-mode");
                darkModeLabel.textContent = "Dark Mode";
                darkModeToggle.checked = false;
            } else {
                body.classList.add("dark-mode");
                darkModeLabel.textContent = "Light Mode";
                darkModeToggle.checked = true;
            }
        }
    </script>

  <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 0;
            background-color: #f4f4f4;
            color: #333;
            transition: background-color 0.3s, color 0.3s;
        }
        .container {
            padding: 20px;
            max-width: 800px;
            margin: 0 auto;
        }
        h1 {
            text-align: center;
            margin-top: 20px;
            margin-bottom: 30px;
        }
        .search-container {
            margin-bottom: 20px;
            text-align: center;
        }
        .search-container form {
            display: inline-block;
        }
        .search-container input[type="text"] {
            width: calc(100% - 100px);
            padding: 8px;
            border-radius: 5px;
            border: 1px solid #ddd;
            box-sizing: border-box;
            font-size: 16px;
        }
        button {
            padding: 10px 15px;
            background-color: #007BFF;
            color: #fff;
            border: none;
            cursor: pointer;
            border-radius: 5px;
            transition: background-color 0.3s;
            font-size: 16px;
        }
        button:hover {
            background-color: #0056b3;
        }
        .card {
            border: 1px solid #ddd;
            border-radius: 10px;
            margin-bottom: 20px;
            padding: 20px;
            background-color: #fff;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-bottom: 20px;
        }
        th, td {
            padding: 10px;
            border-bottom: 1px solid #ddd;
        }
        th {
            background-color: #f2f2f2;
        }
        .dark-mode {
            background-color: #222;
            color: #fff;
        }
        .dark-mode .card, .dark-mode th, .dark-mode td {
            background-color: #333;
            color: #fff;
            border-color: #555;
        }
        .dark-mode button {
            background-color: #0056b3;
        }
        .dark-mode button:hover {
            background-color: #007BFF;
        }
        .switch {
            text-align: center;
            margin-top: 20px;
        }
        .switch input { 
            display: none;
        }
        .slider {
            position: relative;
            display: inline-block;
            width: 60px;
            height: 34px;
        }
        .slider:before {
            position: absolute;
            content: "";
            height: 26px;
            width: 26px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
        }
        input:checked + .slider {
            background-color: #2196F3;
        }
        input:checked + .slider:before {
            transform: translateX(26px);
        }
        .slider.round {
            border-radius: 34px;
        }
        .slider.round:before {
            border-radius: 50%;
        }
        canvas {
            max-width: 100%;
            height: auto;
        }
        .action-input {
            width: 100px;
            padding: 8px;
            border-radius: 5px;
            border: 1px solid #ddd;
            box-sizing: border-box;
            font-size: 16px;
            margin-left: 10px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Monitor GrANAGrow</h1>

        <?php
        try {
            $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria', 'santocyber_telemetria', 'telemetria');
            $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            $query = "SELECT identificador, nome, ip, temperatura, umidade, pressao, mac, datalocal, horalocal, wifilocal, last_update, acoes, parceiros, geolocalizacao, credito, dados_serializados FROM telemetria";

            if ($_SERVER['REQUEST_METHOD'] === 'GET') {
                if (isset($_GET['searchTerm'])) {
                    $searchTerm = $_GET['searchTerm'];
                    $query .= " WHERE mac LIKE :searchTerm OR nome LIKE :searchTerm OR wifilocal LIKE :searchTerm OR identificador LIKE :searchTerm OR acoes LIKE :searchTerm OR parceiros LIKE :searchTerm";
                } elseif (isset($_GET['listAll'])) {
                    // Se a opção de listar todas for selecionada, não é necessário modificar a consulta
                }
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
                echo "<tr><td></td></tr>"; 
                echo "<tr><td><input type='text' class='action-input' placeholder='prompt'><br><button type='submit'>Executar Ação</button></td></tr>";
                echo "<tr><td>Nome: {$linha['nome']}</td></tr>";
                echo "<tr><td>Temperatura: {$linha['temperatura']}</td></tr>";
                echo "<tr><td>Umidade: {$linha['umidade']}</td></tr>";
                echo "<tr><td>Pressão: {$linha['pressao']}</td></tr>";
                echo "<tr><td>Endereço MAC: {$linha['mac']}</td></tr>";
                echo "<tr><td>IP local: {$linha['ip']}</td></tr>";
                echo "<tr><td>WiFilocal: {$linha['wifilocal']}</td></tr>";
                echo "<tr><td>Hora no local: {$linha['horalocal']}</td></tr>";
                echo "<tr><td>Dia no local: {$linha['datalocal']}</td></tr>";
                echo "<tr><td>Última Atualização: {$linha['last_update']}</td></tr>";
                echo "<tr><td>Geolocalização: {$linha['geolocalizacao']}</td></tr>";
                echo "</table>";
                echo "</div>";

                // Processamento dos dados para os gráficos
                $dados_serializados = json_decode($linha['dados_serializados'], true);
                if (is_array($dados_serializados)) {
                    $chartData = [
                        'mac' => $linha['mac'],
                        'labels' => array_column($dados_serializados, 'timestamp'),
                        'datasets' => [
                            [
                                'label' => 'Temperatura (°C)',
                                'data' => array_column($dados_serializados, 'temperatura'),
                                'backgroundColor' => 'rgba(255, 99, 132, 0.2)',
                                'borderColor' => 'rgba(255, 99, 132, 1)',
                                'borderWidth' => 1
                            ],
                            [
                                'label' => 'Umidade (%)',
                                'data' => array_column($dados_serializados, 'umidade'),
                                'backgroundColor' => 'rgba(54, 162, 235, 0.2)',
                                'borderColor' => 'rgba(54, 162, 235, 1)',
                                'borderWidth' => 1
                            ],
                            [
                                'label' => 'Pressão (hPa)',
                                'data' => array_column($dados_serializados, 'pressao'),
                                'backgroundColor' => 'rgba(255, 206, 86, 0.2)',
                                'borderColor' => 'rgba(255, 206, 86, 1)',
                                'borderWidth' => 1
                            ]
                        ]
                    ];

                    // Gere o gráfico para este conjunto de dados
                    echo "<script>generateChart('grafico_" . $linha['mac'] . "', " . json_encode($chartData) . ");</script>";
                }
            }
            echo "<div class='card'>";
            echo "<table>";
            echo "<tr><th>by SantoCyber</th></tr>";
            echo "</table>";
            echo "</div>";
        } catch (PDOException $e) {
            echo "Erro: " . $e->getMessage();
        }
        ?>
    </div>

 

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
    
    <!-- Toggle para Dark Mode -->
	
	<!-- Toggle para Dark Mode -->
<div class="switch">
    <label class="switch" for="dark-mode-toggle">
        <span id="dark-mode-label">Dark Mode</span>
        <input type="checkbox" id="dark-mode-toggle" onclick="toggleDarkMode()">
        <span class="slider round"></span>
    </label>
</div>

    </div>
</body>
</html>
