/* 
 * -------------------------------------------------------------------------
 * Arduino Button Box
 * -------------------------------------------------------------------------
 * Copyright Kevin Peat 2014
 * kevin@kevinpeat.com
 * This sketch is licensed public domain
 * -------------------------------------------------------------------------
 * Original code provided by Dan Nixon on his blog
 * Multiplexing Switches on Arduino without additional ICs
 * http://www.dan-nixon.com/2012/04/multiplexing-switches-on-arduino.html
 * -------------------------------------------------------------------------
 * Requires an Arduino Leonardo or other similar device that is able to 
 * emulate a USB HID Keyboard
 * -------------------------------------------------------------------------
 * Hardware to be used with iRacing and other racing sims. The simple 
 * hardware design means that only one button can be pressed at a time 
 * which is not a problem for this type of hardware. Also there is no need 
 * for key modifiers so only simple key presses can be sent.
 * -------------------------------------------------------------------------
 */

/*
 * -------------------------------------------------------------------------
 * Arduino Leonardo key codes
 * -------------------------------------------------------------------------
      0 = 0x30
      1 = 0x31
      2 = 0x32
      3 = 0x33
      4 = 0x34
      5 = 0x35
      6 = 0x36
      7 = 0x37
      8 = 0x38
      9 = 0x39
      a = 0x61
      b = 0x62
      c = 0x63
      d = 0x64
      e = 0x65
      f = 0x66
      g = 0x67
      h = 0x68
      i = 0x69
      j = 0x6A
      k = 0x6B
      l = 0x6C
      m = 0x6D
      n = 0x6E
      o = 0x6F
      p = 0x70
      q = 0x71
      r = 0x72
      s = 0x73
      t = 0x74
      u = 0x75
      v = 0x76
      v = 0x76
      w = 0x77
      x = 0x78
      y = 0x79
      z = 0x7A
      ' = 0x27
      , = 0x2C
      - = 0x2D
      . = 0x2E
      / = 0x2F 
      ; = 0x3B
      = = 0x3D
      [ = 0x5B
      ] = 0x5D
      #~ = 0x7E
      # = 0xE0
      F1 = 0xC2
      F2 = 0xC3
      F3 = 0xC4
      F4 = 0xC5
      F5 = 0xC6
      F6 = 0xC7
      F7 = 0xC8
      F8 = 0xC9
      F9 = 0xCA
      F10 = 0xCB
      F11 = 0xCC
      F12 = 0xCD
      BACKSPACE = 0xB2
      DELETE = 0xD4
      ESC = 0xB1
      HOME = 0xD2
      INSERT = 0xD1
      RETURN = 0xB0
      SPACE = 0x20
      TAB = 0xB3
      LEFT_ARROW = 0xD8
      RIGHT_ARROW = 0xD7
      UP_ARROW = 0xDA
      DOWN_ARROW = 0xD9
 * -------------------------------------------------------------------------
 */

// Array of pins for the columns
int cPins[] = {2, 3, 4, 5, 6};

// Number of pins in the column array
int cPinsNo = 5;

// Array of pins for the rows
int rPins[] = {8, 9, 10, 11};

// Number of pins in the row array
int rPinsNo = 4;

// Array for the last known switch states [cPinsNo][rPinsNo]
int colPrev[5][4] = {0};

// Key codes to be used for each button
// (see table above for codes to use)
// (columns and rows are transposed on device)
uint8_t buttonCodes[5][4] = {
  {0xC2, 0xC8, 0x2D, 0x35},
  {0xC4, 0xC9, 0x3D, 0x36},
  {0xC5, 0x39, 0xD8, 0x38},
  {0xC6, 0xDA, 0xD9, 0x31},
  {0xC7, 0x20, 0xD7, 0x32}
};

void setup()
{
  //Start Serial
  Serial.begin(9600);
  Serial.println("Multiplexed Buttons Test");

  //Set the Column Pin Mode
  Serial.println("Setting Column Pins...");

  for(int cPin = 0; cPin < cPinsNo; cPin++)
  {
    pinMode(cPins[cPin], OUTPUT);
    digitalWrite(cPins[cPin], HIGH);
  }

  //Set the Row Pin Mode
  Serial.println("Setting Row Pins...");

  for(int rPin = 0; rPin < rPinsNo; rPin++)
  {
    pinMode(rPins[rPin], INPUT);
    digitalWrite(rPins[rPin], HIGH);
  }
  Serial.println("Ready!");
} 

void loop()
{
  // Loop through the columns
  for(int cPin = 0; cPin < cPinsNo; cPin++)
  {
    digitalWrite(cPins[cPin], LOW);

    // Loop through the rows
    for(int rPin = 0; rPin < rPinsNo; rPin++)
    {
      //Check if each switch is pressed
      if(digitalRead(rPins[rPin]) == LOW)
      {
        // Check to see if the state has changed since last time
        if(colPrev[cPin][rPin] == 0)
        {
          // Do action here, switch is on
          Serial.print(cPins[cPin]);
          Serial.print(", ");
          Serial.print(rPins[rPin]);
          Serial.println(" ON");

          Keyboard.press(buttonCodes[cPin][rPin]);
          delay(150);
          Keyboard.release(buttonCodes[cPin][rPin]); 

          // Update last known state of this switch
          colPrev[cPin][rPin] = 1;
        }
      } 
      else {

        // Check to see if the state has changed since last time
        if(colPrev[cPin][rPin] == 1)
        {
          // Do action here, switch is off
          Serial.print(cPins[cPin]);
          Serial.print(", ");
          Serial.print(rPins[rPin]);
          Serial.println(" OFF");

          // Update last known state of this switch
          colPrev[cPin][rPin] = 0;
        }
      }
    }
    digitalWrite(cPins[cPin], HIGH);
  }
}

