/*************************PINAGEM*****************
  RFID-RC522            Nodemcu
  RST/Reset RST         D3 [1*]      GPIO-0  [1*]
  SPI SS  SDA[3]        D8 [2*]      GPIO-15 [2*]
  SPI MOSI MOSI           D7           GPIO-13
  SPI MISO MISO           D6           GPIO-12
  SPI SCK SCK             D5           GPIO-14
  Led Vermelho            D1           GPIO-16
  Led Verde               D2           GPIO-05
  Led Azul                D4           GPIO-04
  Buzzer                  D0           
**************************************************/

#include <SPI.h> //biblioteca para comunicação do barramento SPI
#include <MFRC522.h> //biblioteca responsável pela comunicação com o módulo RFID-RC522
#include <EEPROM.h> // biblioteca para gravação de dados na memória



#define RST_PIN    0 // D3   
#define SS_PIN     15 // D8   

 int LedVermelho = 2;      //refere-se ao pino digital 16.
 int LedVerde = 5;     //refere-se ao pino digital 05.
 int LedAzul = 4; //refere-se ao pino digital 04.
 int buzzer = 16; // buzzer

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup()
{
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI
  mfrc522.PCD_Init();   // Inicia MFRC522

  digitalWrite(LedVermelho, HIGH);
  digitalWrite(buzzer, LOW);
  
  pinMode(LedVermelho, OUTPUT);
  pinMode(LedVerde,    OUTPUT);
  pinMode(LedAzul,     OUTPUT);
  pinMode(buzzer,      OUTPUT);

}

//variaveis global para tratamento dos dados
int cont, addr, fim = 0;
long int inicio = 0, tempo = 3600000, Max; /// 3600 000 = 3 horas
String professor;
String ID_Professor = "ID_Professor:";
String ID_Aluno = ", ID_Aluno:";
String Entrada = ", Entrada: ";



void Fbuzzer(){
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
}

String leitura(String *cartao_atual) {

  digitalWrite(LedVermelho, LOW);
  digitalWrite(LedAzul, HIGH);
  
  Serial.println("Esperando confirmação do Professor");
  delay(800);
  Fbuzzer();
  return *cartao_atual;
}

int Verifica(int *cont, String *cartao_atual, String professor, int *fim, long int *inicio) {
  
  Serial.print("cont = ");
  Serial.println(*cont);
  Serial.print("cartao_atual = ");
  Serial.println(*cartao_atual);
  
  
  
  if (*cont == 1 && *cartao_atual == professor) {

    Serial.print("professor = ");
    Serial.println(professor);
    Serial.println("Inicio da Aula ");
    digitalWrite(LedAzul, LOW);
    digitalWrite(LedVerde, HIGH);
    Serial.println("lido o Professor");
    Serial.print("Começou a Aula em :");
    Serial.println(*inicio);
    Serial.print("Termino da aula: ");
    Serial.println(*inicio+tempo);
    Serial.println("Leitura dos Alunos ");

    delay(800);
    Fbuzzer();
    delay(200);
    Fbuzzer();
    
    Max = *inicio + tempo;
  }
    String Tam_P = professor.length();

  //isso tem que ser salvo na memória
  if (*inicio < Max && *cartao_atual != professor) {

    *fim++;

    for (long int x = inicio; x< Tam_p; x++){
       EEPROM.write(x, byte(*professor.charAt(x)));
    
    
    
    Serial.println("Lendo do Aluno ");
    
    Serial.print("qtde alunos: ");
    Serial.println(*cont -1);
      
    Serial.print("ID_Professor:");
    Serial.print(*professor);
    Serial.print(", ID_Aluno:");
    Serial.print(*cartao_atual);
    Serial.print(", Horário de Entrada: ");
    Serial.println(*inicio);
  
  }

  Serial.println("qtde fim" + *fim);
  return *fim;
}


void termino(){

    Serial.println("Terminou a aula");
    digitalWrite(LedVerde, LOW);
    digitalWrite(LedVermelho, HIGH);
    Fbuzzer();
    cont = 0; 
  
}



void loop()
{

  //escreve em outros endereços da EEPROM:
  addr = addr + 1;
  if (addr == EEPROM.length()) addr = 0;
  
  delay(900);
  inicio++;
  // Aguarda a aproximação do cartão RFID
  if ( ! mfrc522.PICC_IsNewCardPresent()) return;
  
  // Seleciona o cartão RFID
  if ( ! mfrc522.PICC_ReadCardSerial()) return;

  // variável que armazena o conteúdo da Tag do Cartão
  String cartao_atual = "";

  //Prepara a chave - todas as chaves estão configuradas para FFFFFFFFFFFFh (Padrão de fábrica).
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    cartao_atual.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    cartao_atual.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  cartao_atual.toUpperCase();

  if (cont == 0) professor = leitura(&cartao_atual);
  
  Verifica(&cont, &cartao_atual, &professor, &fim, &inicio);

  //if (fim > 3) termino();

  cont++;

}
