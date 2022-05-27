# Lora485
----------------------------
Uma interface entre RS485 (modbus) e LoRa com ESP32, MAX485 e Transceiver Ebyte E32 100mW.



# Descrição
----------------------------
Este projeto surgiu da necessidade de obter dados de dispositivos que possuam interface para comunicação de dados RS485 (por rede modbus) e posterior envio
das mesmas via rádio LoRa.

Ainda em desenvolvimento, visa integrar uma rede de nós LoRa ao gateway LoRa principal (o master). Quando completo, o master poderá efetuar requisições para cada um dos nós e enviará tais dados para um servidor rodando um broker MQTT na nuvem. O master receberá configurações de quais nós ele quer dados e de quais dados ele deseja receber, bem como o intervalo de amostragem. 

# Aplicações Possíveis
---------------------------
Este projeto poderá ser aplicado principalmente em ambientes rurais e industriais onde sensores RS485/modbus precisem ser lidos, mas que o local não disponibilze infraestrutura de energia ou rede de internet (WiFi). São exemplos: 

Leitura de sensores de umidade de solo, temperatura do ar, dados de estações meteorológicas remotas, realização de acionamentos como irrigação, sensores industriais como de vibração, temperatura, etc. 




