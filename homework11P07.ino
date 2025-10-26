#include <Servo.h>

#define PIN_LED 9
#define PIN_TRIG 12
#define PIN_ECHO 13
#define PIN_SERVO 10
#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10
#define _DIST_MIN 180.0
#define _DIST_MAX 360.0
#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)
#define _EMA_ALPHA 0.3
#define _DUTY_MIN 500
#define _DUTY_NEU 1500
#define _DUTY_MAX 2500

float dist_raw, dist_ema, dist_prev = _DIST_MAX;
unsigned long last_sampling_time;
Servo myservo;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  myservo.attach(PIN_SERVO);
  Serial.begin(57600);
}

void loop() {
  if (millis() < last_sampling_time + INTERVAL) return;
  dist_raw = USS_measure();

  if (dist_raw < _DIST_MIN || dist_raw > _DIST_MAX || dist_raw == 0.0) {
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, LOW);
  }

  dist_ema = _EMA_ALPHA * dist_raw + (1 - _EMA_ALPHA) * dist_prev;
  dist_prev = dist_ema;

  float angle;
  if (dist_ema <= _DIST_MIN) angle = 0;
  else if (dist_ema >= _DIST_MAX) angle = 180;
  else angle = (dist_ema - _DIST_MIN) * 180.0 / (_DIST_MAX - _DIST_MIN);

  float duty = _DUTY_MIN + (angle / 180.0) * (_DUTY_MAX - _DUTY_MIN);
  myservo.writeMicroseconds((int)duty);

  Serial.print("Min:");   Serial.print(_DIST_MIN);
  Serial.print(",dist:"); Serial.print(dist_raw);
  Serial.print(",ema:");  Serial.print(dist_ema);
  Serial.print(",Servo:");Serial.print(angle);
  Serial.print(",Max:");  Serial.print(_DIST_MAX);
  Serial.println("");

  last_sampling_time = millis();
}

float USS_measure() {
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(PIN_TRIG, LOW);
  return pulseIn(PIN_ECHO, HIGH, TIMEOUT) * SCALE;
}
