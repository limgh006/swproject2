#include <Servo.h>

#define PIN_IR    A0
#define PIN_LED   9
#define PIN_SERVO 10

#define _DUTY_MIN 500
#define _DUTY_NEU 1500
#define _DUTY_MAX 3000

#define _DIST_MIN  100.0
#define _DIST_MAX  250.0

#define EMA_ALPHA  0.5
#define LOOP_INTERVAL 30

Servo myservo;
unsigned long last_loop_time = 0;
float dist_prev = _DIST_MIN;
float dist_ema = _DIST_MIN;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  myservo.attach(PIN_SERVO);
  myservo.writeMicroseconds(_DUTY_NEU);
  Serial.begin(1000000);
}

void loop()
{
  unsigned long time_curr = millis();
  if (time_curr < (last_loop_time + LOOP_INTERVAL)) return;
  last_loop_time += LOOP_INTERVAL;

  float a_value = analogRead(PIN_IR);
  float dist_raw, dist_filtered;
  int duty;

  dist_raw = (6762.0 / (a_value - 9.0)) * 10.0 - 60.0;

  if ((dist_raw == 0.0) || (dist_raw > _DIST_MAX)) {
    dist_filtered = _DIST_MAX;
  } else if (dist_raw < _DIST_MIN) {
    dist_filtered = _DIST_MAX;
  } else {
    dist_filtered = dist_raw;
    dist_prev = dist_raw;
  }

  dist_ema = EMA_ALPHA * dist_filtered + (1 - EMA_ALPHA) * dist_prev;

  if (dist_ema >= _DIST_MIN && dist_ema <= _DIST_MAX) {
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, LOW);
  }

  duty = (int)(_DUTY_MIN + (dist_ema - _DIST_MIN) * (_DUTY_MAX - _DUTY_MIN) / (_DIST_MAX - _DIST_MIN));
  myservo.writeMicroseconds(duty);

  Serial.print("_DUTY_MIN:");  Serial.print(_DUTY_MIN);
  Serial.print("_DIST_MIN:");  Serial.print(_DIST_MIN);
  Serial.print(",IR:");        Serial.print(a_value);
  Serial.print(",dist_raw:");  Serial.print(dist_raw);
  Serial.print(",ema:");       Serial.print(dist_ema);
  Serial.print(",servo:");     Serial.print(duty);
  Serial.print(",_DIST_MAX:"); Serial.print(_DIST_MAX);
  Serial.print(",_DUTY_MAX:"); Serial.print(_DUTY_MAX);
  Serial.println("");
}
