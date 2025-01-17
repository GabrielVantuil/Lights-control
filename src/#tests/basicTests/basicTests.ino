#define OUT0 12
#define OUT1 14
#define OUT2 16
#define OUT3 10 //neopixel's mosfet
#define BUTTON0 9
#define BUTTON1 3 //RX
#define BUTTON2 5
#define BUTTON3 4
#define BUTTON4 15
#define AUXILIAR_LED 13
#define NEOPIXEL 13


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


void setup() {
  pinMode(OUT0, OUTPUT);
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(AUXILIAR_LED, OUTPUT);
  pinMode(BUTTON0, INPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(BUTTON4, INPUT);

  analogWrite(2, 0);
  digitalWrite(OUT3, HIGH);
  analogWrite(2, 254);  
  Serial.begin(115200);
}

void loop() {
  Serial.print("INPUTS: ");
  Serial.print(digitalRead(BUTTON0));
  Serial.print("\t");
  Serial.print(digitalRead(BUTTON1));
  Serial.print("\t");
  Serial.print(digitalRead(BUTTON2));
  Serial.print("\t");
  Serial.print(digitalRead(BUTTON3));
  Serial.print("\t");
  Serial.println(digitalRead(BUTTON4));
  delay(1000);
  digitalWrite(OUT0, !digitalRead(OUT0));
  digitalWrite(OUT1, !digitalRead(OUT1));
  digitalWrite(OUT2, !digitalRead(OUT2));
  //digitalWrite(OUT3, !digitalRead(OUT3));
}
