/*

  Mouse virtual com Arduino Pro Micro/Micro/Leornado
  Autor: d4nkali
  Data: 13/11/2023

*/

// Inclusão das bibliotecas

  #include <Mouse.h>
  #include <I2Cdev.h>
  #include <MPU6050.h>
  #include <Wire.h>

// Chama a classe da biblioteca

  MPU6050 mpu;

// Criação das variaveis

  int16_t accx, accy; // Variavel da leitura inicial do sensor
  int prevX, prevY; // Variavel da leitura apos o filtro

  const float valor_filtro = 0.95; // Filtro para suavizar as leituras
  const int veloc_mouse = 2; // Velocidade do mouse

  const int botao_esq = 6; // Define o pino 6 como do botão da esquerda
  const int botao_dir = 7; // Define o pino 7 como do botão da direita

void setup() {

  Wire.begin(); // Inicia a comunicação Wire
  mpu.initialize(); // Inicia o MPU
  Mouse.begin(); // Inicia a biblioteca Mouse

  pinMode(botao_esq, INPUT_PULLUP); // Botão da esquerda como entrada com o sensor pullup interno
  pinMode(botao_dir, INPUT_PULLUP); // Botão da direita como entrada com o sensor pullup interno

}

void loop() {

  mpu.getMotion6(&accx, &accy, NULL, NULL, NULL, NULL); // Ler e adiciona nas variaveis os eixos x e y do sensor

  // Cria variaveis mapeadas como a variavel antiga e ate os limites para o mouse

    int deltaX = map(accx, -20000, 20000, -20, 20);
    int deltaY = map(accy, -20000, 20000, -20, 20);

  // Cria a variavel final apos os calculos e filtros

    int newX = prevX + int(valor_filtro * (deltaX - prevX));
    int newY = prevY + int(valor_filtro * (deltaY - prevY));

  // Cria variaveis para ler o estado dos botões

    int state_esq = digitalRead(botao_esq);
    int state_dir = digitalRead(botao_dir); 
  
  Mouse.move(veloc_mouse * newX, veloc_mouse * newY, 0); // Emula o movimento do mouse pela variavel ja com os filtros e calculos

  // Define a variavel final para ser igual a antiga para novos movimentos

    prevX = newX;
    prevY = newY;

  delay(10); // Aguarda ms para leitura do sensor e do mouse

  if (state_esq == LOW) { // Se botão do esquerdo do mouse for precionado

    Mouse.press(MOUSE_LEFT); // Emula o acionamento do botão
    delay(100);  // Aguarda
    Mouse.release(MOUSE_LEFT); // Solta o botão
    delay(200); // Aguarda

  }

  else if (state_dir == LOW) {  // Se botão da direita do mouse for precionado

    Mouse.press(MOUSE_RIGHT); // Emula o acionamento do botão
    delay(100); // Aguarda
    Mouse.release(MOUSE_RIGHT); // Solta o botão
    delay(200); // Aguarda

  }

}

// FIM!