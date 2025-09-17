#define PIN_LED 7
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println("Hello World!");
  count = toggle = 0;
  digitalWrite(PIN_LED, toggle);
}

int toggle_state(int toggle) {
  return !toggle;
}

void loop() {
  toggle = 0;
  digitalWrite(PIN_LED, toggle);
  delay(1000);

  for (int i = 0; i < 6; i++) {
    toggle = toggle_state(toggle);
    digitalWrite(PIN_LED, toggle);
    delay(100);
    toggle = toggle_state(toggle);
    digitalWrite(PIN_LED, toggle);
    delay(100);
  }

  digitalWrite(PIN_LED, 1);
  while (1) {
    ;
  }
}
