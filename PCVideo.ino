#define PIN_SCE   7
#define PIN_RESET 6
#define PIN_DC    5
#define PIN_SDIN  4
#define PIN_SCLK  3

#define LCD_C     LOW
#define LCD_D     HIGH

#define LCD_X     84
#define LCD_Y     6

#define bufferSize  4
byte frameStart[] = {0x80, 0x01, 0x80, 0x01};
byte buffer[bufferSize];

void setup(){
  LcdInitialise();
  delay(500);
  Serial.begin(115200);
}

void loop(){
   while(Serial.available() > 0){
     byte b = Serial.read();
     
     for(int i = 0; i < bufferSize-1; i++){
       buffer[i] = buffer[i+1];
     }
     buffer[bufferSize-1] = b;
     boolean isFrameStart = true;
     for(int i = 0; i < bufferSize; i++){
         if(buffer[i] != frameStart[i])
           isFrameStart = false;
     }
     
     if(isFrameStart){
        LcdWrite(LCD_C, 0x80 ); //select X Address 0 of the LCD Ram
        LcdWrite(LCD_C, 0x40 ); //select Y Address 0 of the LCD Ram - Reset is not working for some reason, so I had to set these addresses
     }else{
         //for(int i = 0; i < bufferSize; i++){
             LcdWrite(LCD_D, b);
         //}
     }
   }
}

void LcdInitialise(void){
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  delay(200);
  digitalWrite(PIN_RESET, LOW);
  delay(500);
  digitalWrite(PIN_RESET, HIGH);
  LcdWrite(LCD_C, 0x21 );  // LCD Extended Commands.
  LcdWrite(LCD_C, 0xB1 );  // Set LCD Vop (Contrast). 
  LcdWrite(LCD_C, 0x04 );  // Set Temp coefficent. //0x04
  LcdWrite(LCD_C, 0x14 );  // LCD bias mode 1:48. //0x13
  LcdWrite(LCD_C, 0x0C );  // LCD in normal mode.
  LcdWrite(LCD_C, 0x20 );
  LcdWrite(LCD_C, 0x80 ); //select X Address 0 of the LCD Ram
  LcdWrite(LCD_C, 0x40 ); //select Y Address 0 of the LCD Ram - Reset is not working for some reason, so I had to set these addresses
  LcdWrite(LCD_C, 0x0C );
}

void LcdWrite(byte dc, byte data){
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}

