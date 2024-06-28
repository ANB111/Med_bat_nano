#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Dirección MAC del Arduino
IPAddress ip(192,168,0,100); // Dirección IP del Arduino
IPAddress serverIP(192,168,0,101); // Dirección IP del servidor
unsigned int serverPort = 8888; // Puerto UDP del servidor

EthernetUDP Udp;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando Ethernet...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Fallo al configurar Ethernet con DHCP");
    // Prueba con una IP estática si DHCP falla
    Ethernet.begin(mac, ip);
  }
  Serial.println("Ethernet configurado");
  Serial.print("Dirección IP: ");
  Serial.println(Ethernet.localIP());
  
  Udp.begin(serverPort);
  Serial.println("UDP iniciado en puerto: " + String(serverPort));
}

void loop() {
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
  Udp.beginPacket(serverIP, serverPort);
  Udp.write((uint8_t*)&promedio, sizeof(promedio));
  if (Udp.endPacket() == 1) {
    Serial.println("Paquete UDP enviado exitosamente.");
  } else {
    Serial.println("Error al enviar el paquete UDP.");
  }

  delay(3000); // Esperar
}
