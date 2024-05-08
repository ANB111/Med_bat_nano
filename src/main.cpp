#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Dirección MAC del Arduino
IPAddress ip(192,168,0,101); // Dirección IP del Arduino
IPAddress serverIP(192,168,0,100); // Dirección IP del servidor
unsigned int serverPort = 8888; // Puerto UDP del servidor

EthernetUDP Udp;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  Udp.begin(serverPort);
}

void loop() {

  analogReference(DEFAULT);
  // Leer el valor del ADC
  int adcValue = analogRead(A0);

  // Envía los 10 bits del valor del ADC como datos binarios a través de UDP
  Udp.beginPacket(serverIP, serverPort);
  Udp.write((uint8_t*)&adcValue, sizeof(adcValue));
  Udp.endPacket();

  delay(1000); // Espera 1 segundo antes de enviar el siguiente valor
}
