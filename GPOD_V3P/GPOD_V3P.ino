/*********************************************************************
GPOD GROWLIGHT_V3P

   Drives OLED in 128 x 64 size.
   Receives input from one button on pin 6 (TIMEPIN)
   Serial-Addressable LEDs (eg NEOPIXEL) are generally avail in strips or rings

     - eg See Adafruit shop...  https://www.adafruit.com/category/168 and other online sources

   As of last update: uses 21128 bytes of pgm space and 455 bytes of dynamic memory.
   
 
   Date               Details
   Jan 19, 2021 etc   Earlier details deleted
   July 29, 2021      Prep for share via Tindie 

   Aug  26, 2021      Minor Updates
   
 
*********************************************************************/
   
// FOR THE OLED DISPLAY LIBRARY USE
#include <Wire.h>                    //   
#include <Adafruit_GFX.h>            //  Ensure you have the Adafruit libraries installed 
#include <Adafruit_SSD1306.h>        //  In Arduino IDE:  Tools > Manage Libraries... 
 
 
// FOR THE NEOPIXEL STRIPS
#include <Adafruit_NeoPixel.h>
 
 
