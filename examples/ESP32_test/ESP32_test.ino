/*
To test connection, type the following into the serial monitor:
  AT Z
  AT E0
  AT S0
  AT AL
  AT TP A0
*/

#include "BluetoothSerial.h"


BluetoothSerial SerialBT;


#define DEBUG_PORT Serial
#define ELM_PORT   SerialBT

#define LED_BUILTIN 2

uint8_t remoteAddress[] = {0x00,0x1d,0xa5,0x00,0x04,0x5c};


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  DEBUG_PORT.begin(115200);

  // ELM_PORT.enableSSP();
  ELM_PORT.setPin("1234");
  ELM_PORT.begin("ArduHUD", true);

  DEBUG_PORT.println("Attempting to connect to ELM327...");

  if (!ELM_PORT.connect(remoteAddress))
  // if (!ELM_PORT.connect("OBDII"))
  {
    DEBUG_PORT.println("Couldn't connect to OBD scanner");
    while(1);
  }

  DEBUG_PORT.println("Connected to ELM327");
  DEBUG_PORT.println("Ensure your serial monitor line ending is set to 'Carriage Return'");
  DEBUG_PORT.println("Type and send commands/queries to your ELM327 through the serial monitor");
  DEBUG_PORT.println();
}


void loop()
{
  if (!ELM_PORT.connected()) DEBUG_PORT.println("no connect");
  if(DEBUG_PORT.available())
  {
    char c = DEBUG_PORT.read();

    DEBUG_PORT.write(c);
    ELM_PORT.write(c);
  }

  if(ELM_PORT.available())
  {
    char c = ELM_PORT.read();

    if(c == '>')
      DEBUG_PORT.println();

    DEBUG_PORT.write(c);
  }
}
