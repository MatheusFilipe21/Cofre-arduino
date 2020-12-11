// Inclusão das blibliotecas utilizadas
#include "Keypad.h"
#include <LiquidCrystal.h>

// Define os GPIOs a serem utilizados
#define TRAVA 2
#define BUZZER 25

// Define uma senha padrão para o acesso
const String SENHA_DO_COFRE = "123A";
// Variável que recebe a senha
String senha_digitada = "";
// Variável que concatena os símbolos de "*" que aparecerão no LCD
String senha_oculta = "";

// Define as quantidades de linhas e colunas que serão utilizadas no teclado
const byte LINHAS = 4;
const byte COLUNAS = 4;

// Matriz dos valores que serão utilizadas quando as teclas forem pressionadas
char teclas[LINHAS][COLUNAS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// Indica os Gpios que serão utilizados no esp-32
byte linhasGpio[LINHAS] = {23, 22, 21, 19};
byte colunasGpio[COLUNAS] = {18, 5, 4, 15};

// Faz o mapeamento das teclas de acordo com o teclado
Keypad keypad = Keypad(makeKeymap(teclas), linhasGpio, colunasGpio, LINHAS, COLUNAS);

// Faz o mapeamento do LCD
LiquidCrystal lcd(27, 13, 14, 17, 16, 26);

// Função que contém a configuração inicial do LCD
void telaInicial()
{
  lcd.clear();
  lcd.print("Digite sua senha");
}

// Função que limpa o que tiver contido nas variáveis de senha
void limpaSenhas()
{
  senha_digitada = "";
  senha_oculta = "";
}

// Função de configuração inicial do microcontrolador
void setup()
{
  // Taxa de transferência de bits por segundo
  Serial.begin(9600);
  // Define os GPIOs como saída
  pinMode(BUZZER, OUTPUT);
  pinMode(TRAVA, OUTPUT);
  // Define o tamanho do LCD, colunas e linhas respectivamente
  lcd.begin(16, 2);
  // Chama a função telaInicial
  telaInicial();
}

// Função de repetição
void loop()
{
  // Variável armazena a tecla digitada
  char tecla_pressionada = keypad.getKey();

  // Verifica se uma tecla foi pressionada
  if (tecla_pressionada)
  {

    // Testa se a tecla pressionada possui o valor "*"
    if (tecla_pressionada == '*')
    // Bloco responsável por retornar a configuração inicial
    {
      telaInicial();
      limpaSenhas();
    } // Se o valor da tecla não foi "*", verifica se a tecla pressionada possui o valor "#"
    else if (tecla_pressionada == '#')
    {
      // Bloco responsável por verificar se a senha digitada está igual a padrão
      if (SENHA_DO_COFRE == senha_digitada)
      // Se a senha estiver correta, executa o bloco de código a seguir
      {
        // Limpa a tela, remove tudo que estiver escrito na tela
        lcd.clear();
        // Imprime na tela "Acesso Liberado!"
        lcd.print("Acesso Liberado!");
        // Emite um sinal sonoro
        digitalWrite(BUZZER, HIGH);
        // Aguarda 250 milissegundos
        delay(250);
        // Omite o sinal sonoro
        digitalWrite(BUZZER, LOW);
        // Abre a trava
        digitalWrite(TRAVA, HIGH);
        // Aguarda 5000 milissegundos
        delay(5000);
        // Fecha a trava
        digitalWrite(TRAVA, LOW);
        // Retorna as configurações iniciais
        limpaSenhas();
        telaInicial();
      }
      else
      // Se a senha estiver incorreta, executa o bloco de código a seguir
      {
        // Limpa a tela, remove tudo que estiver escrito na tela
        lcd.clear();
        // Imprime na tela "Acesso Negado!"
        lcd.print("Acesso Negado!");
        // Repete o bloco a seguir três vezes
        for (int i = 0; i < 3; i++)
        {
          // Aguarda 150 milissegundos
          delay(150);
          // Emite um sinal sonoro
          digitalWrite(BUZZER, HIGH);
          // Aguarda 150 milissegundos
          delay(150);
          // Omite o sinal sonoro
          digitalWrite(BUZZER, LOW);
        }
        // Aguarda 3000 milissegundos
        delay(3000);
        // Retorna as configurações iniciais
        limpaSenhas();
        telaInicial();
      }
    } // Se a tecla não teve o valor "*" nem "#", executa o bloco de código a seguir
    else
    {
      // Concatena na váriável as teclas que foram pressionadas
      senha_digitada += tecla_pressionada;
      // Concatena "*" na váriável a quantidade de vezes que a tecla foi pressionada
      senha_oculta += "*";
      // Imprime na tela o valor contido na variável senha_oculta
      lcd.print(senha_oculta);
      // Emite um sinal sonoro
      digitalWrite(BUZZER, HIGH);
      // Aguarda 150 milissegundos
      delay(150);
      // Omite o sinal sonoro
      digitalWrite(BUZZER, LOW);
    }
  }
  // Define a posição do ponteiro
  lcd.setCursor(0, 1);
}
