<!DOCTYPE html>
<html>
<head>
  <title>GrANA Tetris</title>
  <meta charset="UTF-8">
  <style>
html, body {
  height: 100%;
  margin: 0;
  display: flex;
  flex-direction: column;
  background: black;
  color: white;
}

#game-container {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
  position: relative;
  border: 2px solid white;
  margin: 20px;
}

canvas {
  border: 1px solid white;
}

#peca-container {
  position: absolute;
  top: 0;
  right: 120;
  padding: 20px;
}

#controls-container {
  display: flex;
  position: absolute;
    top: 10;
  right: 0;
  width: calc(100% - 40px);
}

#moveLeftButton, #moveRightButton, #rotateButton {
  padding: 10px;
  border-radius: 50%;
  cursor: pointer;
}

#moveLeftButton {
  background-color: green;
  position: absolute;
  top: 900px;
  left: 500px;
}

#moveRightButton {
  background-color: red;
  position: absolute;
  top: 900px;
  right: 500px;
}

#rotateButton {
  background-color: yellow;
  position: absolute;
  top: 900px;
  left: calc(50% - 25px);
}

#startButton, #backButton {
  padding: 15px 30px;
  border-radius: 10px;
  cursor: pointer;
}

#startButton {
  background-color: blue;
  position: absolute;
  top: 1000px; /* Posição abaixo da pontuação */
  left: calc(50% - 100px);
}

#backButton {
  background-color: green;
  position: absolute;
  top: 20px; /* Posição abaixo da pontuação */
  left: calc(50% + 100px);
}

#scorex {
  position: absolute;
  top: 40px; /* Distância do topo */
		  right: 100px;

}

#pontuacao {
  position: absolute;
  top: 40px; /* Distância do fundo */
	  right: 100px;

}



  </style>
</head>
<body>
	
	
	 <h1>Bem-Vindo
            <?php
            session_start();
            if(isset($_SESSION['username'])) {
                echo htmlspecialchars($_SESSION['username']);
            }
            ?> ao GrANA Tetris</h1>
<?php
session_start();

// Verifica se o usuário está logado
if(!isset($_SESSION['username'])) {
    // Redireciona para a página de login, ou exibe uma mensagem de erro, dependendo do seu caso
    echo "Erro: Você não está logado.";
    exit;
}

// Conectar ao banco de dados
$host = 'santocyber.helioho.st';
$dbname = 'santocyber_telemetria';
$username_db = 'santocyber_telemetria';
$password_db = 'telemetria';

try {
    $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username_db, $password_db);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // Obtém o score do usuário
    $username = $_SESSION['username']; // Obtém o nome de usuário da sessão
    $stmt = $pdo->prepare("SELECT score FROM usuario WHERE usuario = :username");
    $stmt->bindParam(':username', $username);
    $stmt->execute();
    $user = $stmt->fetch(PDO::FETCH_ASSOC);

    // Verifica se o score foi encontrado
    if ($user && isset($user['score'])) {
        // Exibe o score
        echo "Seu SCORE GERAL: " . htmlspecialchars($user['score']);
    } else {
        echo "Erro: Score não encontrado para este usuário.";
    }
} catch(PDOException $e) {
    // Em caso de erro de conexão com o banco de dados, exibe mensagem de erro
    echo "Erro de conexão: " . $e->getMessage();
}
?>
	
	
	
<div id="pecas-container">
    <canvas id="preview" width="100" height="100"></canvas>
</div>
<div id="game-container">
    <canvas width="320" height="640" id="game"></canvas>
</div>
<div id="controls-container">
    <button id="moveLeftButton">Mover para Esquerda</button>
<button id="rotateButton">GIRAR</button>
	<button id="moveRightButton">Mover para Direita</button>
    <button id="startButton">Iniciar Partida</button>
</div>
<div id="scorex">
    Pontuação: <span id="score-value">0</span>
</div>
<br><br>
<div id="controls-container">
    <br><br>
    <button id="backButton" onclick="goBack()">Voltar</button>
</div>

<script>
  const canvas = document.getElementById('game');
    let moveLeftButton = document.getElementById('moveLeftButton');
    let moveRightButton = document.getElementById('moveRightButton');

    // Adicionar eventos de clique aos botões
    moveLeftButton.addEventListener('click', moveLeft);
    moveRightButton.addEventListener('click', moveRight);
	 // Adicione um novo evento de clique para o botão de girar peça
     rotateButton.addEventListener('click', rotate);

    // Funções para mover e girar a peça
    function moveLeft() {
        if (!gameOver) {
            const col = tetromino.col - 1;
            if (isValidMove(tetromino.matrix, tetromino.row, col)) {
                tetromino.col = col;
            }
        }
    }

    function moveRight() {
        if (!gameOver) {
            const col = tetromino.col + 1;
            if (isValidMove(tetromino.matrix, tetromino.row, col)) {
                tetromino.col = col;
            }
        }
    }



// Função para girar a peça
document.getElementById('rotateButton').addEventListener('click', rotateTetromino);

function rotateTetromino() {
    const rotatedMatrix = rotate(tetromino.matrix);
    if (isValidMove(rotatedMatrix, tetromino.row, tetromino.col)) {
        tetromino.matrix = rotatedMatrix;
    }
}

function rotate(matrix) {
    const N = matrix.length - 1;
    const result = matrix.map((row, i) =>
        row.map((val, j) => matrix[N - j][i])
    );
    return result;
}

	
	
	
	function updateScore(score) {
    const scoreElement = document.getElementById('score-value');
    scoreElement.textContent = score;}
	
	      // Definir a função goBack fora do escopo DOMContentLoaded
        function goBack() {
            window.history.back();
        }
	
// get a random integer between the range of [min,max]
function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

// generate a new tetromino sequence
function generateSequence() {
  const sequence = ['I', 'J', 'L', 'O', 'S', 'T', 'Z'];
  while (sequence.length) {
    const rand = getRandomInt(0, sequence.length - 1);
    const name = sequence.splice(rand, 1)[0];
    tetrominoSequence.push(name);
  }
}
// Obtenha a próxima peça no jogo
function getNextTetromino() {
    if (tetrominoSequence.length === 0) {
        generateSequence();
    }
    const name = tetrominoSequence.pop();
    const matrix = tetrominos[name];
    const col = playfield[0].length / 2 - Math.ceil(matrix[0].length / 2);
    const row = name === 'I' ? -1 : -2;

    const nextTetromino = {
        name: name,
        matrix: matrix,
        row: row,
        col: col
    };

    // Desenha a próxima peça na visualização
    drawNextTetromino(nextTetromino);

    return nextTetromino;
}

// rotate an NxN matrix 90deg
function rotate(matrix) {
  const N = matrix.length - 1;
  const result = matrix.map((row, i) =>
    row.map((val, j) => matrix[N - j][i])
  );
  return result;
}

// check if the new matrix/row/col is valid
// check if the new matrix/row/col is valid
function isValidMove(matrix, cellRow, cellCol) {
  for (let row = 0; row < matrix.length; row++) {
    for (let col = 0; col < matrix[row].length; col++) {
      if (matrix[row][col]) {
        // Check if the move is out of bounds at the top
        if (cellRow + row < 0) {
          continue; // Skip collision check for top rows
        }
        if (
          cellCol + col < 0 ||
          cellCol + col >= playfield[0].length ||
          cellRow + row >= playfield.length ||
          playfield[cellRow + row][cellCol + col]
        ) {
          return false;
        }
      }
    }
  }
  return true;
}

let score = 0; // I
// place the tetromino on the playfield
// place the tetromino on the playfield
function placeTetromino() {
  for (let row = 0; row < tetromino.matrix.length; row++) {
    for (let col = 0; col < tetromino.matrix[row].length; col++) {
      if (tetromino.matrix[row][col]) {

        // game over if piece has any part offscreen
        if (tetromino.row + row < 0) {
          return showGameOver();
        }

        playfield[tetromino.row + row][tetromino.col + col] = tetromino.name;
      }
    }
  }

	
	   let linesCleared = 0; // Adiciona a variável para armazenar o número de linhas apagadas nesta jogada

  // check for line clears starting from the bottom and working our way up
  for (let row = playfield.length - 1; row >= 0; ) {
    if (playfield[row].every(cell => !!cell)) {
		
				linesCleared++; // Incrementa o número de linhas apagadas

      // drop every row above this one
      for (let r = row; r >= 0; r--) {
        for (let c = 0; c < playfield[r].length; c++) {
          playfield[r][c] = playfield[r-1][c];
        }
      }
		
     score += linesCleared;
    updateScore(score);
	
    }
    else {
      row--;
    }
  }
    // Incrementa a pontuação com base no número de linhas apagadas nesta jogada


  tetromino = getNextTetromino();
}

	
	
	
// show the game over screen
function showGameOver() {
  cancelAnimationFrame(rAF);
  gameOver = true;
  context.fillStyle = 'black';
  context.globalAlpha = 0.75;
  context.fillRect(0, canvas.height / 2 - 30, canvas.width, 60);
  context.globalAlpha = 1;
  context.fillStyle = 'white';
  context.font = '36px monospace';
  context.textAlign = 'center';
  context.textBaseline = 'middle';
  context.fillText('QUE PENA', canvas.width / 2, canvas.height / 4);
  context.fillText('ACABOU o JOGO!', canvas.width / 2, canvas.height / 2);
  //gameOver = false; // Defina o status do jogo como não terminado
  startButton.disabled = false;

  //resetGame();
	
	
	// Dentro da função showGameOver(), após cancelAnimationFrame(rAF)
const xhr = new XMLHttpRequest();
xhr.open('POST', 'atualizar_score.php');
xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
xhr.onload = function() {
  if (xhr.status === 200) {
    console.log('Pontuação atualizada no banco de dados com sucesso!');
  } else {
    console.error('Erro ao atualizar pontuação no banco de dados:', xhr.responseText);
  }
};
xhr.send('score=' + encodeURIComponent(score)); // Envie a pontuação atual como um parâmetro POST


	
}

const context = canvas.getContext('2d');
const grid = 32;
const tetrominoSequence = [];
const playfield = [];
for (let row = -2; row < 20; row++) {
  playfield[row] = [];
  for (let col = 0; col < 10; col++) {
    playfield[row][col] = 0;
  }
}
const tetrominos = {
  'I': [
    [0,0,0,0],
    [1,1,1,1],
    [0,0,0,0],
    [0,0,0,0]
  ],
  'J': [
    [1,0,0],
    [1,1,1],
    [0,0,0],
  ],
  'L': [
    [0,0,1],
    [1,1,1],
    [0,0,0],
  ],
  'O': [
    [1,1],
    [1,1],
  ],
  'S': [
    [0,1,1],
    [1,1,0],
    [0,0,0],
  ],
  'Z': [
    [1,1,0],
    [0,1,1],
    [0,0,0],
  ],
  'T': [
    [0,1,0],
    [1,1,1],
    [0,0,0],
  ]
};
const colors = {
  'I': 'cyan',
  'O': 'yellow',
  'T': 'purple',
  'S': 'green',
  'Z': 'red',
  'J': 'blue',
  'L': 'orange'
};
let count = 0;
let tetromino = getNextTetromino();
let rAF = null;  // keep track of the animation frame so we can cancel it
let gameOver = false;

// game loop
function loop() {
  rAF = requestAnimationFrame(loop);
  context.clearRect(0,0,canvas.width,canvas.height);

  // draw the playfield
  for (let row = 0; row < 20; row++) {
    for (let col = 0; col < 10; col++) {
      if (playfield[row][col]) {
        const name = playfield[row][col];
        context.fillStyle = colors[name];

        // drawing 1 px smaller than the grid creates a grid effect
        context.fillRect(col * grid, row * grid, grid-1, grid-1);
      }
    }
  }

  // draw the active tetromino
  if (tetromino) {

    // tetromino falls every 35 frames
    if (++count > 35) {
      tetromino.row++;
      count = 0;

      // place piece if it runs into anything
      if (!isValidMove(tetromino.matrix, tetromino.row, tetromino.col)) {
        tetromino.row--;
        placeTetromino();
      }
    }

    context.fillStyle = colors[tetromino.name];

    for (let row = 0; row < tetromino.matrix.length; row++) {
      for (let col = 0; col < tetromino.matrix[row].length; col++) {
        if (tetromino.matrix[row][col]) {

          // drawing 1 px smaller than the grid creates a grid effect
          context.fillRect((tetromino.col + col) * grid, (tetromino.row + row) * grid, grid-1, grid-1);
        }
      }
    }
  }
}

// listen to keyboard events to move the active tetromino
document.addEventListener('keydown', function(e) {
  if (gameOver) return;

  // left and right arrow keys (move)
  if (e.which === 37 || e.which === 39) {
    const col = e.which === 37
      ? tetromino.col - 1
      : tetromino.col + 1;

    if (isValidMove(tetromino.matrix, tetromino.row, col)) {
      tetromino.col = col;
    }
  }

  // up arrow key (rotate)
  if (e.which === 38) {
    const matrix = rotate(tetromino.matrix);
    if (isValidMove(matrix, tetromino.row, tetromino.col)) {
      tetromino.matrix = matrix;
    }
  }

  // down arrow key (drop)
  if(e.which === 40) {
    const row = tetromino.row + 1;

    if (!isValidMove(tetromino.matrix, row, tetromino.col)) {
      tetromino.row = row - 1;

      placeTetromino();
      return;
    }

    tetromino.row = row;
  }
});
	
	
	
	
		
function resetGame() {
	
	
  context.clearRect(0, 0, canvas.width, canvas.height);
  tetrominoSequence.length = 0;
  playfield.length = 0;
  for (let row = -2; row < 20; row++) {
    playfield[row] = [];
    for (let col = 0; col < 10; col++) {
      playfield[row][col] = 0;
    }
  }
  tetromino = getNextTetromino(); // Atualize a peça tetromino
    // Reinicia o jogo
	 // Redefinir a pontuação para zero
  score = 0;
  updateScore(score);

  
}
	
	
	function startGame() {
		//drawNextTetromino(getNextTetromino());
const startButton = document.getElementById('startButton'); // Obter o botão de iniciar dentro da função
  resetGame();

  startButton.disabled = true;
  gameOver = false; // Defina o status do jogo como não terminado
 // attachEventListeners(); // Adiciona eventos de teclado e mouse ao iniciar o jogo
   // Limpar o contexto do canvas antes de iniciar o jogo
  const context = canvas.getContext('2d'); // Definir o contexto do canvas dentro da função

  context.clearRect(0, 0, canvas.width, canvas.height);
	
	 context.clearRect(0, 0, canvas.width, canvas.height);
  tetrominoSequence.length = 0;
  playfield.length = 0;
  for (let row = -2; row < 20; row++) {
    playfield[row] = [];
    for (let col = 0; col < 10; col++) {
      playfield[row][col] = 0;
    }
  }
  tetromino = getNextTetromino(); // Atualize a peça tetromino
	
	
	
  rAF = requestAnimationFrame(loop);
}
	

	
	
	
		

const startButton = document.getElementById('startButton');
startButton.addEventListener('click', startGame);
	//function attachEventListeners() {


		
// Função para desenhar a próxima peça no canvas de visualização
function drawNextTetromino(nextTetromino) {
    const previewCanvas = document.getElementById('preview');
    const previewContext = previewCanvas.getContext('2d');
    const blockSize = 20; // Tamanho do bloco da peça de visualização
    const padding = 5; // Espaçamento entre a borda do canvas e a peça de visualização

    previewContext.clearRect(0, 0, previewCanvas.width, previewCanvas.height);

    for (let row = 0; row < nextTetromino.matrix.length; row++) {
        for (let col = 0; col < nextTetromino.matrix[row].length; col++) {
            if (nextTetromino.matrix[row][col]) {
                const x = col * blockSize + padding;
                const y = row * blockSize + padding;
                previewContext.fillStyle = colors[nextTetromino.name];
                previewContext.fillRect(x, y, blockSize - 1, blockSize - 1);
            }
        }
    }
}
	
	
	

</script>
</body>
</html>
