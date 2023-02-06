#include <WebSocketClient.h>
using namespace net;


byte mac[]{0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// IPAddress ip(192, 168, 46, 179);


WebSocketClient client;

void setup() {
  Serial.begin(115200);

  Serial.println(F("Initializing ... "));

  Ethernet.begin(mac); //, ip);
  Serial.print(F("Device IP: "));
  Serial.println(Ethernet.localIP());



  client.onOpen([](WebSocket &ws) {
  Serial.println(F("Connected"));
  const auto protocol = ws.getProtocol();
  if (protocol) {
    Serial.print(F("Client protocol: "));
    Serial.println(protocol);
  }

    const char message[]{"Hello from Arduino client!"};
    ws.send(WebSocket::DataType::TEXT, message, strlen(message));
  });

  client.onMessage([](WebSocket &ws, const WebSocket::DataType dataType,
                     const char *message, uint16_t length) {
    switch (dataType) {
    case WebSocket::DataType::TEXT:
        Serial.print(F("Received: "));
        Serial.println(message);
      break;
    case WebSocket::DataType::BINARY:
        Serial.print(F("Received bin data. Length = "));Serial.print(length);Serial.println(" bytes\n");
        Serial.print("Data:   ");
        for(int i=0;i<length;i++){
          Serial.print(uint8_t(message[i]));Serial.print(", ");
        }
        Serial.println("\n\n");
        break;
    }

    //ws.send(dataType, message, length); // Echo back to server
  });

  client.onClose([](WebSocket &, const WebSocket::CloseCode, const char *,uint16_t) 
  {
    Serial.println(F("Disconnected\n")); 

  });

  if (!client.open("obu-78.arbura.one", 4478, "/ca/uper")) {
      Serial.println(F("Connection failed!"));
    while (1);
  }

}

void loop() { client.listen(); }
