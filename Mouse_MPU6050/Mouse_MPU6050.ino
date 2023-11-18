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
  
  bool esq_press = false; // Variavel de leitura de pressão do botão esquerdo
  bool dir_press = false;  // Variavel de leitura de pressão do botão direito

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

  // Botão Esquerdo

    if (state_esq == LOW && !esq_press) { // Se o botão ficar precionado, então:

      Mouse.press(MOUSE_LEFT); // Emula o acionamento do botão esquerdo
      esq_press = true; // Define a variavel de pressão como verdadeiro

    } 

    else if (state_esq == HIGH && esq_press) { // Se soltar, então:

      Mouse.release(MOUSE_LEFT); // Emula o desacionamento do botão esquerdo
      esq_press = false; // Define a variavel de pressão como falso

    }

  // Botão direito

    if (state_dir == LOW && !dir_press) {  // Se o botão ficar precionado, então:

      Mouse.press(MOUSE_RIGHT); // Emula o acionamento do botão direito
      dir_press = true; // Define a variavel de pressão como verdadeiro

    } 

    else if (state_dir == HIGH && dir_press) {  // Se soltar, então:

      Mouse.release(MOUSE_RIGHT); // Emula o desacionamento do botão direito
      dir_press = false; // Define a variavel de pressão como falso

    }

}

// FIM!