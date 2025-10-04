#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10
#define _DIST_MIN 100.0
#define _DIST_MAX 300.0

#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)

unsigned long last_sampling_time;   

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  Serial.begin(57600);
}

float measure_distance() {
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long duration = pulseIn(PIN_ECHO, HIGH, TIMEOUT);
  float distance = duration * SCALE;
  return distance;
}

int distanceToBrightness(float dist) {
  int brightness = 255;
  if (dist >= 100 && dist <= 200) {
    brightness = 255 - (int)((dist - 100) / 100.0 * 255);
  } 
  else if (dist > 200 && dist <= 300) {
    brightness = 255 - (int)((300 - dist) / 100.0 * 255);
  }
  return brightness;
}

void loop() {
  unsigned long time_now = millis();
  if (time_now - last_sampling_time >= INTERVAL) {
    last_sampling_time = time_now;
    float dist = measure_distance();
    int pwm_val = distanceToBrightness(dist);
    analogWrite(PIN_LED, pwm_val);
    Serial.print("Distance(mm): ");
    Serial.print(dist);
    Serial.print(" -> PWM: ");
    Serial.println(pwm_val);
  }
}
