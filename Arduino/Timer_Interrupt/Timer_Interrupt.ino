
#define pressDelay 150

//Пин подключен к ST_CP входу 74HC595
#define latch_Pin 8
//Пин подключен к SH_CP входу 74HC595
#define clock_Pin 12
//Пин подключен к DS входу 74HC595
#define data_Pin 11

volatile boolean toggle1 = 0;

volatile long lastPressed = 0;

int i = 0;

void setup(){
  
  pinMode(latch_Pin, OUTPUT);
  pinMode(data_Pin, OUTPUT); 
  pinMode(clock_Pin, OUTPUT);
  
  //pinMode(13, OUTPUT);

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

  attachInterrupt(0, button_press, RISING);

  sei();//allow interrupts

}//end setup

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz
  i++;
  if (i > 59){
    i=0;
  }
  registerWrite(i);
}

void button_press(){
  long currentMillis = millis();
  if ( currentMillis - lastPressed > pressDelay || currentMillis - lastPressed < 0){ //TODO: overflow
    lastPressed = currentMillis;
    i=0;
  }
}

// Этот метот записывает байт в регистр
void registerWrite(int digitToSet) {
 
  //Отключаем вывод на регистре
  digitalWrite(latch_Pin, LOW);
 
  // проталкиваем байт в регистр
  shiftOut(data_Pin, clock_Pin, MSBFIRST, digitToSet);
 
    // "защелкиваем" регистр, чтобы байт появился на его выходах
  digitalWrite(latch_Pin, HIGH);
}

void loop(){
  //do other things here
}

