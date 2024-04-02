<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Receber e exibir dados em tempo real</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background-color: #f4f4f4;
            color: #333;
        }
        .container {
            max-width: 600px;
            margin: 0 auto;
            padding: 20px;
        }
        h1, h2 {
            text-align: center;
        }
        #dados-atuais, #historico {
            background-color: #fff;
            border: 1px solid #ccc;
            padding: 10px;
            margin-bottom: 20px;
        }
        button {
            display: block;
            margin: 0 auto;
            padding: 10px 20px;
            background-color: #4CAF50;
            color: white;
            border: none;
            cursor: pointer;
        }
        button:hover {
            background-color: #45a049;
        }
		
		 
    </style>
    <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
    <script>
        $(document).ready(function(){
            // Função para atualizar os dados em tempo real
            function atualizarDados() {
                $.ajax({
                    url: "receber_dados.php",
                    type: "GET",
                    success: function(data) {
                        $("#dados-atuais").html(data);
                    }
                });
            }

            // Atualizar os dados a cada 3 segundos
            setInterval(atualizarDados, 3000);

            // Botão para limpar o histórico
            $("#limpar-historico").click(function(){
                $.ajax({
                    url: "receber_dados.php?limpar=true",
                    type: "GET",
                    success: function(response) {
                        if(response === 'success') {
                            $("#historico").html("<p>Histórico limpo com sucesso!</p>");
                        } else {
                            $("#historico").html("<p>Ocorreu um erro ao limpar o histórico.</p>");
                        }
                    }
                });
            });
        });
    </script>
</head>
<body>
    <div class="container">
        <h1>Exibindo dados em tempo real</h1>
        <div id="dados-atuais">
            <!-- Os dados atuais serão exibidos aqui -->
        </div>
        <h2>Histórico de mensagens:</h2>
        <div id="historico">
            <!-- O histórico de mensagens será exibido aqui -->
        </div>
        <button id="limpar-historico">Limpar Histórico</button>
    </div>
</body>
</html>
