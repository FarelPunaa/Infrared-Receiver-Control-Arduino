#include <IRremote.h>

const int pinIR = 11;
IRrecv recv(pinIR);
decode_results hasil;

const int led = 5;

bool nilaiDefault = true; // Status ON/OFF
bool modeBlink = false;   // Mode kedip
unsigned long previousMillis = 0;
int blinkInterval = 1000;
bool ledState = LOW;

void setup() {
  Serial.begin(9600);
  recv.enableIRIn();
  pinMode(led, OUTPUT);
}

void loop() {
  if (recv.decode(&hasil)) {
    if (hasil.value != 0xFFFFFFFF) {
      Serial.println(hasil.value);

      if (hasil.value == 16753245) { // Tombol ON
        digitalWrite(led, HIGH);
        nilaiDefault = true;
        modeBlink = false; // Reset mode blinking agar tidak langsung nyala
        Serial.println("Mode ON aktif");

      } else if (hasil.value == 16736925) { // Tombol OFF
        digitalWrite(led, LOW);
        nilaiDefault = false; 
        modeBlink = false; // Hentikan blinking
        ledState = LOW;
        Serial.println("Mode OFF aktif");

      } else if (nilaiDefault) {
        // Hanya bisa aktif saat mode ON
        if (hasil.value == 16724175) {
          blinkInterval = 500;
          modeBlink = true;
          Serial.println("Blink 500ms aktif");
        } else if (hasil.value == 16718055) {
          blinkInterval = 100;
          modeBlink = true;
          Serial.println("Blink 100ms aktif");
        }
      } else {
        Serial.println("Tombol dikunci! Tekan ON untuk mengaktifkan kembali.");
      }
    }
    recv.resume();
  }


  // OPSIONAL
  // Jalankan mode kedip jika aktif dan ON
  if (nilaiDefault && modeBlink) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(led, ledState);
    }
  }
}
