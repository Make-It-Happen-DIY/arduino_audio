#include <SD.h>                           //include SD module library
#include <TMRpcm.h>                       //include speaker control library  https://github.com/TMRh20/TMRpcm/archive/master.zip

#define SD_ChipSelectPin 10                //define CS pin
#define button 2

TMRpcm sound;                            //crete an object for speaker library			


//debounce variables
int debounce = 500;       // wanted debounce time in milliseconds
long last_updated = 0;     //timestamp of previous interrupt
long curr_time;      // current time in as reported by millis()

// change lighting mode on button click
void button_ISR(){
  
  curr_time = millis();
  if ((curr_time - last_updated) > debounce){
      sound.stopPlayback();
      sound.play("like.wav");     // 8-bit, mono, 16-24khz at best
    }
  last_updated = curr_time;
}

void setup(){

    //interrupt setup
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(0, button_ISR, FALLING);

  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }                                       
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_ChipSelectPin)) {      //see if the card is present and can be initialized
    Serial.println("Initialization failed...");
    return;                               //don't do anything more if not
  }
  else {
      Serial.println("Initialization done...");
  }


  sound.speakerPin = 9;                  //define speaker pin. 
                                         //you must use pin 9 of the Arduino Uno and Nano
                                         //the library is using this pin
  sound.quality(1);                      // 0 or 1  -- 1 equals 2x oversampling
  sound.setVolume(5);                    //0 to 7. Set volume level
}

void loop(){
  delay(5);
  if(!sound.isPlaying()) {
   digitalWrite(sound.speakerPin, LOW);
 }
}
