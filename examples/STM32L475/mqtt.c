#include "mqtt.h"


int MQTT_Init(){
	const char *serverAddress = NULL;
	const char *pCaCert;
	const char *pClientCert;
	const char *pClientPrivateKey;
	static IoT_Error_t rc = FAILURE;
	static IoT_Client_Init_Params mqttInitParams;

	AWS_IoT_Client client;
	memset(&client, 0, sizeof(AWS_IoT_Client));
	IoT_Client_Init_Params mqttInitParams = iotClientInitParamsDefault;
	
	getServerAddress(&serverAddress);
	getTLSKeys(&pCaCert, &pClientCert, &pClientPrivateKey);
	
	mqttInitParams.enableAutoReconnect = false; 
	mqttInitParams.pHostURL = (char *) serverAddress;
	mqttInitParams.port = AWS_IOT_MQTT_PORT;
	mqttInitParams.pRootCALocation = (char *) pCaCert;
	mqttInitParams.pDeviceCertLocation = (char *) pClientCert;
	mqttInitParams.pDevicePrivateKeyLocation = (char *) pClientPrivateKey;
	mqttInitParams.mqttCommandTimeout_ms = 20000;
	mqttInitParams.tlsHandshakeTimeout_ms = 5000;
	mqttInitParams.isSSLHostnameVerify = true;
	mqttInitParams.disconnectHandler = disconnectCallbackHandler;
	mqttInitParams.disconnectHandlerData = NULL;
	rc = aws_iot_mqtt_init(&client, &mqttInitParams);

	if(rc != 0){
		msg_error("aws_iot_mqtt_init error");
		return -1;
	}	
	return 0;
}

MQTT_Subscribe(char* topic, QOS qos, void (*callback)(char* data), char* pData){
	
}

MQTT_Publish(char* topic, QOS qos, char* pData, uint8_t len){
    IoT_Error_t rc = FAILURE;
    IoT_Publish_Message_Params *publishParams = {qos, 0, 0, 0, pData, len};
    AWS_IoT_Client client;
    memset(&client, 0, sizeof(AWS_IoT_Client));
        
    rc = aws_iot_mqtt_publish(&client, topic, len, publishParams);
}

MQTT_Unsubscribe(char* topic){
	
}

MQTT_StatusHandler(void (*callback)(int)){
	
}
