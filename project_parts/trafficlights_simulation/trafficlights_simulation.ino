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




