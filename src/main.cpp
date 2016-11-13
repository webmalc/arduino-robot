/**
 * Remote for robot
 */
#include "Arduino.h"
#include "IRremote.h"

//Config
const int REMOTE_PIN = 2;
const long ROTATE_LEFT_CODE = 0xFF22DD;
const long ROTATE_RIGHT_CODE = 0xFFC23D;
const long FORWARD_CODE = 0xFF629D;
const long BACKWARD_CODE = 0xFF02FD;
const long AI_CODE = 0xFF52AD;

//Setup
IRrecv irrecv(REMOTE_PIN);
decode_results remoteResults;
const int ROTATE_LEFT = 1;
const int ROTATE_RIGHT = 2;
const int FORWARD = 3;
const int BACKWARD = 4;
const int AI_MODE = 9;
const int DEFAULT_ACTION = 0;
int ACTION = DEFAULT_ACTION;
bool AI = false;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
}

//Get remote control data
int remote()
{
  if (irrecv.decode(&remoteResults)) {
    // Serial.println(remoteResults.value, HEX);
    switch (remoteResults.value) {

      case ROTATE_LEFT_CODE:
        ACTION = ROTATE_LEFT;
        break;

      case ROTATE_RIGHT_CODE:
        ACTION = ROTATE_RIGHT;
        break;

      case FORWARD_CODE:
        ACTION = FORWARD;
        break;

      case BACKWARD_CODE:
        ACTION = BACKWARD;
        break;

      case AI_CODE:
        ACTION = AI_MODE;
        break;
      case 0xFFFFFFFF:
          break;
      default:
        ACTION = DEFAULT_ACTION;
        break;
    }
    irrecv.resume();
  } else {
    ACTION = DEFAULT_ACTION;
  }

  return ACTION;
}

//Main loop
void loop()
{
  int input = remote();
  if (input == AI_MODE) {
    AI = !AI;
  }
  Serial.println(input);
  delay(500);
}
