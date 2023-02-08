#include <WebSocketClient.h>

/*          НАСТРОЙКИ CAM         */
#define STATION_ID    206267362
#define STATION_TYPE  11

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
// Время работы определенного цвета в тактах

#define TIME_ON_START 0

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
uint32_t lightsTime=TIME_ON_START;
uint8_t  blink_flag,cur_light;

void setup() {

  pinMode(R_LIGHT,OUTPUT);
  pinMode(Y_LIGHT,OUTPUT);
  pinMode(G_LIGHT,OUTPUT);
  digitalWrite(R_LIGHT,1);
  digitalWrite(Y_LIGHT,1);
  digitalWrite(G_LIGHT,1);
  delay(1000);
  digitalWrite(R_LIGHT,0);
  digitalWrite(Y_LIGHT,0);
  digitalWrite(G_LIGHT,0);
  Serial.begin(115200);
  Serial.println(F("IOT traffic lights SPBGUT 2022"));
  


}

void loop() { 
  lights_thread();
  
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