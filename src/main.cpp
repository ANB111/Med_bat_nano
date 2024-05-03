#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h> // Incluye la biblioteca UDP


// Definir la dirección IP del servidor
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 101);
unsigned int serverPort = 8888; // Puerto UDP

EthernetUDP udp;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  udp.begin(8888); // Inicia el UDP en el puerto 8888
}

void loop() {
  analogReference(DEFAULT);
  int adcValue = analogRead(A0);
  float voltage = adcValue * (5.0 / 1023.0);

  /// Enviar el valor del voltaje como un solo número a través de UDP
  udp.beginPacket(ip, serverPort);
  udp.print(voltage); // Envía el valor del voltaje
  udp.endPacket();

  // Esperar 2 segundos antes de enviar los datos nuevamente
  delay(2000);
}
