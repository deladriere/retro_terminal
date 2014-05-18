#include <TVout.h>
#include <pollserial.h>
#include <fontALL.h>

// attention orginal dans arduino (ne compile pas sur imac2)


unsigned long  maintenant=0;
unsigned long  last_serial=0;
static boolean output = HIGH;

TVout TV;


pollserial pserial;
int audio=11;
int period=500;
/*
  see modif https://code.google.com/p/arduino-tvout/issues/detail?id=34
Line 119 in pollserial.cpp is:
    if (rxbuffer.tail == BUFFER_SIZE)
but I believe it should be:
    if (rxbuffer.tail == (BUFFER_SIZE-1))
*/


void setup()  {
  TV.begin(PAL,184,72); // 184,72
  TV.select_font(font8x8ext); // was 6x8
  TV.println("Serial Terminal 22 cr.");
  TV.println("---- Version 0.2 -----");
   TV.print(">");
  TV.set_hbi_hook(pserial.begin(300));
  pinMode(audio, OUTPUT);
  
}

void loop() {
  int car;
 
 
  if (pserial.available()) {
     
    car= pserial.read();
    //TV.print((char)pserial.read()); // was this line
   
   if (car!= 32) beep(car*10,5); // @300 bps 10,5 //
  if (car== 13){
     //TV.print(char(10));
    // TV.print(">");
     TV.print(char(13));
     TV.print(char(10));
     TV.print(">");
     output=HIGH; // to make sure next blink is a _
     last_serial=millis(); // to avoid starting to blink to soon
     
     }
     else  
     {
       if(output==LOW){ // means : We just printed _ then delete it
        TV.print(char(8));
       }
       TV.print((char)car);
       last_serial=millis(); // to avoid starting to blink to soon
       output=HIGH; //to make sure next blink is a _ 
     }
     
 
    
     
  }
  
  if((millis()-last_serial)>=500){  // Start blinking after 1/2 second only
 
  if ((millis()-maintenant)>=200){ // speed of blink
    
    maintenant=millis();
    
     if (output==HIGH)
     {
     TV.print("_");
     output=LOW;
    
     
   }
     else
     {
        TV.print(char(8));
        output=HIGH;
     }
     
   
     }
  }
  
}



void beep (int ton,int duration){

 for (long i = 0; i < duration * 1000L; i += ton * 2) {
    digitalWrite(audio, HIGH);
    delayMicroseconds(ton);
    digitalWrite(audio, LOW);
    delayMicroseconds(ton);
  }

}

