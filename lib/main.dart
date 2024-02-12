import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Consulta PostgreSQL',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  String data = '';

  Future<void> fetchData() async {
    final response = await http.get(Uri.parse('SUA_URL_AQUI'));
    if (response.statusCode == 200) {
      setState(() {
        data = json.decode(response.body)['data'];
      });
    } else {
      setState(() {
        data = 'Erro ao carregar dados';
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Consulta PostgreSQL'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            ElevatedButton(
              onPressed: fetchData,
              child: Text('Buscar Informação'),
            ),
            SizedBox(height: 20),
            Text(
              'Informação do Banco de Dados:',
              style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
            ),
            SizedBox(height: 10),
            Text(
              data,
              style: TextStyle(fontSize: 16),
            ),
          ],
        ),
      ),
    );
  }
}
