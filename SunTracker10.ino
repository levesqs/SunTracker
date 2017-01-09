
/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control
For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/

#include <SPI.h>  
#include <Pixy.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 
// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(400, 2);
int method = INTERLEAVE;//MICROSTEP SINGLE DOUBLE INTERLEAVE
// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotorDC = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);
int iBT=3;
int iBTr=2;
int iBTDC=4;
int iBTDCr=5;
int inH=11;//int inL=6;
int ledH=10;//int ledL=4;
int valH = 1;//int valL = 1;
int tour=10740;//5372;// step par tour
int StpAng=0;// step count
int dest=0;
char * buf ;
char * Serbuf;
char * com ;
char ch;
int inited=0;
int i=0;
int BTf=0;
int BTr=0;
float vi;// vitesse stepper
float angleX,angleY,SL,SR,MV;
int runn = 1;
Pixy pixy;
uint8_t brightness=25;
int8_t retbr;
int nb=0;
int xs,ys,hs,ws=0;
double xm,ym,hm,wm,xmp,ymp=0.0;
SoftwareSerial mySerial(6, 7,false); // RX, TX

void setup() {
  Serbuf = new char[32 + 1];
  Serbuf[0] = '\0';
  buf = new char[32 + 1];
  buf[0] = '\0';
  com = new char[16 + 1];
  Serial.begin(9600);   // set up Serial library at 9600 bps
  while (!Serial) { ; /*  native USB only */  }
  mySerial.begin(57600);
  pixy.init();
  if (method==INTERLEAVE)  {tour=10742;}
  //pinMode(outpin, OUTPUT);
  pinMode(ledH, OUTPUT);//pinMode(ledL, OUTPUT);
  pinMode(inH,INPUT);  //pinMode(inL,INPUT); 
  pinMode(iBT,INPUT);
  pinMode(iBTr,INPUT);
  pinMode(iBTDC,INPUT);
  pinMode(iBTDCr,INPUT);
  Serial.println("init motor");
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  initNP();  
  digitalWrite(ledH, LOW);
  quickinit();
  runn=2;
}// setup

void loop() 
{
  //int i=0;// local loop
  //char ch;//local ch
  static int j = 0;//objet class
  uint16_t blocks;// objects lists
  //int ret;  //float FY=0;//facteur amplif Y  
  float prX;//angle move relative


  int srX=0;// step relative
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  while (Serial.available()) {
	  Serialstr();
  }
  if ((Serbuf[0] == '1') && (Serbuf[strlen(Serbuf)-1] == 10))
  {
      //Serial.print("Newport>");
      //Serial.print(Serbuf);
      mySerial.print(Serbuf);
      Serbuf[0]='\0';
  }
  if ((Serbuf[0] == '0') && (Serbuf[strlen(Serbuf)-1] == 10))
  {//0
    //Serial.print("Stepper>");
    //Serial.print(Serbuf);
    if(strncmp (Serbuf,"0PA",3) == 0)
    {//pa
      MV=atof(&Serbuf[3]);
      if(MV>SR)
      {
        MV=SR;
        Serial.print("Max limit=");
        Serial.println(SR);
      } 
      if(MV<SL)
      {
        MV=SL;
        Serial.print("Min limit=");
        Serial.println(SL);
      }
      prX=atof(&Serbuf[3])-360.0*StpAng/tour;
      srX=abs(int(prX/360.0*tour));
      if ((prX>0)&&(prX<360.0))
      {
        myMotor->step(srX,BACKWARD  , method);
        StpAng=StpAng+srX;
      }
      if ((prX<0)&&(prX>-360.0))
      {
        myMotor->step(srX, FORWARD, method);
        StpAng=StpAng-srX;;
      }
      Serbuf[0]='\0';
    }//pa
    if(strncmp (Serbuf,"0PR",3) == 0)
    {//pr
      prX=atof(&Serbuf[3]);
      if ((360.0*StpAng/tour + prX) > SR)
      {
        prX=SR-360.0*StpAng/tour;        
        Serial.print(" Max limit:");
        Serial.println(360.0*StpAng/tour+prX);
      }
      if (360.0*StpAng/tour+prX<SL)
      {
        prX=SL-360.0*StpAng/tour;
        Serial.print("Min limit:");
        Serial.println(360.0*StpAng/tour+prX);
      }
      srX=abs(int(prX/360.0*tour));
      if ((prX>0)&&(prX<360.0))
      {
        myMotor->step(srX,BACKWARD  , method);
        StpAng=StpAng+srX;
      }
      if ((prX<0)&&(prX>-360.0))
      {
        myMotor->step(srX, FORWARD, method);
        StpAng=StpAng-srX;;
      }
      Serbuf[0]='\0';
    }//pr
    if(strncmp (Serbuf,"0TP",3) == 0) 
    {//tp
      Serial.print("0TP");
      Serial.println(360.0*StpAng/tour,6);
      Serbuf[0]='\0';
    }//tp
    if(strncmp (Serbuf,"0SU?",4) == 0) 
    {//su
      Serial.print("0SU");
      Serial.println(360.0*1/tour,8);
      Serbuf[0]='\0';
    }//su
    if(strncmp (Serbuf,"0VA?",4) == 0) 
    {//va?
      Serial.print("0VA");
      Serial.println(vi);
      Serbuf[0]='\0';
    }//va?
    if(strncmp (Serbuf,"0VA",3) == 0) 
    {//va
      vi=atof(&Serbuf[3]);
      myMotor->setSpeedFloat(vi); //float version slv
      Serbuf[0]='\0';
    }//va
    if(strlen(Serbuf))
    {//unk 
      Serial.print(">");
      for (i=0;i<strlen(Serbuf);i++)
      {
        ch=Serbuf[i];
        if(ch>31)
        {  
          Serial.write(ch); 
        }
        else
        { 
          Serial.print(".");
          Serial.print(ch,DEC); 
          Serial.print(".");
        }
      }
      Serial.println("< Unknown command");
    }//unk
    Serbuf[0]='\0';
  }//0
  
  if (runn==2)
  {//runn2
    
    BTf=digitalRead(iBT);
    if (!BTf)
    {//fw
      strcpy(Serbuf,"0PR");//-1.5
      com=ftoa(com, -1.5, 1000L);
      Serbuf=strcat(Serbuf,com);
      append(Serbuf, 13);
      append(Serbuf, 10);
    }//fw
  
    BTr=digitalRead(iBTr);
    if (!BTr){
      strcpy(Serbuf,"0PR1.5");
      append(Serbuf, 13);
      append(Serbuf, 10);
    }
  
    if ((!BTf) && (!BTr)) {
      // stop motor
      myMotor->release();
      Serial.println("no run");
      runn=3;
      digitalWrite(ledH, LOW);
    }//both button

  blocks = pixy.getBlocks(20);
  if (blocks)
  {//blocks
    digitalWrite(ledH, HIGH);
    //j=0;//premier objet
    //if (pixy.blocks[j].signature == 2)
      nb++;
      xs=xs+pixy.blocks[j].x;
      ys=ys+pixy.blocks[j].y;
      hs=hs+pixy.blocks[j].height;
      ws=ws+pixy.blocks[j].width;
    if (nb>9)
    {//nb reach
      xm=160.0-1.0*xs/nb;
      ym=100.0-1.0*ys/nb;
      wm=1.0*ws/nb;
      hm=1.0*hs/nb;
      if (digitalRead(iBTDCr) && digitalRead(iBTDC) &&
          BTf && BTr)
        {//nobutton
          angleX=360.0/tour*StpAng;
          Serial.print(angleX,1);
          Serial.print(" ");
          buf[0]='\0';
          mySerial.println("1tp");
          mystr();
          angleY=atof(&buf[3]);
          //Serial.print(&buf[3]);
          Serial.print(angleY,1);
          //Serial.print("=");
          mychk();          
          Serial.print(" x");
          Serial.print(xm,1);
          Serial.print(" y");//dy
          Serial.print(ym,1);
          Serial.print(" h");
          Serial.print(hm,1);
          Serial.print(" w");//dy
          Serial.print(wm,1);
          Serial.print("   ");
          float angleYrad=angleY/180.0*PI;
          float angleXrad=angleX/180*PI;
          float x1=xm*cos(angleYrad);//*abs(cos(angleXrad));
          float x2=-ym*sin(angleYrad);
          xmp=x1+x2;
          if (angleX>90.0) 
          { xmp=-xmp; }
          Serial.print(x1,2);
          Serial.print("+");
          Serial.print(x2,2);
          Serial.print("=");
          Serial.print(xmp,2);
          Serial.print(" | ");
          float y1=ym*cos(angleYrad);
          float y2=xm*sin(angleYrad);//*abs(cos(angleXrad));
          ymp=y1+y2;
          Serial.print(y1,3);
          Serial.print("+");
          Serial.print(y2,3);
          Serial.print("=");
          Serial.print(ymp,2);
          Serial.print(" # ");
//          float FX=0;
//          FX=1.0*abs(cos(angleXrad));
//          if (angleX<89.5)
//            { FY = 0.2/cos(angleX/180*PI); }
//          else if (angleX > 90.5)
//            { FY = -0.2/cos(angleX/180*PI); }
//          else FY=60.0;                         
//          xmp=xmp*FX*1.0;    
//          Serial.print(ymp,2);
//          Serial.print(" ; ");     
//          Serial.print("=");//dx:
//          ret=scanf(buf,"1TP%f",&angleY);
//          if (ret == EOF) 
//          {
//            Serial.println("EOF");
//          }
          xm=xmp;
          ym=ymp;
        }//nobutton
  float gx=0.03;//pixel to angle
  float gy=0.03;
      if ((xm <170.0) && (xm>-170.0) && BTf && BTr)
      { //servo x
        if (abs(xm)*gx > 0.03351331)
        {//minx
          com=StrDouble( xm*gx, 5);
          strcpy(Serbuf,"0PR");//-1.5
          Serbuf=strcat(Serbuf,com);
          Serial.print(Serbuf);
          append(Serbuf, 13);
          append(Serbuf, 10);
        }//minx  
      }// servo x
      if ((ym <170) && (ym>-170) && digitalRead(iBTDCr) && digitalRead(iBTDC))
      { //servo y
        if (abs(ym)*gy>0.0005) 
        {//miny
          mySerial.print("1pr");
          Serial.print(" 1pr");
          mySerial.println(ym*gy,4);
          Serial.print(ym*gy,4);
          mychk();
        }//miny
      }// servo y    
      Serial.println(" ");  
      xs=0;ys=0;hs=0;ws=0;
      nb=0;
    }//nb reach
  } //blocks   
  /*
  else
  {
    // no blocks
    //Serial.print("0");
    //digitalWrite(ledH, LOW);   
    if (digitalRead(iBTDCr) && digitalRead(iBTDC) &&
          BTf && BTr)
    {
      buf[0]='\0';
      mySerial.println("1tp");
      mystr();
      Serial.print(&buf[3]);
      mychk();          
      Serial.print(" ");
      Serial.print(360.0/tour*StpAng,4);
      Serial.print(" :");
      Serial.println(StpAng);
    }
  }// no blocks
*/
  while (!digitalRead(iBTDC)){
    NewFor();
  }
  while (!digitalRead(iBTDCr)){
    NewRev();
  } 
  }// runn2
} // loop

////////////////////////////////////// stepper   ///////////////////////////
void quickinit(){
  //tour=10740; 
  Serial.println("init Stepper");
  StpAng=2592;//if -90.xx add xx/360*tour
  method = INTERLEAVE;//MICROSTEP SINGLE DOUBLE INTERLEAVE
  vi=40;
  myMotor->setSpeedFloat(vi); //float version slv
  
  // backward si on est dessus
  while(digitalRead(inH)){
    digitalWrite(ledH, digitalRead(inH));
    myMotor->step(12, FORWARD, method);
  }  
  // avance fast  speed
  Serial.println("search first index high speed");
  //method = INTERLEAVE;//MICROSTEP SINGLE DOUBLE INTERLEAVE
  //vi=40;
  //myMotor->setSpeedFloat(vi); //float version slv
  while(!digitalRead(inH)){
    digitalWrite(ledH, digitalRead(inH));
    myMotor->step(1, BACKWARD, method);
  } 
  digitalWrite(ledH, HIGH);
  Serial.println("found first index");
  
  //method = INTERLEAVE;//MICROSTEP SINGLE DOUBLE INTERLEAVE
  vi=10;
  myMotor->setSpeedFloat(vi); //float version slv
  Serial.println("backward a bit");
  // backward si on est dessus
  myMotor->step(10, FORWARD, method);
  
  vi=1;
  myMotor->setSpeedFloat(vi); //float version slv
  Serial.println("search index slow speed");
  //method = INTERLEAVE;//MICROSTEP SINGLE DOUBLE INTERLEAVE
  while(!digitalRead(inH)){
    myMotor->step(1, BACKWARD, method);
    digitalWrite(ledH, digitalRead(inH));
  }
  digitalWrite(ledH, HIGH);
  method = INTERLEAVE;//MICROSTEP SINGLE DOUBLE INTERLEAVE
  vi=40;
  myMotor->setSpeedFloat(vi); //float version slv
  SL=-10.0;
  SR=190.0;
  Serial.println("init Stepper DONE");
}

//////////////////////////// Newport //////////////////////////

void NewRev()
{
  buf[0]='\0';
  mySerial.println("1pr-1.5");
  delay(224);// 2=0.1 450=2.5 223=1.5
  mychk();
 /* 
  //Serial.write(buf);
  mySerial.print("1TS\r\n");
  mystr();
  //Serial.println(buf);
  while( strcmp("1TS000033", buf) ) 
  {
    Serial.print(".");
    //Serial.println(buf); 
    buf[0]='\0';
    mySerial.print("1TS\r\n");
    mystr();
  }
  //Serial.write("\r\n");
  */
  buf[0]='\0';
  //mychk();
}//NewRev

void NewFor()
{
  /*float angf=0.1;
  float delms;
  mySerial.print("1pt");
  mySerial.println(angf,2);
  mySerial.print("\r\n");
  mystr();
  com[0]='\0';
  com=strcat(com, &buf[3]);
  Serial.print(atof(com),4);
  */
  //delms=int(1000*atof(com)); 160 = 0.1
  //Serial.write(com);
  //Serial.write("\r\n");
  buf[0]='\0';
  //mySerial.print("1pr");
  //mySerial.println(angf,2);
  //mySerial.print("\r\n");
  mySerial.println("1pr1.5");
  delay(224);//delms);      2=0.1
  mychk();
  buf[0]='\0';
  //mychk();
}//NewFor

void initNP()
{//init newport
  Serial.println("init Newport");
  // status loop
  buf[0]='\0';
  delay(500);
  mySerial.print("1TE\r\n");
  mystr();
  while(strcmp("1TE@", buf))
  {
    Serial.write(buf);
    Serial.print("\r\nsend 1TE  ");
    mySerial.print("1TE\r\n");
    delay(200);
    buf[0]='\0';
    mystr();
  }
  Serial.write(buf);
  Serial.write(" No error\r\n");
  buf[0]='\0';
  // end status loop
  // ve loop
  Serial.print("send 1VE  ");
  mySerial.print("1VE\r\n");
  delay(200);
  mystr();
  Serial.write(buf);
  Serial.write("\r\n");
  buf[0]='\0';
  mychk();
  // ve loop end
  //init loop
  while( (strcmp("1TS000032", buf)) && (strcmp("1TS000033", buf))) 
  {
    Serial.print("send 1TS  ");
    mySerial.print("1TS\r\n");
    buf[0]='\0';
    delay(300);
    mystr();
    Serial.write(buf);
    if(!strcmp("1TS00000A", buf))
    {
      Serial.print(" send 1OR  \r\n");
      mySerial.print("1OR\r\n");   
    }
    if(!strcmp("1TS00001E", buf))
    {
      Serial.print("  initializing... \r\n");    
    }
  }//init loop end
  Serial.write("\r\n");
  buf[0]='\0';
  mychk();
  
  // va loop
  Serial.print("send 1VA5  ");
  mySerial.print("1VA5\r\n");
  //delay(100);
  //Serial.write(buf);
  //Serial.write("\r\n");
  buf[0]='\0';
  mychk();
  // va loop end
  // va? loop
  Serial.print("send 1VA?  ");
  mySerial.print("1VA?\r\n");
  mystr();
  Serial.write(buf);
  Serial.print("\r\n");
  buf[0]='\0';
  mychk();
  // va? loop end
  // SR loop
  Serial.print("send 1SR90  ");
  mySerial.print("1SR90\r\n");
  buf[0]='\0';
  mychk();
  Serial.print("send 1SR?  ");
  mySerial.print("1SR?\r\n");
  mystr();
  Serial.write(buf);
  Serial.print("\r\n");
  buf[0]='\0';
  mychk();
  Serial.print("send 1SL-85  ");
  mySerial.print("1SL-85\r\n");
  buf[0]='\0';
  mychk();
  Serial.print("send 1SL?  ");
  mySerial.print("1SL?\r\n");
  mystr();
  Serial.write(buf);
  Serial.print("\r\n");
  buf[0]='\0';
  mychk();
  // SL? loop end  
  Serial.write("init Newport DONE\r\n");
}//init newport

void mychk()
{  // status chk
  buf[0]='\0';
  mySerial.print("1TE\r\n");
  mystr();
  if(strcmp("1TE@", buf))
  {
    Serial.write(buf);
    com[0]='\0';
    com=strcat(com, "1TB");
    com=strcat(com, &buf[3]);
    Serial.println(com);
    Serial.println(" ");
    mySerial.println(com);
    delay(200);
    buf[0]='\0';
    mystr();
    Serial.write(buf);
    Serial.write("\r\n");
  }
  buf[0]='\0';
}  // end status chk

void mystr()
{
  char ch;
  int count=0;
  while((!mySerial.available()) && (count<50)) {
      delay(100);
      count++;
  }
  while (mySerial.available() > 0) 
  {
    ch=(mySerial.read());
    if(ch>31)
    { append(buf, ch); }
    else
    { append(buf,0);   }
    delay(10);
  }
}
  
void Serialstr()
{
  char ch;
  while (Serial.available() > 0) 
  {
    ch=(Serial.read());
    append(Serbuf, toupper(ch));
    /*  if(ch>31) {  Serial.write(ch);   }
    else
    { Serial.print("."); Serial.print(ch,DEC);   Serial.print("."); }  */
  }
  //Serial.println("<");
}
  
void append(char* s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
        s[len+2] = '\0';
}

char *ftoa(char *a, double f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000}; 
  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') 
    a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}

char * StrDouble( double f, unsigned int digits){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)
  int d;
  static char s[26];  // formated number as string
  int index=0;
  // handle sign
  if (f < 0.0)
  {
    s[index++] = '-';
    f = -f;
  }
 // handle infinite values
 if (isinf(f))
 {
   strcpy(&s[index], "INF");
   return s;
 }
 // handle Not a Number
 if (isnan(f))
 {
   strcpy(&s[index], "NaN");
   return s;
 }
double g = f;
//if ((g < 1.0) && (g != 0.0))      
//  {
//    s[index++] = '0';
//  }
  d = int(g);
  sprintf(&s[index++],"%d",d);  
  if (digits > 0) sprintf(&s[index++],".");
  for (int i=0;i<digits;i++) {
    g = (g - d) * 10.0;  // shift one decimal place to the left
    d = int(g);
    sprintf(&s[index++],"%d",d);
 }
 //sprintf(&s[index], " E%+d", exponent);
 return s;    
} 


/*
// ====== double2s(): print out up to 16 digits of input double-precision value
// This version enables double-precision for Teensy 3, etc.
// by J.Beale March 2013
// modified from original float2s() posted by davekw7x on December, 2010
// http://arduino.cc/forum/index.php/topic,46931.0.html

char * double2s(double f)
{
 return double2s(f, 2);
}

char * double2s(double f, unsigned int digits) {    
int d;
static char s[26];  // formated number as string
int index=0;

 // handle sign
 if (f < 0.0)
 {
   s[index++] = '-';
   f = -f;
 }
 // handle infinite values
 if (isinf(f))
 {
   strcpy(&s[index], "INF");
   return s;
 }
 // handle Not a Number
 if (isnan(f))
 {
   strcpy(&s[index], "NaN");
   return s;
 }

 // max digits
 if (digits > 16) digits = 16;
 int exponent = int(log10(f));
 double g = f / pow(10, exponent);
 if ((g < 1.0) && (g != 0.0))      
 {
   g *= 10;
   exponent--;
 }
 if (exponent < -330) {  // lower limit of double-precision on Teensy 3
   g = 0;
   exponent = 0;
 }
 if (digits < 16) {  // display number rounded at last digit
   g += 0.5 / pow(10,digits);
 }
 
 d = g;  
 sprintf(&s[index++],"%d",d);  
 if (digits > 0) sprintf(&s[index++],".");
 for (int i=0;i<digits;i++) {
     g = (g - d) * 10.0;  // shift one decimal place to the left
     d = int(g);
     sprintf(&s[index++],"%d",d);
 }
 sprintf(&s[index], " E%+d", exponent);
 return s;    
} // ===== end double2s()
*/
