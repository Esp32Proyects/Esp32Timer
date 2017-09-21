
/* create a hardware timer */
hw_timer_t * timer = NULL;
/* LED pin */
const int led = 14;
const int controlDrimer = 12;
int pA=1;
/* LED state */
volatile byte state = LOW;
String inString="";
volatile int valor=1000000;

void IRAM_ATTR Dimmer(){
//  timerAlarmDisable(timer);
//  digitalWrite(led,HIGH);
 Serial.println("Dimmer");

}

void task1( void * parameter )
{

 while(true){
 
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("loop task 1 HIGH");
  delay(4000);                       // wait for a second
  
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("loop task 1 LOW");
  delay(4000); 
  
 }
    vTaskDelete( NULL );
}


void setup() {
  
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(controlDrimer, INPUT_PULLUP);
  /* Use 1st timer of 4 */
  /* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
  timer = timerBegin(0, 80, true);
 timerAttachInterrupt(timer, &Dimmer, true);
  xTaskCreate( task1,"Task1",10000,NULL,pA,NULL); 
  attachInterrupt(digitalPinToInterrupt(controlDrimer),zeroCross,RISING );
}

void loop() {
  
 while (Serial.available() > 0) {
    
    int inChar = Serial.read();
        
    if (isDigit(inChar)) {     
      inString += (char)inChar;
       }
    if (inChar == '\r') {
        valor=inString.toInt();   
        Serial.print(valor);Serial.print("\r"); 
        inString = "";
       }
  }
}


void zeroCross(){
 //digitalWrite(led,LOW);
  Serial.println("zeroCross");
 noInterrupts();
 
 timerAlarmWrite(timer, valor, false);
 timerAlarmEnable(timer);
 interrupts();
}

