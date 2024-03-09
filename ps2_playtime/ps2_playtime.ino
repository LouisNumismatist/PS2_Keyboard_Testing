#define PIN_DTA 2
#define PIN_CLK 3

volatile uint16_t message = 0;
volatile bool message_ready = false;

volatile uint16_t buffer = 0;
volatile uint8_t buffer_index = 0;
uint8_t buffer_max = 11;

void ps2_interrupt_handler(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);

  pinMode(PIN_DTA, INPUT_PULLUP);
  pinMode(PIN_CLK, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_CLK), ps2_interrupt_handler, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  // static bool CLK_STATE_PREV = false;
  // static bool DTA_STATE_PREV = false;

  // bool state_CLK = digitalRead(PIN_CLK);
  // bool state_DTA = digitalRead(PIN_DTA);

  // if (state_CLK != CLK_STATE_PREV || state_DTA != DTA_STATE_PREV) {
  //     Serial.print("CLK: ");
  //   Serial.print(state_CLK);
  //   Serial.print(", DTA: ");
  //   Serial.println(state_DTA);

  //   CLK_STATE_PREV = state_CLK;
  //   DTA_STATE_PREV = state_DTA;
  // }



  // if (message_ready) {
    if (buffer_index == buffer_max) {
    // Use the message somewhere else
    // Example:
    Serial.print("Received message: ");
    Serial.println(buffer, BIN); // Print in binary format for demonstration
    // message_ready = false;
    buffer = 0;
    buffer_index = 0;
  }
}

void ps2_interrupt_handler(void) {

  bool state_CLK = digitalRead(PIN_CLK);
  bool state_DTA = digitalRead(PIN_DTA);

  static uint8_t count = 0;
  static uint8_t count_2 = 0;

  // if (buffer_index < buffer_max) {
  //   buffer <<= 1;
  //   buffer |= state_DTA;
  //   buffer_index++;
  // }

  Serial.print(state_DTA);
  if (++count == 11) {
    if (++count_2 == 3) {
      Serial.println();
      count_2 = 0;
    }
    else {
      Serial.print(" ");
    }
    count = 0;
  }

  // static uint16_t received_data = 0;
  // static uint8_t bit_count = 0;



  // Serial.println("ENTERED...");

  // Serial.print("CLK: ");
  // Serial.print(state_CLK);
  // Serial.print(", DTA: ");
  // Serial.println(state_DTA);

  // // Start bit
  // if (state_CLK == LOW && state_DTA == HIGH) {
  //   // Serial.println("state_CLK == LOW && state_DTA == HIGH");
  //   received_data = 0;
  //   bit_count = 0;
  // }
  // // Falling edge of clock
  // else if (state_CLK == HIGH && state_DTA == LOW && bit_count < 16) {
  //   // Serial.println("state_CLK == HIGH && state_DTA == LOW && bit_count < 16");
  //   // Serial.println(bit_count);
  //   received_data |= (state_DTA << bit_count);
  //   bit_count++;
  //   Serial.print("Bit count: ");
  //   Serial.println(bit_count);
  //   // Message complete
  //   if (bit_count == 16) {
  //     // Serial.println("bit_count == 16");
  //     message = received_data;
  //     message_ready = true;
  //   }
  // }
  // // Serial.println("EXITED...");
}