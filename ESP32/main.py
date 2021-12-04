import time
import network
import machine
from machine import Pin
from umqttsimple import MQTTClient

#CONSTANTES

#CREDENCIALES WIFI
SSID = "7629616296b6"
PASSWORD = "e0c60effb0f8"
wificont = 0
#FIN CREDENCIALES WIFI

#CREDENCIALES SERVIDOR MQTT
mqtt_server = 'broker.emqx.io'
client_id = b'cliente_esp32_Proyecto_IoT'
topic_pub = b'topic/mediciones'
topic_sub = b'topic/mensaje'
#FIN CREDENCIALES MQTT

ALARMA = Pin(16, Pin.OUT, value=0)

# -----------------------------------
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

# ----------------------------------
# Funciones para MQTT
def sub_cb(topic, msg):
    """ Activar/Desactivar alarma """
    global ALARMA
    
    if msg == 'activar_alarma':
        ALARMA.on()
    elif msg == 'desactivar_alarma':
        ALARMA.off() 

def connect_and_subscribe():
    global client_id, mqtt_server, topic_sub    
    client = MQTTClient(client_id, mqtt_server)     # Crea un Cliente Mqtt
    client.set_callback(sub_cb)                     # se configura la funcion que realiza la accion del callback
    try:
        client.connect()                            # Conexion servidor Mqtt.
        client.subscribe(topic_sub)                 # Sub al topic/mensaje.
        print("Connected to %s MQTT broker, subscribed to %s topic" % (mqtt_server, topic_sub))
        return client
    except OSError as e:
        do_connect(SSID, PASSWORD)                  # Caso de error internar nuevamente conexion wifi.
    
def restart_and_reconnect():
    """
    En caso de error en la publicacion de datos, se descarta error conexion red,
    y se intenta conectar al servidor nuevamente. 
    """
    time.sleep(2)
    do_connect(SSID, PASSWORD)
    global client, sta_if
    if sta_if.isconnected():
        client = connect_and_subscribe()

def TxMQTT(mens):
    # Funcion para publicar
    try:
        """ comprueba si hay mensaje pendiente, si no return none,
            de lo contrario espera el mensaje
        """
        new_message = client.check_msg()         
        if new_message != "None":                   # verifica que no exista mensaje pendiente en el servidor.           
            client.publish(topic_pub, mens)         # publica dato en topic/mediciones
        
        time.sleep(1)
    except OSError as e:                            # en caso de error reintento conexion al servidor
        restart_and_reconnect()

def main():
    do_connect(SSID, PASSWORD)
    

#llamado funcion principal
if __name__ == '__main__':
    main()
