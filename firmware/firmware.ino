// ledcSetup etc isn't meant for motors, but that's what the reference code used and I won't refactor that too much since hackathon and time crunch

#define LED_ONBOARD 2
#define MOTOR_1_FORWARD 0b00000010
#define MOTOR_2_FORWARD 0b00000001
// motor 1=left, 2=right

// Motor A
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14; 

int motor2Pin1 = 25; 
int motor2Pin2 = 33; 
int enable2Pin = 32; 

// Setting PWM properties
const int freq = 30000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int resolution = 8;
int dutyCycle1 = 200;
int dutyCycle2 = 200;


void blink() {
  digitalWrite(LED_ONBOARD, HIGH);
  delay(100);
  digitalWrite(LED_ONBOARD, LOW);
  delay(300);
  digitalWrite(LED_ONBOARD, HIGH);
  delay(100);
  digitalWrite(LED_ONBOARD, LOW);
  delay(300);
  digitalWrite(LED_ONBOARD, HIGH);
  delay(100);
  digitalWrite(LED_ONBOARD, LOW);
  delay(300);
}

void blinkN(int n) {
  for (int i = 1; i <= n; ++i)
  {
    digitalWrite(LED_ONBOARD, HIGH);
    delay(50);
    digitalWrite(LED_ONBOARD, LOW);
    delay(50);
  }
}

void superBlink() {
  blinkN(6);
}

void setup() {
  
  pinMode(LED_ONBOARD, OUTPUT);


  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel1, freq, resolution);
  ledcSetup(pwmChannel2, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel1);
  ledcAttachPin(enable2Pin, pwmChannel2);

  // Serial.begin(115200);
  Serial.begin(9600);

  // testing
  Serial.print("Testing DC Motor...");

  blink();
  
  ledcWrite(pwmChannel1, dutyCycle1);   
  ledcWrite(pwmChannel2, dutyCycle2);  
}

void setMotor1(bool forward, unsigned char val) {
  digitalWrite(motor1Pin1, forward ? LOW : HIGH);
  digitalWrite(motor1Pin2, forward ? HIGH : LOW);
  dutyCycle1 = val;
  ledcWrite(pwmChannel1, dutyCycle1);  
}
void setMotor2(bool forward, unsigned char val) {
  digitalWrite(motor2Pin1, forward ? LOW : HIGH);
  digitalWrite(motor2Pin2, forward ? HIGH : LOW);
  dutyCycle2 = val;
  ledcWrite(pwmChannel2, dutyCycle2);  
}

int incomingByte = 0; // for incoming serial data
int header = 0; // for incoming serial data

unsigned char pwmIn = 0;

unsigned char foo = 0;
char bar = 0;

char simple;
void simpleControls() {
  // 
  
  if (Serial.available() > 0) {
    simple = Serial.read();
    if (simple == 'w') {
      setMotor1(true, 250);
      setMotor2(true, 250);
    }
    if (simple == 's') {
      setMotor1(false, 250);
      setMotor2(false, 250);
    }
    if (simple == 'a') {
      setMotor1(true, 250);
      setMotor2(false, 250);
    }
    if (simple == 'd') {
      setMotor1(false, 250);
      setMotor2(true, 250);
    }
    if (simple == ' ') {
      setMotor1(false, 0);
      setMotor2(true, 0);
    }
    
    // setMotor1(true, 250);
    // setMotor2(true, 189);
    // delay(1000);
    
    // setMotor1(false, 250);
    // setMotor2(false, 250);
    // delay(1000);
    // setMotor1(false, 0);
    // setMotor2(false, 0);
    // delay(1000);

    
  
    // ledcWrite(pwmChannel1, dutyCycle1);   
    // ledcWrite(pwmChannel2, dutyCycle2); 
    // ledcWrite(pwmChannel1, dutyCycle1);   
    // ledcWrite(pwmChannel2, dutyCycle2);  
  }  
}

void controlViaSerialLoopSimple() {
  if (Serial.available() > 0) {
    blinkN(Serial.available());
    foo = Serial.read();
    
    Serial.println(foo, BIN);
    
    setMotor1(true, 250);
    setMotor2(true, 189);
    delay(1000);
    
    setMotor1(false, 250);
    setMotor2(false, 250);
    delay(1000);
    setMotor1(false, 0);
    setMotor2(false, 0);
    delay(1000);

    
  
    // ledcWrite(pwmChannel1, dutyCycle1);   
    // ledcWrite(pwmChannel2, dutyCycle2); 
    // ledcWrite(pwmChannel1, dutyCycle1);   
    // ledcWrite(pwmChannel2, dutyCycle2);  
  }  
}

void controlViaSerialLoop() {
  
  if (Serial.available() > 0) {
    // read the incoming byte:    
    // incoming byte is a header
    header = Serial.read();

    
    blinkN(Serial.available());

    if (header & 0b00010000) {
      // this flag signals that the signal is for updating the motors

      
      Serial.print("GOT A HEADER");
      blinkN(20);

      if (Serial.available() >= 2) {
        incomingByte = Serial.read();
        setMotor1(header & MOTOR_1_FORWARD != 0, incomingByte);
        incomingByte = Serial.read();
        setMotor2(header & MOTOR_2_FORWARD != 0, incomingByte);
      }

    }
    else {
      // TODO: handle unexpected headers
    }


    // say what you got:
    // Serial.print("I received: ");
    // Serial.println(incomingByte, DEC);
  }
}

void controlLoop() {
  superBlink(); //  to sigfnal this is what's runnigng

  
  Serial.print("motor header (one byte):");
  Serial.println(0b00010000, DEC);
  Serial.print("left motor forward flag:");
  Serial.println(0b00000010, DEC);
  Serial.print("right motor forward flag:");
  Serial.println(0b00000001, DEC);
  Serial.println("then a left pwm byte, then right pwm byte");
  while (true) {
    controlViaSerialLoop();
  }
}

void loop() {
  // choose one
  simpleControls();
  // controlLoop();
  // debugLoop();
  // controlViaSerialLoopSimple();
}

void debugLoop() {
  blink();
  
  ledcWrite(pwmChannel1, dutyCycle1);   
  ledcWrite(pwmChannel2, dutyCycle2);  
  
  // Move the DC motor forward at maximum speed
  Serial.println("Moving Forward");
  digitalWrite(LED_ONBOARD, HIGH);  // turn the LED on (HIGH is the voltage level)
  setMotor1(true, 200);
  setMotor2(true, 200);
  // digitalWrite(motor1Pin1, LOW);
  // digitalWrite(motor1Pin2, HIGH); 
  // digitalWrite(motor2Pin1, LOW);
  // digitalWrite(motor2Pin2, HIGH); 
  delay(2000);

  // Stop the DC motor
  digitalWrite(LED_ONBOARD, LOW);   // turn the LED off by making the voltage LOW
  Serial.println("Motor stopped");
  setMotor1(true, 0);
  setMotor2(true, 0);
  // digitalWrite(motor1Pin1, LOW);
  // digitalWrite(motor1Pin2, LOW);
  // digitalWrite(motor2Pin1, LOW);
  // digitalWrite(motor2Pin2, LOW);
  delay(1000);

  // Move DC motor backwards at maximum speed
  Serial.println("Moving Backwards");
  setMotor1(false, 200);
  setMotor2(false, 200);
  // digitalWrite(motor1Pin1, HIGH);
  // digitalWrite(motor1Pin2, LOW); 
  // digitalWrite(motor2Pin1, HIGH);
  // digitalWrite(motor2Pin2, LOW); 
  delay(2000);

  // Stop the DC motor
  Serial.println("Motor stopped");
  setMotor1(true, 0);
  setMotor2(true, 0);
  // digitalWrite(motor1Pin1, LOW);
  // digitalWrite(motor1Pin2, LOW);
  // digitalWrite(motor2Pin1, LOW);
  // digitalWrite(motor2Pin2, LOW);
  delay(1000);

  // // Move DC motor forward with increasing speed
  // digitalWrite(LED_ONBOARD, HIGH);  // turn the LED on (HIGH is the voltage level)
  // digitalWrite(motor1Pin1, LOW);
  // digitalWrite(motor1Pin2, HIGH);
  // digitalWrite(motor2Pin1, LOW);
  // digitalWrite(motor2Pin2, HIGH);
  // while (dutyCycle1 < 250){
  //   ledcWrite(pwmChannel1, dutyCycle1);   
  //   ledcWrite(pwmChannel2, dutyCycle2);   
  //   Serial.print("Forward with duty cycle: ");
  //   Serial.println(dutyCycle1);
  //   Serial.println(dutyCycle2);
  //   dutyCycle1 = dutyCycle1 + 5;
  //   dutyCycle2 = dutyCycle2 + 5;
  //   delay(500);
  // }
  dutyCycle1 = 200;
  dutyCycle2 = 200;
}