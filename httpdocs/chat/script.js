document.addEventListener('DOMContentLoaded', function() {
    const chatBox = document.getElementById('chat-box');
    const messageInput = document.getElementById('message-input');
    const sendButton = document.getElementById('send-button');
    const deleteMessagesButton = document.getElementById('delete-messages-button');

    // Função para adicionar uma mensagem ao chat box
    function addMessage(user, message, timestamp) {
        const messageElement = document.createElement('div');
        messageElement.classList.add('message');
        
        const userElement = document.createElement('span');
        userElement.classList.add('user');
        userElement.textContent = user + ':';
        
        const messageContentElement = document.createElement('span');
        messageContentElement.classList.add('message-content');
        messageContentElement.textContent = message;
        
        const timestampElement = document.createElement('span');
        timestampElement.classList.add('timestamp');
        timestampElement.textContent = timestamp;
        
        messageElement.appendChild(userElement);
        messageElement.appendChild(document.createElement('br'));
        messageElement.appendChild(messageContentElement);
        messageElement.appendChild(document.createElement('br'));
        messageElement.appendChild(timestampElement);
        
        chatBox.prepend(messageElement);
        scrollChatToBottom();
    }

    // Função para rolar o chat para a parte inferior
    function scrollChatToBottom() {
        chatBox.scrollTop = chatBox.scrollHeight;
    }

    // Função para enviar mensagem para o backend
    function sendMessage() {
        const message = messageInput.value.trim();
        if (message !== '') {
            fetch('chat.php', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                },
                body: 'message=' + encodeURIComponent(message)
            })
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    messageInput.value = '';
                    loadChatHistory(); // Atualiza o chat após enviar a mensagem
                } else {
                    console.error(data.message);
                }
            })
            .catch(error => {
                console.error('Erro ao enviar mensagem:', error);
            });
        }
    }

    // Evento de clique para enviar mensagem
    sendButton.addEventListener('click', sendMessage);

    // Evento de pressionar Enter para enviar mensagem
    messageInput.addEventListener('keypress', function(event) {
        if (event.key === 'Enter') {
            sendMessage();
        }
    });

    // Função para carregar as mensagens do histórico do chat
    function loadChatHistory() {
        fetch('chat.php')
            .then(response => response.json())
            .then(data => {
                if (data.status === 'success') {
                    chatBox.innerHTML = '';
                    data.messages.forEach(message => {
                        addMessage(message.username, message.message, message.timestamp);
                    });
                } else {
                    console.error(data.message);
                }
            })
            .catch(error => {
                console.error('Erro ao carregar histórico de chat:', error);
            });
    }

    // Função para enviar uma requisição para apagar todas as mensagens do chat
    function deleteAllMessages() {
        fetch('delete_messages.php', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded'
            }
        })
        .then(response => response.json())
        .then(data => {
            if (data.status === 'success') {
                // Não é necessário recarregar o histórico de mensagens aqui, pois o polling fará isso automaticamente
            } else {
                console.error(data.message);
            }
        })
        .catch(error => {
            console.error('Erro ao enviar requisição para apagar mensagens:', error);
        });
    }

    // Evento de clique para o botão de apagar mensagens
    deleteMessagesButton.addEventListener('click', deleteAllMessages);

    // Função para atualizar o chat em tempo real
    function updateChat() {
        loadChatHistory();
        setTimeout(updateChat, 5000); // Atualiza o chat a cada 5 segundos (5000 milissegundos)
    }

    // Inicia o processo de atualização do chat em tempo real
    updateChat();
});
