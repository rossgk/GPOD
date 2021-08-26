/*********************************************************************
                 GPOD GROWLIGHT V3P  OLED DISPLAY FUNCTIONS 
 
*********************************************************************/


// Explicit prototype block - required due to how IDE compiles 
void WakeupScreenGo( Adafruit_SSD1306 &myOLED);   
bool wakeDIP_check(Adafruit_SSD1306 &dispo );  
bool TimeSetInput(Adafruit_SSD1306 &dispo );      
void showTimeOLED(byte placeSpot, float tvalu, Adafruit_SSD1306 &theDisp );   
void doDIP_show( Adafruit_SSD1306 &theDisp );  
void showDayPeriod(byte phazeValue,  Adafruit_SSD1306 &myOLED );   
// Complete Explicit prototypes



 
void WakeupScreenGo( Adafruit_SSD1306 &myOLED) {
  // Identify the device's name and function to the user
  //
  
  myOLED.clearDisplay();

  myOLED.setTextSize(3);                 
  myOLED.setTextColor(WHITE);
  myOLED.setCursor(0, 10);
  // Display static text
  myOLED.println("GROWPOD");    

  // Add second line of smaller text
  myOLED.setTextSize(2);                 
  myOLED.setTextColor(WHITE);
  myOLED.setCursor(0, 38);
  // Display static text
  myOLED.println("Grow Light");
  
  myOLED.display();  // Show accumulated stuff
  
  wakeDIP_check(myOLED);
  
}
 
 
bool TsetScreenGo( ) {
    // Show the time-setting display to the user
     
    bool goodResult = true; 
    
    Adafruit_SSD1306 myOLED = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);     //    << LOCAL scope 
   
    if(!myOLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  
        goodResult = false;
    }
    else {
        // only if goodResult on allocation
        
        myOLED.clearDisplay();   // clears inner adafruit logo 
        delay(50);   
        myOLED.clearDisplay();   //  
      
        myOLED.setTextSize(2);                 
        myOLED.setTextColor(WHITE);
        myOLED.setCursor(0, 10);
        // Display static text
        myOLED.println("Set time:");    
        
        myOLED.display();   
        delay(50);  
 
        if (!TimeSetInput(myOLED))  goodResult = false;        
    }
     
    
    return goodResult;
}
 
 
void niceTime(char* outTime, float tval) {
    // Return a nicely formatted time stamp
    // passing in a buffer instead of returning

 
    #define TFMT_SZ 9                     // formatted time chars  8chars + eol
    #define BASETEN 10                    // for itoa conversions
    
    #define ASC_ZER 0x30                  // zero
    #define ASC_SPC 0x20                  // A space

    
    const int NOON = 12;
    const int MINS_PHR = 60;
    const char PMTAG[] = "PM\0";                         //  
    const char AMTAG[] = "AM\0"; 
    bool itsPM = false;
    
    char timeStp[9] = "";                                //  
    int hrsPart;                                         //  
    char hrSTR[3] ="";                                   //  
    int minsPart;                                        //  
    char mnSTR[4] ="";                                   //  

 
    hrsPart = int(tval);                                //  
    if (hrsPart >= NOON) {
        itsPM = true;                                   //  
        if (hrsPart > NOON) hrsPart = hrsPart - NOON;   //  
    } 
    
    minsPart = (int)((tval - (int)tval) * MINS_PHR);    //   
      
    if (hrsPart == 0) hrsPart = 12;                     //  
       
    itoa(hrsPart, hrSTR, BASETEN);                      //  
    if (hrsPart < 10 ) {                                //  
      // leading space
      hrSTR[1] = hrSTR[0];                              // 
      hrSTR[0] = ASC_SPC;
    }

 
    strcat(timeStp, hrSTR);                             //  
    
    //itoa(minsPart, mnSTR, BASETEN);                   //  
    niceDigi( (char*)mnSTR, minsPart );                 //  
    strcat(timeStp, mnSTR);
    
    if (itsPM) strcat(timeStp, PMTAG);
      else strcat(timeStp, AMTAG);
 
    strcpy(outTime, timeStp);                           //  
    
}


void niceDigi( char* buff, int digits) {
  // take an integer time element and prepend a '0' if it's less than 10 

  #define ASC_COL 0x3A                     // 
  char digs[3];
  *buff = ASC_COL;                         // 
  buff++;                                  //  

  if(digits < 10) {
    *buff = ASC_ZER;
    buff++;                                //  
  } 
  itoa(digits, digs, BASETEN);             //  
  strcat(buff,digs);
}


byte getDayPhase(float timeVal) {
    // Given a time of day in decimal 24hr time
    // determine the phase of the day
    // returns a byte defining sunrise, daylight, sunset or night-time.

    byte dayPhz = 0x00;
 
    if (timeVal < SUN_UP)          dayPhz =   NIGHT;      // before 5am  -> night
      else if (timeVal < LITE_TM)  dayPhz = SUNRISE;      // after night but before 7am  -> sunrise 
      else if (timeVal < SUN_DN)   dayPhz = DAYTIME;      // after sunrise, but before 19:00  -> daylight 
      else if (timeVal < DARK_TM)  dayPhz =  SUNSET;      // after daylight, before 21:00 -> sunset
      else                         dayPhz =   NIGHT;      // after sunset -> night time again. 
 
    return dayPhz;
}

 
void showTimeOLED(byte placeSpot, float tvalu, Adafruit_SSD1306 &theDisp ) {
  // Put the time on the display in the appropriate spot
  // SPOTS are setup spot, or operational info spot. 
  // a negative time value means no setting has been done, show 12:00 for that

  // Y-coords SETUP_ROW and INFO_ROW are declared in defines, and used in other modules too
  #define SETUP_X    10
  #define INFO_X     35                   //  

  char timeTxtBuff[]= "        \0";       //  
 
  int cursX = 0;
  int cursY = 0;
  const int BOXR = 80;                    //  
  const int BOXT = 12;                    // 

  theDisp.setTextSize(2);                 // 
  theDisp.setTextColor(WHITE);

  if (placeSpot == SETUP_ROW) { 
      theDisp.setCursor(SETUP_X, SETUP_ROW);
      cursX = SETUP_X;
      cursY = SETUP_ROW;
      
  } else if (placeSpot == INFO_ROW) {
      //display.setCursor(INFO_X, INFO_ROW);
      theDisp.setCursor(INFO_X, INFO_ROW);
      cursX = INFO_X;
      cursY = INFO_ROW;
  }

  theDisp.setCursor(cursX, cursY);            //  
  theDisp.fillRect(cursX, cursY, cursX + BOXR, cursY + BOXT, BLACK);  // 
  theDisp.display();                          //  
  theDisp.setCursor(cursX, cursY);            //  
  
  // Display static text
  if (tvalu < 0.0) theDisp.println("12:00");
  
  else {
     niceTime(timeTxtBuff, tvalu);           // 
     theDisp.println(timeTxtBuff);
  }
  theDisp.display();
   
}




void doDIP_show( Adafruit_SSD1306 &theDisp ) {
  // Read the DIP switches and show them on the display
  // We get here if user was holding button down during start-up screen for 1500 mS
    

  String D1_txt, D2_txt, D3_txt, D4_txt;
  String txtRPT = "";
  
  theDisp.clearDisplay();                 //  

  delay(50);                              // 

 
  if (D1_val) 
    D1_txt = "L";                         // NB Strings
  else 
    D1_txt = "H"; 
   
  if (D2_val) 
    D2_txt = "L";  
  else 
    D2_txt = "H"; 
    
  if (D3_val) 
    D3_txt = "L";  
  else 
    D3_txt = "H"; 
    
  if (D4_val) 
    D4_txt = "L";  
  else 
    D4_txt = "H"; 
 
  txtRPT = " " + D1_txt + " " + D2_txt + "  " + D3_txt +  " " + D4_txt;        

  // Now put it on the display
 
  theDisp.setTextSize(1);                 //  rev info
  theDisp.setTextColor(WHITE);
  theDisp.setCursor(70, 0);               //  
  theDisp.println(SWREV);
  
  theDisp.setTextSize(2);                 //  
  theDisp.setTextColor(WHITE);
  theDisp.setCursor(0, 8);

  // Display static text
  theDisp.println("Chan Prog:");          //  
  theDisp.display();   

  // Option 1 for a display configuration
  theDisp.setCursor(0, 32);               //  
  theDisp.println(" 1 2  3 4");           //  
  theDisp.println(txtRPT); 
  
  theDisp.display();                      //  

  delay(DIPVIEW);
  
  return;
}



bool wakeDIP_check(Adafruit_SSD1306 &dispo ) {
    // If a button is held upon wakeup for DIPRQ_TIME, then show DIP switch state.
    // NB timeNow is a global  

    bool goodChk = true;
    
    unsigned long timeout = 0;                            //  
    int showButt = HIGH;                                  //  
    const unsigned long holdMin = DIPRQ_TIME;             //  
    
    unsigned long snapTime = millis() + holdMin;          //  
                
    showButt = digitalRead(TIMEPIN);                      //  
 
    while ((showButt == LOW) && (timeout < snapTime)) {    //   
        showButt = digitalRead(TIMEPIN);                   //  
        timeout = millis();                                //  
    }
     

    if ((showButt == LOW) && (timeout >= snapTime)) {
         // timed out while button down, so put time DIP info on display
         
         doDIP_show( dispo);  // update shown value
         
    } else {
        // Either button was release before timeout, or button wasn't LOW to begin with. 
        // Either way, no show request to handle; Weird button press
   
        goodChk = false;
    }

    
    return goodChk;
}

 
bool TimeSetInput(Adafruit_SSD1306 &dispo ) {
    // Allow users to set the time, so our day-night cycles will be roughly real-time sync'd
    // timeNow is a global  
   
    int timeout = 0;                //  
    int timeAdj = HIGH;             //  

    bool goodSet = true;            //  
    
    float decimalTime = timeNow;    //  
 

    showTimeOLED(SETUP_ROW, decimalTime, dispo);         // 
    
    while (timeout < TIMEOUT_PWZ) {                       //   
       
        timeAdj = digitalRead(TIMEPIN);                   //  
        if (timeAdj == LOW) {
           if (decimalTime < 0.0) decimalTime = 0.0;      //  
           
           decimalTime += TINCREM;                        //  
           timeout = 0;                                   //  

           // put time chosen on display
           showTimeOLED(SETUP_ROW, decimalTime, dispo);   //  
   
           delay(150);                                    //  
        }
        // roll over after 11:45 pm. next is midnight aka 0.0
        if (decimalTime >= HRSDAY) decimalTime = 0.0;     //  
        
        
        delay(TSLOW);                                     //   
         // don't timeout unless user interacted
        if (decimalTime > 0.0)  timeout += 1;             //  
    }
 
    if ((decimalTime <= 0.0) || (decimalTime > 25.0)) {
        goodSet = false;                                  //  
                                                          //  
    }
    else {
        globalMillis = millis();                          //  
        timeNow = decimalTime;                            //  
    }
   
    return goodSet;
}

 
  
 
void showDayPeriod(byte phazeValue,  Adafruit_SSD1306 &myOLED ) {
    // show the period of the day currently.
    // Use an indicator of the phase passed along, 
    // Assoc with phase names as in local char array 
      
    #define PHSPOT_X   35       
    #define PHSPOT_Y   46
 
    //   8chars each  >KEEP EQUAL LENGTHS 
    const char phazes[5][8] =  { "STARTUP\0",          // compiler complains - doesn't see \0 as a single char  (warn only)
                                "DayTime\0", 
                                "  Night\0", 
                                "Sunrise\0", 
                                " Sunset\0"           // Right Justified 8char incl eol
                               };
                               
    myOLED.setTextSize(2);                            //  
    myOLED.setTextColor(WHITE);
    myOLED.setCursor(PHSPOT_X, PHSPOT_Y );            //  
    
    myOLED.println( phazes[phazeValue] );             //  
    myOLED.display();
                               
    delay(1);
}
 
bool PrepRunScreen(byte dayFaze ) {
    // Place MASTHEAD on the run screen.
    // Below the GPOD name masthead, we show period, and time
    // Period can be any of Day, night, sunset or sunrise

    bool goodAlloc = true;
  
    Adafruit_SSD1306 myOLED = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  
     
    if(!myOLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  
 
       goodAlloc = false;                            
    }
  
     myOLED.clearDisplay();
     myOLED.setTextSize(2);                          
     myOLED.setTextColor(WHITE);
     
     myOLED.setCursor(0, 5);
     myOLED.print("GPOD");          
     
     myOLED.setTextSize(1);                          

     if (togglebit) {
          myOLED.setCursor(75, 5);                   
          myOLED.println("RUNNING"); 
      } else { 
          myOLED.setCursor(75, 13);                  
          myOLED.println("RUNNING");                
      }
      
     togglebit = !togglebit;                         
     showTimeOLED(INFO_ROW, timeNow, myOLED);        

     showDayPeriod(dayFaze, myOLED);                 
    
     return goodAlloc;                               
}
 
   
bool showLeafLogo( ) {
  // Our startup logo - a leaf
  // show it on the display
  bool goodAlloc = true;
   
  Adafruit_SSD1306 mydispl = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  
  
  if(!mydispl.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {          
    
   }
   else {
      
        mydispl.clearDisplay();
        mydispl.display();
        
        mydispl.drawBitmap(
          (mydispl.width()  - LOGO_WIDTH ) / 2,
          (mydispl.height() - LOGO_HEIGHT) / 2,
          logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
        mydispl.display();
        delay(LOGO_SHOW_TME); 
        
        mydispl.invertDisplay(true);                       
        delay(LOGO_SHOW_TME);                              
        mydispl.invertDisplay(false);
        delay(LOGO_SHOW_TME);

        WakeupScreenGo(mydispl);
   }
   
  return goodAlloc;          
}
 
