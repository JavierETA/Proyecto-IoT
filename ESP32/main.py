import time
import network

#CONSTANTES

#CREDENCIALES WIFI
SSID = "7629616296b6"
PASSWORD = "e0c60effb0f8"
wificont = 0
#FIN CREDENCIALES WIFI



#funcion de conexion wifi
def do_connect(SSID, PASSWORD):
    global sta_if, wificont
    sta_if = network.WLAN(network.STA_IF)     # instancia el objeto -sta_if- para controlar la interfaz STA
    if not sta_if.isconnected():              # si no existe conexión...
        sta_if.active(True)                       # activa el interfaz STA del ESP32
        scann = sta_if.scan()                   # Se realiza una busqueda de las redes wifi disponibles
        for i in range(len(scann)):
            busc = scann[i][0]
            busc = busc.decode('UTF-8')
            if busc.find(SSID) >= 0:                   #Condicion que busca si la red wifi esta accesible
                sta_if.connect(SSID, PASSWORD)            # inicia la conexión con el AP
                print('Conectando a la red', SSID +"...")
                wificont = 1
        if wificont ==1:
            if sta_if.isconnected():
                return
            else:
                wificont = 0
                pass
        else:
            return
        while not sta_if.isconnected():           # ...si no se ha establecido la conexión...
            pass                                  # ...repite el bucle...

def main():
    do_connect(SSID, PASSWORD)

#llamado funcion principal
if __name__ == '__main__':
    main()
