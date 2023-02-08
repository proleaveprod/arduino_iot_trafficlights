#include <WebSocketClient.h>

/*          НАСТРОЙКИ CAM         */
#define STATION_ID    206267362
#define STATION_TYPE  15

  //При каком остатке времени при зеленом свете срабатывает добавка времени по сообщению CAM
#define CAM_TRIGGER_GREEN_TIME  10
  //Сколько добавляется времени при этом
#define CAM_ADD_GREEN_TIME      20



/*          СЕТЕВЫЕ НАСТРОЙКИ         */
#define SERVER_ADDR "obu-78.arbura.one"
#define SERVER_CDIR "/ca/uper"
#define SERVER_PORT 4478
      // При раскомментировании устройство получает ip-адрес автоматически от AP
#define ETHERNET_DHCP     
      // Если DHCP не используется
#define CLIENT_IP_ADDR  192, 168, 8, 179



/*          НАСТРОЙКИ СВЕТОФОРА         */
      // Время одного такта светофора в мсек
#define LIGHTS_THREAD_PERIOD  500 

#define TIME_ON_START 0
// Время работы определенного цвета в тактах
#define GREEN_TIME 40
#define TRANS_TIME 8
#define YELLOW_TIME 4
#define RED_TIME    40
      // При раскомментировании в Seiral-терминал будет выводиться локальное время тактов светофора
#define PRINT_TRAFFIC_TIME
      // Цифровые пины для ламп
#define R_LIGHT 2
#define Y_LIGHT 3
#define G_LIGHT 4





#define GREEN_ABSTIME    GREEN_TIME
#define YELLOW_ABSTIME1  GREEN_ABSTIME+YELLOW_TIME
#define RED_ABSTIME YELLOW_ABSTIME1 + RED_TIME
#define YELLOW_ABSTIME2  RED_ABSTIME+YELLOW_TIME
#ifndef ETHERNET_DHCP
IPAddress ip(CLIENT_IP_ADDR);
#endif
using namespace net;
WebSocketClient client;
byte mac[]{0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

unsigned long stationID;
uint32_t deltaTime;
uint8_t stationType;

long lightsMillis=0;
int32_t lightsTime=TIME_ON_START;
uint8_t  blink_flag,cur_light;

void setup() {

  pinMode(R_LIGHT,OUTPUT);
  pinMode(Y_LIGHT,OUTPUT);
  pinMode(G_LIGHT,OUTPUT);


  Serial.begin(115200);
  Serial.println(F("IOT traffic lights SPBGUT 2022"));

  digitalWrite(R_LIGHT,1);
  digitalWrite(Y_LIGHT,1);
  digitalWrite(G_LIGHT,1);
  delay(1000);
  digitalWrite(R_LIGHT,0);
  digitalWrite(Y_LIGHT,0);
  digitalWrite(G_LIGHT,0);

  #ifdef ETHERNET_DHCP
  Ethernet.begin(mac);
  #else 
  Ethernet.begin(mac, ip);
  #endif
  Serial.print(F("Device IP: "));
  Serial.println(Ethernet.localIP());


  client.onOpen([](WebSocket &ws) {
  Serial.print("Connected to ");
  Serial.print(SERVER_ADDR);
  Serial.print(SERVER_CDIR);
  Serial.print(" : ");
  Serial.println(SERVER_PORT);

  const auto protocol = ws.getProtocol();
  if (protocol) {
    Serial.print(F("Client protocol: "));
    Serial.println(protocol);
  }

    // const char message[]{"Hello from Arduino client!"};
    // ws.send(WebSocket::DataType::TEXT, message, strlen(message));
  });

  client.onMessage([](WebSocket &ws, const WebSocket::DataType dataType,
                     const char *message, uint16_t length) {
    switch (dataType) {
    case WebSocket::DataType::TEXT:
        Serial.print(F("Received: "));
        Serial.println(message);
      break;
    case WebSocket::DataType::BINARY:
        getCAMdata((uint8_t*)message);

        //Serial.print("  #WS:  Received ");Serial.print(length);Serial.println(" bytes");
        //printCAM();

        cam_alarm_func();
        break;
    }

  });

  client.onClose([](WebSocket &, const WebSocket::CloseCode, const char *,uint16_t) 
  {
    Serial.println(F("Disconnected\n")); 

  });

  if (!client.open(SERVER_ADDR, SERVER_PORT, SERVER_CDIR)) {
      Serial.println(F("Connection failed!"));
    while (1);
  }

}

void loop() { 
  client.listen(); 
  lights_thread();
  
  }

static void getCAMdata(uint8_t *dataRx){
  // Get stationID data from buffer[2:6]
  reverse(&dataRx[2],4);
  memcpy(&stationID,&dataRx[2],4);

  // Get stationID data from buffer[2:4]
  reverse(&dataRx[6],2);
  memcpy(&deltaTime,&dataRx[6],2);

  // Get stationID data from half-byte of buffer[9]
  stationType = ((dataRx[9] & 0xF0)>>4);
  reverse(&dataRx[10],4);
}

static void printCAM(){
  Serial.println("  -------CAM-DATA-------");
  Serial.print("  StationID: ");
  Serial.println(stationID);
  Serial.print("  DeltaTime: ");
  Serial.println(deltaTime);
  Serial.print("  StationType: ");
  Serial.println(stationType);
  Serial.println("  ----------------------");
}

static void cam_alarm_func(){
  if(stationID==STATION_ID && stationType==STATION_TYPE){
    Serial.println("                      -CAM ALARM-");

    if(lightsTime>=GREEN_ABSTIME-CAM_TRIGGER_GREEN_TIME && lightsTime<GREEN_ABSTIME){
      lightsTime-=CAM_ADD_GREEN_TIME;
      Serial.print("                        ADD ");Serial.print(CAM_ADD_GREEN_TIME);Serial.println(" TICKS TO GREEN LIGHT");
      setLight(1,1,1);
      delay(200);
      setLight(0,0,1);
    } 
  }
  
   

}



static void reverse(uint8_t *data, size_t n)
{
    size_t i;
    for (i=0; i < n/2; ++i) {
        uint8_t tmp = data[i];
        data[i] = data[n - 1 - i];
        data[n - 1 - i] = tmp;
    }
}

static void lights_thread(){

    if(millis()-lightsMillis > LIGHTS_THREAD_PERIOD){
      lightsMillis = millis();

      #ifdef PRINT_TRAFFIC_TIME
      Serial.print("T = ");Serial.print(lightsTime);Serial.print("   ");
      #endif

      if(lightsTime<GREEN_ABSTIME-TRANS_TIME){
        if(cur_light!=1){
        cur_light = 1;
        Serial.print("GREEN");
        setLight(0,0,1);
        }

      }else if(lightsTime<GREEN_ABSTIME && lightsTime>=GREEN_ABSTIME-TRANS_TIME){
        if(cur_light!=2){
        cur_light = 2;
        Serial.print("TRANS GREEN");
        }
        if(blink_flag==0){
            Serial.print(" ");
            setLight(0,0,0);
            blink_flag=1;
        }else{
            Serial.print("*");
            setLight(0,0,1);
            blink_flag=0;
        }
      }else if(lightsTime>=GREEN_ABSTIME && lightsTime<YELLOW_ABSTIME1){
        if(cur_light!=3){
          cur_light = 3;
          Serial.print("YELLOW");
          setLight(0,1,0);
        } 
      }else if(lightsTime>=YELLOW_ABSTIME1 && lightsTime<RED_ABSTIME-TRANS_TIME){
        if(cur_light!=4){
        cur_light = 4;
        Serial.print("RED");
        setLight(1,0,0);
        }
      }else if(lightsTime<RED_ABSTIME && lightsTime>=RED_ABSTIME-TRANS_TIME){
        if(cur_light!=5){
        cur_light = 5;
        Serial.print("TRANS RED");
        }
        if(blink_flag==0){
            Serial.print(" ");
            setLight(0,0,0);
            blink_flag=1;
        }else{
            Serial.print("*");
            setLight(1,0,0);
            blink_flag=0;
        }

      }else if(lightsTime>=RED_ABSTIME && lightsTime<YELLOW_ABSTIME2){
        if(cur_light!=6){
        cur_light = 6;
        Serial.print("YELLOW");
        setLight(0,1,0);
        }

      }
      lightsTime++;   

      if(lightsTime==YELLOW_ABSTIME2){
        lightsTime=0;
      }
      Serial.println();
    
    }else return;
}

void setLight(uint8_t r,uint8_t y,uint8_t g){
  digitalWrite(R_LIGHT,r);
  digitalWrite(Y_LIGHT,y);
  digitalWrite(G_LIGHT,g);
}
