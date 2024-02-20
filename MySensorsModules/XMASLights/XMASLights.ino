/*
 * Karácsonyi világítás
 * Egyenlőre csak a teljes ki és bekapcsolás működik
 * 1.0
*/
// Enable debug prints to serial monitor

//#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_RF24
//#define MY_RADIO_NRF5_ESB
//#define MY_RADIO_RFM69
//#define MY_RADIO_RFM95
#define MY_RF24_CHANNEL (124) //Udvar
// Enable repeater functionality for this node
#define MY_REPEATER_FEATURE

#include <MySensors.h>


#define NUMBER_OF_OUTPUTS 20 // Total number of attached relays
#define ALL_ON 255
#define ALL_OFF 0
#define ALL_ON2 247
#define ALL_OFF2 8
//Pin connected to ST_CP of 74HC595
#define latchPin  4
//Pin connected to SH_CP of 74HC595
#define clockPin  5
////Pin connected to DS of 74HC595
#define dataPin  3

//              00000000
//              87654321
byte AllOn3  = 0b11111111;
byte AllOff3 = 0b00000000;
//               11111110
//               65432109
byte AllOn2  = 0b11111111;
byte AllOff2 = 0b00000000;
//               22222111
//               43210987
byte AllOn1 =  0b11110111;
byte AllOff1 = 0b00001000;

void before()
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin,  ALL_OFF2);
    shiftOut(dataPin, clockPin,  ALL_OFF);
    shiftOut(dataPin, clockPin,  ALL_OFF);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
}

void setup()
{

}

void presentation()
{
	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("LATCH", "1.0");

	for (int sensor=1;  sensor<=NUMBER_OF_OUTPUTS; sensor++ ) {
		// Register all sensors to gw (they will be created as child devices)
		present(sensor, S_BINARY);
	}
}


void loop()
{

}

void receive(const MyMessage &message)
{
	// We only expect one type of message from controller. But we better check anyway.
	if (message.getType()==V_STATUS) {
  /*  if (message.getSensor()==1){
      digitalWrite(latchPin, message.getBool()?ALL_ON:ALL_OFF);
    }
    if (message.getSensor()==2){
      digitalWrite(dataPin, message.getBool()?ALL_ON:ALL_OFF);
    }
    if (message.getSensor()==3){
      digitalWrite(clockPin, message.getBool()?ALL_ON:ALL_OFF);
    }*/
		// Change relay state
    digitalWrite(latchPin, LOW);

    // shift out the bits:
    shiftOut(dataPin, clockPin,  message.getBool()?ALL_ON2:ALL_OFF2);
    shiftOut(dataPin, clockPin,  message.getBool()?ALL_ON:ALL_OFF);
    shiftOut(dataPin, clockPin,  message.getBool()?ALL_ON:ALL_OFF);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
	}
}

// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {

  int i=0;

  int pinState;

  digitalWrite(myDataPin, 0);

  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut&#xFFFD;

  //NOTICE THAT WE ARE COUNTING DOWN in our for loop

  //This means that %00000001 or "1" will go through such

  //that it will be pin Q0 that lights.

  for (i=7; i>=0; i--)  {

    digitalWrite(myClockPin, 0);

   if ( myDataOut & (1<<i) ) {

      pinState= 1;

    }

    else {

      pinState= 0;

    }

    //Sets the pin to HIGH or LOW depending on pinState

    digitalWrite(myDataPin, pinState);


    //register shifts bits on upstroke of clock pin

    digitalWrite(myClockPin, 1);


    //zero the data pin after shift to prevent bleed through

    digitalWrite(myDataPin, 0);


  }

  //stop shifting

  digitalWrite(myClockPin, 0);

}
