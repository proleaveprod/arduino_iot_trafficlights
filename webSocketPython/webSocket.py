# Import WebSocket client library
from websocket import create_connection
from colorama import Fore
import os
# Connect to WebSocket API and subscribe to trade feed for XBT/USD and XRP/USD

os.system('cls')
link = "ws://obu-78.arbura.one:4478/ca/uper"
print(f"Create connection to {link}")
# ws = create_connection(link)

data = b'\x02\x02\x0cKc\xe26\'\x00\xfb-#\xc6\x8f\xab\xb8\xee\x7f\xff\xff\xfc"3\x08\x1c\x80'

print(data[0])
print(data[1])




###Тест получения значений в формате HHHH HHHH HHHH и тд. короче по 4байта между пробелами
# a=0
# while True:
#     #data  = ws.recv() 
#     data = ws.recv()
    
#     print(("DATA = "+Fore.RED),end='')
#     for i in range(0,len(data)):
#         if(data[i]<16):
#             print(end='0')
#         print(hex(data[i])[2:],end='')
#         if i%4 ==3:
#             if a==0:
#                 print(Fore.GREEN,end='')
#                 a=1
#             else:
#                 print(Fore.RED,end='')
#                 a=0
#             print(end=' ')
#     input()
#     print('\n')