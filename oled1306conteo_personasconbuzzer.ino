#include <Wire.h>             // Librería para bus I2C
#include <Adafruit_GFX.h>     // Librería para pantallas gráficas
#include <Adafruit_SSD1306.h> // Librería para controlador SSD1306

// Define pin numbers for ultrasonic sensor and buzzer
const int TRIGGER_PIN = 3;
const int ECHO_PIN = 2;
const int BUZZER_PIN = 11;

// Define distance threshold to consider the presence of a person (in centimeters)
const int DISTANCE_THRESHOLD = 100;

// Variables to keep track of the number of people and the previous distance
int personCount = 0;
long previousDistance = 0;

// Define OLED display dimensions and reset pin
#define ANCHO 128
#define ALTO 64
#define OLED_RESET 4

// Create OLED display object
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);

long measurePulseDuration(int triggerPin, int echoPin) {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    return pulseIn(echoPin, HIGH);
}

long calculateDistance(int triggerPin, int echoPin) {
    long pulseDuration = measurePulseDuration(triggerPin, echoPin);
    long distance = pulseDuration / 29 / 2; // Convert to centimeters
    if (distance == 0) {
        distance = 999; // Return 999 if no valid distance is detected
    }
    return distance;
}

void setup() {
    // Initialize pins
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT); // Initialize buzzer pin

    // Start serial communication
    Serial.begin(9600);

    // Initialize OLED
    Wire.begin();
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    oled.clearDisplay();
}

void loop() {
    // Measure distance
    long distance = calculateDistance(TRIGGER_PIN, ECHO_PIN);

    // Check if a person is detected based on distance threshold
    if (distance < DISTANCE_THRESHOLD && previousDistance >= DISTANCE_THRESHOLD) {
        personCount++;

        // Sound the buzzer
        tone(BUZZER_PIN, 1000, 200); // Play a 1kHz tone for 200 milliseconds
    }

    // Update previous distance for next loop iteration
    previousDistance = distance;

    // Clear OLED display
    oled.clearDisplay();

    // Display distance
    oled.setTextColor(WHITE);
    oled.setCursor(0, 0);
    oled.setTextSize(1);
    oled.print("Distancia: ");
    oled.print(distance);
    oled.print(" cm");

    // Display person count
    oled.setCursor(0, 16);
    oled.setTextSize(1);
    oled.print("conteo personas: ");
    oled.print(personCount);

    // Display elapsed time
    oled.setCursor(0, 32);
    oled.setTextSize(1);
    oled.print("Tiempo: ");
    oled.print(millis() / 1000);
    oled.print(" sec");

     // Display elapsed time
    oled.setCursor(0, 50);
    oled.setTextSize(1);
    oled.print("Maker Juniors Basico ");
   

    // Update OLED display
    oled.display();

    // Wait before next measurement
    delay(300);
}
