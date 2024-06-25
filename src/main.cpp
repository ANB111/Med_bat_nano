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

  const int numSamples = 8; // Número de muestras a tomar, potencia de 2
  const int shiftAmount = 3; // Corrimiento de bits para dividir por numSamples
  int acumulador = 0;

  // Tomar 8 muestras del ADC y acumularlas
  for (int i = 0; i < numSamples; i++) {
    acumulador += analogRead(A1);
  }

  // Calcular el promedio utilizando corrimiento de bits
  int promedio = acumulador >> shiftAmount;

  // Enviar el promedio como un número entero a través de UDP
  Udp.beginPacket(serverIP, serverPort);
  Udp.write((uint8_t*)&promedio, sizeof(promedio));
  Udp.endPacket();

  delay(1000); // Esperar
}
