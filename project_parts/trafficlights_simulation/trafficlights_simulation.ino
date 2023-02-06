#define R_LIGHT 2
#define Y_LIGHT 3
#define G_LIGHT 13

#define LIGHTS_THREAD_PERIOD  1000 

#define GREEN_TIME 30
#define YELLOW_TIME 5
#define RED_TIME    30

#define GREEN_ABSTIME    GREEN_TIME
#define YELLOW_ABSTIME1  GREEN_ABSTIME+YELLOW_TIME
#define RED_ABSTIME YELLOW_ABSTIME1 + RED_TIME
#define YELLOW_ABSTIME2  RED_ABSTIME+YELLOW_TIME






long lightsMillis=0;
uint32_t lightsTime=0;
uint8_t  yellowBlink=0;

void setup() {
  Serial.begin(115200);
  pinMode(R_LIGHT,OUTPUT);
  pinMode(Y_LIGHT,OUTPUT);
  pinMode(G_LIGHT,OUTPUT);

  // Serial.print("GREEN_ABSTIME = ");Serial.println(GREEN_ABSTIME);
  // Serial.print("YELLOW_ABSTIME1 = ");Serial.println(YELLOW_ABSTIME1);
  // Serial.print("RED_ABSTIME = ");Serial.println(RED_ABSTIME);
  // Serial.print("YELLOW_ABSTIME2 = ");Serial.println(YELLOW_ABSTIME2);

}

void loop() {
  lights_thread();

  

}

void lights_thread(){

    if(millis()-lightsMillis > LIGHTS_THREAD_PERIOD){
      lightsMillis = millis();

      Serial.print("T = ");Serial.print(lightsTime);Serial.print("   ");
      if(lightsTime<GREEN_TIME){
        Serial.println("GREEN");
        digitalWrite(R_LIGHT,0);
        digitalWrite(Y_LIGHT,0);
        digitalWrite(G_LIGHT,1);
        

      }else if(lightsTime>=GREEN_ABSTIME && lightsTime<YELLOW_ABSTIME1){
        
        if(yellowBlink==0){
            Serial.println("YELLOW1");
            digitalWrite(R_LIGHT,0);
            digitalWrite(Y_LIGHT,1);
            digitalWrite(G_LIGHT,1);
            yellowBlink=1;
        }else{
            digitalWrite(R_LIGHT,0);
            digitalWrite(Y_LIGHT,0);
            digitalWrite(G_LIGHT,1);
            Serial.println();
            yellowBlink=0;
        }
      }else if(lightsTime>=YELLOW_ABSTIME1 && lightsTime<RED_ABSTIME){
            Serial.println("RED");
            digitalWrite(R_LIGHT,1);
            digitalWrite(Y_LIGHT,0);
            digitalWrite(G_LIGHT,0);
      }else if(lightsTime>=RED_ABSTIME && lightsTime<YELLOW_ABSTIME2){
        
        if(yellowBlink==0){
            Serial.println("YELLOW2");
            digitalWrite(R_LIGHT,1);
            digitalWrite(Y_LIGHT,1);
            digitalWrite(G_LIGHT,0);
            yellowBlink=1;
        }else{
            Serial.println();
            digitalWrite(R_LIGHT,1);
            digitalWrite(Y_LIGHT,0);
            digitalWrite(G_LIGHT,0);
            yellowBlink=0;
        }
      }
      lightsTime++;   

      if(lightsTime==YELLOW_ABSTIME2){
        lightsTime=0;
      }

    
    }else return;
}

void setTrafficLights(uint8_t color){
  switch(color){
    case 0:
    digitalWrite(R_LIGHT,0);
    digitalWrite(Y_LIGHT,0);
    digitalWrite(G_LIGHT,0);
    break;
    case 1:
    digitalWrite(R_LIGHT,1);
    digitalWrite(Y_LIGHT,0);
    digitalWrite(G_LIGHT,0);
    break;
    case 2:
    digitalWrite(R_LIGHT,0);
    digitalWrite(Y_LIGHT,1);
    digitalWrite(G_LIGHT,0);
    break;
    case 3:
    digitalWrite(R_LIGHT,0);
    digitalWrite(Y_LIGHT,0);
    digitalWrite(G_LIGHT,1);
    break;
  }

}
