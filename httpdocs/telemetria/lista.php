<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Monitor GrANANET</title>
</head>
<body>
        <?php
        session_start();

        if (!isset($_SESSION['username'])) {
            echo "Erro: Você não está logado.";
            exit;
        }

        $username = $_SESSION['username'];

        try {
            $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_telemetria', 'santocyber_telemetria', 'telemetria');
            $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            $numLabels = 60;

            if (isset($_GET['numLabels'])) {
                $numLabels = $_GET['numLabels'];
            }

            $query = "SELECT identificador, nome, ip, mac, datalocal, horalocal, wifilocal, last_update, acoes, geolocalizacao, usuarios, ping, hrliga, hrdesliga, objsaida, noteiroentrada, moedeiroentrada, cartaoentrada FROM telemetria WHERE usuarios = :username";

            if (strpos($query, 'WHERE') !== false) {
                $query .= " ORDER BY identificador ASC";
            }

            $stmt = $pdo->prepare($query);
            $stmt->bindParam(':username', $username, PDO::PARAM_STR);
            $stmt->execute();

            echo "<table>";
            echo "<tr><th>Nome</th><th>Identificador</th><th>IP</th><th>MAC</th><th>Obj. Saida</th><th>Noteiro Entrada</th><th>Moe. Entrada</th><th>Cartao Entrada</th><th>Hr Liga</th><th>Hr Desliga</th><th>Data Local</th><th>Hora Local</th><th>Wi-Fi Local</th><th>Últ. Atualização</th><th>Ações</th><th>Geolocalização</th><th>Usuário</th><th>Ping</th></tr>";

            while ($linha = $stmt->fetch(PDO::FETCH_ASSOC)) {
                echo "<tr>";
                echo "<td>{$linha['nome']}</td>";
                echo "<td>{$linha['identificador']}</td>";
                echo "<td>{$linha['ip']}</td>";
                echo "<td>{$linha['mac']}</td>";
                echo "<td>{$linha['objsaida']}</td>";
                echo "<td>{$linha['noteiroentrada']}</td>";
                echo "<td>{$linha['moedeiroentrada']}</td>";
                echo "<td>{$linha['cartaoentrada']}</td>";
                echo "<td>{$linha['hrliga']}</td>";
                echo "<td>{$linha['hrdesliga']}</td>";
                echo "<td>{$linha['datalocal']}</td>";
                echo "<td>{$linha['horalocal']}</td>";
                echo "<td>{$linha['wifilocal']}</td>";
                echo "<td>{$linha['last_update']}</td>";
                echo "<td>{$linha['acoes']}</td>";
                echo "<td>{$linha['geolocalizacao']}</td>";
                echo "<td>{$linha['usuarios']}</td>";
                echo "<td>{$linha['ping']} ms</td>";
                echo "</tr>";
            }

            echo "</table>";

        } catch (PDOException $e) {
            echo "Erro: " . $e->getMessage();
        }
        ?>
   <br>
        <button onclick="window.history.back()">Voltar</button>
</body>
</html>
