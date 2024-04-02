let diasPassados = 0;
let phSolo = 7;
let umidadeAmbiente = 50;
let temperaturaAmbiente = 25;
let luzLigada = false;
let exaustorLigado = false;
let desumidificadorLigado = false;
let pontosColheita = 0;
let pontosJogo = 0; // Pontuação do jogo

// Atualiza o valor da colheita na interface


function iniciarJogo() {
    modificarValoresIniciais(); // Gerar valores aleatórios para as variáveis iniciais
    atualizarInterface(); // Atualiza a interface com os novos valores
}



function modificarValoresIniciais() {
    umidadeAmbiente = Math.floor(Math.random() * 101);
    phSolo = Math.floor(Math.random() * 14) + 1;
    temperaturaAmbiente = Math.floor(Math.random() * 20) + 15;

    
}

let regouVezes = 0; // Variável para rastrear quantas vezes a planta foi regada
function regarPlanta() {
    if (regouVezes === 0) {
        if (phSolo > 8) {
            pontosColheita += 1; // Ganha apenas 1 ponto na colheita ao regar a planta pela primeira vez com pH acima de 8
            registrarAcao("Ganhou 1 ponto na colheita ao regar a planta pela primeira vez com pH acima de 8");
        } else {
            pontosColheita += 20; // Ganha 20 pontos na colheita ao regar a planta pela primeira vez
            registrarAcao("Ganhou 20 pontos na colheita ao regar a planta pela primeira vez");
        }
    } else if (regouVezes === 2) {
        pontosColheita -= 10; // Perde 10 pontos na colheita ao regar a planta pela terceira vez
        registrarAcao("Perdeu 10 pontos na colheita ao regar a planta pela terceira vez");
    } else if (regouVezes > 2) {
        pontosColheita -= 50; // Perde 50 pontos na colheita ao regar a planta mais de duas vezes
        registrarAcao("Perdeu 50 pontos na colheita ao regar a planta mais de duas vezes");
    }

    regouVezes++; // Incrementa o contador de vezes que a planta foi regada

    if (phSolo < 5) {
        pontosColheita -= 20; // Perde 20 pontos na colheita ao regar com pH abaixo de 5
        registrarAcao("Perdeu 20 pontos na colheita ao regar com pH abaixo de 5");
    }

    atualizarInterface();
    registrarAcao("Regou a planta");
}




function controlarPhAgua(acao) {
    if (acao === "aumentar" && phSolo < 14) {
        phSolo += 1;
    } else if (acao === "diminuir" && phSolo > 1) {
        phSolo -= 1;
    }
    atualizarInterface();
    registrarAcao("Controlou pH da água");
}

function ligarExaustor() {
    let exaustorAnterior = exaustorLigado;
    exaustorLigado = !exaustorLigado;
    
    if (temperaturaAmbiente > 30 && exaustorLigado && !exaustorAnterior) {
        pontosColheita++; // Ganha 1 ponto no valor da colheita ao ligar o exaustor devido à alta temperatura
        registrarAcao("Ligou exaustor devido à alta temperatura");
    } else if (temperaturaAmbiente < 20 && !exaustorLigado && exaustorAnterior) {
        pontosColheita++; // Ganha 1 ponto no valor da colheita ao desligar o exaustor devido à baixa temperatura
        registrarAcao("Desligou exaustor devido à baixa temperatura");
    } else {
        pontosColheita--; // Desconta 1 ponto no valor da colheita se o exaustor for ligado/desligado fora da faixa de temperatura permitida
        registrarAcao("Exaustor ligado/desligado fora da faixa de temperatura permitida");
    }
    
    atualizarInterface();
}

let vezesLigadaDesligadaLuz = 0; // Variável para contar quantas vezes a luz foi ligada e desligada
let ligadaUltimaVez = false; // Variável para rastrear se a luz estava ligada na última ação

function toggleLuz() {
    if (luzLigada) {
        luzLigada = false;
        registrarAcao("Desligou a luz");
    } else {
        luzLigada = true;
        registrarAcao("Ligou a luz");
    }

    if (luzLigada !== ligadaUltimaVez) {
        // Se o estado da luz mudou desde a última vez
        ligadaUltimaVez = luzLigada;
        vezesLigadaDesligadaLuz++;
        
        if (vezesLigadaDesligadaLuz > 5) {
            pontosColheita -= 20; // Perde 20 pontos na colheita ao ligar e desligar a luz mais de 5 vezes
            registrarAcao("Perdeu 20 pontos na colheita ao ligar e desligar a luz mais de 5 vezes");
        } else if (vezesLigadaDesligadaLuz > 1) {
            pontosColheita--; // Perde 1 ponto na colheita a cada vez que ligar e desligar a luz
            registrarAcao("Perdeu 1 ponto na colheita ao ligar e desligar a luz");
        } else {
            pontosColheita += 5; // Ganha 5 pontos na colheita ao ligar a luz pela primeira vez
        }
    }

    atualizarInterface();
}

function toggleDesumidificador() {
    let desumidificadorAnterior = desumidificadorLigado;
    desumidificadorLigado = !desumidificadorLigado;
    
    if (umidadeAmbiente > 70 && desumidificadorLigado && !desumidificadorAnterior) {
        pontosColheita++; // Ganha um ponto na colheita ao ligar o desumidificador dentro da faixa necessária
        registrarAcao("Ligou o desumidificador devido à umidade alta");
    } else if (umidadeAmbiente < 60 && !desumidificadorLigado && desumidificadorAnterior) {
        pontosColheita--; // Perde um ponto na colheita ao desligar o desumidificador fora da faixa necessária
        registrarAcao("Desligou o desumidificador devido à umidade baixa");
    } else {
        pontosColheita--; // Perde um ponto na colheita se ligar/desligar o desumidificador fora das faixas necessárias
        registrarAcao("Ligou/Desligou o desumidificador fora das faixas de umidade permitidas");
    }
    
    atualizarInterface();
}

function enviarManutencao() {

        atualizarInterface();

}


function atualizarInterface() {
    let planta = document.getElementById('planta');
    let statusLuz = document.getElementById('statusLuz');
    let statusExaustor = document.getElementById('statusExaustor');
    let umidadeAmbienteElem = document.getElementById('umidadeAmbiente');
    let phSoloElem = document.getElementById('phSolo');
    let temperaturaElem = document.getElementById('temperatura');
    let statusDesumidificador = document.getElementById('statusDesumidificador');
    let valorColheitaElem = document.getElementById('valorColheita'); // Elemento para exibir os pontos de colheita

    // Atualiza o emoji da planta de acordo com o tamanho
    if (tamanhoPlanta < 0) {
        planta.innerHTML = '💩'; // Emoji de bosta
    } else if (tamanhoPlanta <= 10) {
        planta.innerHTML = '🌰'; // semente
    } else if (tamanhoPlanta > 10 && tamanhoPlanta <= 20) {
        planta.innerHTML = '🌱'; // Broto
    } else if (tamanhoPlanta > 20 && tamanhoPlanta <= 30) {
        planta.innerHTML = '🌿'; // Planta bebê
    } else if (tamanhoPlanta > 30 && tamanhoPlanta <= 40) {
        planta.innerHTML = '🌳'; // Planta grande
    } else {
        planta.innerHTML = '🌸'; // Planta florida
    }
    umidadeAmbienteElem.textContent = umidadeAmbiente;
    phSoloElem.textContent = phSolo;
    temperaturaElem.textContent = temperaturaAmbiente + " °C";
    statusLuz.textContent = luzLigada ? "Ligada" : "Apagada";
    statusExaustor.textContent = exaustorLigado ? "Ligado" : "Desligado";
    statusDesumidificador.textContent = desumidificadorLigado ? "Ligado" : "Desligado";
    valorColheitaElem.textContent = pontosColheita; // Exibe os pontos de colheita na interface
	let plantaEmoji = document.getElementById('planta');

	
	
}


function prepararEnvio() {
    // Atualiza os valores dos campos do formulário
    document.getElementById("tamanhoPlantaInput").value = tamanhoPlanta;
    document.getElementById("diasPassadosInput").value = diasPassados;
    document.getElementById("phSoloInput").value = phSolo;
    document.getElementById("umidadeAmbienteInput").value = umidadeAmbiente;
    document.getElementById("temperaturaAmbienteInput").value = temperaturaAmbiente;
    document.getElementById("luzLigadaInput").value = luzLigada;
    document.getElementById("exaustorLigadoInput").value = exaustorLigado;
    document.getElementById("desumidificadorLigadoInput").value = desumidificadorLigado;
    document.getElementById("pontosColheitaInput").value = pontosColheita;
    document.getElementById("pontosJogoInput").value = pontosJogo;

    // Submete o formulário
    document.getElementById("gameForm").submit();
}

function registrarAcao(acao) {
    console.log("Ação registrada: " + acao);
}


