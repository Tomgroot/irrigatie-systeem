#define LED_PIN 13  // Built-in LED

int humidityPin[] = {A0, A2};
int pumpPin[] = {10, 9};
// In approx 31 pumps, it moves 15cl of water. 6 times a day * 7 = 42 pumps is approx 20cl
int pumpsPerDay[] = {0, 0};
float prevPumpedM[] = {0, 0};
float prevDayM = 0;
float elapsedM = 0;
float humidity[] = {0.0, 0.0};
int maxPumpsPerDay[] = {5, 5};
int maxHumidity[] = {320, 320};
int nrOfPlants = 2;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < nrOfPlants; i++) {
    pinMode(pumpPin[i], OUTPUT);
  }
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  unsigned long elapsedMs = millis();
  elapsedM = elapsedMs / 1000 / 60;

  Serial.println();
  Serial.print("Elapsed time: ");
  Serial.print(elapsedMs);
  Serial.print(" ms or ");
  Serial.print(elapsedM);
  Serial.print(" m, Day start time: ");
  Serial.print(prevDayM);
  Serial.print(" m, Nr plants ");
  Serial.println(nrOfPlants);
  
  
  for (int i = 0; i < nrOfPlants; i++) {
    checkPlant(i);
    delay(5000);
  }
  

  if (elapsedM - prevDayM > 24 * 60) {
    Serial.println("Day elapsed!");
    for (int i = 0; i < nrOfPlants; i++) {
      pumpsPerDay[i] = 0;
    }
  }
  
  delay(10000);
}

void checkPlant(int id) {
  humidity[id] = analogRead(humidityPin[id]);
  printPlantData(id);
  showPumpsPerDayOnLed(id);

  if (elapsedM - prevPumpedM[id] >= 30 && pumpsPerDay[id] < maxPumpsPerDay[id] && humidity[id] > maxHumidity[id]){
    prevPumpedM[id] = elapsedM;
    pumpsPerDay[id] ++;
    Serial.println("PUMPING!");
    digitalWrite(pumpPin[id], HIGH);
    delay(1000);
    digitalWrite(pumpPin[id], LOW);
    delay(1000);
  }
}

void showPumpsPerDayOnLed(int id) {
  // To show on the arduino board how much we have pumped today
  digitalWrite(LED_PIN, HIGH);
  delay(2000);
  digitalWrite(LED_PIN, LOW);
  for (int i = 0; i < pumpsPerDay[id]; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

void printPlantData(int id) {
  Serial.print("Plant ");
  Serial.print(id);
  Serial.print(". Previously pumped time: ");
  Serial.print(prevPumpedM[id]);
  Serial.print(" m, Elapsed since last pump: ");
  Serial.print(elapsedM - prevPumpedM[id]);
  Serial.print(" m, Max pumps per day: ");
  Serial.print(maxPumpsPerDay[id]);
  Serial.print(", Nr pumps today: ");
  Serial.print(pumpsPerDay[id]);
  Serial.print(", Max humidity: ");
  Serial.print(maxHumidity[id]);
  Serial.print(", Humidity: ");
  Serial.println(humidity[id]);
}