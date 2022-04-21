#include "BluetoothSerial.h"
#include "ELMduino.h"


BluetoothSerial SerialBT;
#define ELM_PORT   SerialBT
#define DEBUG_PORT Serial


ELM327 myELM327;

uint8_t remoteAddress[] = {0x00,0x1d,0xa5,0x00,0x04,0x5c};


uint32_t rpm = 0;


void setup()
{
#if LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif

  DEBUG_PORT.begin(115200);
  SerialBT.setPin("1234");
  ELM_PORT.begin("ArduHUD", true);

  DEBUG_PORT.println("begin");
  
  if (!ELM_PORT.connect(remoteAddress))
  // if (!ELM_PORT.connect("OBDII"))
  {
    DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 1");
    while(1);
  }

  if (!myELM327.begin(ELM_PORT, true, 2000))
  {
    Serial.println("Couldn't connect to OBD scanner - Phase 2");
    while (1);
  }

  Serial.println("Connected to ELM327");

  Serial.println("Check ECU");
  myELM327.rpm();
  if (myELM327.nb_rx_state != ELM_GETTING_MSG){
    Serial.println("Couldn't connect to ECU");
    while (1);
  }
}


void loop()
{
  float tempRPM = myELM327.rpm();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    rpm = (uint32_t)tempRPM;
    Serial.print("RPM: "); Serial.println(rpm);
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
    myELM327.printError();
}
