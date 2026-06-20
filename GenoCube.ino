// ================= LIBRARIES =================
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_GFX.h>
#include <Adafruit_VL53L0X.h>
#include <FastLED.h>

#define WHITE SH110X_WHITE

void handleCommand();
void bootAnim();
void stopMotors();

// ===== FUNCTION DECLARATIONS =====
void executeCommand();
void aiDecision(float distance, int soundLevel);
void detectSound();
float readDistance();
void updateRoboEyesEmotion();
void renderEmotionLED8();
void matrixVisualizer();
void matrixWave();
void matrixFire();
void updateSpeed();
void danceMode();
void applyDecision(int d);
float readRearDistance();
float readRearDistanceRaw();
void followHuman();
void learnDecision(int decision, bool success);
void forward();
void backward();
void left();
void right();

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#include <FluxGarage_RoboEyes.h>
RoboEyes<Adafruit_SH1106G> roboEyes(display);

// ================= LED =================
#define LED_PIN 5
#define NUM_LEDS 8
CRGB leds[NUM_LEDS];

#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8

#define MATRIX_PIN 23
#define MATRIX_NUM 64
CRGB matrixLeds[MATRIX_NUM];

byte heat[MATRIX_WIDTH][MATRIX_HEIGHT];

// ================= EMOTIONS =================
#define HAPPY        1
#define CURIOUS      2
#define ANGRY        3
#define SLEEPY       4
#define EXCITED      5
#define SAD          6
#define SCARED       7
#define LOVE         8
#define PARTY        10
#define FOCUSED      11
#define BORED        12

// ================= EYE POSITION =================
#define DEFAULT 0
#define LEFT    1
#define RIGHT   2
#define UP      3
#define DOWN    4

// ================= SENSOR =================
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// ================= SOUND =================
#define MIC_PIN 34

// ================= MOTOR =================
#define AIN1 25
#define AIN2 26
#define BIN1 27
#define BIN2 14

// ===== TB6612FNG EXTRA PINS =====
#define PWMA 32
#define PWMB 33
#define STBY 4

// ========== HC-SR04 ULTRASONIC =============
#define TRIG_PIN 18
#define ECHO_PIN 19

// ================= WIFI =================
WebServer server(80);

// ================= STATE =================
int mode = 0;
int currentSpeed = 0;
int targetSpeed = 0;
int emotion = 1;
bool musicMode = false;
int moodType = 0;
int blinkTimer = 0;
bool isBlinking = false;
int eyeOffsetX = 0;
int eyeOffsetY = 0;
String command = "";

unsigned long lastDecisionTime = 0;
int currentDecision = 0; // 0=stop, 1=forward, 2=left, 3=right, 4=backward

unsigned long lastMoveTime = 0;
bool isResting = false;

int soundLevel = 0;
int lastSound = 0;
unsigned long lastBlink = 0;
bool blinkState = false;

bool manualMode = false;
int led8Mode = 0;
int matrixMode = 0;
bool matrixMusic = false;

int scoreForward = 0;
int scoreBackward = 0;
int scoreLeft = 0;
int scoreRight = 0;
int scoreStop = 0;

bool followMode = false;
bool autoMode = true;
float previousDistance = 0;

int XY(int x, int y) {
  return (y * MATRIX_WIDTH) + x;
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("GenoCube","12345678");
  
  Serial.println(WiFi.softAPIP());

  Wire.begin(21, 22);
  Wire.setClock(400000); // fix noise issue

  display.begin(0x3C, true);

  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100);

  roboEyes.setAutoblinker(ON, 3, 2);
  roboEyes.setIdleMode(ON, 2, 2);
  roboEyes.setCuriosity(ON);
  display.setTextColor(WHITE);

  display.clearDisplay();
  display.display();

  randomSeed(analogRead(34));  // randomize behavior

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, MATRIX_PIN>(matrixLeds, MATRIX_NUM);

  if (!lox.begin()) {
  Serial.println("VL53L0X failed!");
  while(1);
}

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

// PWM setup (NEW for ESP32 3.x)
ledcAttach(PWMA, 5000, 8);
ledcAttach(PWMB, 5000, 8);

// STBY pin
pinMode(STBY, OUTPUT);
digitalWrite(STBY, HIGH); // enable motors

// Ultrasonic rare eye
pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);

// Initialize heat array
memset(heat, 0, sizeof(heat));

  
server.on("/", [](){   // Web UI

  String page = R"====(
 <!DOCTYPE html>
 <html>
 <head>
 <meta charset="UTF-8">
 <meta name="viewport" content="width=device-width, initial-scale=1.0">
 <title>GenoCube PRO</title>

 <style>
 body {
   margin:0;
   font-family: Arial;
   background:#111;
   color:white;
   text-align:center;
   }

 h2 { margin:10px; }

 button, select {
   padding:10px;
   margin:5px;
   border:none;
   border-radius:10px;
   background:#00adb5;
   color:white;
   }

 button:active {
    transform:scale(0.95);
   }

 .section {
    margin:15px;
   padding:10px;
   border:1px solid #333;
   border-radius:12px;
   }
 </style>
 </head>

 <body>

 <h2>🚀 GenoCube PRO Control</h2>

 <!-- MODE -->
 <div class="section">
  <h3>Mode</h3>
  <button onclick="send('manual')">Manual</button>
  <button onclick="send('auto')">Auto</button>
  <button onclick="send('follow')">Follow Me</button>
 </div>

 <!-- MOTION -->
 <div class="section">
  <h3>Motion</h3>
  <button onclick="send('forward')">⬆ Forward</button><br>
  <button onclick="send('left')">⬅ Left</button>
  <button onclick="send('stop')">⏹ Stop</button>
  <button onclick="send('right')">➡ Right</button>
  <button onclick="send('backward')">⬇ Backward</button>
 </div>

 <!-- 8 LED CONTROL -->
 <div class="section">
  <h3>8 LED Strip</h3>

  <button onclick="send('led_solid')">Solid</button>
  <button onclick="send('led_wave')">Wave</button>
  <button onclick="send('led_rainbow')">Rainbow</button>
 </div>

 <!-- DANCE MODE -->
 <button onclick="send('dance_on')">Dance ON</button>
 <button onclick="send('dance_off')">Dance OFF</button>

 <!-- MATRIX CONTROL -->
 <div class="section">
  <h3>64 LED Matrix</h3>

  <button onclick="send('matrix_music_on')">Music ON</button>
  <button onclick="send('matrix_music_off')">Music OFF</button><br>

  <button onclick="send('matrix_wave')">Wave</button>
  <button onclick="send('matrix_fire')">Fire</button>
 </div>

 <!-- VOICE -->
 <div class="section">
  <h3>🎤 Voice Control</h3>
  <button onclick="startVoice()">Start Voice</button>
  <p id="status"></p>
 </div>

 <script>

 function send(cmd){
  fetch('/cmd?c=' + cmd);
 }

 // 🎤 Voice Control
 function startVoice(){
  const rec = new (window.SpeechRecognition || window.webkitSpeechRecognition)();
  rec.lang = "en-IN";
  rec.start();

  rec.onresult = (e)=>{
    let text = e.results[0][0].transcript.toLowerCase();
    document.getElementById("status").innerHTML = text;

    if(text.includes("forward")) send("forward");
    else if(text.includes("left")) send("left");
    else if(text.includes("right")) send("right");
    else if(text.includes("stop")) send("stop");

    else if(text.includes("manual")) send("manual");
    else if(text.includes("auto")) send("auto");

    else if(text.includes("wave")) send("led_wave");
    else if(text.includes("rainbow")) send("led_rainbow");

    else if(text.includes("fire")) send("matrix_fire");
    else if(text.includes("music on")) send("matrix_music_on");
    else if(text.includes("music off")) send("matrix_music_off");
  };
 }

 </script>

 </body>
 </html>
 )====";

 server.send(200,"text/html; charset=UTF-8",page);

});

  server.on("/cmd", handleCommand);
  
  server.begin();

  bootAnim();
}

//===========SMARTEMOTIONSYSTEM============//
void smartEmotionSystem(float d){

  static int lastEmotion = -1;
  static unsigned long lastRandomTime = 0;
  static unsigned long randomHold = 0;
  static bool randomActive = false;

  // DISTANCE LOGIC
  if(d > 0 && d < 80){
    emotion = ANGRY;
  }
  else if(d < 110){
    emotion = SCARED;
  }
  else if(d < 150){
    emotion = CURIOUS;
  }
  else if(d < 250){
    emotion = HAPPY;
  }
  else if(d < 300){
    emotion = LOVE;
  }
  else if(d < 400){
    emotion = FOCUSED;
  }
  else{
    emotion = BORED;
  }

  if(soundLevel < 10 && d > 500){
    emotion = SAD;
  }

  // SOUND
  if(soundLevel > 400){
    emotion = EXCITED;
  }

  if(soundLevel > 800){
    emotion = PARTY;
  }

  // TRANSITION
  if(emotion != lastEmotion){
    lastEmotion = emotion;
  }
}
// ================= LOOP =================
void loop(){

  server.handleClient();

  float distance = readDistance();
  
  float rearDistance = readRearDistance();
  if(previousDistance == 0){
    previousDistance = distance;
  }

  Serial.print("Front: ");
  Serial.print(distance);

  Serial.print(" mm | Rear: ");
  Serial.print(rearDistance);

  Serial.println(" cm");

  static unsigned long lastAIPrint = 0;

  if(millis() - lastAIPrint > 3000){

      Serial.print("F=");
      Serial.print(scoreForward);

      Serial.print(" L=");
      Serial.print(scoreLeft);

      Serial.print(" R=");
      Serial.print(scoreRight);

      Serial.print(" B=");
      Serial.print(scoreBackward);

      Serial.print(" S=");
      Serial.println(scoreStop);

      lastAIPrint = millis();
  
  }

  detectSound();

  if(distance > previousDistance + 30){

    learnDecision(currentDecision,true);

  }
  if(distance < previousDistance - 30){

    learnDecision(currentDecision,false);

  }

  if(followMode){
    followHuman();
  }
  
  // ===== MANUAL MODE =====
  if(manualMode){
    executeCommand();
  }

  // ===== AUTO MODE WITH REST =====
  else if(autoMode && distance > 0){

    smartEmotionSystem(distance);   // emotion first

    if(isResting){

      stopMotors();   // 😴 REST MODE

      if(millis() - lastMoveTime > 3000){
        isResting = false;
      }

    } else {

      aiDecision(distance, soundLevel);   // 🤖 MOVE

      if(millis() - lastMoveTime > 100000){
        isResting = true;
        lastMoveTime = millis();
      }
    }

    static unsigned long lastPrint = 0;

     if(millis() - lastPrint > 500){

      Serial.print("Distance: ");
      Serial.println(distance);

      lastPrint = millis();
    }
  }

  // ===== DANCE MODE (MANUAL ONLY) =====
  if(musicMode && soundLevel > 500){
    danceMode();
  }

  // ===== EYE MOVEMENT =====
  int t1 = millis() / 500;

  if(t1 % 4 == 0) roboEyes.setPosition(LEFT);
  else if(t1 % 4 == 1) roboEyes.setPosition(RIGHT);
  else if(t1 % 4 == 2) roboEyes.setPosition(UP);
  else roboEyes.setPosition(DEFAULT);

  int t2 = millis() / 400;

  if(emotion == ANGRY){
    if(t2 % 2 == 0) roboEyes.setPosition(LEFT);
    else roboEyes.setPosition(DEFAULT);
  }

  if(emotion == SCARED){
    if(t2 % 2 == 0) roboEyes.setPosition(UP);
    else roboEyes.setPosition(RIGHT);
  }

  // ===== EMOTION OUTPUT =====
  if(isResting){
    emotion = SLEEPY;   // 😴 looks natural
  }

  updateRoboEyesEmotion();
  renderEmotionLED8();

  // ===== MATRIX =====
  if(matrixMusic){
    matrixVisualizer();
  }
  else{
    if(matrixMode == 1) matrixWave();
    else if(matrixMode == 2) matrixFire();
  }

  updateSpeed();

  previousDistance = distance;

  roboEyes.update();
  FastLED.show();

  delay(1);

}

// ========= FOLLOW HUMAN RSSI ==========
void followHuman(){

    int rssi = WiFi.RSSI();

    if(rssi > -55){
        stopMotors();
    }
    else if(rssi > -70){
        forward();
    }
    else{
        targetSpeed = 255;
        forward();
    }
}

// ================= MOTOR =================
void forward(){
  digitalWrite(AIN1,LOW); digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW); digitalWrite(BIN2,HIGH);
  targetSpeed = 255;
}
void left(){
  digitalWrite(AIN1,HIGH); digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); digitalWrite(BIN2,HIGH);
  targetSpeed = 200;
}
void right(){
  digitalWrite(AIN1,LOW); digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH); digitalWrite(BIN2,LOW);
  targetSpeed = 200;
}
void backward(){
  digitalWrite(AIN1,HIGH); digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); digitalWrite(BIN2,LOW);
  targetSpeed = 255;
}
void stopMotors(){
  digitalWrite(AIN1,LOW); 
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,LOW);
  targetSpeed = 0;

  lastMoveTime = millis();
}


void followDistance(float d){

  if(d > 400){
    forward();
  }
  else if(d < 100){
    stopMotors();
  }
  else{
    if(((int)d) % 2 == 0) left();
    else right();
  }
}

// ================= SMOOTH ACCELERATION =================
void updateSpeed(){
  if(currentSpeed < targetSpeed) currentSpeed += 5;
  else if(currentSpeed > targetSpeed) currentSpeed -= 5;

  ledcWrite(PWMA, currentSpeed);
  ledcWrite(PWMB, currentSpeed);
}

// ================= SMOOTH FOLLOW =================
float readDistance(){
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure,false);

  if(measure.RangeStatus != 4){
    return measure.RangeMilliMeter;
  }

  return -1;
}

// ============ RARE EYE =============
float readRearDistanceRaw(){

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 10000);

  if(duration == 0){
    return -1;
  }

  float distance = duration * 0.0343 / 2.0;

  return distance * 10;
}

// =========== RARE EYE 2ND FXN ============
float readRearDistance(){

  float sum = 0;
  int valid = 0;

  for(int i=0;i<5;i++){

      float d = readRearDistanceRaw();

      if(d > 0){

          sum += d;

          valid++;

      }

      delay(5);
  }

  if(valid == 0){
      return -1;
  }

  return sum / valid;
}

// ================= SOUND =================
void detectSound(){

  int val = analogRead(MIC_PIN);

  int diff = abs(val - lastSound);

  // smoothing for LM393
  soundLevel = (soundLevel * 0.7) + (diff * 0.3);

  lastSound = val;

}

// ================= LED PRO =================
void renderLEDPro(){
  uint8_t t = millis()/10;

  if(emotion == 1){
    for(int i=0;i<NUM_LEDS;i++){
      leds[i] = CHSV(t + i*10, 200, sin8(t + i*20));
    }
  }
  else if(emotion == 2){
    for(int i=0;i<NUM_LEDS;i++){
      leds[i] = CHSV(160 + sin8(t+i*10), 255, beatsin8(8,50,255));
    }
  }
  else if(emotion == 3){
    for(int i=0;i<NUM_LEDS;i++){
      leds[i] = CHSV(0,255, beatsin8(20,0,255));
    }
  }
}

// ================= MATRIX VISUALIZER =================
void matrixVisualizer(){
  for(int x=0; x<8; x++){
    int height = map(soundLevel, 0, 500, 0, 8);
    height += random(-1,2);
    height = constrain(height,0,8);

    for(int y=0; y<8; y++){
      int index = XY(x, y);

      if(y < height){
        matrixLeds[index] = CHSV(x*30 + millis()/5,255,255);
      } else {
        matrixLeds[index] = CRGB::Black;
      }
    }
  }
}

// =========== MATRIX IN ANIMATION ===========
void matrixWave() {
  static uint16_t t = 0;
  t += 1;

  for (int y = 0; y < MATRIX_HEIGHT; y++) {
    for (int x = 0; x < MATRIX_WIDTH; x++) {

      uint8_t wave = sin8(x * 10 + t) + cos8(y * 10 + t);
      uint8_t brightness = wave;

      CRGB color = CHSV(wave, 255, brightness);

      int index = XY(x, y);  // your matrix mapping function
      matrixLeds[index] = color;
    }
  }
}

#define COOLING  55
#define SPARKING 120

void matrixFire() {

  // Step 1: Cool down every cell a little
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
      heat[x][y] = qsub8(heat[x][y], random8(0, ((COOLING * 10) / MATRIX_HEIGHT) + 2));
    }
  }

  // Step 2: Heat rises
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = MATRIX_HEIGHT - 1; y > 1; y--) {
      heat[x][y] = (heat[x][y - 1] + heat[x][y - 2] + heat[x][y - 3]) / 3;
    }
  }

  // Step 3: Random sparks at the bottom
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    if (random8() < SPARKING) {
      int y = 0;
      heat[x][y] = qadd8(heat[x][y], random8(160, 255));
    }
  }

  // Step 4: Map heat to LED colors
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = 0; y < MATRIX_HEIGHT; y++) {

      uint8_t colorIndex = heat[x][y];

      CRGB color = HeatColor(colorIndex);

      int index = XY(x, y);  // your mapping function
      matrixLeds[index] = color;
    }
  }
}

// ================= CLEAR MATRIX =================
void clearMatrix(){
  for(int i=0;i<MATRIX_NUM;i++){
    matrixLeds[i] = CRGB::Black;
  }
}

// ================= BOOT =================
void bootAnim(){
  for(int i=0;i<NUM_LEDS;i++){
    leds[i]=CHSV(i*30,255,255);
    FastLED.show();
    delay(70);
  }
}

// ===== RECEIVE COMMAND =====
void handleCommand(){

  if(server.hasArg("c")){

    command = server.arg("c");

    Serial.println("Command: " + command);

    // ===== MODE =====
    if(command == "manual"){
      manualMode = true;
      autoMode = false;
      followMode = false;
    }
    else if(command == "auto"){
      manualMode = false;
      autoMode = true;
      followMode = true;
    }
    else if(command == "follow"){
      followMode = true;
      autoMode = true;
      manualMode = false;
    }


    else if(command == "dance_on"){
      musicMode = true;
    }
    else if(command == "dance_off"){
      musicMode = false; 
    }
  

    // ===== MOTION =====
    else if(command == "forward") forward();
    else if(command == "left") left();
    else if(command == "right") right();
    else if(command == "stop") stopMotors();
    else if(command == "backward") backward();

    // ===== 8 LED MODES =====
    else if(command == "led_solid") led8Mode = 0;
    else if(command == "led_wave") led8Mode = 1;
    else if(command == "led_rainbow") led8Mode = 2;

    // ===== MATRIX MODES =====
    else if(command == "matrix_music_on") matrixMusic = true;
    else if(command == "matrix_music_off") matrixMusic = false;
    else if(command == "matrix_wave") matrixMode = 1;
    else if(command == "matrix_fire") matrixMode = 2;
  }

  server.send(200, "text/plain", "OK");

}

// ===== EXECUTE COMMAND =====
void executeCommand(){

  if(command == "forward"){
    forward();
  }
  else if(command == "backward"){
    backward();
  }
  else if(command == "left"){
    left();
  }
  else if(command == "right"){
    right();
  }
  else if(command == "stop"){
    stopMotors();
  }
  else if(command == "happy"){
    emotion = 1;
  }
  else if(command == "angry"){
    emotion = 3;
  }
  else if(command == "music"){
    musicMode = true;
  }
  else if(command == "nomusic"){
    musicMode = false;
  }

}

// ==============ROBOEYES UPDATE=================
void updateRoboEyesEmotion(){

  switch(emotion){

    case HAPPY:
      roboEyes.setMood(HAPPY);
      roboEyes.setPosition(DEFAULT);
      break;

    case ANGRY:
      roboEyes.setMood(ANGRY);
      roboEyes.setPosition(LEFT);   // angry glare
      break;

    case SLEEPY:
      roboEyes.setMood(TIRED);
      roboEyes.setPosition(DOWN);   // sleepy look
      break;

    case SAD:
      roboEyes.setMood(TIRED);
      roboEyes.setPosition(DOWN);
      break;

    case EXCITED:
      roboEyes.setMood(HAPPY);
      roboEyes.setPosition(RIGHT);
      break;

    case SCARED:
      roboEyes.setMood(DEFAULT);
      roboEyes.setPosition(UP);
      break;

    case LOVE:
      roboEyes.setMood(HAPPY);
      roboEyes.setPosition(DEFAULT);
      break;

    case PARTY:
      roboEyes.setMood(HAPPY);
      roboEyes.setPosition(RIGHT);
      break;  

    case FOCUSED:
      roboEyes.setMood(DEFAULT);
      roboEyes.setPosition(DEFAULT);
      break;

    case BORED:
      roboEyes.setMood(TIRED);
      roboEyes.setPosition(DOWN);
      break;
  }
}

//================ROBOEYES EMOTION LED8=============//
void renderEmotionLED8(){

  switch(emotion){

    case HAPPY:
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      break;

    case CURIOUS:
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
      break;

    case ANGRY:
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      break;

    case SCARED:
      fill_solid(leds, NUM_LEDS, CRGB::Purple);
      break;

    case LOVE:
      fill_solid(leds, NUM_LEDS, CRGB::Pink);
      break;

    case PARTY:
      for(int i=0;i<NUM_LEDS;i++){
      leds[i] = CHSV(millis()/10 + i*20,255,255);
      }
      break;  

    case FOCUSED:
      fill_solid(leds, NUM_LEDS, CRGB::Yellow);
      break;

    case BORED:
      fill_solid(leds, NUM_LEDS, CRGB::Orange);
      break;

  }
}

// ========== ROBOT APPLYDECISION ==========
void applyDecision(int d){

  switch(d){
    case 1: forward(); break;
    case 2: left(); break;
    case 3: right(); break;
    case 4: backward(); break;
    default: stopMotors(); break;
  }
}
// ========= AI LEARNING DECISION FUNCTION ===========
void learnDecision(int decision, bool success){

  int reward = success ? 1 : -2;

  switch(decision){

    case 1:
      scoreForward += reward;
      break;

    case 2:
      scoreLeft += reward;
      break;

    case 3:
      scoreRight += reward;
      break;

    case 4:
      scoreBackward += reward;
      break;

    case 0:
      scoreStop += reward;
      break;
  }

  scoreForward = constrain(scoreForward,-100,100);
  scoreLeft = constrain(scoreLeft,-100,100);
  scoreRight = constrain(scoreRight,-100,100);
  scoreBackward = constrain(scoreBackward,-100,100);
  scoreStop = constrain(scoreStop,-100,100);

}

// ========== AI DECISION FUNCTION (FIXED) ==========
void aiDecision(float distance, int soundLevel) {

  // 🛑 HOLD decision to prevent flickering
  if(millis() - lastDecisionTime < 600){
    applyDecision(currentDecision);
    return;
  }

  // ===== CLEAN DISTANCE LOGIC =====
  if(distance > 400){
    currentDecision = 1; // forward
  }
  else if(distance > 250){
    currentDecision = 1; // still forward (smooth)
  }
  else if(distance > 150){

    if(scoreLeft > scoreRight){

        currentDecision = 2;

    }
    else{

        currentDecision = 3;

    }

  }
  else if(distance > 100){
    currentDecision = 0; // stop zone
  }
  else{

    float rearDistance = readRearDistance();

    if(rearDistance < 0){
        currentDecision = 0;
    }
    else if(rearDistance > 300){
        currentDecision = 4;
    }
    else{

        if(currentDecision == 2){
            currentDecision = 3;
        }
        else{
            currentDecision = 2;
        }

    }

  }
  // ===== APPLY DECISION =====
  lastDecisionTime = millis();
  applyDecision(currentDecision);
  
}

// ========== DANCE MODE ===========
void danceMode(){

  int beat = millis() / 300;

  emotion = EXCITED;

  if(beat % 4 == 0){
    left();
  }
  else if(beat % 4 == 1){
    right();
  }
  else if(beat % 4 == 2){
    forward();
  }
  else{
    backward();
  }
}

added main code of genocube
