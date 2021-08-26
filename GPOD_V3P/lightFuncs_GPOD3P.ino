/*********************************************************************
                 GPOD GROWLIGHT V3P   LED LIGHT CTL FUNCTIONS 
 
*********************************************************************/
  
  

uint8_t makeFlags( bool DIPstate ) {
    // Make the flags used to instantiate the pixel strip
    // We do it often, so let's not repeat ourselves
    // The var passed is the DIP Switch setting - OFF (HIGH, true) for RGBW, ON (LOW, false) for RGB
    
    uint8_t pixelFMT;
    
    if ( DIPstate == PGMDIP_ON ) {                  //    
      pixelFMT = NEO_KHZ800 + NEO_RGB;              // 
      
    } else if (DIPstate == PGMDIP_OFF ) {          //  
      pixelFMT = NEO_KHZ800 + NEO_RGBW;            //  
    }
    return pixelFMT;
}

void scanLeftRight( int bankID, uint8_t sWait, uint8_t reps ) {           //  
    // TwoPixels illuminated moving forward then backwards 

    uint32_t  CLR;                            //  
    int drvPin;
    int pixct;
    uint8_t typeRef;                          // .h file line 129 says 8bit sufficient for 800khz pixelstrips
    //uint32_t FLIP_CLR;
 
    if (bankID == SBANK_DAT) {
      pixct = SBANK_PIXELS;
      drvPin = SBANK_DAT;                     // a bit redundant, but I might wanna chg selector later so hold for now
      typeRef = makeFlags(D1_val);            // 
      
 
    } else if (bankID == RBANK_DAT) { 
      pixct = RBANK_PIXELS;
      drvPin = RBANK_DAT; 
      typeRef = makeFlags(D2_val);            // 
 
      
    } else if (bankID == AUXA_DAT) { 
      pixct = AUXA_PIXELS;
      drvPin = AUXA_DAT; 
      typeRef = makeFlags(D3_val);            //  
 
      
    } else if (bankID == AUXB_DAT) { 
      pixct = AUXB_PIXELS;
      drvPin = AUXB_DAT; 
      typeRef = makeFlags(D4_val);            //  
 
    }
 
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixct, drvPin, typeRef );

    strip.begin();
    strip.show();                               //  
  
    CLR = strip.Color(80, 255, 0);              // 
    //FLIP_CLR = strip.Color(255, 80, 0);       // 
     
    strip.setBrightness(LO_BRITE);             //  
  
    for( byte rp=0; rp<reps; rp++) { 
 
          for(int i=0; i< pixct; i++) {
 
              strip.setPixelColor(i, CLR);
              if ((i-1) >= 0) strip.setPixelColor(i-1, PIXOFF);                   
                 
              strip.show();                                          
              delay(sWait);     // persistence   
          }
  
          // REVERSE direction
          for(int i = pixct-1; i >= 0; --i) {
 
              strip.setPixelColor(i, CLR);
              if (i+1 < pixct) strip.setPixelColor(i+1, PIXOFF);    // 
                
              strip.show();                                         // 
              delay(sWait);                                         // persistence   
          }
          strip.setPixelColor(0, PIXOFF);                           //  
          strip.show(); 
 
      }     

    return;
}   

 


void lightMeUp( int pxDataPin, bool pixDIPSET, int pixCount, byte phaseSet, int brightSEL) {
    // illuminate our pixel strips given selectors passed from lightThosPixels()
    // e.g Do a direct call with constants: SBANK_DAT, STRP_CHOICE, SBANK_PIXELS, DAYTIME
    
    uint8_t dataFMT;                              // 
    uint32_t go_CLR;                              //  
 
    dataFMT = makeFlags( pixDIPSET );
 
    // in scope declaration
    Adafruit_NeoPixel pxStrip = Adafruit_NeoPixel(pixCount, pxDataPin, dataFMT );        

    // initialize the instantiated strip
    pxStrip.begin();
    pxStrip.show();                                
    delay(10);          
    
    if (phaseSet == DAYTIME)         go_CLR = pxStrip.Color( PURP_CLR );       // Daytime UV-ish purplse
      else if (phaseSet == SUNRISE)  go_CLR = pxStrip.Color( PGOLD_CLR);       // Sunrise Gold    etc
      else if (phaseSet == SUNSET)   go_CLR = pxStrip.Color( PRED_CLR );       // Sunset Reddish 
      else if (phaseSet == NIGHT)    go_CLR = pxStrip.Color( POFF_CLR );       // nothing at all
      else                           go_CLR = pxStrip.Color( POFF_CLR );       // nothing at all < as default for if stmt
  
    pxStrip.setBrightness(LO_BRITE);                 // 

    // Fill colour all-at-once
    pxStrip.fill(go_CLR);                            // one-line colour invoke
    pxStrip.show();
    
    // Manage up brightness choices
    
    if (brightSEL == LO_BRITE) {
 
    } else if (brightSEL == MED_BRITE){
        pxStrip.setBrightness(MED_BRITE);  
        pxStrip.fill(go_CLR);   
        pxStrip.show();  
 
    } else if (brightSEL == HIGH_BRITE){
      pxStrip.setBrightness(HIGH_BRITE);
      pxStrip.fill(go_CLR);   
      pxStrip.show();  
   
    } else if (brightSEL == RAMP_BRITE) {
   
          delay(BRITE_RAMP);                  //     
          pxStrip.setBrightness(MED_BRITE);  
          pxStrip.fill(go_CLR);   
          pxStrip.show();  
          
          delay(BRITE_RAMP);                  // 
          pxStrip.setBrightness(HIGH_BRITE);
          pxStrip.fill(go_CLR);   
          pxStrip.show();  
    }
 
}


void lightThosePixels( byte thisPhase, int BRITEchoice ) {
    // Based on the current phase of the day, select pixel strips for illumination
    // Uses an array of currently installed neoPixel hardware as defined in DEFs file
    // Format is { DataPinReference, typeOfpixelStrip, LEDCount } for each element
    // Number of LED sets is in const  NUM_PXSETS
    
    const int NUM_PXSETS = 4;                            
    
    const int PIXEL_SET[NUM_PXSETS][3] =  {          
          //   DataPin, data fmt bool-as-int, numPixels   <-- array format
          { SBANK_DAT, (int)D1_val, SBANK_PIXELS },     //  
          { RBANK_DAT, (int)D2_val, RBANK_PIXELS },     //  
          { AUXA_DAT,  (int)D3_val, AUXA_PIXELS },      //  
          { AUXB_DAT,  (int)D4_val, AUXB_PIXELS },      //  
          };

    // NEED TO look up type for each bank based on GLOBALs of DIP settings.
    for (int i = 0;  i < NUM_PXSETS; i++ ) {

       //                 DataPin,  DIP-set type bool,     pixel Count,     phase    brightness       << var specifics
       lightMeUp( PIXEL_SET[i][0], (bool)PIXEL_SET[i][1], PIXEL_SET[i][2],  thisPhase, BRITEchoice );         
       delay(10);      
        
    }
  
}
