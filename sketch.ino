#include <IRremote.h>

const byte IR_RECEIVE_PIN = 12;

void setup()
{
   Serial.begin(9600);
   Serial.println("IR receive test");
   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);// onboard LED blinks with received IR signal
}

void loop()
{
   if (IrReceiver.decode())
   {
      unsigned long keycode = IrReceiver.decodedIRData.command;
      Serial.println(keycode, HEX);
      if ((IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) // ignore repeat code
      {
         IrReceiver.resume();
         return;
      }
      IrReceiver.resume();
   }
}
