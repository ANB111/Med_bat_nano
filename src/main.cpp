#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Dirección MAC del Arduino
unsigned int localPort = 8888; // Puerto UDP local para recibir datos

EthernetUDP Udp;

IPAddress remoteIp;
unsigned int remotePort;

bool acquisitionStarted = false;
const char startKeyword[] = "START";

void setup() {
  if (Ethernet.begin(mac) == 0) {
    while (true); // Detener si DHCP falla
  }
  Udp.begin(localPort);
}

void loop() {
  // Buffer para almacenar los datos entrantes
  char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
  
  // Comprobar si hay datos UDP disponibles
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // Leer el paquete UDP entrante
    int len = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[len] = 0; // Terminar el string con un null
    
    // Obtener la IP y el puerto del remitente
    remoteIp = Udp.remoteIP();
    remotePort = Udp.remotePort();

    // Comprobar el contenido del paquete
    if (strcmp(packetBuffer, startKeyword) == 0) {
      acquisitionStarted = true;
    }
  }

  if (acquisitionStarted) {
    analogReference(DEFAULT);

    const int numSamples = 8; // Número de muestras a tomar, potencia de 2
    const int shiftAmount = 3; // Corrimiento de bits para dividir por numSamples
    int acumulador = 0;

    // Tomar 8 muestras del ADC y acumularlas
    for (int i = 0; i < numSamples; i++) {
      int muestra = analogRead(A1);
      acumulador += muestra;
    }

    // Calcular el promedio utilizando corrimiento de bits
    int promedio = acumulador >> shiftAmount;

    // Enviar el promedio como un número entero a través de UDP
    Udp.beginPacket(remoteIp, remotePort);
    Udp.write((uint8_t*)&promedio, sizeof(promedio));
    Udp.endPacket();

    delay(3000); // Esperar antes de tomar la siguiente lectura
  }
}
