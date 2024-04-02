

// Função para limitar o número de rótulos exibidos
function limitLabels(labels, numLabels) {
    if (labels.length > numLabels) {
        // Retorna apenas os últimos 'numLabels' rótulos
        return labels.slice(-numLabels);
    } else {
        // Se houver menos de 'numLabels' rótulos, retorna todos os rótulos
        return labels;
    }
}

// Função para alternar entre modo escuro e claro
function toggleDarkMode() {
    var body = document.body;
    var darkModeToggle = document.getElementById("dark-mode-toggle");
    var darkModeLabel = document.getElementById("dark-mode-label");

    if (body.classList.contains("dark-mode")) {
        body.classList.remove("dark-mode");
        darkModeLabel.textContent = "Dark Mode";
        darkModeToggle.checked = false;
        localStorage.setItem("darkMode", "false");
        console.log("Modo escuro desativado. Valor do darkMode no localStorage:", localStorage.getItem("darkMode"));
    } else {
        body.classList.add("dark-mode");
        darkModeLabel.textContent = "Light Mode";
        darkModeToggle.checked = true;
        localStorage.setItem("darkMode", "true");
        console.log("Modo escuro ativado. Valor do darkMode no localStorage:", localStorage.getItem("darkMode"));
    }
}

// Função para atualizar o gráfico
function updateChart(mac) {
    var numLabels = parseInt(localStorage.getItem("numLabels")) || 1440;
    localStorage.setItem("numLabels", numLabels);
    console.log("Valor de numLabels atualizado:", numLabels);
    location.reload();
}

document.addEventListener("DOMContentLoaded", function () {
    var darkModeToggle = document.getElementById("dark-mode-toggle");
    var darkModeLabel = document.getElementById("dark-mode-label");
    var numLabelsElement = document.getElementById("num-labels");

    if (darkModeToggle && darkModeLabel && numLabelsElement) {
        // Verifica se há um valor válido no armazenamento local para o modo escuro
        var darkMode = localStorage.getItem("darkMode");
        if (darkMode === null || darkMode === "false") {
            darkMode = window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches;
            localStorage.setItem("darkMode", darkMode);
        }
        setDarkMode(darkMode === "true");

        darkModeToggle.addEventListener("change", function () {
            var darkMode = this.checked;
            localStorage.setItem("darkMode", darkMode);
            setDarkMode(darkMode);
        });

        var numLabels = localStorage.getItem("numLabels");
        if (numLabels !== null) {
            numLabelsElement.value = numLabels;
            console.log("Valor de numLabels atualizado:", numLabels);
        } else {
            localStorage.setItem("numLabels", "1440");
        }
    } else {
        console.error("Elementos necessários não encontrados.");
    }

    function setDarkMode(enabled) {
        var body = document.body;
        if (enabled) {
            body.classList.add("dark-mode");
            darkModeLabel.textContent = "Light Mode";
            darkModeToggle.checked = true;
        } else {
            body.classList.remove("dark-mode");
            darkModeLabel.textContent = "Dark Mode";
            darkModeToggle.checked = false;
        }
    }

	
	
});

// Função para gerar o gráfico
function generateChart(canvasId, data, numLabels) {
    var ctx = document.getElementById(canvasId).getContext('2d');
    var limitedLabels = limitLabels(data.labels, numLabels);
    var formattedLabels = limitedLabels.map(function (timestamp) {
        var date = new Date(timestamp * 1000);
        return date.toLocaleString('pt-BR', {
            day: '2-digit',
            month: 'numeric',
            year: 'numeric',
            hour: 'numeric',
            minute: 'numeric'
        });
    });

    var myChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: formattedLabels,
            datasets: data.datasets.map(function (dataset) {
                return {
                    label: dataset.label,
                    data: dataset.data.slice(-numLabels),
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
