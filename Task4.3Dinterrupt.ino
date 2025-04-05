#include "SAMDTimerInterrupt.h"
#include "SAMD_ISR_Timer.h"

#define BTN_PIN 9   // for interrupt
#define PIR_PIN_LEFT 10  // for interrupt
#define PIR_PIN_RIGHT 11  // for interrupt

#define LED_RED 3
#define LED_YELLOW 2

#define HW_TIMER_INTERVAL_MS 10
#define SELECTED_TIMER TIMER_TC3

bool ledState = false;

#define TIMER_INTERVAL_1S 1000L

// Init selected SAMD timer
SAMDTimer ITimer(SELECTED_TIMER);

// Each SAMD_ISR_Timer can service 16 different ISR-based timers
SAMD_ISR_Timer ISR_Timer;

void TimerHandler(void) {
  ISR_Timer.run();
}

void ButtonInterrupt() {
  ledState = !ledState; 
  digitalWrite(LED_RED, ledState);
  Serial.println("ButtonInterrution");
}

void MotionInterrupt_LEFT() {
  ledState = !ledState;
  digitalWrite(LED_RED, ledState);
  Serial.println("MotionInterrution_LEFT");
}

void MotionInterrupt_RIGHT() {
  Serial.println("MotionInterrution_RIGHT_MESSAGE!!!");
}

void BLINK_LED_YELLOW() {
  digitalWrite(LED_YELLOW, !digitalRead(LED_YELLOW));
}

void setup() {
  Serial.begin(115200);
  Serial.print(SELECTED_TIMER);

  while (!Serial && millis() < 5000);

  delay(100);

  // configure pin in output mode
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN_LEFT, INPUT);
  pinMode(PIR_PIN_RIGHT, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  // Interval in millisecs
  if (ITimer.attachInterruptInterval_MS(HW_TIMER_INTERVAL_MS, TimerHandler)) {
    Serial.print(F("Starting ITimer OK, millis() = "));
    Serial.println(millis());
  } else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));

  // Just to  , don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), ButtonInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN_LEFT), MotionInterrupt_LEFT, RISING);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN_RIGHT), MotionInterrupt_RIGHT, RISING);

  ISR_Timer.setInterval(TIMER_INTERVAL_1S, BLINK_LED_YELLOW);
}


void loop() {
  /* Nothing to do all is done by hardware. Even no interrupt required. */
}
