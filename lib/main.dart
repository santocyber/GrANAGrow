import 'package:flutter/material.dart';
import 'package:postgres/postgres.dart';

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: Text('Flutter + PostgreSQL'),
        ),
        body: Center(
          child: MyDataTable(),
        ),
      ),
    );
  }
}

class MyDataTable extends StatefulWidget {
  @override
  _MyDataTableState createState() => _MyDataTableState();
}

class _MyDataTableState extends State<MyDataTable> {
  List<DataRow> _rows = [];
  bool _loading = false;

  // Conexão com o banco de dados PostgreSQL
  Future<List<DataRow>> _fetchData() async {
    var connection = PostgreSQLConnection(
      'host', // host do banco de dados
      5432, // porta do banco de dados
      'database', // nome do banco de dados
      username: 'username', // nome de usuário do banco de dados
      password: 'password', // senha do banco de dados
    );
    await connection.open();
    // Consulta SQL para obter os dados da tabela
    var results = await connection.query('SELECT * FROM table');
    await connection.close();
    // Converter os resultados em uma lista de DataRow
    var rows = results.map((row) {
      return DataRow(cells: [
        DataCell(Text(row[0].toString())), // primeira coluna da tabela
        DataCell(Text(row[1].toString())), // segunda coluna da tabela
        // Adicionar mais células conforme o número de colunas da tabela
      ]);
    }).toList();
    return rows;
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        ElevatedButton(
          onPressed: () async {
            setState(() {
              _loading = true;
            });
            // Buscar os dados do banco de dados ao clicar no botão
            var rows = await _fetchData();
            setState(() {
              _rows = rows;
              _loading = false;
            });
          },
          child: Text('Requisitar dados'),
        ),
        SizedBox(height: 16),
        _loading
            ? CircularProgressIndicator()
            : _rows.isEmpty
                ? Text('Nenhum dado encontrado')
                : DataTable(
                    columns: [
                      DataColumn(label: Text('Coluna 1')), // nome da primeira coluna da tabela
                      DataColumn(label: Text('Coluna 2')), // nome da segunda coluna da tabela
                      // Adicionar mais colunas conforme o número de colunas da tabela
                    ],
                    rows: _rows,
                  ),
      ],
    );
  }
}
