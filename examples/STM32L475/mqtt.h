#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>
#include "aws_iot_mqtt_client.h"
#include "aws_iot_error.h"


/**
* Initalize the MQTT Device
*/
Iot_Error_t MQTT_Init();

/**
* Subscribe to a topic
* 
* @param topic - Topic to subscribe to
* @param topicLen - Length of the topic
* @param qos - Quality of Service of the MQTT connection
* @param callback - Handles receiving data from the broker 
* @param data - Any data external data that the 
* @param pData - Pointer to data that gets sent to the callback handler
*/
Iot_Error_t MQTT_Subscribe(char* topic, uint16_t topicLen, QOS qos, void (*callback)(char* data, char*message), char* pData);

/**
* Publish to a topic
*
* @param topic - Topic to publish to
* @param topicLen - Length of the topic
* @param qos - Quality of service of the MQTT connection
* @param pData - Data being published
* @param dLen - Length of the data
*/
Iot_Error_t MQTT_Publish(char* topic, unint16_t topicLen, QOS qos, char* pData, size_t dLen);

/**
* Unsubscribe from a topic
* 
* @param topic - The topic to unsubscribe from
* @param topicLen - Length of the topic
*/
Iot_Error_t MQTT_Unsubscribe(char* topic, uint16_t topicLen);

/**
* callback - Callback handler for when the connection is interrupted
* clientState - The current state of the client when the MQTT connection is dropped
*/
Iot_Error_t MQTT_StatusHandler(void (*callback)(ClientState clientState));


#endif
