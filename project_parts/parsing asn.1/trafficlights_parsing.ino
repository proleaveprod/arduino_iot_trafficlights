uint8_t dataRx[] = {0x2,0x02    ,0x0c,0x4b,0x63,0xe2,   0x55,0x5a,   0x00,0xfb,    0x2d,0x23 ,0xc6,0x8f,    0xab,0xb8 ,0xee,0x7f,0xff,0xff ,0xfc,0x22,0x33,0x08 ,0x1c,0x80};

unsigned long stationID;
uint32_t deltaTime;
uint8_t stationType;


void setup() {
  Serial.begin(115200);
  
  getCAMdata();
  printCAM();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void printCAM(){
  Serial.println("-------CAM-DATA-------");
  Serial.print("StationID: ");
  Serial.println(stationID);
  Serial.print("DeltaTime: ");
  Serial.println(deltaTime);
  Serial.print("StationType: ");
  Serial.println(stationType);
  Serial.println("----------------------");


}

// function for copying websocket buffer in CAM-variables
void getCAMdata(){
  // Get stationID data from buffer[2:6]
  reverse(&dataRx[2],4);
  memcpy(&stationID,&dataRx[2],4);

  // Get stationID data from buffer[2:4]
  reverse(&dataRx[6],2);
  memcpy(&deltaTime,&dataRx[6],2);

  // Get stationID data from half-byte of buffer[9]
  stationType = dataRx[9] & 0xF;
  reverse(&dataRx[10],4);
}

// function for swapping "data" array with "n" elements
void reverse(uint8_t *data, size_t n)
{
    size_t i;
    for (i=0; i < n/2; ++i) {
        uint8_t tmp = data[i];
        data[i] = data[n - 1 - i];
        data[n - 1 - i] = tmp;
    }
}