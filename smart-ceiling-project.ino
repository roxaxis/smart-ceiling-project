// The following is a simple stepper motor control procedures
# define EN 8 // stepper motor enable , active low
# define Y_DIR 7 // y -axis stepper motor direction control
# define Y_STP 4 // y -axis stepper control

#include <SoftwareSerial.h>   //Software Serial Port
#define RxD 2
#define TxD 3

SoftwareSerial BLE(RxD, TxD);

int stepCount = 500;
//bool flag for step motor
boolean turned = false;
boolean systemPosition = true;
 boolean numberIsNegative = false;

#define DEBUG_ENABLED  1

/*
  / / Function : step . function: to control the direction of the stepper motor , the number of steps .
  / / Parameters : dir direction control , dirPin corresponding stepper motor DIR pin , stepperPin corresponding stepper motor " step " pin , Step number of step of no return value.
*/

float  GetValue(String pString){
  char vTempValue[10];
  pString.toCharArray(vTempValue,sizeof(vTempValue));
  return  atof(vTempValue);
}

void step (boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite (dirPin, dir);
  delay (50);
  for (int i = 0; i < steps; i++)
  {
    digitalWrite (stepperPin, HIGH);
    delayMicroseconds (800);
    digitalWrite (stepperPin, LOW);
    delayMicroseconds (800);
  }
}

void setup () {   // The stepper motor used in the IO pin is set to output
  Serial.begin(9600);

  //FOR BLUETOOTH SHIELD
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBleConnection();

  //FOR STEP MOTOR
  pinMode (Y_DIR, OUTPUT); pinMode (Y_STP, OUTPUT);
  pinMode (EN, OUTPUT);
  digitalWrite (EN, HIGH);
}

void loop () {
  //STEP MOTOR  
  
  //FOR BLUETOOTH SHIELD
  char recvChar;
  String recvString;
  
  while (1) {
     digitalWrite(EN,HIGH);
    if (BLE.available()) { //check if there's any data sent from the remote BLE shield
      recvString = BLE.readString();
      recvChar = BLE.read();

      int recvStringInt = recvString.toInt();
      float n =  GetValue(recvString);

      
           if (n > 0) {
              digitalWrite(EN,LOW);
              BLE.write("up");
              step (true, Y_DIR, Y_STP, n); // y axis motor reverse 1 ring, the 200 step is a circle.
            } else {
              digitalWrite(EN,LOW);
              BLE.write("down");
              step (false, Y_DIR, Y_STP, -n); // y axis motor reverse 1 ring, the 200 step is a circle.
              
          }
      }
  }
}

void setupBleConnection() {
  BLE.begin(9600); //Set BLE BaudRate to default baud rate 9600
  BLE.print("AT+CLEAR"); //clear all previous setting
  BLE.print("AT+ROLE0"); //set the bluetooth name as a slaver
  BLE.print("AT+SAVE1");  //don't save the connect information
}


