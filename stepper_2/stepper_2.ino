const int PIN_STEP = 23;
const int PIN_DIR = 22;
const int PIN_EN = 21;
const int LED = 13;
const int potens = 36;

int currentSpeed = 5;
bool clockwise = true;
bool rotating = false;
bool led_on = true;
bool is_on = false;

bool able_get_potens_val = false;


String commandBuffer = ""; 

void setup() {
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_EN, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(potens, INPUT);
  digitalWrite(PIN_STEP, 1);
  digitalWrite(PIN_DIR, 0);
  Serial.begin(115200);
  delay(1000);
}

void executeCommand(String command) {
  command.trim(); // Удалить начальные и конечные пробелы
  if (!command.isEmpty()) { // Проверка на непустую строку
    if (command.startsWith("s")) {
      int newSpeed = command.substring(1).toInt();
      currentSpeed = newSpeed;
    } else if (command.startsWith("mode")) {
      int mode = command.substring(4).toInt();
      if (mode == 0) {
        rotating = false; // Сброс флага вращения
        is_on = false;
      } else if (mode == 10) {
        rotating = true;
        clockwise = false;
        is_on = true;
      } else if (mode == 11) {
        rotating = true;
        clockwise = true;
        is_on = true;
      }
    } else if (command.startsWith("led")) {
      int isledon = command.substring(3).toInt();
      if (isledon == 0)
        led_on = false;
      else
        led_on = true;
    } else if (command.startsWith("p")) {
      int ispon = command.substring(1).toInt();
      if (ispon == 0)
        able_get_potens_val = false;
      else
        able_get_potens_val = true;
    }
    
//    // Выводим команду в монитор порта
//    Serial.print("Get from serial: ");
//    Serial.println(command);
  }
}

void loop() {
  while (Serial.available() > 0) {
    char incomingChar = Serial.read();
    if (incomingChar == '#') {
      executeCommand(commandBuffer);
      commandBuffer = ""; // Очистить буфер после выполнения команды
    } else {
      commandBuffer += incomingChar;
    }
  }

  digitalWrite(PIN_EN, is_on ? LOW : HIGH);
  
  if (rotating) {
    digitalWrite(PIN_DIR, clockwise ? HIGH : LOW);
    digitalWrite(PIN_STEP, HIGH);
    delay(currentSpeed);
    digitalWrite(PIN_STEP, LOW);
    delay(currentSpeed);
  }
  
  digitalWrite(LED, led_on ? LOW : HIGH);

  if (able_get_potens_val) {
    int analogValue = analogRead(potens);
    Serial.print("p" + String(analogValue) + "#"); 
  }
}
