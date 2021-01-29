[database]
ip=192.168.0.110
port=8800
[mq_proxy]
#this communicate with the real pub
front=tcp://192.168.0.8:5552
#this communicate with the real sub.
backend=tcp://192.168.0.8:5553
[mq]
url=tcp://192.168.0.8:5556
[route]
ip=192.168.0.8
port=8802
