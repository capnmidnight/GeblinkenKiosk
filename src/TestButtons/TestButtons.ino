#include "io.h"

void setup()
{
  Serial.begin(115200);
  ledOn(37);
}
void func1();
void func2();

void loop()
{
}
void func1()
{
  for(int i = MIN_INPUT_PIN; i <= MAX_INPUT_PIN; ++i)
  {
    int v = readColumn(i);
    Serial.print(v);
    Serial.print(" ");
  }
  Serial.println();
}
void func2()
{
  static boolean buffer[2][12] = {
    {
      false, false, false, 
      false, false, false, 
      false, false, false, 
      false, false, false    }
    ,
    {
      false, false, false, 
      false, false, false, 
      false, false, false, 
      false, false, false    }
  };

  boolean b = scanInput(buffer);
  if(b)
  {
    for(int y = 0; y < 4; ++y)
    {
      for(int x = 0; x < 6; ++x)
      {
        int bank = x / 3;
        int button = (x % 3) * 4 + y;
        Serial.print(buffer[bank][button]);
        Serial.print(" ");
        if(bank == 0 && x == 2)
        {
          Serial.print("| ");
        }
      }
      Serial.println();
    }
    Serial.println();
    delay(250);
  }
}

