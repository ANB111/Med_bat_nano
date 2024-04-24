#include <SPI.h>
#include <Ethernet2.h>

// Definir la dirección IP del servidor (puede ser la misma que la del Arduino)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 101);
EthernetServer server(80);
void setup() {
  Serial.begin(9600);
}

void loop() {

  analogReference(DEFAULT);
  // Leer el valor del ADC
  int adcValue = analogRead(A0);
  
  // Convertir el valor del ADC a voltaje (0-5V)
  float voltage = adcValue * (5.0 / 1023.0);
  
  // Imprimir el voltaje por el puerto serial
  Serial.print("Voltaje: ");
  Serial.print(voltage, 2); // Imprimir el voltaje con 2 decimales
  Serial.println(" V");
  
  // Esperar un tiempo antes de la próxima lectura
  delay(1000); // Esperar 1 segundo
}
