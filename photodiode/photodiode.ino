const int serialSpeed = 9600;
const int photodiode = A0;

const byte high_threshold = 50;
const byte low_threshold = 30;

const size_t packetCapacity = 256;
byte packet[packetCapacity] = { 0 };
size_t packetSize = 0;

void sendPacket() {
  for (size_t i = 0; i < packetCapacity; ++i)
    Serial.write(packet[i]);
  packetSize = 0;
}

void setup() {
  Serial.begin(serialSpeed);
  pinMode(photodiode, INPUT);
}

void loop() {
  if (packetSize == 0) {
    while (analogRead(photodiode) < high_threshold)
      continue;
  }
  packet[packetSize++] = analogRead(photodiode);
  if (packetSize == packetCapacity)
    sendPacket();
}
