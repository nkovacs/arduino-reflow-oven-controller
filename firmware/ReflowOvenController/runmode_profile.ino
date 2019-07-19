
volatile bool refresh_trigger;

// Run profile
void runAs_profile(int profile)
{
  display_printTitle(F("Running profile"));
  keyboard_waitForNokey();

  if (profile == REFLOW_PROFILE_LEADED) {
    currentReflowProfile[0].name = "Pre-heat";
    currentReflowProfile[0].targetTemperature = 190;
    currentReflowProfile[0].durationInSeconds = 90;
    currentReflowProfile[0].elapsedTime = 0;
    currentReflowProfile[0].pid_kp = PID_KP_PREHEAT;
    currentReflowProfile[0].pid_ki = PID_KI_PREHEAT;
    currentReflowProfile[0].pid_kd = PID_KD_PREHEAT;
    
    currentReflowProfile[1].name = "Soak";
    currentReflowProfile[1].targetTemperature = 190;
    currentReflowProfile[1].durationInSeconds = 30;
    currentReflowProfile[1].elapsedTime = 0;
    currentReflowProfile[1].pid_kp = PID_KP_SOAK;
    currentReflowProfile[1].pid_ki = PID_KI_SOAK;
    currentReflowProfile[1].pid_kd = PID_KD_SOAK;
    
    currentReflowProfile[2].name = "Reflow";
    currentReflowProfile[2].targetTemperature = 190;
    currentReflowProfile[2].durationInSeconds = 30;
    currentReflowProfile[2].elapsedTime = 0;
    currentReflowProfile[2].pid_kp = PID_KP_REFLOW;
    currentReflowProfile[2].pid_ki = PID_KP_REFLOW;
    currentReflowProfile[2].pid_kd = PID_KP_REFLOW;
    
    currentReflowProfile[3].name = "Cool";
    currentReflowProfile[3].targetTemperature = 50;
    currentReflowProfile[3].durationInSeconds = 60;
    currentReflowProfile[3].elapsedTime = 0;
    currentReflowProfile[3].pid_kp = PID_KP_PREHEAT;
    currentReflowProfile[3].pid_ki = PID_KP_PREHEAT;
    currentReflowProfile[3].pid_kd = PID_KP_PREHEAT;
  }

   /*
   if (profile == REFLOW_PROFILE_LEADED) {

      currentReflowProfile[0].name = "Pre-heat";
      currentReflowProfile[0].targetTemperature = 150;
      currentReflowProfile[0].durationInSeconds = 90;
      currentReflowProfile[0].elapsedTime = 0;
      currentReflowProfile[0].pid_kp = PID_KP_PREHEAT;
      currentReflowProfile[0].pid_ki = PID_KI_PREHEAT;
      currentReflowProfile[0].pid_kd = PID_KD_PREHEAT;

      currentReflowProfile[1].name = "Soak";
      currentReflowProfile[1].targetTemperature = 180;
      currentReflowProfile[1].durationInSeconds = 90;
      currentReflowProfile[1].elapsedTime = 0;
      currentReflowProfile[1].pid_kp = PID_KP_SOAK;
      currentReflowProfile[1].pid_ki = PID_KI_SOAK;
      currentReflowProfile[1].pid_kd = PID_KD_SOAK;

      currentReflowProfile[2].name = "Reflow";
      currentReflowProfile[2].targetTemperature = 220;
      currentReflowProfile[2].durationInSeconds = 30;
      currentReflowProfile[2].elapsedTime = 0;
      currentReflowProfile[2].pid_kp = PID_KP_REFLOW;
      currentReflowProfile[2].pid_ki = PID_KP_REFLOW;
      currentReflowProfile[2].pid_kd = PID_KP_REFLOW;

      currentReflowProfile[3].name = "Cool";
      currentReflowProfile[3].targetTemperature = 50;
      currentReflowProfile[3].durationInSeconds = 60;
      currentReflowProfile[3].elapsedTime = 0;
      currentReflowProfile[3].pid_kp = PID_KP_PREHEAT;
      currentReflowProfile[3].pid_ki = PID_KP_PREHEAT;
      currentReflowProfile[3].pid_kd = PID_KP_PREHEAT;
   }
   */

   if (profile == REFLOW_PROFILE_LEADFREE) {

      currentReflowProfile[0].name = "Pre-heat";
      currentReflowProfile[0].targetTemperature = 150;
      currentReflowProfile[0].durationInSeconds = 90;
      currentReflowProfile[0].elapsedTime = 0;
      currentReflowProfile[0].pid_kp = PID_KP_PREHEAT;
      currentReflowProfile[0].pid_ki = PID_KI_PREHEAT;
      currentReflowProfile[0].pid_kd = PID_KD_PREHEAT;

      currentReflowProfile[1].name = "Soak";
      currentReflowProfile[1].targetTemperature = 220;
      currentReflowProfile[1].durationInSeconds = 90;
      currentReflowProfile[1].elapsedTime = 0;
      currentReflowProfile[1].pid_kp = PID_KP_SOAK;
      currentReflowProfile[1].pid_ki = PID_KI_SOAK;
      currentReflowProfile[1].pid_kd = PID_KD_SOAK;

      currentReflowProfile[2].name = "Reflow";
      currentReflowProfile[2].targetTemperature = 255;
      currentReflowProfile[2].durationInSeconds = 60;
      currentReflowProfile[2].elapsedTime = 0;
      currentReflowProfile[2].pid_kp = PID_KP_REFLOW;
      currentReflowProfile[2].pid_ki = PID_KP_REFLOW;
      currentReflowProfile[2].pid_kd = PID_KP_REFLOW;

      currentReflowProfile[3].name = "Cool";
      currentReflowProfile[3].targetTemperature = 50;
      currentReflowProfile[3].durationInSeconds = 60;
      currentReflowProfile[3].elapsedTime = 0;
      currentReflowProfile[3].pid_kp = PID_KP_PREHEAT;
      currentReflowProfile[3].pid_ki = PID_KP_PREHEAT;
      currentReflowProfile[3].pid_kd = PID_KP_PREHEAT;
   }

   timerSeconds = 0;
   currentStage = 0;


  // Start timer
  FlexiTimer2::set(PID_SAMPLE_TIME, runAs_profile_refresh_trigger);
  FlexiTimer2::start();

  for(boolean exit = false; !exit; )
  {
    waitForKeyInterrupted = false;
    refresh_trigger = false;
    keyboard_waitForAnyKey();
    if (refresh_trigger) {
      runAs_profile_refresh();
    }
    if(lastKey==KEY_AH) exit = true;
  }
  analogWrite(PINS_SSR, 0);
  FlexiTimer2::stop();
  buzzer_beep(1000);

  display_printAborting();
  waitForKeyInterrupted = false;
  keyboard_waitForNokey();
}

void runAs_profile_refresh_trigger() {
  waitForKeyInterrupted = true;
  refresh_trigger = true;
}

void runAs_profile_refresh()
{
   // Update PID input
   pid_input = temperature_read();

   // Display status
   profile_displayStatus();

   // Check if is complete
   if (currentStage > REFLOW_STAGE_COOL) {

      // Turn off the oven
      analogWrite(PINS_SSR, 0);

      // Return doing nothing
      return;
   }

   // Configure PID refresh
   pid_setPoint = profile_calculateSetPoint();
   reflowOvenPID.SetTunings(currentReflowProfile[currentStage].pid_kp, currentReflowProfile[currentStage].pid_ki, currentReflowProfile[currentStage].pid_kd);
   reflowOvenPID.Compute();

   // Put PID output to SSR
   analogWrite(PINS_SSR, pid_output);

   // Log to serial for charts
   logger_log(timerSeconds, pid_setPoint, pid_input, pid_output, currentReflowProfile[currentStage].name);

   // Check if temperature is reached
   if (currentStage < REFLOW_STAGE_COOL) {

      if ( pid_input >= currentReflowProfile[currentStage].targetTemperature &&
         currentReflowProfile[currentStage].elapsedTime >= currentReflowProfile[currentStage].durationInSeconds) {
         currentStage++;
         buzzer_beep(1000);
      } else {
         currentReflowProfile[currentStage].elapsedTime++;
      }
   } else {
      if ( pid_input <= currentReflowProfile[currentStage].targetTemperature &&
         currentReflowProfile[currentStage].elapsedTime >= currentReflowProfile[currentStage].durationInSeconds) {
         currentStage++;
      } else {
         currentReflowProfile[currentStage].elapsedTime++;
      }
   }

   // Increase seconds timer for reflow curve analysis
   timerSeconds++;

}

void profile_displayStatus(){

   if (currentStage > REFLOW_STAGE_COOL) {
      display_printTitle(F("Finished!"));
   }
   else
   {
      display_printTitle(currentReflowProfile[currentStage].name);
   }

   lcd.print(timerSeconds);
   lcd.print("s");
   lcd.setCursor(8,1);
   lcd.print((int)pid_input);
   lcd.write((uint8_t)SYMBOL_DEGREE);
   lcd.print(F("C"));
}

double profile_calculateSetPoint(){

   // Set start temperature for this stage
   double startTemperature = 0;
   if (currentStage == REFLOW_STAGE_PREHEAT) {
      startTemperature = ROOM_TEMPERATURE;
   } else {
      startTemperature = currentReflowProfile[currentStage-1].targetTemperature;
   }

   // Set end temperature for this stage
   double endTemperature = currentReflowProfile[currentStage].targetTemperature;
   int duration = currentReflowProfile[currentStage].durationInSeconds;

   // Set elapsed time for this stage
   int elapsedTime = 0;
   if(currentReflowProfile[currentStage].elapsedTime >= duration) {
      elapsedTime = duration;
   } else {
      elapsedTime = currentReflowProfile[currentStage].elapsedTime+1;
   }

   // returns the calculated set point
   return startTemperature+(((endTemperature-startTemperature)/duration)*elapsedTime);
}
