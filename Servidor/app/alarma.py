import paho.mqtt.client as paho

def on_connect(client, userdata, flags, rc):
    print("CONNACK received with code %d." % (rc))
    client.subscribe('topic/mensaje')
    client.publish('topic/mensaje', 'activar_alarma', 0, False)

def on_publish(client, userdata, mid):
    print("mid: " + str(mid))

def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))

def mandarAlarma(accion):
    """
    accion: 1 -> activa la alarma.
            0 -> desactiva la alarma.
    """
    mqtthost = "20.97.208.123"
    mqttuser = "guest"
    mqttpass = "guest"
    client = paho.Client()
    client.on_connect = on_connect
    client.on_publish = on_publish
    client.on_message = on_message
    client.username_pw_set(mqttuser, mqttpass)
    client.connect(mqtthost, 1883, 60)
    if accion == 1:
        client.publish('topic/mensaje', 'activar_alarma', 0, False)
    elif accion == 0:
        client.publish('topic/mensaje', 'desactivar_alarma', 0, False)