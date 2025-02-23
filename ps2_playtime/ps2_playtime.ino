#include <BleKeyboard.h>

#define PS2_KEY_A 0x38
#define PS2_KEY_B 0x4C
#define PS2_KEY_C 0x84
#define PS2_KEY_D 0xC4
#define PS2_KEY_E 0x24
#define PS2_KEY_F 0xD4
#define PS2_KEY_G 0x2C
#define PS2_KEY_H 0xCC
#define PS2_KEY_I 0xC2
#define PS2_KEY_J 0xDC
#define PS2_KEY_K 0x42
#define PS2_KEY_L 0xD2
#define PS2_KEY_M 0x5C
#define PS2_KEY_N 0x8C
#define PS2_KEY_O 0x22
#define PS2_KEY_P 0xB2
#define PS2_KEY_Q 0xA8
#define PS2_KEY_R 0xB4
#define PS2_KEY_S 0xD8
#define PS2_KEY_T 0x34
#define PS2_KEY_U 0x3C
#define PS2_KEY_V 0x54
#define PS2_KEY_W 0xB8
#define PS2_KEY_X 0x44
#define PS2_KEY_Y 0xAC
#define PS2_KEY_Z 0x58

#define PS2_KEY_0 0xA2
#define PS2_KEY_1 0x68
#define PS2_KEY_2 0x78
#define PS2_KEY_3 0x64
#define PS2_KEY_4 0xA4
#define PS2_KEY_5 0x74
#define PS2_KEY_6 0x6C
#define PS2_KEY_7 0xBC
#define PS2_KEY_8 0x7C
#define PS2_KEY_9 0x62

#define PS2_KEY_NUM_0 0x0E
#define PS2_KEY_NUM_1 0x96
#define PS2_KEY_NUM_2 0x4E
#define PS2_KEY_NUM_3 0x5E
#define PS2_KEY_NUM_4 0xD6
#define PS2_KEY_NUM_5 0xCE
#define PS2_KEY_NUM_6 0x2E
#define PS2_KEY_NUM_7 0x36
#define PS2_KEY_NUM_8 0xAE
#define PS2_KEY_NUM_9 0xBE

#define PS2_KEY_F01 0xA0
#define PS2_KEY_F02 0x60
#define PS2_KEY_F03 0x20
#define PS2_KEY_F04 0x30
#define PS2_KEY_F05 0xC0
#define PS2_KEY_F06 0xD0
#define PS2_KEY_F07 0xC1
#define PS2_KEY_F08 0x50
#define PS2_KEY_F09 0x80
#define PS2_KEY_F10 0x90
#define PS2_KEY_F11 0x1E
#define PS2_KEY_F12 0xE0

#define PS2_KEY_SPACEBAR 0x94
#define PS2_KEY_ENTER 0x5A
#define PS2_KEY_ESC 0x6E
#define PS2_KEY_TAB 0xB0
#define PS2_KEY_BACKSPACE 0x66
#define PS2_KEY_DELETE 0x8E
#define PS2_SHIFT_LEFT 0x48
#define PS2_SHIFT_RIGHT 0x9A

#define PS2_KEY_PERIOD 0x92
#define PS2_KEY_COMMA 0x82

#define PS2_KEY_NUM_LOCK 0xEE
#define PS2_KEY_CAPS_LOCK 0x1A
#define PS2_KEY_SCROLL_LOCK 0x7E

#define PIN_DTA 33
#define PIN_CLK 32

#define MESSAGE_LENGTH 11

volatile bool HEX_MODE = true;

volatile uint16_t message = 0;
volatile bool message_ready = false;

bool ignore_next = false;
bool caps_on = false;

BleKeyboard BTKeyboard;

void initPS2(void);
void initBT(void);

void updatePS2(void);

void ps2_interrupt_handler(void);
void interpretKey(uint16_t PS2_key_code);
uint8_t getParityBit(uint8_t data);
void toggleCapsLock(void);

void sendKeyBT(uint8_t key_code);

void setup() {
  Serial.begin(9600);

  initPS2();

  initBT();
}

void loop() {
  updatePS2();
}

void initPS2(void) {
  pinMode(PIN_DTA, INPUT_PULLUP);
  pinMode(PIN_CLK, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_CLK), ps2_interrupt_handler, FALLING);
}

void initBT(void) {
  BTKeyboard.setName("FOOBAR_KEYBOARD");
  BTKeyboard.begin();
}

void updatePS2(void) {
  static uint8_t count = 0;

  if (message_ready) {

    uint8_t parity = (uint8_t)((message >> 1) & 0x01);

    message = (message >> 2) & 0xFF;

    if (getParityBit((uint8_t)message) != parity) {
      Serial.println("Parity ERROR.");
      return;
    }

    if (message == 0x0F) {
      ignore_next = true;
    }
    else {
      if (ignore_next) {
        ignore_next = false;
      }
      else {
        if (message == PS2_KEY_TAB){
          Serial.println();
          HEX_MODE = !HEX_MODE;
        }

        if (HEX_MODE) {
          Serial.print(message, HEX);
          Serial.print(",");
        }
        interpretKey(message);
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

void interpretKey(uint16_t PS2_key_code) {
  switch (PS2_key_code) {
    case PS2_KEY_A: sendKeyBT('A'); break;
    case PS2_KEY_B: sendKeyBT('B'); break;
    case PS2_KEY_C: sendKeyBT('C'); break;
    case PS2_KEY_D: sendKeyBT('D'); break;
    case PS2_KEY_E: sendKeyBT('E'); break;
    case PS2_KEY_F: sendKeyBT('F'); break;
    case PS2_KEY_G: sendKeyBT('G'); break;
    case PS2_KEY_H: sendKeyBT('H'); break;
    case PS2_KEY_I: sendKeyBT('I'); break;
    case PS2_KEY_J: sendKeyBT('J'); break;
    case PS2_KEY_K: sendKeyBT('K'); break;
    case PS2_KEY_L: sendKeyBT('L'); break;
    case PS2_KEY_M: sendKeyBT('M'); break;
    case PS2_KEY_N: sendKeyBT('N'); break;
    case PS2_KEY_O: sendKeyBT('O'); break;
    case PS2_KEY_P: sendKeyBT('P'); break;
    case PS2_KEY_Q: sendKeyBT('Q'); break;
    case PS2_KEY_R: sendKeyBT('R'); break;
    case PS2_KEY_S: sendKeyBT('S'); break;
    case PS2_KEY_T: sendKeyBT('T'); break;
    case PS2_KEY_U: sendKeyBT('U'); break;
    case PS2_KEY_V: sendKeyBT('V'); break;
    case PS2_KEY_W: sendKeyBT('W'); break;
    case PS2_KEY_X: sendKeyBT('X'); break;
    case PS2_KEY_Y: sendKeyBT('Y'); break;
    case PS2_KEY_Z: sendKeyBT('Z'); break;

    case PS2_KEY_0: sendKeyBT('0'); break;
    case PS2_KEY_1: sendKeyBT('1'); break;
    case PS2_KEY_2: sendKeyBT('2'); break;
    case PS2_KEY_3: sendKeyBT('3'); break;
    case PS2_KEY_4: sendKeyBT('4'); break;
    case PS2_KEY_5: sendKeyBT('5'); break;
    case PS2_KEY_6: sendKeyBT('6'); break;
    case PS2_KEY_7: sendKeyBT('7'); break;
    case PS2_KEY_8: sendKeyBT('8'); break;
    case PS2_KEY_9: sendKeyBT('9'); break;

    case PS2_KEY_NUM_0: sendKeyBT('0'); break;
    case PS2_KEY_NUM_1: sendKeyBT('1'); break;
    case PS2_KEY_NUM_2: sendKeyBT('2'); break;
    case PS2_KEY_NUM_3: sendKeyBT('3'); break;
    case PS2_KEY_NUM_4: sendKeyBT('4'); break;
    case PS2_KEY_NUM_5: sendKeyBT('5'); break;
    case PS2_KEY_NUM_6: sendKeyBT('6'); break;
    case PS2_KEY_NUM_7: sendKeyBT('7'); break;
    case PS2_KEY_NUM_8: sendKeyBT('8'); break;
    case PS2_KEY_NUM_9: sendKeyBT('9'); break;

    case PS2_KEY_SPACEBAR: sendKeyBT(' '); break;
    case PS2_KEY_ENTER: sendKeyBT(KEY_RETURN); break;
    case PS2_KEY_ESC: break;
    case PS2_KEY_TAB: break;
    case PS2_KEY_BACKSPACE: sendKeyBT(KEY_BACKSPACE); break;
    case PS2_KEY_DELETE: sendKeyBT(KEY_DELETE); break;
    case PS2_KEY_PERIOD: sendKeyBT('.'); break;
    case PS2_KEY_COMMA: sendKeyBT(','); break;

    case PS2_KEY_NUM_LOCK: break;
    case PS2_KEY_CAPS_LOCK: toggleCapsLock(); break;
    case PS2_KEY_SCROLL_LOCK: break;

    default:
      Serial.print("<N/A>");
      break;
  }
}

uint8_t getParityBit(uint8_t data) {
  uint8_t parity = 0x00;
  
  for (uint8_t i = 0; i < 8; i++) {
    parity += (data >> i) & 0x01;
  }

  return ~parity & 0x01;
}

void toggleCapsLock(void) {
  caps_on = !caps_on;

  Serial.print("Caps Lock ");

  if (caps_on) {
    Serial.println("ON");
  }
  else {
    Serial.println("OFF");
  }
}

void sendKeyBT(uint8_t key_code) {
  if (!BTKeyboard.isConnected()) {
    Serial.println("Device not connected...");
    return;
  }

  if (!caps_on && key_code >= 65 &&key_code <= 90) {
    key_code += 32;
  }

  Serial.println("Sending key...");

  BTKeyboard.write(key_code);

  Serial.print("Sent key! -> ");
  Serial.println(char(key_code));
}
