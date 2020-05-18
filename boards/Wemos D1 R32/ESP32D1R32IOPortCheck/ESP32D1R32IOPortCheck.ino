int ports[] ={2,4,35,34,36,39,18,19,23,5,13,12,14,27,16,17,25,26};
 int count = 18;
 
 void setup() {
  Serial.begin(115200);
 for(int i = 0; i < count ; i++){
    pinMode(ports[i], INPUT);
    digitalWrite(ports[i],LOW);
 }

}

void loop() {
 for(int i = 0; i < count ; i++){
    if( digitalRead(ports[i])){
      Serial.println(ports[i]);
    }
 }


}
