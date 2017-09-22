/* create a hardware timer */
hw_timer_t * timer = NULL;
/* LED pin */
const int controlDrimer = 14;
const int detectorZeroCross = 12;
int pA=1;
/* LED state */
volatile byte state = LOW;
String inString="";
volatile int valor=5;// arranca al 80%
volatile int timing=5000;

void IRAM_ATTR Dimmer(){
  digitalWrite(controlDrimer,HIGH);
  timerStop(timer);
//  Serial.println("Dimmer "+String(millis()));
}

/*
void task1( void * parameter ){
 while(true){
    Serial.println("task1: "+String(millis()));
    digitalWrite(controlDrimer, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(retardo);                       // wait for a second
    digitalWrite(controlDrimer, LOW);    // turn the LED off by making the voltage LOW
    delay(retardo); 
  }
  vTaskDelete( NULL );
}
*/

void setup() {
  Serial.begin(115200);
  pinMode(controlDrimer, OUTPUT);
  digitalWrite(controlDrimer,LOW);
  pinMode(detectorZeroCross, INPUT);
  noInterrupts();
//  xTaskCreate( task1,"Task1",10000,NULL,pA,NULL); 
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &Dimmer, true);
  timerAlarmWrite(timer, timing, true);
  Serial.println(valor);
  attachInterrupt(digitalPinToInterrupt(detectorZeroCross),zeroCross,RISING );
  interrupts();
}

void loop(){  
  
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    //  Serial.println(inChar);
    if (isDigit(inChar)) {    
        inString += (char)inChar; 
        valor=inString.toInt();  
        Serial.print(valor);Serial.print("\r");
        
        inString = "";
       }
 /*   if (inChar == '\r') {
        valor=inString.toInt();   
        Serial.println(valor);
        inString = "";
       }*/
  }
}

void zeroCross(){
  
  digitalWrite(controlDrimer,LOW);
  noInterrupts();
  timing = 10000 -(valor*1000)  ;//1 seg = 1.000.000 us    1 ms = 1.000ms
  timerAlarmWrite(timer, timing, true);
  timerAlarmEnable(timer);
  timerRestart(timer);
  interrupts();
}

