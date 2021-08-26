/*********************************************************************
                 GPOD GROWLIGHT V3P  Main LOOP 
 
*********************************************************************/
 
void loop() {
  // Our main line loop
  loopCt += 1;
  unsigned long newMillis;
  char prettyTime[9];                                    
  byte nowPhase = 0x00;                                  
 
  int butPush = LOW;                                     
  bool updateTime = false;                              

  bool goodStateWork = false;
 
 
  switch (GSTATE) {
 
    case STARTS:
        // Initialization state. Show a logo and splash screen. 
        // Also checks for button press and show DIP switches if held
        
        if (newState) newState = false;                       
 
        goodStateWork = showLeafLogo();                     

        // Scan thru all banks, with DIP-chosen settings
        scanLeftRight(SBANK_DAT, 40, 1);                   
        delay(100);
        scanLeftRight(RBANK_DAT, 40, 1);                   
        delay(100);

 
        if (goodStateWork)
            GSTATE = TIMESET;                                 
        else
            GSTATE = FAIL_IDLE;                              
            
        newState = true;                                      

        break;



     case TIMESET: 
         // Have user set time before anything else can proceed
         if (newState){
 
              newState = false;
         }
 
         goodStateWork = TsetScreenGo();                   
      
 
         if (goodStateWork) {  
           GSTATE = RUNNER; 
           newState = true;                                
         } else  {
           GSTATE = FAIL_IDLE; 
           newState = true;                               
         }
            
         break;

 

    case RUNNER:
          // Once time is set we spend most of our time in the RUNNER state. 
          // A bit of sanity checking and watching for a new timeset request
          if (newState){ 
 
              newState = false;
              
               #ifdef SERIAL_CHK   
                 Serial.println(F("  RUN  state entry"));
               #endif
          }
 
          // every loop in this state
          niceTime(prettyTime, timeNow);                        //  
          
          if (loopCt % OCCASIONAL == 0) {                       //  
            
 
              if ((timeNow < 0.0) || (timeNow >= 25.0))  {      //  
                  goodTime = false;                             //  
                  GSTATE = FAIL_IDLE;                           // 
                  
 
              } else goodTime = true;                            //  

             // flicker the running label for visual effect. 
             nowPhase = getDayPhase(timeNow);                    //  
             if (!PrepRunScreen(nowPhase)) GSTATE = FAIL_IDLE;   //  
          
          }

          // Check for stale Run Screen and update time/phase if required      

          if (strcmp (last_timeTXT, prettyTime) != 0) {           //  
              nowPhase = getDayPhase(timeNow);                    //                    
              if (!PrepRunScreen(nowPhase)) GSTATE = FAIL_IDLE;   //   

              if (lastPhase != nowPhase) {                        //    
                    lightThosePixels(nowPhase, RAMP_BRITE);       //  
               
                    lastPhase = nowPhase;                         //  
               } 
               
              strcpy(last_timeTXT, prettyTime);                   //  
           }
      
            
            butPush = digitalRead(TIMEPIN);                       //  
            while (butPush == LOW) {                              //  
                   
                    updateTime = true;
                    butPush = digitalRead(TIMEPIN);               //  
                    delay(25);                                    // 
                }
                
            if (updateTime){
                updateTime = false;

                if (!PrepRunScreen(nowPhase)) GSTATE = FAIL_IDLE;  // 
                 
                GSTATE = TIMESET;                                  //  
                newState = true;                                   //
            }
        
          break;

    case FAIL_IDLE:
          // Problems = molasses
 
          delay(10000);          
          
          break;

    default: {                      // watch bracket use in default          
          delay(1000); 
          }
          
          break;
    
  }   
 
  
  // EVERY loop, independent of state machine
  if (goodTime) {
      newMillis = millis();                       
      
      timeNow = timeNow + (float)((newMillis - globalMillis)/(MSEC * HRSEC));      
      globalMillis = newMillis;                    
  
      if (timeNow >= HRSDAY) {
        timeNow = timeNow - HRSDAY;                
      }
  }     

  delay(LOOP_REGULATOR);                       
 
}
 
