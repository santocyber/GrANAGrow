<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Monitor GrANANET</title>
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
      <?php
session_start();

// Verifica se o usuário está logado
if (!isset($_SESSION['username'])) {
    // Redireciona para a página de login, ou exibe uma mensagem de erro, dependendo do seu caso
   // echo "Erro: Você não está logado.";
	//echo '<a href="https://santocyber.helioho.st">Ir para a página inicial</a>';
    header("Location: https://santocyber.helioho.st");

    exit;
}

// Armazene o nome de usuário atual em uma variável
$username = $_SESSION['username'];

try {
    $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria', 'santocyber_telemetria', 'telemetria');
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // Obtendo numLabels do localStorage
    // Defina o valor padrão de numLabels
    $numLabels = 60;

    // Verifique se o valor foi recebido do menu suspenso
    if (isset($_GET['numLabels'])) {
        $numLabels = $_GET['numLabels'];
    }

    $query = "SELECT identificador, nome, ip, mac, datalocal, horalocal, wifilocal, last_update, acoes, estado, geolocalizacao, usuarios, ping, hrliga, hrdesliga, objsaida, granaentrada, noteiroentrada, moedeiroentrada, cartaoentrada, timerautomatico FROM telemetria WHERE usuarios = :username";

    if (strpos($query, 'WHERE') !== false) {
        $query .= " ORDER BY last_update DESC"; // Ordenar por última atualização, os mais recentes primeiro
    }

    $stmt = $pdo->prepare($query);
    $stmt->bindParam(':username', $username, PDO::PARAM_STR);
    $stmt->execute();

    while ($linha = $stmt->fetch(PDO::FETCH_ASSOC)) {
echo "<div class='card'>";
echo "<table>";
echo "<tr><th>{$linha['nome']}</th></tr>";
echo "<tr><td><canvas id='grafico_" . $linha['mac'] . "'></canvas></td></tr>";

echo "<tr><td>";
echo "<div style='text-align: right; vertical-align: top;'>";
echo "<form method='post' action='apagar_dados.php' onsubmit='return confirm(\"Tem certeza que deseja apagar os dados?\")'>";
echo "<input type='hidden' name='mac' value='" . $linha['mac'] . "'>";
echo "<button class='custom-button' type='submit'>Apagar dados</button>";
echo "</form>";
echo "</div>";
echo "</td></tr>";

echo "<tr><td>";
echo "<table border='0'>";
echo "<tr><td>Identificador</td><td>{$linha['identificador']}</td></tr>";
echo "<tr><td>IP</td><td>{$linha['ip']}</td></tr>";
echo "<tr><td>MAC</td><td>{$linha['mac']}</td></tr>";
echo "<tr><td>Saidas de objeto</td><td>{$linha['objsaida']}</td></tr>";
echo "<tr><td>Entrada NOTEIRO</td><td>{$linha['noteiroentrada']}</td></tr>";
echo "<tr><td>Entrada MOEDEIRO</td><td>{$linha['moedeiroentrada']}</td></tr>";
echo "<tr><td>Entrada POS</td><td>{$linha['cartaoentrada']}</td></tr>";
echo "<tr><td>Total GRANA</td><td>{$linha['granaentrada']}</td></tr>";

echo "<tr class='highlighted-row'>";
echo "<form method='post' action='salvar_hora.php'>";
echo "<td>Hora para ligar:<br><input type='time' name='hrliga' value='" . $linha['hrliga'] . "'></td>";
echo "<td>Hora para desligar:<br><input type='time' name='hrdesliga' value='" . $linha['hrdesliga'] . "'></td>";

// Botão switch para timerautomatico
echo "<tr>";
echo "<td>";

echo "<label class='switch'>";
// Se timerautomatico for 1, mostra como "Desativado", caso contrário, mostra como "Ativado"
$timerautomaticoStatus = $linha['timerautomatico'] == 1 ? 'Desativar' : 'Ativar';
echo "<button type='submit' name='timerautomatico' value='" . ($linha['timerautomatico'] == 1 ? '0' : '1') . "'>$timerautomaticoStatus Timer Automático</button>";
echo "</label>";

// Botão "Salvar"
echo "<input type='hidden' name='mac' value='" . $linha['mac'] . "'>";
echo "</td>";
echo "<td>";
echo "</td>";
echo "</form>";
echo "</tr>";

echo "<tr><td>Data Local</td><td>{$linha['datalocal']}</td></tr>";
echo "<tr><td>Hora Local</td><td>{$linha['horalocal']}</td></tr>";
echo "<tr><td>Wi-Fi Local</td><td>{$linha['wifilocal']}</td></tr>";
echo "<tr><td>Última Atualização</td><td>{$linha['last_update']}</td></tr>";

		
		
echo "<tr>";
echo "<td>Estado: </td>";
echo "<td>";
echo "<form method='post' action='atualizar_acao.php'>";
echo "<input type='hidden' name='mac' value='{$linha['mac']}'>";
echo "<label class='switch'>";
// Se o estado for 'liga', mostra como "Desativar", caso contrário, mostra como "Ativar"
$estadoStatus = $linha['estado'] == 'liga' ? 'Desligar' : 'Ligar';
echo "<button type='submit' name='estado' value='" . ($linha['estado'] == 'liga' ? 'desliga' : 'liga') . "'>$estadoStatus</button>";
echo "</label>";
echo "</form>";
echo "</td>";
echo "</tr>";

echo "<tr>";
echo "<td>";
echo "<form method='post' action='processar_credito.php'>";
echo "<input type='hidden' name='mac' value='{$linha['mac']}'>";
echo "<label for='credito-input'>Insira o crédito:</label>";
echo "</td>";
echo "<td>";
echo "<input type='number' id='credito-input' size='5' name='credito' required>";
echo "<button type='submit'>Enviar</button>";
echo "</form>";
echo "</td></tr>";

echo "<tr><td>Geolocalização</td><td>{$linha['geolocalizacao']}</td></tr>";
echo "<tr><td>Usuário</td><td>{$linha['usuarios']}</td></tr>";
echo "<tr><td>Ping</td><td>{$linha['ping']} ms</td></tr>";
echo "</table>";
echo "</td></tr>";
echo "</table>";
echo "</div>";



        // Processamento dos dados para os gráficos
        $stmtDados = $pdo->prepare("SELECT ping, timestamp FROM dados_serializados WHERE mac = :mac ORDER BY timestamp DESC LIMIT :numLabels");
        $stmtDados->bindValue(':mac', $linha['mac']);
        $stmtDados->bindValue(':numLabels', $numLabels, PDO::PARAM_INT);

        // Execute a consulta SQL
        if ($stmtDados->execute()) {
            $dados_serializados = $stmtDados->fetchAll(PDO::FETCH_ASSOC);

            if (!empty($dados_serializados)) {
                $serializedData = json_encode($dados_serializados);

                $chartData = [
                    'labels' => array_reverse(array_column(json_decode($serializedData, true), 'timestamp')),
                    'datasets' => [
                        [
                            'label' => 'Ping (ms)',
                            'data' => array_reverse(array_column(json_decode($serializedData, true), 'ping')),
                            'backgroundColor' => 'rgba(255, 99, 132, 0.2)',
                            'borderColor' => 'rgba(255, 99, 132, 1)',
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
    echo "<option value='40320' " . ($numLabels == 40320 ? "selected" : "") . ">1 mês</option>";
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
    // Botão simples com referência para lista.php
    echo "<div class='button'>";
    echo "<a href='lista.php'><button>LISTA</button></a>";
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
