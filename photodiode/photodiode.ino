const int serialSpeed = 9600;
const int photodiode = A0;

const size_t packetCapacity = 512;
byte packet[packetCapacity] = { 0 };
size_t packetSize = 0;

void sendPacket() {
  for (size_t i = 0; i < packetCapacity; ++i)
    Serial.write(packet[i]);
  packetSize = 0;
  delay(1000);
}

void setup() {
  Serial.begin(serialSpeed);
  pinMode(photodiode, INPUT);
}

void loop() {
  packet[packetSize++] = analogRead(photodiode);
  if (packetSize == packetCapacity)
    sendPacket();
}
