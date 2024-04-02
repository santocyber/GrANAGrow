
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>River Raid</title>
    <style>
        body {
            margin: 0;
            padding: 0;
            overflow: hidden;
            background-color: #333;
        }

        #game-container {
            width: 100%;
            height: 100vh;
            position: relative;
            overflow: hidden;
        }

        #background {
            width: 100%;
            height: 100%;
            position: absolute;
            background-image:
                linear-gradient(90deg, green 33.33%, blue 33.33%, blue 66.66%, green 66.66%);
            background-size: 100% 100%;
            z-index: -1;
        }

        #centralLane {
            position: absolute;
            width: 50%;
            height: 100%;
            top: 0;
            left: 25%;
            background-color: blue;
        }

        #sideLaneLeft,
        #sideLaneRight {
            position: absolute;
            width: 25%;
            height: 100%;
            background-color: green;
            display: flex;
            flex-wrap: wrap;
            justify-content: space-around;
            align-items: center;
            font-size: 36px;
        }

        .tree {
            margin: 5px;
            position: absolute;
        }

        #player {
            font-size: 50px;
            position: absolute;
            bottom: 20px;
            left: 50%;
            transform: translateX(-50%) rotate(-45deg);
            transition: left 0.1s ease;
        }

        .enemy,
        .helicopter,
        .fuelStation,
        .grana {
            width: 64px;
            height: 64px;
            position: absolute;
            background-size: cover;
        }

        .enemy {
            background-image: url('https://emojicdn.elk.sh/🦅');
        }

        .helicopter {
            font-size: 64px;
            background-image: url('https://emojicdn.elk.sh/🚁');
        }

        .fuelStation {
            font-size: 64px;
            background-image: url('https://emojicdn.elk.sh/⛽');
        }

        .grana {
            color: yellow;
            font-size: 24px;
        }

        .explosion {
            width: 100px;
            height: 100px;
            font-size: 48px;
            position: absolute;
            display: none;
        }

        .score-container {
            position: absolute;
            bottom: 20px;
            left: 20px;
            color: white;
            font-size: 24px;
        }

        .fuel-container {
            position: absolute;
            bottom: 20px;
            right: 20px;
            color: white;
            font-size: 24px;
        }

        #backButton {
            position: absolute;
            top: 20px;
            left: 20px;
            padding: 10px 20px;
            font-size: 18px;
            background-color: #007bff;
            color: white;
            border: none;
            cursor: pointer;
            border-radius: 5px;
        }

        #startButton {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            padding: 10px 20px;
            font-size: 24px;
            background-color: green;
            color: white;
            border: none;
            cursor: pointer;
            display: block;
            z-index: 1;
        }
    </style>
</head>
<body>
    <button id="startButton" onclick="startGame()">Start</button>
    <div id="game-container">
        <div id="background">
            <div id="centralLane"></div>
            <div id="sideLaneLeft"></div>
            <div id="sideLaneRight"></div>
        </div>
        <div id="player">✈️</div>
        <div class="explosion" id="explosion">💥</div>
        <div class="score-container">
            Score: <span id="score">0</span>
        </div>
        <div class="fuel-container" id="fuelContainer">
            Fuel: <span id="fuel">100%</span>
        </div>
    </div>
    <script>
        // Variáveis globais
        const player = document.getElementById('player');
        const explosion = document.getElementById('explosion');
        const scoreDisplay = document.getElementById('score');
        const fuelDisplay = document.getElementById('fuel');
        const startButton = document.getElementById('startButton');
        const backButton = document.getElementById('backButton');
        const gameContainer = document.getElementById('game-container');
        const fuelContainer = document.getElementById('fuelContainer');
        let score = 0;
        let fuel = 100;
        let gamePaused = true;
        let gameInterval;
        let playerPosition = 50;
        let playerSpeed = 0;
        let objectSpeed = 5;

        // Funções de utilidade
        function updateScore() {
            scoreDisplay.textContent = score;
        }

        function updateFuel() {
            fuelDisplay.textContent = fuel.toFixed(0) + '%';
        }

        function adjustFuel() {
            if (score % 100 === 0) {
                if (score > 0) {
                    fuel -= 10;
                } else {
                    fuel += 10;
                }
                updateFuel();
            }
        }

        function explode(x, y) {
            explosion.style.left = x + 'px';
            explosion.style.top = y + 'px';
            explosion.style.display = 'block';
            setTimeout(() => {
                explosion.style.display = 'none';
            }, 500);
        }

        function checkCollision() {
            const playerRect = player.getBoundingClientRect();
            const enemies = document.querySelectorAll('.enemy');
            const helicopters = document.querySelectorAll('.helicopter');

            enemies.forEach(enemy => {
                const enemyRect = enemy.getBoundingClientRect();
                if (
                    playerRect.left < enemyRect.right &&
                    playerRect.right > enemyRect.left &&
                    playerRect.top < enemyRect.bottom &&
                    playerRect.bottom > enemyRect.top
                ) {
                    explode(playerRect.x, playerRect.y);
                    score -= 100;
                    updateScore();
                    if (score <= 0) {
                        resetGame();
                    }
                }
            });

            helicopters.forEach(helicopter => {
                const helicopterRect = helicopter.getBoundingClientRect();
                if (
                    playerRect.left < helicopterRect.right &&
                    playerRect.right > helicopterRect.left &&
                    playerRect.top < helicopterRect.bottom &&
                    playerRect.bottom > helicopterRect.top
                ) {
                    explode(playerRect.x, playerRect.y);
                    score -= 1000;
                    updateScore();
                    if (score <= 0) {
                        resetGame();
                    }
                }
            });

            const fuelStations = document.querySelectorAll('.fuelStation');

            fuelStations.forEach(station => {
                const stationRect = station.getBoundingClientRect();
                if (
                    playerRect.left < stationRect.right &&
                    playerRect.right > stationRect.left &&
                    playerRect.top < stationRect.bottom &&
                    playerRect.bottom > stationRect.top
                ) {
                    fuel = Math.min(fuel + 10, 100); 
                    station.remove(); 
                    updateFuel();
                    score += 50;
                    updateScore();
                }
            });

            const grana = document.querySelectorAll('.grana');

            grana.forEach(grana => {
                const granaRect = grana.getBoundingClientRect();
                if (
                    playerRect.left < granaRect.right &&
                    playerRect.right > granaRect.left &&
                    playerRect.top < granaRect.bottom &&
                    playerRect.bottom > granaRect.top
                ) {
                    score += 1000;
                    updateScore();
                    grana.remove();
                }
            });
        }

        function resetGame() {
            score = 0;
            updateScore();
            fuel = 100;
            updateFuel();
            player.style.bottom = '20px';
            player.style.left = '50%';
            playerPosition = 50;
            playerSpeed = 0;
            objectSpeed = 5;
            gamePaused = true;
            clearInterval(gameInterval);
            startButton.style.display = 'block';

            const enemies = document.querySelectorAll('.enemy');
            const helicopters = document.querySelectorAll('.helicopter');
            const fuelStations = document.querySelectorAll('.fuelStation');
            const grana = document.querySelectorAll('.grana');
            enemies.forEach(enemy => {
                enemy.remove();
            });
            helicopters.forEach(helicopter => {
                helicopter.remove();
            });
            fuelStations.forEach(station => {
                station.remove();
            });
            grana.forEach(grana => {
                grana.remove();
            });
        }

    function startGame() {
    resetGame();
    startButton.style.display = 'none';
    gamePaused = false;
    score += 10;
    updateScore();

    let fuelStationInterval = 4000; // Intervalo inicial de 4 segundos
    let fuelStationTimer;

    let enemyFrequency = 4000; // Frequência inicial de 4 segundos para helicópteros e urubus
    let enemyTimer;

    // Criação inicial de um elemento de cada tipo
    createEnemy();
    createHelicopter();
    createFuelStation();
    createGrana();

    gameInterval = setInterval(() => {
        if (!gamePaused) {
            createEnemy();
            createHelicopter();
            createFuelStation();
            createGrana();
            checkCollision();
            adjustSpawnRates();
        }
    }, 1000);
}
        function createEnemy() {
            const enemy = document.createElement('div');
            enemy.classList.add('enemy');
            enemy.style.left = Math.random() * (window.innerWidth / 2 - 64) + 'px';
            enemy.style.top = -Math.random() * 200 + 'px';
            gameContainer.appendChild(enemy);

            enemy.addEventListener('animationend', () => {
                enemy.remove();
                updateScore();
                score += 10;
                adjustFuel();
            });
        }

        // Adicionando lógica para movimento horizontal e vertical dos helicópteros
        function moveHelicopters() {
            const helicopters = document.querySelectorAll('.helicopter');
            helicopters.forEach(helicopter => {
                const top = parseFloat(helicopter.style.top);
                const left = parseFloat(helicopter.style.left);
                helicopter.style.top = (top + objectSpeed) + 'px';
            });
        }

        setInterval(moveHelicopters, 100); // Atualiza o movimento dos helicópteros a cada 100 milissegundos

        // Event listeners para controle do jogador

        function createHelicopter() {
            const helicopter = document.createElement('div');
            helicopter.classList.add('helicopter');
            helicopter.style.left = Math.random() * (window.innerWidth - 64) + 'px';
            helicopter.style.top = -Math.random() * 200 + 'px';
            gameContainer.appendChild(helicopter);

            helicopter.addEventListener('animationend', () => {
                helicopter.remove();
            });
        }

        function createFuelStation() {
            const fuelStation = document.createElement('div');
            fuelStation.classList.add('fuelStation');
            fuelStation.style.left = Math.random() * (window.innerWidth / 2 - 64) + 'px';
            fuelStation.style.top = -Math.random() * 200 + 'px';
            gameContainer.appendChild(fuelStation);

            fuelStation.addEventListener('animationend', () => {
                fuelStation.remove();
            });
        }

        function createGrana() {
            const grana = document.createElement('div');
            grana.textContent = 'GrANA';
            grana.classList.add('grana');
            grana.style.left = Math.random() * (window.innerWidth / 2 - 64) + 'px';
            grana.style.top = -Math.random() * 200 + 'px';
            gameContainer.appendChild(grana);

            grana.addEventListener('animationend', () => {
                grana.remove();
            });
        }

        function adjustSpawnRates() {
            if (score % 100 === 0) {
                fuelStationInterval += 1000;
                clearInterval(fuelStationTimer);
                fuelStationTimer = setInterval(createFuelStation, fuelStationInterval);

                enemyFrequency -= 1000;
                clearInterval(enemyTimer);
                enemyTimer = setInterval(createEnemy, enemyFrequency);
            }
        }

        function movePlayer() {
            if (!gamePaused) {
                playerPosition += playerSpeed;
                playerPosition = Math.max(Math.min(playerPosition, 100), 0);
                player.style.left = playerPosition + '%';

                const objects = document.querySelectorAll('.enemy, .helicopter, .fuelStation, .grana');
                objects.forEach(object => {
                    const top = parseFloat(object.style.top);
                    const left = parseFloat(object.style.left);
                    object.style.top = (top + objectSpeed) + 'px';

                    if (object.classList.contains('enemy') || object.classList.contains('fuelStation') || object.classList.contains('grana')) {
                        if (left < window.innerWidth / 4) {
                            object.style.left = (left + 1) + 'px';
                        } else if (left > window.innerWidth / 2) {
                            object.style.left = (left - 1) + 'px';
                        }
                    }
                });

                fuel -= 0.1;
                updateFuel();
                if (fuel <= 0) {
                    resetGame();
                    explode(player.getBoundingClientRect().x, player.getBoundingClientRect().y);
                }
            }
        }

        document.addEventListener('keydown', function(event) {
            if (!gamePaused) {
                const key = event.key;
                if (key === 'ArrowLeft') {
                    playerSpeed = -0.5;
                } else if (key === 'ArrowRight') {
                    playerSpeed = 0.5;
                }
            }
        });

        document.addEventListener('keyup', function(event) {
            if (!gamePaused) {
                const key = event.key;
                if (key === 'ArrowLeft' || key === 'ArrowRight') {
                    playerSpeed = 0;
                }
            }
        });

        window.addEventListener('mousemove', function(event) {
            if (!gamePaused) {
                const x = event.clientX;
                const playerRect = player.getBoundingClientRect();
                const halfPlayerWidth = playerRect.width / 2;
                playerPosition = ((x - halfPlayerWidth) / window.innerWidth) * 100;
            }
        });

        window.addEventListener('touchmove', function(event) {
            if (!gamePaused) {
                const touch = event.touches[0];
                const x = touch.clientX;
                const playerRect = player.getBoundingClientRect();
                const halfPlayerWidth = playerRect.width / 2;
                playerPosition = ((x - halfPlayerWidth) / window.innerWidth) * 100;
            }
        });

        setInterval(movePlayer, 20);
    </script>
</body>
</html>
