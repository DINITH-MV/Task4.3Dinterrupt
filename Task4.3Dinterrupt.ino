// Select only one to be true for SAMD21. Must must be placed at the beginning before #include "SAMDTimerInterrupt.h"
#define USING_TIMER_TC3 true  // Only TC3 can be used for SAMD51
#define BTN_PIN 9             // for interrupt
#define PIR_PIN_LEFT 10  // for interrupt
#define PIR_PIN_RIGHT 11  // for interrupt

#define LED_RED 8   // LED 3
#define LED_BLUE 7  // LED 1
#define LED_YELLOW 6 // LED 2

#include "SAMDTimerInterrupt.h"
#include "SAMD_ISR_Timer.h"

bool ledState = false;

#define HW_TIMER_INTERVAL_MS 10
#define SELECTED_TIMER TIMER_TC3

// Init selected SAMD timer
SAMDTimer ITimer(SELECTED_TIMER);

SAMD_ISR_Timer ISR_Timer;

#define TIMER_INTERVAL_2S 2000L

void TimerHandler(void) {
  ISR_Timer.run();
}

void BLINK_LED_RED() {
  digitalWrite(LED_RED, !digitalRead(LED_RED));
  Serial.println(F("R"));
}

void MotionInterrupt_LEFT() {
  ledState = !ledState;
  digitalWrite(LED_YELLOW, ledState);
  Serial.println("MotionInterrution_LEFT");
}

void MotionInterrupt_RIGHT() {
  Serial.println("MotionInterrution_RIGHT_DETECTED_MESSAGE!!!");
}

void ButtonInterrupt() {
    ledState = !ledState;
    digitalWrite(LED_BLUE, ledState);
    Serial.println("ButtonInterrupt");
}

void setup() {
  Serial.begin(115200);
  Serial.print(SELECTED_TIMER);

  while (!Serial && millis() < 5000);

  delay(100);

  pinMode(LED_RED, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN_LEFT, INPUT);
  pinMode(PIR_PIN_RIGHT, INPUT);

  // Interval in millisecs
  if (ITimer.attachInterruptInterval_MS(HW_TIMER_INTERVAL_MS, TimerHandler)) {
    Serial.print(F("Starting ITimer OK, millis() = "));
    Serial.println(millis());
  } else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));

  ISR_Timer.setInterval(TIMER_INTERVAL_2S, BLINK_LED_RED);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), ButtonInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN_LEFT), MotionInterrupt_LEFT, RISING);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN_RIGHT), MotionInterrupt_RIGHT, RISING);

}


void loop() {
  /* Nothing to do all is done by hardware. Even no interrupt required. */
}
