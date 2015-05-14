
#define pressMinDelay 50
#define pressMaxDelay 750

//Пин подключен к ST_CP входу 74HC595
#define latch_Pin 8
//Пин подключен к SH_CP входу 74HC595
#define clock_Pin 12
//Пин подключен к DS входу 74HC595
#define data_Pin 11

volatile int state = LOW;

volatile long lastPressed = 0;

int i = 0;

void setup(){
  
  pinMode(latch_Pin, OUTPUT);
  pinMode(data_Pin, OUTPUT); 
  pinMode(clock_Pin, OUTPUT);
  
  pinMode(13, OUTPUT);

  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);


  //set digital input 2 as interrupt

  attachInterrupt(0, button_up_down, CHANGE);

  sei();//allow interrupts
  Serial.begin(9600);
  Serial.println("reset0");
}//end setup

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz
  i++;
  if (i > 59){
    i=0;
  }
  registerWrite(i);
}

void button_up_down(){
      Serial.println("change");
  if (digitalRead(2) == HIGH) button_down();  // you can use direct port read to be faster - http://www.arduino.cc/en/Reference/PortManipulation -
  else button_up();

}

void button_down(){
    Serial.println("down");
  lastPressed = millis();
}

void button_up(){
    Serial.println("up");
  long elapsedMillis = millis() - lastPressed;
      Serial.println(elapsedMillis);
  if ( (elapsedMillis > pressMinDelay && elapsedMillis < pressMaxDelay) || elapsedMillis < 0){ //overflow
    button_press();
    lastPressed = millis();
  }
}

void button_press(){
    Serial.println("press");
  i=0;
  registerWrite(i);
  //DEBUG
  state=!state;
  digitalWrite(13, state);

}

int powInt(int base, byte power){
  int result = 1;
  for (int i=0; i < power; i++){
    result *= base;
  }
  return result;
}

// Этот метод записывает байт в регистр
void registerWrite(int digitToSet) {
  byte convertedDigit = 0;
  //Отключаем вывод на регистре
  digitalWrite(latch_Pin, LOW);
  byte j = 0;
  for (int i = 1; digitToSet / i > 0; i*=10){
    convertedDigit += (digitToSet % (i * 10) / i) << (4 * j);
    j++;
  }
  //for(int i = 0; i < (digitToSet / 10) + 1; i++){
  //  convertedDigit += (digitToSet % powInt(10,i+1) / powInt(10,i)) >> (4 * i);
  //}
  Serial.println(digitToSet);
  // проталкиваем байт в регистр
  shiftOut(data_Pin, clock_Pin, MSBFIRST, convertedDigit);
 
    // "защелкиваем" регистр, чтобы байт появился на его выходах
  digitalWrite(latch_Pin, HIGH);
}

void loop(){
  //do other things here
}

