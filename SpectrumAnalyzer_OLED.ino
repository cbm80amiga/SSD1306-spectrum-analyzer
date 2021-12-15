// http://www.radiolocman.com/shem/schematics.html?di=71070
// http://blurtime.blogspot.com/2010/11/arduino-realtime-audio-spectrum.html
// http://blurtime.blogspot.com/2010/11/arduino-spectrum-with-video-out.html

// microphone amplifier without C2:
// http://electronics.stackexchange.com/questions/57683/can-i-use-a-pnp-transistor-with-an-electret-microphone-to-get-an-non-inverting-o
// linein amplifier without C2 and R1
// C1=10uF, R1=10k, R2=100k, R3=10k, npn

#include "fix_fft.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

char im[128], data[128];
char x = 0, ylim = 60;
int i = 0, val;

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  analogReference(DEFAULT); // Use default (5v) aref voltage.
};

void loop()
{
  int min=1024, max=0;
  for (i = 0; i < 128; i++) {
    val = analogRead(A0);
    data[i] = val / 4 - 128;
    //data[i] = (val-128)/8;
    im[i] = 0;
    if(val>max) max=val;
    if(val<min) min=val;
  };
    
  fix_fft(data, im, 7, 0);
  
  display.clearDisplay();
  for (i = 1; i < 64; i++) { // In the current design, 60Hz and noise
    int dat = sqrt(data[i] * data[i] + im[i] * im[i]);
    display.drawLine(i*2 + x, ylim, i*2 + x, ylim - dat, WHITE); // skip displaying the 0-500hz band completely.
  }; // rather than 1.
  display.drawLine(0, 10, data[0], 10, WHITE);
  display.drawLine(0, 11, data[0], 11, WHITE);
  display.setCursor(0,0);
  display.print("min=");
  display.print(min,1);
  display.setCursor(8*6,0);
  display.print("max=");
  display.print(max);
  display.setCursor(16*6,0);
  display.print("v=");
  display.print((int)data[0]);
  display.display();
};
