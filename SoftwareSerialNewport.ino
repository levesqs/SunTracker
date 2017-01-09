/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>
int iBTDC=4;
int iBTDCr=5;
int i=0;
char* buf = new char[32 + 1];
char* com = new char[16 + 1];
char ch;
int inited=0;
SoftwareSerial mySerial(6, 7,false); // RX, TX

void setup() {
  //buf = new char[32 + 1];
  // Open serial communications and wait for port to open:
  Serial.begin(9600,SERIAL_8N1);
  while (!Serial) {; /* wait */ }
  pinMode(iBTDC,INPUT);
  pinMode(iBTDCr,INPUT);
  mySerial.begin(57600);
  initNP();
}

void loop() { // run over and over
  
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  while (Serial.available()) {
    mySerial.write(Serial.read());
  }

  while (!digitalRead(iBTDC)){
    NewFor();
    i--;
  }
  while (!digitalRead(iBTDCr)){
    NewRev();
    i++;
  }
  if (!digitalRead(2)){
    Serial.println("exit");
    Serial.end();
    mySerial.end();
    while(true){
      delay(5000);
    }
  }
} //loop

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
    //Serial.println(buf);
  }//init loop end
  //Serial.write("\r\n");
  */
  buf[0]='\0';
//mychk();
}//NewFor


void initNP()
{//init newport
  Serial.println("\r\ninit Newport");
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
/*
  // oh loop
  Serial.println("send 1OH5  ");
  mySerial.print("1OH5\r\n");
  delay(200);
  mychk();
  //mystr();
  //Serial.write(buf);
  //Serial.write("\r\n");
  buf[0]='\0';
  // oh loop end
  
  // oh? loop
  Serial.print("send 1OH?  ");
  mySerial.print("1OH?\r\n");
  delay(200);
  mystr();
  Serial.write(buf);
  Serial.write("\r\n");
  buf[0]='\0';
  mychk();
  // oh? loop end
*/  
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
      inited=0;    
    }
    if(!strcmp("1TS00001E", buf))
    {
      Serial.print("  initializing... \r\n");
      inited=0;    
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
  // SR? loop end

  // SL loop
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
  
  inited=1;
  i=0;
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
  //char buf[32]="";
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
  //return(buf);
}
  
void append(char* s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}
