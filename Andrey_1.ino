//Импорт библиотеки ПИД-регулятора
#include <GyverPID.h>
GyverPID regulator(15, 3, 5, 10);
#include <AsyncStream.h>  // асинхронное чтение сериал
AsyncStream<50> serial(&Serial, ';');   // указываем обработчик и стоп символ
#define MOTOR1_IN3 2
#define MOTOR1_PWM 3
#define ENC_POWER 6
#define ENC_A 7
#define ENC_B 8

//Переменные отвечающие за время импульса на каждом канале 
unsigned long tA=0;
unsigned long tB=0;
//Переменная отвечающая за количество оборотов 
unsigned long RPM_A=0;
// Выходное значение регулятора
unsigned int output_r;
// Уставка реуглятора
unsigned int val;

void setup() {
  pinMode(MOTOR1_IN3, OUTPUT);
  pinMode(MOTOR1_PWM, OUTPUT);
  pinMode(ENC_POWER, OUTPUT);
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  
  Serial.begin(19200);
  Serial.setTimeout(1);
}

void loop() {
  
  static uint32_t tmr = 0;
  if (millis() - tmr > 5) {
    Serial.print(0);
    Serial.print(',');
    Serial.println(RPM_A);
    Serial.print(1);
    Serial.print(',');
    Serial.println(output_r);
    if (Serial.available()) {
    val = serial.buf;
    regulator.setpoint = val;
    }
  }
  
   //HIGH-считаем длинну положительного импульса 
   tA=pulseIn(ENC_A,HIGH);
   tB=pulseIn(ENC_B,HIGH);
   // Расчет оборотов на выходном валу 
   // RPM_A=((1000000/(2*tA))*60)/1640; 18292.683
   // Преобразованное значение количества оборотов на выходном валу 
   RPM_A=18292.683/tA;
   regulator.input = RPM_A;
  
  
  //===============================
  output_r = regulator.getResultTimer();
  if ( regulator.setpoint > 0 ){
    digitalWrite(ENC_POWER, 1);
    analogWrite(MOTOR1_PWM, output_r);
  }
  else{
    analogWrite(MOTOR1_PWM, 0);
    digitalWrite(ENC_POWER, 0);
  }
  
  
  

}
