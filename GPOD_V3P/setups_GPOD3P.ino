/*********************************************************************
           SETUP section -  GPOD GROWLIGHT V3P
 
*********************************************************************/
//#define SERIAL_CHK    

void setup() {
 
 
  #if defined (SERIAL_CHK)  // print WORKS during debug-print OFF testing, pre-production
    Serial.begin(19200);
 
    Serial.println(F("\n\n *** **************************************** ***"));
    Serial.println(F(    " ***               GROW LIGHT SYSTEM GPOD     ***"));
    Serial.print(F  (    " ***  rev " );
    Serial.print(               SWREV));
    Serial.println(F(               " SERIAL_CHECK MODE...              ***\n"));
  #endif
  
  pinMode( TIMEPIN, INPUT_PULLUP);           // Time adjust request button
  
  pinMode(SBANK_DAT, OUTPUT);                // Instantiating PixelStrips prob do this but ensure they're set  
  pinMode(RBANK_DAT, OUTPUT);
  pinMode(AUXA_DAT, OUTPUT);
  pinMode(AUXB_DAT, OUTPUT);


  pinMode(DIP1, INPUT_PULLUP);               // DIP switches are wired to GND. Will read HI unless flipped on
  pinMode(DIP2, INPUT_PULLUP);
  pinMode(DIP3, INPUT_PULLUP);
  pinMode(DIP4, INPUT_PULLUP);

  delay(50);
  
  // Get the DIPswitch info into globals
  D1_val = (bool)digitalRead(DIP1);
  D2_val = (bool)digitalRead(DIP2);
  D3_val = (bool)digitalRead(DIP3);
  D4_val = (bool)digitalRead(DIP4);
  
    
  GSTATE = STARTS;                            // configure the STATE MACHINE before we finish setup

}
