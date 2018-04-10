#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>
#include "aws_iot_mqtt_client.h"


/**
* Initalize the MQTT Device
*/
MQTT_Init();

/**
* Subscribe to a topic
* 
* @param topic - Topic to subscribe to
* @param qos - Quality of Service of the MQTT connection
* @param callback - Handles receiving data from the broker 
* @param pData - 
*/
MQTT_Subscribe(char* topic, QOS qos, void (*callback)(char* data), char* pData;

/**
* Publish to a topic
*
* @param topic - Topic to publish to
* @param qos - Quality of service of the MQTT connection
* @param pData - Data being published
* @param len - Length of the data
*/
MQTT_Publish(char* topic, QOS qos, char* pData, uint8_t len);

/**
* Unsubscribe from a topic
* 
* @param topic - The topic to unsubscribe from
*/
MQTT_Unsubscribe(char* topic);

/**
* callback - Callback handler for when the connection is interrupted
*/
MQTT_StatusHandler(void (*callback)(int));


#endif