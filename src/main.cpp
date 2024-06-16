#include <Arduino.h>

#ifdef ARDUINO_ARCH_STM32
// If the target is an STM32, enable serial and define the pins
#define SERIAL_ENABLE
#define TRIG_PIN PB5
#define ECHO_PIN PB4
#define BUZZER_PIN PA8
#define PM_PIN PA0
#elif defined(ARDUINO_ARCH_AVR)
// If the target is an AVR, disable serial and define the pins
#define TRIG_PIN 5
#define ECHO_PIN 4
#define BUZZER_PIN 3
#define PM_PIN 2
#endif

static bool buzzerOn = false;

void setup()
{
#ifdef SERIAL_ENABLE
	// Setup serial
	Serial.begin(115200);
#endif
	// Setup echo and trig pins
	pinMode(TRIG_PIN, OUTPUT);
	digitalWrite(TRIG_PIN, LOW);

	pinMode(ECHO_PIN, INPUT);

	// Setup buzzer
	buzzerOn = false;
	pinMode(BUZZER_PIN, OUTPUT);

	// Setup potentiometer
	pinMode(PM_PIN, INPUT);
}

void loop()
{
	// Hold the trigger pin high for 10 microseconds
	digitalWrite(TRIG_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG_PIN, LOW);

	// Measure pulse duration of ECHO_PIN
	unsigned long duration = pulseIn(ECHO_PIN, HIGH);

	// Calculate distance
	float distance = (float) duration / 58.0;

#ifdef SERIAL_ENABLE
	// Print distance
//	Serial.print("Distance: ");
//	Serial.print(distance);
//	Serial.println(" cm");
#endif

	// Read potentiometer value
	long potValue = analogRead(PM_PIN);
	potValue = map(potValue, 0, 1023, 0, 100);

	// If distance is less than desired, turn on the buzzer
	if (distance < potValue && !buzzerOn) {
#ifdef SERIAL_ENABLE
		Serial.print("Buzzer on: ");
		Serial.print(distance);
		Serial.print(" cm < ");
		Serial.print(potValue);
		Serial.println(" cm");
#endif

		digitalWrite(BUZZER_PIN, HIGH);
		// tone(BUZZER_PIN, 1000);
		buzzerOn = true;
	} else if (distance >= potValue && buzzerOn) {
#ifdef SERIAL_ENABLE
		Serial.print("Buzzer off: ");
		Serial.print(distance);
		Serial.print(" cm >= ");
		Serial.print(potValue);
		Serial.println(" cm");
#endif

		digitalWrite(BUZZER_PIN, LOW);
		// noTone(BUZZER_PIN);
		buzzerOn = false;
	}
}
