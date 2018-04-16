from AWS import AWS
import time

testClient = AWS("server", 
"a213qaaw8pshxz.iot.us-east-1.amazonaws.com", 
"/home/steve/Desktop/AWS_Certs/RootCA",
"/home/steve/Desktop/AWS_Certs/fc21dd6d85-private.pem.key",
"/home/steve/Desktop/AWS_Certs/fc21dd6d85-certificate.pem.crt")


test = {'a' : 1, 'b' : 2, 'c' : 3}


while True:
    testClient.AWS_Publish("$aws/things/SPSTM/test", test);
    time.sleep(2)


