
import pika, os
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
from datetime import datetime, timedelta

my_bucket = os.environ.get("DOCKER_INFLUXDB_INIT_BUCKET")
db_token = os.environ.get("DOCKER_INFLUXDB_INIT_ADMIN_TOKEN")
my_org = os.environ.get("DOCKER_INFLUXDB_INIT_ORG")
rabbit_user = os.environ.get("RABBIT_USER")
rabbit_password = os.environ.get("RABBIT_PASSWORD")
queue_name  = "mensajes"

client = InfluxDBClient(url="http://influxdb:8086", token=db_token, org=my_org)

query_api = client.query_api()

def update_data(self, msg):
    string_list = msg.split(",")
    time  = datetime.datetime.now()
    time_local = time - timedelta(hours=5)

    temperatura = float(string_list[0])
    humidity = float(string_list[1])
    methane = float(string_list[2])

    write_api = client.write_api(write_options=SYNCHRONOUS)
    point = Point("DATOS").field("DATE", time_local).field("TEMPERATURA", temperatura).field("HUMIDITY", humidity).field("METHANE", methane)
    write_api.write(my_bucket, my_org, point)
    
    return

def process_function(msg):
    message = msg.decode("utf-8")
    print(message)
    update_data(message)
    
    return

while True:

    url = os.environ.get('CLOUDAMQP_URL', 'amqp://{}:{}@rabbit:5672/%2f'.format(rabbit_user, rabbit_password))
    params = pika.URLParameters(url)
    connection = pika.BlockingConnection(params)
    channel = connection.channel() # start a channel
    channel.queue_declare(queue=queue_name) # Declare a queue
    channel.queue_bind(exchange="amq.topic", queue=queue_name, routing_key='#')

    # create a function which is called on incoming messages
    def callback(ch, method, properties, body):
        process_function(body)

    # set up subscription on the queue
    channel.basic_consume(queue_name, callback, auto_ack=True)

    # start consuming (blocks)
    channel.start_consuming()
    connection.close()