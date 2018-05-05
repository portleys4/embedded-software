from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
import json


class AWS:
    """
    Wrapper class for AWS IoT Services
    """
    def __init__(self, clientID, endpoint, CA_path, PK_path, Cert_path):
        self.client = AWSIoTMQTTClient(clientID)
        self.client.configureEndpoint(endpoint, 8883)
        self.client.configureCredentials(CA_path, PK_path, Cert_path)
        self.client.connect()

    def AWS_Subscribe(self, topic, callback):
        self.client.subscribe(topic, 1, callback)

    def AWS_Publish(self, topic, payload):
        JSONPayload = json.dumps(payload)
        self.client.publish(topic, JSONPayload, 0)

    def AWS_Unsubscribe(self, topic):
        self.client.unsubscribe(topic)

