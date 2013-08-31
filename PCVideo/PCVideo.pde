import processing.video.*;
import processing.serial.*;

final int lcdWidth = 84;
final int lcdHeight = 48;

Capture video;
Serial myPort;
int frameStart[] = {0x80, 0x01, 0x80, 0x01};
int[] lcdPixels = new int[lcdWidth * lcdHeight];

public void setup() {
  size(lcdWidth, lcdHeight, P2D);
  video = new Capture(this, lcdWidth, lcdHeight, 15);
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 115200);
  loadPixels();
}

public void captureEvent(Capture c) {
  c.read();
}

void draw() {
  background(0);
  int pixelWidth = width / video.width;
  int pixelHeight = height / video.height;

  int index = 0;
  for (int y = 0; y < video.height; y++) {
    float luminance = 0.0;
    for (int x = 0; x < video.width; x++) {
      int pixelColor = video.pixels[index];
      int r = (pixelColor >> 16) & 0xff;
      int g = (pixelColor >> 8) & 0xff;
      int b = pixelColor & 0xff;

      luminance = 0.3*r + 0.59*g + 0.11*b;
      
      pixels[index] = video.pixels[index];
      if(luminance > 80)
        lcdPixels[index] = 0xffffffff;
      else
        lcdPixels[index] = 0x00000000;
      
      index++;
    }
  }
  updatePixels();
  
  //send to Serial port
  
  for(int i = 0; i< frameStart.length; i++)
      myPort.write((byte)frameStart[i]);

  for(int y = 0; y < lcdHeight / 8; y++){
     for(int x = 0; x < lcdWidth; x++){
        int by = 0x00;
        for(int a = 0; a < 8; a++){
           by = by >> 1;
           if(lcdPixels[(y*8+a)*lcdWidth+x] == 0x00000000){
              by = by | 0x80;
           } 
        }
        myPort.write((byte)by);
     } 
  }
  delay(50);
}
