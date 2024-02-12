// Importar os pacotes necessários
import 'package:flutter/material.dart';
import 'package:postgres/postgres.dart';

// Criar uma classe para representar os dados da tabela
class Data {
  final int id;
  final String name;
  final double value;

  Data(this.id, this.name, this.value);
}

// Criar um widget para mostrar os dados em uma lista
class DataList extends StatelessWidget {
  final List<Data> data;

  DataList(this.data);

  @override
  Widget build(BuildContext context) {
    return ListView.builder(
      itemCount: data.length,
      itemBuilder: (context, index) {
        return ListTile(
          leading: Text(data[index].id.toString()),
          title: Text(data[index].name),
          trailing: Text(data[index].value.toString()),
        );
      },
    );
  }
}

// Criar um widget para mostrar um botão de atualização
class RefreshButton extends StatelessWidget {
  final Function onPressed;

  RefreshButton(this.onPressed);

  @override
  Widget build(BuildContext context) {
    return ElevatedButton(
      child: Text('Atualizar'),
      onPressed: onPressed,
    );
  }
}

// Criar um widget para mostrar a tela principal do app
class HomeScreen extends StatefulWidget {
  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  // Criar uma conexão com o banco de dados PostgreSQL
  final connection = PostgreSQLConnection(
    'localhost', // Hostname
    5432, // Porta
    'database', // Nome do banco de dados
    username: 'user', // Usuário
    password: 'password', // Senha
  );

  // Criar uma lista para armazenar os dados da tabela
  List<Data> data = [];

  // Criar uma função para obter os dados da tabela
  Future<void> getData() async {
    // Abrir a conexão com o banco de dados
    await connection.open();

    // Executar uma consulta SQL para selecionar todos os registros da tabela
    final results = await connection.query('SELECT * FROM table');

    // Fechar a conexão com o banco de dados
    await connection.close();

    // Converter os resultados em uma lista de objetos Data
    data = results.map((row) => Data(row[0], row[1], row[2])).toList();

    // Atualizar o estado do widget
    setState(() {});
  }

  // Sobrescrever o método initState para obter os dados da tabela na inicialização do widget
  @override
  void initState() {
    super.initState();
    getData();
  }

  // Sobrescrever o método build para construir a interface do app
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('App de Android com PostgreSQL'),
      ),
      body: Center(
        child: Column(
          children: [
            // Mostrar os dados em uma lista
            Expanded(child: DataList(data)),
            // Mostrar um botão de atualização
            RefreshButton(getData),
          ],
        ),
      ),
    );
  }
}

// Criar um widget para o app
class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: HomeScreen(),
    );
  }
}

// Executar o app
void main() {
  runApp(MyApp());
}
