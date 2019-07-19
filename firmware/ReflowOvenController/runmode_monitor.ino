
volatile bool temp_refresh_trigger;

void runAs_tempMonitor()
{
  display_printTitle(F("Temp monitor"));

  keyboard_waitForNokey();
  runAs_tempMonitor_updateTemp();

  // Start timer
  FlexiTimer2::set(1000, runAs_tempMonitor_updateTemp);
  FlexiTimer2::start();

  for(boolean exit = false; !exit; )
  {
    waitForKeyInterrupted = false;
    temp_refresh_trigger = false;
   	keyboard_waitForAnyKey();
    double temperature;

    if (temp_refresh_trigger) {
      temperature = temperature_read();
      display_printTitle(F("Temp monitor"));

      lcd.print((int)temperature);
      lcd.write((uint8_t)SYMBOL_DEGREE);
      lcd.print(F("C"));
    }
   	if(lastKey==KEY_AH) {
   	  exit = true;
    }
  }

  FlexiTimer2::stop();
  waitForKeyInterrupted = false;
  keyboard_waitForNokey();
}

void runAs_tempMonitor_updateTemp()
{
  waitForKeyInterrupted = true;
  temp_refresh_trigger = true;
  /*
  double lastTemperature = temperature_read();

  display_printTitle(F("Temp monitor"));

  lcd.print((int)lastTemperature);
  lcd.write((uint8_t)SYMBOL_DEGREE);
  lcd.print(F("C"));
  */
}
