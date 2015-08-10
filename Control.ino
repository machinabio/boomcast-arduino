/**
* Reads payload buffer and sets new setpoint
*
*/
void changeProgram()
{
  if ( payloadBuffer[0] == '1') {  // all off
    demo = false;
    colorWipe(strip.Color(  0,   0, 0), 0);
  }  
  if ( payloadBuffer[0] == '2') { // test program
    demo = true;
  }
  if ( payloadBuffer[0] == '3') { // individual control
      demo = false;
  }
  clearCommand();
}

/**
* Reads payload buffer and sets valve states
*
*/
void updateLED()
{
  clearCommand();
  uint8_t r, g, b, n;
  char rt[3];
  char gt[3];
  char bt[3];

  rt[0] = payloadBuffer[0];
  rt[1] = payloadBuffer[1];
  rt[2] = '\0';
  gt[0] = payloadBuffer[2];
  gt[1] = payloadBuffer[3];
  gt[2] = '\0';
  bt[0] = payloadBuffer[4];
  bt[1] = payloadBuffer[5];
  bt[2] = '\0';
  
  r = String(rt).toInt();
  g = String(gt).toInt();
  b = String(bt).toInt();
  String validCharacters = String(VALIDCOMMANDCHARACTERS);
  n = validCharacters.indexOf(_command);
//  
//  Serial.print("setting LED ");
//  Serial.println(n, DEC); 
//  Serial.print("red = ");
//  Serial.println(r, DEC); 
//    Serial.print("green = ");
//  Serial.println(g, DEC); 
//    Serial.print("blue = ");
//  Serial.println(b, DEC); 
  
  strip.setPixelColor(n, r, g, b);
  strip.show();
}

