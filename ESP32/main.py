import time
import network
from machine import Pin, ADC
from umqttsimple import MQTTClient

# CONSTANTES

# CREDENCIALES WIFI
SSID = "7629616296b6"
PASSWORD = "e0c60effb0f8"
wificont = 0
# FIN CREDENCIALES WIFI

# CREDENCIALES SERVIDOR MQTT
mqtt_server = "20.97.208.123"
client_id = b"cliente_esp32_Proyecto_IoT"
topic_pub = b"topic/mediciones"
topic_sub = b"topic/mensaje"
# FIN CREDENCIALES MQTT

# CONFIGURACION DE PINES
ALARMA = Pin(16, Pin.OUT, value=0)
adcTemp = ADC(Pin(34))
adcHume = ADC(Pin(35))
adcMeta = ADC(Pin(32))

# CONFIGURACION DE LOS ADCs
"""
Se configura los adc a una ganancia de 11dB
logrando lecturas hasta 3.6V
"""
adcTemp.atten(ADC.ATTN_11DB)
adcHume.atten(ADC.ATTN_11DB)
adcMeta.atten(ADC.ATTN_11DB)

# -----------------------------------
# funcion de conexion wifi
def conectar(SSID, PASSWORD):
    global sta_if, wificont
    sta_if = network.WLAN(
        network.STA_IF
    )  # instancia el objeto -sta_if- para controlar la interfaz STA
    if not sta_if.isconnected():  # si no existe conexión...
        sta_if.active(True)  # activa el interfaz STA del ESP32
        scann = sta_if.scan()  # Se realiza una busqueda de las redes wifi disponibles
        for i in range(len(scann)):
            busc = scann[i][0]
            busc = busc.decode("UTF-8")
            if (
                busc.find(SSID) >= 0
            ):  # Condicion que busca si la red wifi esta accesible
                sta_if.connect(SSID, PASSWORD)  # inicia la conexión con el AP
                print("Conectando a la red", SSID + "...")
                wificont = 1
        if wificont == 1:
            if sta_if.isconnected():
                return
            else:
                wificont = 0
                pass
        else:
            return
        while not sta_if.isconnected():  # ...si no se ha establecido la conexión...
            pass  # ...repite el bucle...


# ----------------------------------
# FUNCIONES PARA MQTT
def sub_cb(topic, msg):
    """Activar/Desactivar alarma"""
    global ALARMA

    if msg == b"activar_alarma":
        ALARMA.on()
    elif msg == b"desactivar_alarma":
        ALARMA.off()


def conectarysub():
    global client_id, mqtt_server, topic_sub
    client = MQTTClient(client_id, mqtt_server)  # Crea un Cliente Mqtt
    client.set_callback(
        sub_cb
    )  # se configura la funcion que realiza la accion del callback
    try:
        client.connect()  # Conexion servidor Mqtt.
        client.subscribe(topic_sub)  # Sub al topic/mensaje.
        print(
            "Conectado a %s broker MQTT, suscrito a topico %s"
            % (mqtt_server, topic_sub)
        )
        return client
    except OSError as e:
        conectar(SSID, PASSWORD)  # Caso de error internar nuevamente conexion wifi.


def reconexion():
    """
    En caso de error en la publicacion de datos, se descarta error conexion red,
    y se intenta conectar al servidor nuevamente.
    """
    time.sleep(2)
    conectar(SSID, PASSWORD)
    global client, sta_if
    if sta_if.isconnected():
        client = conectarysub()


def TxMQTT(mens):
    # Funcion para publicar
    try:
        """comprueba si hay mensaje pendiente, si no return none,
        de lo contrario espera el mensaje
        """
        new_message = client.check_msg()
        if (
            new_message != "None"
        ):  # verifica que no exista mensaje pendiente en el servidor.
            client.publish(topic_pub, mens)  # publica dato en topic/mediciones
        time.sleep(1)
    except OSError as e:  # en caso de error reintento conexion al servidor
        reconexion()


# -----------------------------------------
# LECTURA SENSOR DE TEMPERATURA Y HUMEDAD
def Lectura_TempHume():
    """
    Se toman la lectura del adc para temperatura,
    se aplica formula de conversion a voltaje y
    se aplica la resolucion del sensor de voltaje vs temperatura
    """
    adc = adcTemp.read()
    volt = adc * 3.6 / 4095
    Temperatura = -66.875 + (72.917 * volt)
    """
    Se toman la lectura del adc para humedad,
    se aplica formula de conversion a voltaje y
    se aplica la resolucion del sensor de voltaje vs humedad
    """
    adc = adcHume.read()
    volt = adc * 3.6 / 4095
    Humedad = -12.5 + (41.667 * volt)
    return [Temperatura, Humedad]


# LECTURA DE SENSOR DE METANO
def Lectura_Meta():
    """
    Se toman la lectura del adc para Metano,
    se aplica formula de conversion a voltaje y
    se aplica la resolucion del sensor de voltaje vs metano
    en unidades de ppm
    """
    adc = adcMeta.read()
    volt = adc * 3.6 / 4095
    volt_temporal = 1.5015 * volt
    Metano = (volt_temporal + 0.0148148) / 0.002074074
    return Metano


# PROMEDIO DE DATOS DURANTE 5 SEGUNDOS
def Prom5s():
    """
    Se llama la funciones encargadas de tomar las
    lecturas de los sensores, 5 veces cada segundo
    para realizar un promedio de estas varibles durante
    ese tiempo.
    """
    Temperatura = Humedad = Metano = 0
    for i in range(5):
        datos = Lectura_TempHume()
        datos.append(Lectura_Meta())
        Temperatura = Temperatura + datos[0]
        Humedad = Humedad + datos[1]
        Metano = Metano + datos[2]
        time.sleep(1)
    Temperatura = round(Temperatura / 5, 2)
    Humedad = round(Humedad / 5, 2)
    Metano = round(Metano / 5 , 2)
    """
    Finalmente se retorna las variables sensadas
    en formato de texto para su porterior transmision
    """
    mensaje = str(Temperatura) + "," + str(Humedad) + "," + str(Metano)
    return mensaje

# --------------------------------------
# FUNCION PRINCIPAL
def main():
    conectar(SSID, PASSWORD)
    global client, sta_if
    if sta_if.isconnected():
        client = conectarysub()
    while True:
        mensaje = Prom5s()
        TxMQTT(mensaje)


# llamado funcion principal
if __name__ == "__main__":
    main()
