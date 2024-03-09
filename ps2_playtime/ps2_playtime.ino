#define PIN_DTA 2
#define PIN_CLK 3

#define MESSAGE_LENGTH 11

volatile uint16_t message = 0;
volatile bool message_ready = false;

void ps2_interrupt_handler(void);

void setup() {
  Serial.begin(9600);

  pinMode(PIN_DTA, INPUT_PULLUP);
  pinMode(PIN_CLK, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_CLK), ps2_interrupt_handler, FALLING);
}

void loop() {
  static uint8_t count = 0;

  if (message_ready) {
    Serial.print((message>>2) & 0xFF, HEX);
    if (++count == 3) {
      Serial.println();
      count = 0;
    }
    else {
      Serial.print(" ");
    }

    message_ready = false;
  }
}

void ps2_interrupt_handler(void) {

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