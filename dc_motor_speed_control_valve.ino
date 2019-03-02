/*
Arduino -. DC Motor
*/
 volatile byte tick;
unsigned int rpm;
unsigned long timeold;
int d;
int relay = 10;
int tdcpin = A0;
void setup() 
{ 
  Serial.begin(9600);
  while (! Serial);
  attachInterrupt(0, tick_count, RISING);  //Execute interrupt function (tick_count) defined below each rising edge on external interrupt 0 (pin 2);
  pinMode(relay , OUTPUT);                      //Set PWM pin 3 to output to motor driver;
 pinMode(tdcpin, INPUT);
} 
 //Interrupt function;
void tick_count()                          //Executed every rising edge on interrupt 0 (pin 2)
{  
    tick++;                                //Increment "tick" by 1;
}

 
void loop() 
{ 
  int tdcState = analogRead(tdcpin);
  if(tdcState>800)
  { 
    Serial.println("TDC");
  }
  int command;
  int pwm;
  int duty_cycle;
  
  command = analogRead(0);                 //Read analog signal on channel 0 of ADC and convert it into a 10-bit digital integer and store it in variable "command";
  pwm = map(command,0,1023,0,255);         //Scale the 10-bit digital value of the ADC with the 8-bit resolution of the PWM output to motor driver;  
  duty_cycle = pwm*100/255;                //Calculate PWM duty cycle in %;
  analogWrite(5,pwm);                      //Send the previously specified pwm signal on pin 5 to motor driver;
  
  //Start RPM calculations;
  if(tick>=4)                                       //When a full motor cycle is completed (motor has 4 blades)
   {
    rpm = 15*1000/(millis() - timeold)*tick;        //Divide tick counter "tick" by the time it took to do the 4 ticks using the "millis()" function. 60/4=15 giving the actual RPM of a motor;
    timeold = millis();                             //Save the previous time to use it for next iteration of speed calculation;
    tick = 0;                                       //Reset "tick" to zero to prepare it for the next iteration of speed calculation;
   }
  //End RPM Calculations
  
  //Send desired data over the Serial COM Port;
  //Serial.print("Command = ");    
  //Serial.print(command);
  //Serial.print("  ");
  //Serial.print("PWM = ");
  //Serial.print(duty_cycle);
  Serial.print("   RPM = ");
  Serial.println(rpm);
  delay(100);
  int k = d*rpm * 0.001885 ;//k = Kilometer Per Hour(km/hr) d = Wheel Diameter(cm) r = Revolution Per Minute(RPM)
  if(k > 100) //if  kmph>100 active relay
  {
    digitalWrite(relay,true);
  }

} 
