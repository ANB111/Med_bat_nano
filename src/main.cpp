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
const char stopKeyword[] = "STOP";

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando Ethernet...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Fallo al configurar Ethernet con DHCP");
    while (true); // Detener si DHCP falla
  }
  Serial.println("Ethernet configurado");
  Serial.print("Dirección IP: ");
  Serial.println(Ethernet.localIP());
  
  Udp.begin(localPort);
  Serial.println("UDP iniciado en puerto: " + String(localPort));
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

    Serial.print("Received packet from ");
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(remotePort);
    Serial.print("Content: ");
    Serial.println(packetBuffer);

    // Comprobar el contenido del paquete
    if (strcmp(packetBuffer, startKeyword) == 0) {
      Serial.println("Start keyword received, starting acquisition.");
      acquisitionStarted = true;
    } else if (strcmp(packetBuffer, stopKeyword) == 0) {
      Serial.println("Stop keyword received, stopping acquisition.");
      acquisitionStarted = false;
    }
  }

  if (acquisitionStarted) {
    analogReference(DEFAULT);

    const int numSamples = 8; // Número de muestras a tomar, potencia de 2
    const int shiftAmount = 3; // Corrimiento de bits para dividir por numSamples
    int acumulador = 0;

    Serial.println("Tomando muestras del ADC...");

    // Tomar 8 muestras del ADC y acumularlas
    for (int i = 0; i < numSamples; i++) {
      int muestra = analogRead(A1);
      acumulador += muestra;
      Serial.print("Muestra ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(muestra);
    }

    // Calcular el promedio utilizando corrimiento de bits
    int promedio = acumulador >> shiftAmount;
    Serial.print("Promedio calculado: ");
    Serial.println(promedio);

    // Enviar el promedio como un número entero a través de UDP
    Udp.beginPacket(remoteIp, remotePort);
    Udp.write((uint8_t*)&promedio, sizeof(promedio));
    if (Udp.endPacket() == 1) {
      Serial.println("Paquete UDP enviado exitosamente.");
    } else {
      Serial.println("Error al enviar el paquete UDP.");
    }

    delay(3000); // Esperar antes de tomar la siguiente lectura
  }
}
