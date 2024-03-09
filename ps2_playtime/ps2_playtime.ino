#define KEY_A 0x38
#define KEY_B 0x4C
#define KEY_C 0x84
#define KEY_D 0xC4
#define KEY_E 0x24
#define KEY_F 0xD4
#define KEY_G 0x2C
#define KEY_H 0xCC
#define KEY_I 0xC2
#define KEY_J 0xDC
#define KEY_K 0x42
#define KEY_L 0xD2
#define KEY_M 0x5C
#define KEY_N 0x8C
#define KEY_O 0x22
#define KEY_P 0xB2
#define KEY_Q 0xA8
#define KEY_R 0xB4
#define KEY_S 0xD8
#define KEY_T 0x34
#define KEY_U 0x3C
#define KEY_V 0x54
#define KEY_W 0xB8
#define KEY_X 0x44
#define KEY_Y 0xAC
#define KEY_Z 0x58

#define KEY_0 0xA2
#define KEY_1 0x68
#define KEY_2 0x78
#define KEY_3 0x64
#define KEY_4 0xA4
#define KEY_5 0x74
#define KEY_6 0x6C
#define KEY_7 0xBC
#define KEY_8 0x7C
#define KEY_9 0x62

#define KEY_NUM_0 0x0E
#define KEY_NUM_1 0x96
#define KEY_NUM_2 0x4E
#define KEY_NUM_3 0x5E
#define KEY_NUM_4 0xD6
#define KEY_NUM_5 0xCE
#define KEY_NUM_6 0x2E
#define KEY_NUM_7 0x36
#define KEY_NUM_8 0xAE
#define KEY_NUM_9 0xBE

#define KEY_SPACEBAR 0x94
#define KEY_ENTER 0x5A
#define KEY_ESC 0x6E
#define KEY_TAB 0xB0

#define KEY_NUM_LOCK 0xEE
#define KEY_CAPS_LOCK 0x1A
#define KEY_SCROLL_LOCK 0x7E

#define PIN_DTA 35
#define PIN_CLK 34

#define MESSAGE_LENGTH 11

volatile bool HEX_MODE = true;

volatile uint16_t message = 0;
volatile bool message_ready = false;

bool ignore_next = false;

void ps2_interrupt_handler(void);
void interpretKey(uint16_t key_code);

void setup() {
  Serial.begin(9600);

  pinMode(PIN_DTA, INPUT_PULLUP);
  pinMode(PIN_CLK, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_CLK), ps2_interrupt_handler, FALLING);
}

void loop() {
  static uint8_t count = 0;

  if (message_ready) {
    message = (message >> 2) & 0xFF;

    if (message == 0x0F) {
      ignore_next = true;
    }
    else {
      if (ignore_next) {
        ignore_next = false;
      }
      else {
        if (message == KEY_TAB){
          Serial.println();
          HEX_MODE = !HEX_MODE;
        }

        if (HEX_MODE) {
          Serial.print(message, HEX);
          Serial.print(",");
        }
        else {
          interpretKey(message);
        }
      }
    }

    message_ready = false;
  }
}

void ps2_interrupt_handler(void) {

  // Serial.print(digitalRead(PIN_DTA));
  // return;

  static uint16_t buffer = 0;
  static uint8_t buffer_index = 0;

  if (buffer_index < MESSAGE_LENGTH) {
    buffer <<= 1;
    buffer |= digitalRead(PIN_DTA);

    if (++buffer_index == MESSAGE_LENGTH) {
      message = buffer;
      buffer = 0;
      buffer_index = 0;
      message_ready = true;
    }
  }
}

void interpretKey(uint16_t key_code) {
  switch (key_code) {
    case KEY_A: Serial.print("A"); break;
    case KEY_B: Serial.print("B"); break;
    case KEY_C: Serial.print("C"); break;
    case KEY_D: Serial.print("D"); break;
    case KEY_E: Serial.print("E"); break;
    case KEY_F: Serial.print("F"); break;
    case KEY_G: Serial.print("G"); break;
    case KEY_H: Serial.print("H"); break;
    case KEY_I: Serial.print("I"); break;
    case KEY_J: Serial.print("J"); break;
    case KEY_K: Serial.print("K"); break;
    case KEY_L: Serial.print("L"); break;
    case KEY_M: Serial.print("M"); break;
    case KEY_N: Serial.print("N"); break;
    case KEY_O: Serial.print("O"); break;
    case KEY_P: Serial.print("P"); break;
    case KEY_Q: Serial.print("Q"); break;
    case KEY_R: Serial.print("R"); break;
    case KEY_S: Serial.print("S"); break;
    case KEY_T: Serial.print("T"); break;
    case KEY_U: Serial.print("U"); break;
    case KEY_V: Serial.print("V"); break;
    case KEY_W: Serial.print("W"); break;
    case KEY_X: Serial.print("X"); break;
    case KEY_Y: Serial.print("Y"); break;
    case KEY_Z: Serial.print("Z"); break;

    case KEY_0: Serial.print("0"); break;
    case KEY_1: Serial.print("1"); break;
    case KEY_2: Serial.print("2"); break;
    case KEY_3: Serial.print("3"); break;
    case KEY_4: Serial.print("4"); break;
    case KEY_5: Serial.print("5"); break;
    case KEY_6: Serial.print("6"); break;
    case KEY_7: Serial.print("7"); break;
    case KEY_8: Serial.print("8"); break;
    case KEY_9: Serial.print("9"); break;

    case KEY_NUM_0: Serial.print("0"); break;
    case KEY_NUM_1: Serial.print("1"); break;
    case KEY_NUM_2: Serial.print("2"); break;
    case KEY_NUM_3: Serial.print("3"); break;
    case KEY_NUM_4: Serial.print("4"); break;
    case KEY_NUM_5: Serial.print("5"); break;
    case KEY_NUM_6: Serial.print("6"); break;
    case KEY_NUM_7: Serial.print("7"); break;
    case KEY_NUM_8: Serial.print("8"); break;
    case KEY_NUM_9: Serial.print("9"); break;

    case KEY_SPACEBAR: Serial.print(" "); break;
    case KEY_ENTER: Serial.print("\n"); break;
    case KEY_ESC: break;
    case KEY_TAB: break;

    case KEY_NUM_LOCK: break;
    case KEY_CAPS_LOCK: break;
    case KEY_SCROLL_LOCK: break;

    default:
      Serial.print("<N/A>");
      break;
  }
}

