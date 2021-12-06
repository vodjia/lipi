const int serialSpeed = 9600;
const int photodiode = A0;

byte upper = 0;
byte lower = 0;
byte average = 0;
int period = 1000;

const size_t packetCapacity = 512;
byte packet[packetCapacity] = { 0 };
size_t packetSize = 0;

void calibratePhotodiode() {
  int sum = 0;
  int max = 0;
  int min = 256;
  for (size_t i = 0; i < packetCapacity; ++i) {
    packet[i] = analogRead(photodiode);
    sum += packet[i];
    if (packet[i] > max)
      max = packet[i];
    if (packet[i] < min)
      min = packet[i];
  }
  int range = max - min;
  int average = sum / packetCapacity;
  upper = average + 0.40 * range;
  lower = average - 0.40 * range;
}

void sendPacket() {
  for (size_t i = 0; i < packetCapacity; ++i)
    Serial.write(packet[i]);
  packetSize = 0;
  delay(1000);
}

void setup() {
  Serial.begin(serialSpeed);
  pinMode(photodiode, INPUT);
  calibratePhotodiode();
}

void loop() {
  delayMicroseconds(1000 - micros() % 1000);
  int reading = analogRead(photodiode);
//  if (reading < upper && reading > lower)
//    period -= 80;
//  else if (period != 1000) 
//    period = 1000;
  packet[packetSize++] = reading;
  if (packetSize == packetCapacity)
    sendPacket();
}
