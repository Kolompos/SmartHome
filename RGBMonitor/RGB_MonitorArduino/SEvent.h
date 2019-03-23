void serialEvent(){
  command = Serial.read();
  delay(1);
  if(command == 's'){
    state = Serial.parseInt();
    Serial.print("State ");
    Serial.print(state);
    Serial.println(" set!");
    if(state != 4)
      EEPROM.write(STATEADDRESS,state);
  }
  else if(command == 'b'){
    brightness = Serial.parseInt();
    Serial.print("Brightness ");
    Serial.print(brightness);
    Serial.println(" set!");
    EEPROM.write(BRIGHTADDRESS, brightness);
  }
  else if(command == 't'){
    sleepTimeout = Serial.parseInt();
    Serial.print("Sleep timeout ");
    Serial.print(sleepTimeout);
    Serial.println(" set!");
    sleepTimeout = sleepTimeout / cycleDelay;
    EEPROMWritelong(TIMEOUTADDRESS, sleepTimeout);
  }
  else if(command == 'd'){
    int helper = cycleDelay;
    cycleDelay = Serial.parseInt();
    sleepTimeout = sleepTimeout * helper / cycleDelay;
    Serial.print("Cycle delay ");
    Serial.print(cycleDelay);
    Serial.println(" set!");
    EEPROM.write(DELAYADDRESS, cycleDelay);
  }
  else if(command == 'p'){
    sleepbrightness = Serial.parseInt();
    Serial.print("Sleep brightness ");
    Serial.print(sleepbrightness);
    Serial.println(" set!");
    EEPROM.write(SLPBRGHTADDRESS, sleepbrightness);
  }
  else if(command == 'e'){
    sleepcycleDelay = Serial.parseInt();
    Serial.print("Sleep delay ");
    Serial.print(sleepcycleDelay);
    Serial.println(" set!");
    EEPROM.write(SLPDELAYADDRESS, sleepcycleDelay);
  }
  else if(command == 'w'){
    Serial.println("<<<<<<<<<<>>>>>>>>>>");
    Serial.println("EEPROM's data:");
    Serial.print("State: ");
    Serial.println(state);
    Serial.print("Delay: ");
    Serial.println(EEPROM.read(DELAYADDRESS));
    Serial.print("Bright: ");
    Serial.println(EEPROM.read(BRIGHTADDRESS));
    Serial.print("Timeout: ");
    Serial.println(EEPROMReadlong(TIMEOUTADDRESS));
    Serial.print("SlpBright: ");
    Serial.println(EEPROM.read(SLPBRGHTADDRESS));
    Serial.print("SlpDelay: ");
    Serial.println(EEPROM.read(SLPDELAYADDRESS));
    Serial.println("--------------------");
    Serial.println("Other data:");
    Serial.print("Tick: ");
    Serial.println(tickTime);
    return;
  }else if(command == 'k'){
    Serial.print("Keeping awake!");
  }
  else{
    Serial.println("No such cmd!");
  }
  
  tickTime = 0;
}
