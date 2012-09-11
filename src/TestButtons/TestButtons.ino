#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h> 
#include <SFEMP3Shield.h>

SFEMP3Shield mp3;

void ledOff(int p)
{
  // this sets the pin into a high-impedance state, essentially "disconnecting" it from the circuit
  pinMode(p, INPUT);
  digitalWrite(p, LOW);
}
void ledOn(int p)
{
  pinMode(p, OUTPUT);
  digitalWrite(p, p < 32 ? LOW : HIGH);
}

int readBank(int column)
{
  static int voltage;
  voltage = analogRead(column);
  Serial.print(voltage);
  Serial.print(" ");
  // these need to be selected based on the resistors in the analog button encoders
  if(voltage < 209) return 0;
  else if(voltage < 465) return 1;
  else if(voltage < 577) return 2;
  else if(voltage < 670) return 3;
  else if(voltage < 727) return 4;
  else if(voltage < 766) return 5;
  else if(voltage < 794) return	6;
  return 7;
}

void setup()
{
  Serial.begin(115200);
int result =   mp3.begin();
Serial.println(result);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  for(int i = 0; i < 18; ++i)
    ledOn(i + 14);
}

void loop()
{
  for(int column = 0; column < 6; ++column)
  {
    Serial.print(column);
    Serial.print(" -> ");
  int code = readBank(column+8);
  Serial.println(code);
  if(code & 1){
      if(mp3.isPlaying())
        mp3.stopTrack();
    mp3.playTrack(0);
    ledOff(column * 3 + 14);
    delay(500);
  }
    else  ledOn(column * 3 + 14);
  if(code & 2)
    ledOff(column * 3 + 15);
    else ledOn(column * 3 + 15);
  if(code & 4)
    ledOff(column * 3 + 16);
    else ledOn(column * 3 + 16);
  }
  Serial.println();
}
