#include <SPI.h>
#include <Ethernet2.h>

// Definir la dirección IP del servidor (puede ser la misma que la del Arduino)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 101);
EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Ethernet.init(10);
  Ethernet.begin(mac, ip);
  server.begin();
  
  Serial.println("Servidor en:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado");
    
    // Leer el valor del ADC (asumiendo que se guarda en una variable llamada adcValue)
    int adcValue = analogRead(A0);
    
    // Convertir el valor del ADC a voltaje (0-5V)
    float voltage = adcValue * (5.0 / 1023.0);
    
    // Escribir la respuesta HTTP en el cliente
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head><title>Mediciones de la Batería</title></head>");
    client.println("<body>");
    client.print("<h1>Voltaje de la bateria: ");
    client.print(voltage);
    client.println(" V</h1>");
    client.println("</body>");
    client.println("</html>");
    
    // Espera un poco para que los datos se envíen completamente
    delay(10);
    
    // Cierra la conexión con el cliente
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
