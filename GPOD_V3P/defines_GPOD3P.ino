/*
 *  D E F I N E S     F I L E    F O R     G P O D Growlight V3P
 * 
 *   The #defines and constants for our project
 * 
 */
#define SWREV "ver3.1P"

// COMPILE FLAGS
 
// DATA Pin numbers for banks of LEDs
#define SBANK_DAT 2             // serial data for the pixels in SBank 
#define RBANK_DAT 3             // drive RBank from pin 
#define AUXA_DAT  4             // Aux bank A's control signal pin
#define AUXB_DAT  5             // Aux bank B's control data pin

#define TIMEPIN 6               // Time Adjust Pushbutton on pin D6

// handy constant tags for the pixel selections
#define RBANK_PIXELS  40        // If you have more pixels on your set, ring, strip, adjust these.
#define SBANK_PIXELS  40        //  ... watch the current usage if you have many more pixels. 
#define AUXA_PIXELS   40        //  Anything more than 2Amps could be a power/heating problem
#define AUXB_PIXELS   40        //  

//constants
const int LO_BRITE   =   8;
const int MED_BRITE  =  80;
const int HIGH_BRITE = 240;
const int RAMP_BRITE  = -1;      // flag to do a ramp of the above, do not change

#define PIXOFF 0                 // EQUIV of setting an RGB of (0,0,0)

//DIP Switche Pins
const int DIP1 = 10;             // our DIP switches for programming data format
const int DIP2 = 9;              // Some serial LEDs have different colour set API
const int DIP3 = 8;
const int DIP4 = 7;              // NOTE - DIP component labels are opposite placement's

bool D1_val, D2_val, D3_val, D4_val;    // global 

const bool PGMDIP_ON = true;      //  
const bool PGMDIP_OFF = false;
  
#define DIPRQ_TIME 1500L         //  
#define DIPVIEW 20000            // 

#define SLOWLOOP   300           //  
#define ILLUM_PAUSE  100         //  
#define BRITE_RAMP  3000         //  
 
// OLED STUFF

#define SCREEN_WIDTH 128       
#define SCREEN_HEIGHT 64    

//#define TINCREM 0.25         //  
#define TINCREM 2.0            // FOR DEBUG TEST - faster

// Time settings, float
#define SUN_UP   5.0               // begin sunrise
#define LITE_TM  7.0               // day starts
#define SUN_DN  19.0               // sunset starts
#define DARK_TM 21.0               // night begins

#define HRSDAY 24.0            // duh

#define LOOP_REGULATOR 100   // 
#define OCCASIONAL 65        //  

#define TSLOW 80             //  
#define TIMEOUT_PWZ  90      // 

#define SETUP_ROW 42         //  
#define INFO_ROW  29         //  
bool togglebit = true;       //   

float timeNow = -1.0;        //  
  
#define OLED_RESET   -1      //  
 
// state machine setup
byte GSTATE = 0x00;                    //   
const byte NOTSETUP = 0x00;
const byte STARTS   = 0x01;            // start-up  initial state
const byte TIMESET  = 0x02;            // Set up the time state
const byte RUNNER   = 0x03;            // normal running state
const byte IDLEPROB = 0x04;            // Problem state, come here and shut stuff off.
const byte NEOPIX_TEST = 0x05;         // test use
const byte FAIL_IDLE = 0x06;           // Do nothing 
bool newState = true;                  // indicates we're in a new state, so first action can be diff

// Phase names
const byte DAYTIME = 0x01;         // daytime phase - light on full
const byte NIGHT   = 0x02;         // daytime phase - light off
const byte SUNRISE = 0x03;         // daytime phase - RAMP UP
const byte SUNSET  = 0x04;         // daytime phase - RAMP DOWN

byte lastPhase = 0x00;             //  

// Colour defns      R    G    B
#define PURP_CLR     10, 255, 180   // VIOLET
#define PGOLD_CLR    80, 255,   0   // WARM GOLD
#define PRED_CLR     40, 255,   0   // RED
#define POFF_CLR      0,   0,   0   // all off
 



char last_timeTXT[]= "NO_INFO \0";   
                                    

int loopCt = 0;
bool goodTime = false;              //  
unsigned long globalMillis = 0;
#define MSEC  1000.0                //  
#define HRSEC 3600.0 


// LOGO STUFF -->  Currently in PROGMEM space 

#define LOGO_HEIGHT   40
#define LOGO_WIDTH    40
 
const int LOGO_SHOW_TME = 1500;               // show logo for 1.5 second

// (Horizontal) byte array of LEAF bitmap of 40 x 40 px:
  static const unsigned char PROGMEM logo_bmp[] = { 
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
  0x0, 0x7c, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xf0, 0x0, 0x0, 0xff, 0xff, 
  0xe0, 0x0, 0x3, 0xff, 0xff, 0xc0, 0x0, 0xf, 0xff, 0xff, 0x80, 0x0, 
  0x1f, 0xff, 0xff, 0x0, 0x0, 0x3f, 0xff, 0xfe, 0x0, 0x0, 0x7f, 0xe7, 
  0xfe, 0x0, 0x0, 0xff, 0xcf, 0xfc, 0x0, 0x0, 0xff, 0x3f, 0xfc, 0x0, 
  0x1, 0xff, 0x3f, 0xfc, 0x0, 0x1, 0xfe, 0x7f, 0xf8, 0x0, 0x3, 0xfc, 
  0xff, 0xf8, 0x0, 0x3, 0xf9, 0xff, 0xf8, 0x0, 0x3, 0xf9, 0xff, 0xf8, 
  0x0, 0x3, 0xf3, 0xff, 0xf0, 0x0, 0x3, 0xf3, 0xff, 0xf0, 0x0, 0x3, 
  0xe3, 0xff, 0xf0, 0x0, 0x3, 0xe7, 0xff, 0xf0, 0x0, 0x3, 0xc7, 0xff, 
  0xe0, 0x0, 0x3, 0xcf, 0xff, 0xe0, 0x0, 0x1, 0x8f, 0xff, 0xc0, 0x0, 
  0x1, 0x8f, 0xff, 0xc0, 0x0, 0x0, 0x9f, 0xff, 0x80, 0x0, 0x0, 0x1f, 
  0xff, 0x80, 0x0, 0x0, 0x1f, 0xff, 0x0, 0x0, 0x0, 0x3f, 0xfe, 0x0, 
  0x0, 0x0, 0x27, 0xf8, 0x0, 0x0, 0x0, 0x20, 0x0, 0x0, 0x0, 0x0, 
  0x40, 0x0, 0x0, 0x0, 0x0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x0, 
  0x0, 0x0, 0x1, 0x80, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x0, 
  0x1c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

  
