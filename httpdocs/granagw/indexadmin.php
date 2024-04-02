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

<?php
session_start();

// Verifica se o usuário está logado
if(!isset($_SESSION['username'])) {
    // Redireciona para a página de login, ou exibe uma mensagem de erro, dependendo do seu caso
 //   echo "Erro: Você não está logado.";
//    echo '<a href="https://santocyber.helioho.st">Ir para a página inicial</a>';
    header("Location: https://santocyber.helioho.st");

    exit;
    exit;
}
 ?>
	
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
    
    $query = "SELECT identificador, nome, ip, temperatura, umidade, pressao, ph, mac, datalocal, horalocal, wifilocal, last_update, acoes, geolocalizacao, usuarios, ping, estado, mensagem, mensagemstatus, timerfoto, timerfotostatus, hrliga, hrdesliga, timerautomatico FROM telemetria";

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
		
   echo "<tr><td>";

// Botão "Ver Fotos" alinhado à esquerda
echo "<a href='verfotos.php?mac=" . $linha['mac'] . "'><button>Ver Fotos</button></a>";

// Div para os botões alinhados à direita
echo "<div style='text-align: right; vertical-align: top;'>";

// Formulário para apagar dados
echo "<form method='post' action='apagar_dados.php' onsubmit='return confirm(\"Tem certeza que deseja apagar os dados?\")'>";
echo "<input type='hidden' name='mac' value='" . $linha['mac'] . "'>";
echo "<button class='custom-button' type='submit'>Apagar dados do grafico</button>";
echo "</form>";

echo "</div>"; // Fechar a div
		echo "<div style='text-align: right; vertical-align: top;'>";

		// Formulário para apagar dados
echo "<form method='post' action='apagar_mac.php' onsubmit='return confirm(\"Tem certeza que deseja apagar o dispositivo?\")'>";
echo "<input type='hidden' name='identificador' value='" . $linha['identificador'] . "'>";
echo "<button class='custom-button' type='submit'>Apagar dispositivo</button>";
echo "</form>";

echo "</div>"; // Fechar a div

echo "</td></tr>";
		

		
        echo "<tr><td>";
        echo "<table border='0'>";
        echo "<tr><td>Identificador</td><td>{$linha['identificador']}</td></tr>";
        echo "<tr><td>IP</td><td>{$linha['ip']}</td></tr>";
        echo "<tr><td>Temperatura</td><td>{$linha['temperatura']}</td></tr>";
        echo "<tr><td>Umidade</td><td>{$linha['umidade']}</td></tr>";
        echo "<tr><td>Pressão</td><td>{$linha['pressao']}</td></tr>";
        echo "<tr><td>Fator de Hidrogenio Ionico</td><td>{$linha['ph']}</td></tr>";
        echo "<tr><td>MAC</td><td>{$linha['mac']}</td></tr>";
        echo "<tr><td>Data Local</td><td>{$linha['datalocal']}</td></tr>";
        echo "<tr><td>Hora Local</td><td>{$linha['horalocal']}</td></tr>";
		
		
		
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
		
		
		
        echo "<tr><td>Wi-Fi Local</td><td>{$linha['wifilocal']}</td></tr>";
        echo "<tr><td>Última Atualização</td><td>{$linha['last_update']}</td></tr>";
        echo "<tr><td>Ações</td><td>{$linha['acoes']}</td></tr>";
		
		
		echo "<tr>";
echo "<td>Estado luz: {$linha['estado']}</td>";
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
echo "<form method='post' action='processar_timerfoto.php'>";
echo "<input type='hidden' name='mac' value='{$linha['mac']}'>";
echo "Temporizador foto em minutos:";
echo "</td>";
echo "<td>";
echo "<input type='number' size='8' value='{$linha['timerfoto']}' name='timerfoto' required min='1'>";

		
		// Botão switch para timerautomatico
echo "<tr>";
echo "<td>";

echo "<label class='switch'>";
// Se timerautomatico for 1, mostra como "Desativado", caso contrário, mostra como "Ativado"
$timerfotostatus = $linha['timerfotostatus'] == 1 ? 'Desativar' : 'Ativar';
echo "<button type='submit' name='timerfotostatus' value='" . ($linha['timerfotostatus'] == 1 ? '0' : '1') . "'>$timerfotostatus Timer Foto</button>";
echo "</label>";

// Botão "Salvar"
echo "</td>";
echo "<td>";
echo "</td>";
echo "</form>";
echo "</tr>";
		

		
		
		
        echo "<tr><td>Geolocalização</td><td>{$linha['geolocalizacao']}</td></tr>";
        echo "<tr><td>Usuário</td><td>{$linha['usuarios']}</td></tr>";
        echo "<tr><td>Ping</td><td>{$linha['ping']} ms</td></tr>";
				
echo "<tr>";
echo "<td>";
echo "<form method='post' action='processar_mensagem.php'>";
echo "<input type='hidden' name='mac' value='{$linha['mac']}'>";
echo "TELA:";
echo "<textarea rows='4' cols='50' name='mensagem' required>{$linha['mensagem']}</textarea>";
echo "</td>";
echo "<td>";

// Início do contêiner para os botões
echo "<div class='button-container'>";

// Botão "Ativar/Desativar Scrolling"
echo "<form method='post' action='processar_mensagem.php'>";
echo "<input type='hidden' name='mac' value='{$linha['mac']}'>";
echo "<label class='switch'>";
$mensagemstatus = $linha['mensagemstatus'] == 1 ? 'Desativar' : 'Ativar';
echo "<button type='submit' name='mensagemstatus' value='" . ($linha['mensagemstatus'] == 1 ? '0' : '1') . "'>$mensagemstatus Scrolling</button>";
echo "</label>";
echo "</form>";

// Botão "Ativar/Desativar HORA"
echo "<form method='post' action='processar_mensagem3.php'>";
echo "<input type='hidden' name='mac' value='{$linha['mac']}'>";
echo "<label class='switch'>";
$mensagemstatus = $linha['mensagemstatus'] == 3 ? 'Desativar' : 'Ativar';
echo "<button type='submit' name='mensagemstatus' value='" . ($linha['mensagemstatus'] == 3 ? '0' : '3') . "'>$mensagemstatus HORA</button>";
echo "</label>";
echo "</form>";

// Botão "Ativar/Desativar CLIMA"
echo "<form method='post' action='processar_mensagem4.php'>";
echo "<input type='hidden' name='mac' value='{$linha['mac']}'>";
echo "<label class='switch'>";
$mensagemstatus = $linha['mensagemstatus'] == 4 ? 'Desativar' : 'Ativar';
echo "<button type='submit' name='mensagemstatus' value='" . ($linha['mensagemstatus'] == 4 ? '0' : '4') . "'>$mensagemstatus CLIMA</button>";
echo "</label>";
echo "</form>";

// Botão "Ativar/Desativar ANALOGICO"
echo "<form method='post' action='processar_mensagem2.php'>";
echo "<input type='hidden' name='mac' value='{$linha['mac']}'>";
echo "<label class='switch'>";
$mensagemstatus = $linha['mensagemstatus'] == 2 ? 'Desativar' : 'Ativar';
echo "<button type='submit' name='mensagemstatus' value='" . ($linha['mensagemstatus'] == 2 ? '0' : '2') . "'>$mensagemstatus ANALOGICO</button>";
echo "</label>";
echo "</form>";

// Botão "Ativar/Desativar MENSAGEM"
echo "<form method='post' action='processar_mensagem5.php'>";
echo "<input type='hidden' name='mac' value='{$linha['mac']}'>";
echo "<label class='switch'>";
$mensagemstatus = $linha['mensagemstatus'] == 5 ? 'Desativar' : 'Ativar';
echo "<button type='submit' name='mensagemstatus' value='" . ($linha['mensagemstatus'] == 5 ? '0' : '5') . "'>$mensagemstatus MENSAGEM</button>";
echo "</label>";
echo "</form>";

// Fim do contêiner para os botões
echo "</div>";

echo "</td>";
echo "<td>";
echo "</td>";
echo "</form>";
echo "</tr>";

		
		
		
		
		
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
				
				  foreach ($dados_serializados as &$dados) {
        // Dividindo o valor de pressao por 10
        $dados['pressao'] /= 10;
    }
				
                $serializedData = json_encode($dados_serializados);

                $chartData = [
                    'labels' => array_reverse(array_column(json_decode($serializedData, true), 'timestamp')), // Inverte os rótulos
                    'datasets' => [
                        [
                            'label' => 'Temperatura (°C)',
                            'data' => array_reverse(array_column(json_decode($serializedData, true), 'temperatura')), // Inverte os dados de temperatura
                            'backgroundColor' => 'rgba(255, 99, 132, 0.2)',
                            'borderColor' => 'rgba(255, 99, 132, 1)',
                            'borderWidth' => 1
                        ],
                        [
                            'label' => 'Umidade (%)',
                            'data' => array_reverse(array_column(json_decode($serializedData, true), 'umidade')), // Inverte os dados de umidade
                            'backgroundColor' => 'rgba(54, 162, 235, 0.2)',
                            'borderColor' => 'rgba(54, 162, 235, 1)',
                            'borderWidth' => 1
                        ],
                        [
                            'label' => 'Pressão (hPa)',
                            'data' => array_reverse(array_column(json_decode($serializedData, true), 'pressao')), // Inverte os dados de pressão
                            'backgroundColor' => 'rgba(255, 206, 86, 0.2)',
                            'borderColor' => 'rgba(255, 206, 86, 1)',
                            'borderWidth' => 1
                        ],
                        [
                            'label' => 'pH',
                            'data' => array_reverse(array_column(json_decode($serializedData, true), 'ph')), // Inverte os dados de pH
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

