

int LED_BUILTIN = 2;
bool isLED = false;

void setup() {
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor to open
  pinMode (LED_BUILTIN, OUTPUT); 
} 

void loop() {

  char comm = Serial.read();
  // put your main code here, to run repeatedly:
//  Serial.printf("test");
//  Serial.printf("\n");



  if (comm == 'f')
  {
    isLED = !isLED;

    if (isLED)
    {
      digitalWrite(LED_BUILTIN, HIGH);
    } else
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
   
  }
}
