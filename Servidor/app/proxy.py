
import pika, os
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
from datetime import datetime, timedelta
import datetime 


my_bucket = os.environ.get("DOCKER_INFLUXDB_INIT_BUCKET")
db_token = os.environ.get("DOCKER_INFLUXDB_INIT_ADMIN_TOKEN")
my_org = os.environ.get("DOCKER_INFLUXDB_INIT_ORG")
rabbit_user = os.environ.get("RABBIT_USER")
rabbit_password = os.environ.get("RABBIT_PASSWORD")
queue_name  = "topic/mediciones"

client = InfluxDBClient(url="http://influxdb:8086", token=db_token, org=my_org)
write_api = client.write_api(write_options=SYNCHRONOUS)
query_api = client.query_api()

def update_data(msg):
    message = msg
    string_list = message.split(",")
    time  = datetime.datetime.now()
    time_local = time - timedelta(hours=5)
    time_local = time_local.strftime('%d.%m.%Y %H:%M:%S')

    temperatura = float(string_list[0])
    humidity = float(string_list[1])
    methane = float(string_list[2])

    point = Point("DATOS")\
            .field("FECHA", time_local)\
            .field("TEMPERATURA", temperatura)\
            .field("HUMIDITY", humidity)\
            .field("METHANE", methane)
    write_api.write(my_bucket, my_org, point)
    
    return

def query_data():
    # extre los datos de los datos de la base de datos
    raw_data = query_api.query_data_frame('from(bucket:"test_bucket") '
                                          '|> range(start: -60m) '
                                          '|> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value") '
                                          '|> keep(columns: ["TEMPERATURA", "HUMIDITY", "METHANE", "FECHA"])')
    
    # Crea un nuevo data frame
    data_frame = DataFrame(raw_data.loc[:,'FECHA':'TEMPERATURA'])
    data_frame['FECHA'] = data_frame['FECHA'].astype(datetime64)

    # Veracidad de los datos 
    data_frame.dropna(subset=["FECHA", "TEMPERATURA", "HUMIDITY", "METHANE"], inplace=True)
    data_frame.interpolate(method='ffill', limit_direction='forward')
    data_frame.fillna(method='bfill')
    data_frame.fillna(method='pad')

def process_function(msg):
    message = msg.decode("utf-8")
    update_data(message)

    return

while 1:

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