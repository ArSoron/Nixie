/*
  Shift Register Example
 for 74HC595 shift register
 
 Created 22 May 2009
 Created 23 Mar 2010
 by Tom Igoe
 
 */
 
//Пин подключен к ST_CP входу 74HC595
int latchPin = 8;
//Пин подключен к SH_CP входу 74HC595
int clockPin = 12;
//Пин подключен к DS входу 74HC595
int dataPin = 11;
int i = 0;
 
void setup() {
  //устанавливаем режим OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT); 
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("reset");
}
 
void loop() {
  if (Serial.available() > 0) {
    // Символы от '0' до '9'
    // представлены в ASCII таблице значения от 48 до 57.
    int digitToSet = Serial.read() - 47;
 
    // Записываем HIGH в позицию соответствующую bitToSet
    registerWrite(digitToSet);
  }
  for (int i=0; i < 6; i++){
    for (int j=0; j<10; j++){
      registerWrite(i);
      delay(500);
      registerWrite(j);
      delay(500);
    }
  }
}
 
// Этот метот записывает байт в регистр
void registerWrite(int digitToSet) {
 
  //Отключаем вывод на регистре
  digitalWrite(latchPin, LOW);
 
  // проталкиваем байт в регистр
  shiftOut(dataPin, clockPin, MSBFIRST, digitToSet);
 
    // "защелкиваем" регистр, чтобы байт появился на его выходах
  digitalWrite(latchPin, HIGH);
}

