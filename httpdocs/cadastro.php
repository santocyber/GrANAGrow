<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Cadastro de Usuário</title>
    <style>
        /* Estilos para o formulário */
        form {
            width: 300px;
            margin: 0 auto;
            text-align: center;
            margin-top: 50px;
            background-color: #f2f2f2;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }

        input[type="text"],
        input[type="password"],
        input[type="email"],
        input[type="tel"] {
            width: calc(100% - 22px);
            padding: 10px;
            margin-bottom: 15px;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
        }

        input[type="submit"] {
            background-color: #4CAF50;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            cursor: pointer;
        }

        input[type="submit"]:hover {
            background-color: #45a049;
        }
    </style>
</head>
<body>
    <form action="processa_cadastro.php" method="POST">
        <h2>Cadastro de Usuário</h2>
        <input type="text" name="nome" placeholder="Nome" required><br>
        <input type="text" name="usuario" placeholder="Usuário" required><br>
        <input type="password" name="senha" placeholder="Senha" required><br>
        <input type="email" name="email" placeholder="Email" required><br>
        <input type="tel" name="telefone" placeholder="Telefone"><br>
        <input type="text" name="rede_social" placeholder="Usuário na Rede Social"><br><br>
        <input type="submit" value="Cadastrar">
    </form>
</body>
</html>
