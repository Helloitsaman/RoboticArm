#include <BluetoothSerial.h>
#include <ESP32Servo.h>

BluetoothSerial SerialBT;

//---------------- Servo Objects ----------------
Servo rotateServo;      // GPIO14
Servo baseServo;        // GPIO25
Servo shoulderServo;    // GPIO26
Servo elbowServo;       // GPIO27
Servo clawLeftServo;    // GPIO32
Servo clawRightServo;   // GPIO33

//---------------- Pins ----------------
#define ROTATE_PIN     14
#define BASE_PIN       25
#define SHOULDER_PIN   26
#define ELBOW_PIN      27
#define CLAW_LEFT_PIN  32
#define CLAW_RIGHT_PIN 33

//---------------- Command characters ----------------
// Everything is upper-cased before it is compared, so 'x' and 'X'
// both work. Change these if your gamepad app sends different chars.
#define CLAW_OPEN_CMD   'S'   // square button -> claw opens while held
#define CLAW_CLOSE_CMD  'X'   // X button      -> claw closes while held
#define STOP_CMD        'Z'   // 'S' is no longer free, so stop moved here
#define HOME_CMD        'H'

//---------------- Angles ----------------
int rotateAngle = 90;
int baseAngle = 90;
int shoulderAngle = 90;
int elbowAngle = 90;
int clawAngle = 45;

//---------------- Settings ----------------
const int STEP = 2;
const unsigned long MOVE_INTERVAL = 20;

// If the app keeps re-sending the character while a button is held,
// this makes the arm stop on its own shortly after you let go.
// Set to 0 to disable and rely only on an explicit release character.
const unsigned long HOLD_TIMEOUT = 250;

char currentCommand = STOP_CMD;
unsigned long previousMillis = 0;
unsigned long lastCommandMillis = 0;

void setup() {

  Serial.begin(115200);

  SerialBT.begin("Robotic_Arm");

  // Reserve all 4 available LEDC timers up front. Without this, the
  // ESP32Servo library can silently let multiple Servo objects share
  // the same timer/channel when you attach() several in a row - which
  // shows up as "only one servo ever moves."
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  rotateServo.setPeriodHertz(50);
  baseServo.setPeriodHertz(50);
  shoulderServo.setPeriodHertz(50);
  elbowServo.setPeriodHertz(50);
  clawLeftServo.setPeriodHertz(50);
  clawRightServo.setPeriodHertz(50);

  rotateServo.attach(ROTATE_PIN, 500, 2400);
  baseServo.attach(BASE_PIN, 500, 2400);
  shoulderServo.attach(SHOULDER_PIN, 500, 2400);
  elbowServo.attach(ELBOW_PIN, 500, 2400);
  clawLeftServo.attach(CLAW_LEFT_PIN, 500, 2400);
  clawRightServo.attach(CLAW_RIGHT_PIN, 500, 2400);

  homePosition();

  Serial.println("Bluetooth Ready");
}

void loop() {

  while (SerialBT.available()) {

    char cmd = SerialBT.read();

    // Ignore line endings some apps append to every packet
    if (cmd == '\r' || cmd == '\n' || cmd == ' ') {
      continue;
    }

    cmd = toupper(cmd);   // so 'x' and 'X' behave identically

    Serial.print("Received: ");
    Serial.println(cmd);

    lastCommandMillis = millis();

    if (cmd == STOP_CMD) {
      currentCommand = STOP_CMD;   // Stop movement
    }
    else if (cmd == HOME_CMD) {
      homePosition();
      currentCommand = STOP_CMD;
    }
    else {
      currentCommand = cmd;
    }
  }

  // Auto-stop if the phone stops repeating the held character
  if (HOLD_TIMEOUT > 0 &&
      currentCommand != STOP_CMD &&
      millis() - lastCommandMillis > HOLD_TIMEOUT) {
    currentCommand = STOP_CMD;
  }

  if (millis() - previousMillis >= MOVE_INTERVAL) {

    previousMillis = millis();

    switch (currentCommand) {

      // ---------- Base Rotation ----------
      case 'Q':
        rotateAngle -= STEP;
        break;

      case 'E':
        rotateAngle += STEP;
        break;

      // ---------- Base ----------
      case 'L':
        baseAngle -= STEP;
        break;

      case 'R':
        baseAngle += STEP;
        break;

      // ---------- Shoulder ----------
      case 'F':
        shoulderAngle += STEP;
        break;

      case 'B':
        shoulderAngle -= STEP;
        break;

      // ---------- Elbow ----------
      case 'A':
        elbowAngle += STEP;
        break;

      case 'C':
        elbowAngle -= STEP;
        break;

      // ---------- Claw ----------
      case CLAW_OPEN_CMD:      // square held
        clawAngle += STEP;
        break;

      case CLAW_CLOSE_CMD:     // X held
        clawAngle -= STEP;
        break;

      default:                 // includes STOP_CMD - hold position
        break;
    }

    rotateAngle = constrain(rotateAngle, 0, 180);
    baseAngle = constrain(baseAngle, 0, 180);
    shoulderAngle = constrain(shoulderAngle, 0, 180);
    elbowAngle = constrain(elbowAngle, 0, 180);
    clawAngle = constrain(clawAngle, 0, 90);

    rotateServo.write(rotateAngle);
    baseServo.write(baseAngle);
    shoulderServo.write(shoulderAngle);
    elbowServo.write(elbowAngle);

    // Opposite motion for gripper
    clawLeftServo.write(clawAngle);
    clawRightServo.write(180 - clawAngle);
  }
}

void homePosition() {

  rotateAngle = 90;
  baseAngle = 90;
  shoulderAngle = 90;
  elbowAngle = 90;
  clawAngle = 45;

  rotateServo.write(rotateAngle);
  baseServo.write(baseAngle);
  shoulderServo.write(shoulderAngle);
  elbowServo.write(elbowAngle);

  clawLeftServo.write(clawAngle);
  clawRightServo.write(180 - clawAngle);

  SerialBT.println("HOME");
}
