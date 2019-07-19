// Import libraries
#include "LiquidCrystal_I2C.h"
#include "FlexiTimer2_local.h"
#include "PID_v1_local.h"
#include "MAX31855_local.h"

// Version
#define CODE_MAJOR_VERSION     1  // major version
#define CODE_MINOR_VERSION     0  // minor version

// LCD settings
#define LCD_COLS              16  // columns
#define LCD_ROWS               2  // rows

// Pin settings
#define PINS_SSR               6
#define PINS_TEMP_SO           9
#define PINS_TEMP_CS           8
#define PINS_TEMP_CLK          7
/*
#define PINS_LCD_LED           13
#define PINS_LCD_RS            A2
#define PINS_LCD_ENABLE        A1
#define PINS_LCD_D4            A0
#define PINS_LCD_D5            12
#define PINS_LCD_D6            11
#define PINS_LCD_D7            10
*/
#define PINS_BUZZER            5
#define PINS_BTN_A             2
#define PINS_BTN_B             3

// Custom characters and symbols addresses at lcd eeprom
#define SYMBOL_DEGREE         0x00 // Degree

// Keycodes
#define KEY_NONE             0 // No keys pressed
#define KEY_A                1 // Button A was pressed
#define KEY_B                2 // Button B was pressed
#define KEY_AH               3 // Button A was pressed and holded (KEY_HOLD_TIME) milisecons
#define KEY_BH               4 // Button B was pressed and holded (KEY_HOLD_TIME) milisecons
#define KEY_ABH              5 // Buttons A+B was pressed and holded (KEY_HOLD_TIME) milisecons

// Keyboard times
#define KEY_DEBOUNCE_TIME    30 // debounce time (ms) to prevent flickering when pressing or releasing the button
#define KEY_HOLD_TIME       400 // holding period (ms) how long to wait for press+hold event
#define KEY_HOLD_TIME_WAIT  100 // Used for double key holding

// PID values
#define PID_SAMPLE_TIME    1000     // Milliseconds
#define PID_KP_PREHEAT      100.000
#define PID_KI_PREHEAT        0.025
#define PID_KD_PREHEAT       20.000
#define PID_KP_SOAK         300.000
#define PID_KI_SOAK           0.050
#define PID_KD_SOAK         250.000
#define PID_KP_REFLOW       300.000
#define PID_KI_REFLOW         0.050
#define PID_KD_REFLOW       350.000

// Minimun temperature to start a profile
#define ROOM_TEMPERATURE          40

#define REFLOW_PROFILE_LEADED      0
#define REFLOW_PROFILE_LEADFREE    1

#define REFLOW_STAGE_PREHEAT       0
#define REFLOW_STAGE_SOAK          1
#define REFLOW_STAGE_REFLOW        2
#define REFLOW_STAGE_COOL          3

// Struct to hold the reflow stage parameters.
typedef struct ReflowStage {
  const char* name;
  double targetTemperature;
  int durationInSeconds;
  int elapsedTime;
  double pid_kp;
  double pid_ki;
  double pid_kd;
} ReflowStage_t;

// Array to hold the individual reflow stages together as a complete reflow profile
ReflowStage currentReflowProfile[5];

// PID variables
double pid_setPoint;
double pid_input;
double pid_output;
double pid_kp = PID_KP_PREHEAT;
double pid_ki = PID_KI_PREHEAT;
double pid_kd = PID_KD_PREHEAT;

// Specify PID control interface
PID reflowOvenPID(&pid_input, &pid_output, &pid_setPoint, pid_kp, pid_ki, pid_kd, DIRECT);

// Thermocouple object instance
MAX31855 thermocouple(PINS_TEMP_SO, PINS_TEMP_CS, PINS_TEMP_CLK);

// LiquidCrystal LCD control object instance
//LiquidCrystal lcd(PINS_LCD_RS, PINS_LCD_ENABLE, PINS_LCD_D4, PINS_LCD_D5, PINS_LCD_D6, PINS_LCD_D7);
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

// Variables used on interrupt mode
volatile boolean cancelFlag = false;    // Flag used to abort interrupt mode

// Global variables
uint8_t lastKey = KEY_NONE;     // Last key pressed
boolean flagHoldKey = false;    // Flag to ignore keys after a hold key
int currentStage = 0;
int timerSeconds = 0;

// Setup before start
void setup()
{
  // Create custom LCD char for degree symbol
  uint8_t char_degree[8]  = {
      B01100,
      B10010,
      B10010,
      B01100,
      B00000,
      B00000,
      B00000,
      B00000
  };

  // Initialize LCD
  // lcd.begin( LCD_COLS,  LCD_ROWS);
  lcd.begin();
  lcd.createChar(SYMBOL_DEGREE, char_degree);

  // Pinmode inputs
  pinMode(PINS_BTN_A, INPUT_PULLUP);
  pinMode(PINS_BTN_B, INPUT_PULLUP);

  // Pinmode outputs
  /*
  pinMode(PINS_LCD_LED,     OUTPUT);
  pinMode(PINS_LCD_RS,      OUTPUT);
  pinMode(PINS_LCD_ENABLE,  OUTPUT);
  pinMode(PINS_LCD_D4,      OUTPUT);
  pinMode(PINS_LCD_D5,      OUTPUT);
  pinMode(PINS_LCD_D6,      OUTPUT);
  pinMode(PINS_LCD_D7,      OUTPUT);
  */
  pinMode(PINS_BUZZER,      OUTPUT);

  // Turn on lcd backlight
  /*
  digitalWrite(PINS_LCD_LED, HIGH);
  */

  // Tell the PID to range between 0 and 255 (PWM Output)
  reflowOvenPID.SetOutputLimits(0, 255);

  // Turn the PID on
  reflowOvenPID.SetMode(AUTOMATIC);
  reflowOvenPID.SetSampleTime(PID_SAMPLE_TIME);

  // Put SSR to 0 duty
  analogWrite(PINS_SSR, 0);

  // Init serial
  Serial.begin(9600);
}

// Main loop
void loop()
{
  controller_run();
}
