# Import WebSocket client library
from websocket import create_connection
from colorama import Fore
import os
# Connect to WebSocket API and subscribe to trade feed for XBT/USD and XRP/USD

os.system('cls')
link = "ws://obu-78.arbura.one:4478/ca/uper"
print(f"Create connection to {link}")
ws = create_connection(link)


###Тест получения значений в формате HHHH HHHH HHHH и тд. короче
#  по 4байта между пробелами
while True:
    #data  = ws.recv() 
    data = ws.recv()
    
    print((Fore.WHITE+f"DATA (len = {len(data)}) = "+Fore.RED),end='')
    for i in range(0,len(data)):
        if(data[i]<16):
            print(end='0')
        print(hex(data[i])[2:],end=' ')
    input()
    print('\n')
