
void showLogo() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);

  // Dimensões do texto "GrANA MINER"
  int logoWidth = 220; // Aproximadamente 10 caracteres em tamanho 4 (cada caractere tem cerca de 17 pixels de largura)
  int logoHeight = 32; // Aproximadamente 8 pixels por tamanho de texto (4 * 8)

  // Centralizar o texto horizontalmente e verticalmente
  int x = (tft.width() - logoWidth) / 2;
  int y = (tft.height() - logoHeight) / 2 - 20; // Ajustar para a metade da altura e um pouco acima
  tft.setCursor(x, y);
  tft.setTextSize(4);
  tft.println("GrANAGrow");

  // Dimensões do texto "by SantoCyber"
  int subtextWidth = 132; // Aproximadamente 11 caracteres em tamanho 2 (cada caractere tem cerca de 12 pixels de largura)
  int subtextHeight = 16; // Aproximadamente 8 pixels por tamanho de texto (2 * 8)

  x = (tft.width() - subtextWidth) / 2;
  y = (tft.height() - subtextHeight) / 2 + 20; // Ajustar para a metade da altura e um pouco abaixo
  tft.setCursor(x, y);
  tft.setTextSize(2);
  tft.println("by SantoCyber");

  drawComplexPattern();

  tft.fillScreen(TFT_BLACK);
}


void drawComplexPattern() {
  int width = tft.width();
  int height = tft.height();
  int numStars = 100;  // Número de estrelas

  // Estrutura para armazenar a posição e velocidade das estrelas
  struct Star {
    float x, y, z;
  };

  Star stars[numStars];

  // Inicializa as estrelas em posições aleatórias
  for (int i = 0; i < numStars; i++) {
    stars[i].x = random(-width, width);
    stars[i].y = random(-height, height);
    stars[i].z = random(1, width);
  }

  // Inicializa a posição e velocidade do cometa
  float cometX = random(-width, width);
  float cometY = random(-height, height);
  float cometZ = width;
  float cometSpeed = 5.0;

  unsigned long startTime = millis();

  while (millis() - startTime < 10000) {  // Termina a função em 10 segundos
    // Desenha estrelas
    for (int i = 0; i < numStars; i++) {
      // Calcula a posição das estrelas em 3D
      float sx = stars[i].x / stars[i].z;
      float sy = stars[i].y / stars[i].z;

      // Projeção perspectiva
      int xPos = (width / 2) + sx * width;
      int yPos = (height / 2) + sy * height;

      // Gera uma cor baseada na profundidade
      uint16_t color = tft.color565(255 - stars[i].z * 255 / width, 255 - stars[i].z * 255 / width, 255);

      // Desenha a estrela na tela
      if (xPos >= 0 && xPos < width && yPos >= 0 && yPos < height) {
        tft.drawPixel(xPos, yPos, color);
      }

      // Move a estrela em direção à tela com aceleração
      stars[i].z -= 0.1 + (width - stars[i].z) / 100.0;

      // Reinicializa a estrela se sair da tela
      if (stars[i].z < 1) {
        stars[i].x = random(-width, width);
        stars[i].y = random(-height, height);
        stars[i].z = width;
      }
    }

    // Desenha o cometa
    float cometSX = cometX / cometZ;
    float cometSY = cometY / cometZ;
    int cometXPos = (width / 2) + cometSX * width;
    int cometYPos = (height / 2) + cometSY * height;

    // Gera uma cor para o cometa
    uint16_t cometColor = tft.color565(255, 255, 255);

    // Desenha o cometa na tela
    if (cometXPos >= 0 && cometXPos < width && cometYPos >= 0 && cometYPos < height) {
      tft.drawPixel(cometXPos, cometYPos, cometColor);
      for (int j = 1; j < 10; j++) {
        int trailXPos = cometXPos - j * (cometSX * width / cometZ);
        int trailYPos = cometYPos - j * (cometSY * height / cometZ);
        if (trailXPos >= 0 && trailXPos < width && trailYPos >= 0 && trailYPos < height) {
          uint16_t trailColor = tft.color565(255 - j * 25, 255 - j * 25, 255 - j * 25);
          tft.drawPixel(trailXPos, trailYPos, trailColor);
        }
      }
    }

    // Move o cometa
    cometZ -= cometSpeed;

    // Reinicializa o cometa se sair da tela
    if (cometZ < 1) {
      cometX = random(-width, width);
      cometY = random(-height, height);
      cometZ = width;
    }

    delay(30);  // Delay para criar o efeito de animação
  }
}
