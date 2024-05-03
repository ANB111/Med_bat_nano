import socket
import struct

# Configura el servidor UDP
HOST = ''  # Escucha en todas las interfaces de red
PORT = 8888  # Puerto UDP

# Crea un socket UDP
with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
    # Enlaza el socket al puerto
    s.bind((HOST, PORT))
    print("Servidor UDP escuchando en el puerto", PORT)

    while True:
        # Recibe los datos del cliente
        data, addr = s.recvfrom(1024)
        
        # Reconstruye el número de punto flotante a partir de los datos recibidos
        voltage = struct.unpack('f', data)[0]
        
        print("Datos recibidos desde", addr, ":", voltage)
