//#include <RH_ASK.h>
//#ifdef RH_HAVE_HARDWARE_SPI
//#include <SPI.h> // Not actually used but needed to compile
//#endif
#define trigPin 3
#define echoPin 4
//RH_ASK driver(2000, 11, 12, 0); //Rx, Tx
void setup() {
  pinMode(trigPin, 1);
  pinMode(echoPin, 0);
  // #ifdef RH_HAVE_SERIAL
  //    Serial.begin(115200);    // Debugging
  //#endif
  //    if (!driver.init())
  //#ifdef RH_HAVE_SERIAL
  //         Serial.println("init failed");
  //#else
  //  ;
  //#endif
  Serial.begin(9600);
}

void loop() {
  String msg = String(measureHeight());
  //  int Size = msg.length();
  //  char arr[Size];
  //  for(int i = 0; i < Size; i++)
  //  {
  //    arr[i] = msg[i];
  //    Serial.print(arr[i]);
  //  }
  //  Serial.print('\n');
  //  driver.send((uint8_t *)arr, Size);
  //  driver.waitPacketSent();
  Serial.println(msg);
  delay(700);
}

float measureHeight()
{
  unsigned long duration;
  digitalWrite(trigPin, 0);
  digitalWrite(trigPin, 1);
  delayMicroseconds(10);
  digitalWrite(trigPin, 0);
  duration = pulseIn(echoPin, HIGH);
  duration /= 2;
  float distance = 0.0332 * duration;
  return distance;
}
