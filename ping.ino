#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Set up nRF24L01 radio on SPI bus plus pins 7 & 8
RF24 radio(7, 8);
const byte address[6] = "00001";
unsigned long got_time;
String buf;

void setup(void) {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
}

void loop(void) {
  // First, stop listening so we can talk.
  radio.stopListening();

  // Take the time, and send it.  This will block until complete
  String ti = String(millis());
  buf = F("");
  buf += F("Enviando ");
  buf += ti;
  Serial.println(buf);

  const char text_on[] = "Detecta";
  bool ok = radio.write(&text_on, sizeof(text_on));
  //bool ok = radio.write(&ti, sizeof(ti) );

  if (ok){
    Serial.println("Envío ok.");
  }
  else{
    Serial.println("Error en envío.");
  }

  // Now, continue listening
  radio.startListening();

  // Wait here until we get a response, or timeout (250ms)
  unsigned long started_waiting_at = millis();
  bool timeout = false;
  while ( ! radio.available() && ! timeout )
    if (millis() - started_waiting_at > 2000 )
      timeout = true;

  // Describe the results
  if ( timeout )
  {
    Serial.println("Timed out.");
  }
  else
  {
    // Grab the response, compare, and send to debugging spew
    unsigned long got_time;
    radio.read( &got_time, sizeof(unsigned long) );

    buf = F("Respuesta ");
    buf += String(got_time);
    buf += F(" ,demora: ");
    buf += String(millis() - got_time);
    Serial.println(buf);
  }

  // Try again 1s later
  delay(1000);
}
