void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
configurePin(13,1);
bool pinstat = 1;
}

void loop() {
  // put your main code here, to run repeatedly:
delay(20000);
Serial.println(12345);

}
