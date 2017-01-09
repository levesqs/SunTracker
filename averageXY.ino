//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//
// This sketch is a good place to start if you're just getting started with 
// Pixy and Arduino.  This program simply prints the detected object blocks 
// (including color codes) through the serial console.  It uses the Arduino's 
// ICSP port.  For more information go here:
//
// http://cmucam.org/projects/cmucam5/wiki/Hooking_up_Pixy_to_a_Microcontroller_(like_an_Arduino)
//
// It prints the detected blocks once per second because printing all of the 
// blocks for all 50 frames per second would overwhelm the Arduino's serial port.
//
   
#include <SPI.h>  
#include <Pixy.h>
int ledH=10;//int ledL=4;
// This is the main Pixy object 
Pixy pixy;
uint8_t brightness=25;
int8_t retbr;
int j,nb=0;
int xs,ys,hs,ws=0;
double xm,ym,hm,wm=0.0;
  
void setup()
{
  Serial.begin(9600);
  Serial.print("Starting...\n");
  pinMode(ledH, OUTPUT);//pinMode(ledL, OUTPUT);
  pixy.init();
  digitalWrite(ledH, HIGH);
  delay(3000);
  digitalWrite(ledH, LOW);
  
  //  pixy.setLED(255, 255, 255);
  //  retbr=pixy.setBrightness( 5);
  //  Serial.print("5:");
  //  Serial.println(retbr);
}

void loop()
{ 
  static int i = 0;
  uint16_t blocks;
  char buf[32]; 
  blocks = pixy.getBlocks(20);
  if (blocks)
  {
    digitalWrite(ledH, HIGH);
    //i++;
    j=0;//premier objet
    if (pixy.blocks[j].signature == 1)
    {
      nb++;
      xs=xs+pixy.blocks[j].x;
      ys=ys+pixy.blocks[j].y;
      hs=hs+pixy.blocks[j].height;
      ws=ws+pixy.blocks[j].width;
    } else
    {
      Serial.println("S");
    }

    if (nb>19)
    {
      xm=160.0-1.0*xs/nb;
      ym=100.0-1.0*ys/nb;
      wm=1.0*ws/nb;
      hm=1.0*hs/nb;
      
      //sprintf(buf, "x: %8.2f y: %8.2f \n", xm, ym);//Serial.print(buf);
      //Serial.print(" N:");Serial.print(nb);
      //Serial.print(" h:");
      Serial.print(hm,1);
      Serial.print("x");Serial.print(wm,1);
      Serial.print(" dx:");Serial.print(xm,1);
      Serial.print(" dy:");Serial.println(ym,1);
//      Serial.print(" xm:");
//      Serial.print(double2s(xm,5));
//      Serial.print(" ym:");
//      Serial.println(double2s(ym,5));
      xs=0;ys=0;hs=0;ws=0;
      nb=0;i=0;
    }
  } //blocks   
  else
  {
    //Serial.print("0");
    digitalWrite(ledH, LOW);   
  }
}//loop

