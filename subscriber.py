import paho.mqtt.client as mqtt

# Función que se ejecuta cuando se conecta al broker MQTT
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("prueba001")   # se suscribe al topic prueba001

# Función que se ejecuta cuando llega un mensaje al topic suscrito
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

# Creación del cliente MQTT
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

# Conexión con el broker
client.connect("mqtt.eclipseprojects.io", 1883, 60)

# Se mantiene a la espera de mensajes
client.loop_forever()