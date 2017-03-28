 /******************************************************************************
 * Optical Tachometer
 *
 * Uses an IR LED and IR phototransistor to implement an optical tachometer.
 * The IR LED is connected to pin 13 and ran continually.
 * Pin 2 (interrupt 0) is connected across the IR detector.
 
Example sketch for SparkFun's QRD1114 Reflectance Proximity Sensor
  (https://www.sparkfun.com/products/246)
  (https://learn.sparkfun.com/tutorials/qrd1114-optical-detector-hookup-guide?_ga=1.57060067.834043405.1459523277)  
  Jim Lindblom @ SparkFun Electronics
  May 2, 2016
and 
Code based on: www.instructables.com/id/Arduino-Based-Optical-Tachometer/
  arduinoprojects101.com

Connect a QRD1114, 330 resistor and 10k resistor as follows:

QRD1114 Pin ---- Arduino ---- Resistors
    1              A0      10k Pull-up to 5V
    2              GND
    3                      330 Resistor to 5V
    4              GND

As an object comes closer to the QRD1114, the voltage on A0 should go down.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/

//volatile byte count;
//float count;
unsigned long count;
unsigned long display_count;
unsigned int time;
unsigned long timeold;
unsigned int rpm;
unsigned int rps;
unsigned long trigNum = 4;  //number of trigger points on encoder wheel


void rpm_fun()
 {
   //Each rotation, this interrupt function is run twice, so take that into consideration for 
   //calculating RPM
   //Update count
   //count++;  // use if count is byte
   count +=1;
      
 }

void setup()
 {   
   Serial.begin(9600);
   //Interrupt 0 is digital pin 2, so that is where the IR detector is connected
   //Triggers on FALLING (change from HIGH to LOW)
//   attachInterrupt(0, rpm_fun, FALLING);
   attachInterrupt(digitalPinToInterrupt(2), rpm_fun, RISING);

   count = 0;
   rpm = 0;
   timeold = 0;
 }

 void loop()
 {
   //Update RPM every second
   delay(1000);
   //Don't process interrupts during calculations
   detachInterrupt(0);
   //Note that this would be 60*1000/(millis() - timeold)*count if the interrupt
   //happened once per revolution instead of twice. Other multiples could be used
   //for multi-bladed propellers or fans
   //Serial.print(count);
   time = millis();
   
   rps = 1000 * count / (trigNum*(time - timeold));
   //rps = count / (trigNum*(time - timeold)/1000);

   rpm = 60 * (1000 * count / (trigNum*(time - timeold)));
   //rpm = 60 * count / (trigNum*(time - timeold)/1000);
   display_count = count;
  
   timeold = millis();
   count = 0;

   //Print out result to serial monitor
   Serial.print("rps=");
   Serial.println(rps);
   Serial.print("RPM=");
   Serial.println(rpm);
   Serial.println(display_count);
   Serial.println("");
   

   //Restart the interrupt processing
//   attachInterrupt(0, rpm_fun, FALLING);
   attachInterrupt(digitalPinToInterrupt(2), rpm_fun, FALLING);
  }
