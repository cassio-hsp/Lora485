//INOMON NODE é um nó LoRa E32 100mW com ESP32 capaz de ler dados de sensores via RS485/MODBUS e efetuar envio via LoRa.

#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <ModbusRTU.h>            //Biblioteca de Emelianov

ModbusRTU mb;

#define EEPROM_SIZE 64            //Podemos utilizar até 512 registros de memoria. 512 bytes.

int address = 0;
String data_rx = "";
String temporaryID = "";
String memoryID = "";
String recuperadoID = "02";     //deixar vazio quando for implementar com a leitura o ID da memoria.

#define SLAVE_ADDR 2
#define REGISTER 2
#define BAUD 9600
#define PIN_TX_ENABLE 5

String str_data[16];
String str_data_concat = "";
String node_id_recebido = "";
String node_slave_recebido = "";
String node_reg_recebido = "";
int node_reg_recebido_int = 0;

int len_lora_message = 0;

SoftwareSerial Lora;

bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data)    // Callback to monitor errors
{ 
  if (event != Modbus::EX_SUCCESS) 
  {
    Serial.print("Valor Registro: ");
    Serial.println(event, HEX);
  }
  return true;
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1);                         //Serial para o Max485
  Lora.begin(9600, SWSERIAL_8N1, 22, 23, false, 256);      //rx -> 22, tx -> 23 usando soffware serial
  
  mb.begin(&Serial2, 5);
  mb.master();
  
  pinMode(2, OUTPUT);
  Lora.println("ND_1 Started");
 
  EEPROM.begin(EEPROM_SIZE);

  //recuperadoID = my_ID();
  Serial.print("This Unit ID = ");  
  Serial.println(recuperadoID);
}

String Lora_Listen()
{
  String rx_conteudo = "";
  char rx_caractere;

  while(Lora.available() > 0)
  {
    rx_conteudo = Lora.readString();
    rx_conteudo.trim();
  }
  return rx_conteudo;
}

void pisca2x()
{
  for(int i=0;i<2;i++)
  {
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
  }
}

void cfg_ID()
{
    Lora.println("Node ID: ");
    digitalWrite(2, HIGH);
            
    while(temporaryID == "")
    {
      data_rx = Lora_Listen();
      temporaryID = data_rx;
    }
    
    Lora.print("OK! Config ID = ");
    Lora.println(temporaryID);

    Lora.print("TemporaryID length = ");
    Lora.println(temporaryID.length());

    EEPROM.write(0, temporaryID[0]);
    
    EEPROM.commit();         
    delay(500);
    digitalWrite(2, LOW);
    data_rx = "";
    temporaryID = "";
}

String my_ID()
{
  memoryID = char(EEPROM.read(0));     //necessario por char devido ao retorno da leitura vir em cod ascii.
  Lora.print("My ID is: ");
  Lora.println(memoryID);

  return memoryID;
}

void loop()
{
  if(Lora.available() > 0)
  {
  data_rx = Lora_Listen();
  //Serial.println(data_rx);

  if (data_rx == "on"){
    digitalWrite(2, HIGH);}

  if (data_rx == "off"){
    digitalWrite(2, LOW);}

  if (data_rx == "cfg_ID"){
    cfg_ID();}

  if(data_rx == "my_ID"){
    my_ID();}

  
  len_lora_message = data_rx.length();
  Serial.print("Message Size: ");
  Serial.println(len_lora_message);

  node_id_recebido += data_rx[0];
  node_id_recebido = node_id_recebido += data_rx[1];
  Serial.print("node_id_recebido = ");
  Serial.println(node_id_recebido);

  node_slave_recebido = data_rx[3];
  node_slave_recebido = node_slave_recebido += data_rx[4];
  Serial.print("node_slave_recebido = ");
  Serial.println(node_slave_recebido);

  node_reg_recebido = data_rx[6];
  node_reg_recebido = node_reg_recebido += data_rx[7];
  Serial.print("node_reg_recebido = ");
  Serial.println(node_reg_recebido);
     
  if (node_id_recebido == recuperadoID)
  {  
     String buf = "";;
     //buf = data_rx[6];
     buf = data_rx[7];
    
     
     //Serial.print("Buf [0] ");
     //Serial.println(buf[0]);
     //Serial.print("Buf [1] ");
     //Serial.println(buf[1]);
              
     //data_rx.toCharArray(buf, 2);
     //node_reg_recebido_int = atoi(buf[1]);
     Serial.print("node_reg_recebido_int = ");
     Serial.print(buf);
     Serial.print("  Size = ");
     Serial.println(buf.length());
     
     
    if (!mb.slave())
    {
      uint16_t res[2];                                   //uint16_t res[REG_COUNT]     
      data_rx = mb.readHreg(2, 2, res, 1, cb);           //mb.readHreg(SLAVE_ID, FIRST_REG, res, REG_COUNT, cb);
      while(mb.slave()) 
      {                                                  // Checa transmissao ativa.
        mb.task();
        delay(10);
      }   
      Serial.print("Slave ");
      Serial.print(node_slave_recebido);
      Serial.print(" --> Registro ");
      Serial.print(buf);
      Serial.print(" = "); 
      Serial.println(res[1]);
      Serial.println(data_rx);
      
      Serial.println("--------------");
      }
    }  
  
  data_rx="";
  node_id_recebido ="";
  node_slave_recebido = "";
  node_reg_recebido = "";
  node_reg_recebido_int = 0;
  }
  //mb.task(); 
  //yield();    
    
}  
