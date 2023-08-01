#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
#define SERVOMIN 102
#define SERVOMAX 500
#include <ESP32Servo.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>






double a0; double a1; double a2; double a3;
int leg1[] = {0,1,2};int leg2[] = {3,4,5}; int leg3[] = {6,7,8};
int leg4[] = {9,10,11}; int leg5[] = {12,13,14}; int leg6[] = {15,0,1};


double theta1;
double theta2;
double theta3; 
double theta4;
double theta5;
double theta6;

int deltatheta=15;
int leftgrab=0;
int rightgrab=0;
    
String header="";

String InitialState = "off";
String StandState = "off";
String ForwardState = "off";
String BackwardState = "off";
String LeftState = "off";
String RightState = "off";
String LeftTurnState = "off";
String RightTurnState ="off";

const char* ssid = "POCO F3";
const char* password = "12345678";
AsyncWebServer server(80); // server port 80


char webpage[] PROGMEM = R"=====(

<!DOCTYPE html><html>
<head><meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
.button{ background-color: #4CAF50; border: none; color: white;padding: 20px 20px;text-decoration: none; font-size: 15px; margin: 0px; cursor: pointer;}
.button.initial { position:absolute;left:150px;top:500px;}
.button.stand { position:absolute;left:250px;top:500px;padding: 20px 60px;}
.button.forward { position:absolute;left:250px;top:200px;}
.button.backward { position:absolute;left:250px;top:300px;}
.button.left { position:absolute;left:150px;top:300px;}
.button.right { position:absolute;left:350px;top:300px;}
.button.leftturn { position:absolute;left:150px;top:200px;}
.button.rightturn { position:absolute;left:350px;top:200px;}
.button2 {background-color: #555555;}</style></head>
<body><h1>ESP32 Web Server</h1>
<form><input type="range" id="send" min="1" max="5" value="1" oninput="result.value=send.value">
<p>Manipulating times<input type="text" id="result" value="1" oninput="send.value=result.value"></p>
<br>
<br>
<p>Theta1:<input type="range" id="send_2" min="0" max="180" value="0" oninput="theta1.value=send_2.value"></p>
<input type="text" id="theta1" value="0" oninput="send_2.value=theta1.value">
<br>
<p>Theta2:<input type="range" id="send_3" min="0" max="180" value="0" oninput="theta2.value=send_3.value"></p>
<input type="text" id="theta2" value="0" oninput="send_3.value=theta2.value">
<br>
<p>GrabLeft:<input type="range" id="send_4" min="0" max="1" value="0" oninput="grableft.value=send_4.value"></p>
<input type="text" id="grableft" value="0" oninput="send_4.value=grableft.value">
<br>
<br>
<p>Theta3:<input type="range" id="send_5" min="0" max="180" value="0" oninput="theta3.value=send_5.value"></p>
<input type="text" id="theta3" value="0" oninput="send_5.value=theta3.value">
<br>
<p>Theta4:<input type="range" id="send_6" min="0" max="180" value="0" oninput="theta4.value=send_6.value"></p>
<input type="text" id="theta4" value="0" oninput="send_6.value=theta4.value">
<br>
<p>GrabRight:<input type="range" id="send_7" min="0" max="1" value="0" oninput="grabright.value=send_7.value"></p>
<input type="text" id="grabright" value="0" oninput="send_7.value=grabright.value">
</form>
<p><button onClick="myFunction()">Submit</a></p>
<script>document.body.addEventListener("keydown", (ev) => {
let url_new = "";
const result_value = document.getElementById("result").value;
if(ev.key == 'x' ||ev.key == 'X'){
url_new="/initial_/on";
}
else if(ev.key == ' '){
url_new="/stand___/on";
}
else if(ev.key == 'w'||ev.key == 'W'){
url_new="/forward_/on";
}
else if(ev.key == 's'||ev.key == 'S'){
url_new="/backward/on";
}
else if(ev.key == 'a'||ev.key == 'A'){
url_new="/left____/on";
}
else if(ev.key == 'd'||ev.key == 'D'){
url_new="/right___/on";
}
else if(ev.key == 'q'||ev.key == 'Q'){
url_new="/leftturn/on";
}
else if(ev.key == 'e'||ev.key == 'E'){
url_new="/rigtturn/on";
}
else {
url_new="";
}
console.log(url_new);
if(url_new != ""){
console.log(result_value);
url_new=url_new+"/?times="+result_value;
window.location.href = url_new;
}})
function myFunction(){
    let url_new = "";
    const theta1 = document.getElementById("theta1").value;
    const theta2 = document.getElementById("theta2").value;
    const grableft = document.getElementById("grableft").value;
    const theta3 = document.getElementById("theta3").value;
    const theta4 = document.getElementById("theta4").value;
    const grabright = document.getElementById("grabright").value;
    url_new=url_new+"/grabhand/?theta1="+theta1+"&theta2="+theta2+"&theta3="+theta3+"&theta4="+theta4+"&grableft="+grableft+"&grabright="+grabright;
    console.log(url_new);
    //return url_new;
    window.location.href = url_new;
  }
</script>
<p><a href="/initial_/on"><button class="button initial">X</button></a></p>
<p><a href="/stand___/on"><button class="button stand">Spacebar</button></a></p>
<p><a href="/forward_/on/?times=1"><button class="button forward">W</button></a></p>
<p><a href="/backward/on/?times=1"><button class="button backward">S</button></a></p>
<p><a href="/left____/on/?times=1"><button class="button left">A</button></a></p>
<p><a href="/right___/on/?times=1"><button class="button right">D</button></a></p>
<p><a href="/leftturn/on/?times=1"><button class="button leftturn">Q</button></a></p>
<p><a href="/rigtturn/on/?times=1"><button class="button rightturn">E</button></a></p>
</body></html>


)=====";


unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 20000;
// 更改tf k
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
  String url = request->url();
  Serial.println(url);
}
void setup() {
    

  pwm.begin();
  pwm.setPWMFreq(50);
  pwm.setOscillatorFrequency(27000000);
  pwm1.begin();
  pwm1.setPWMFreq(50);
  pwm1.setOscillatorFrequency(27000000);
  Serial.begin(115200);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", [](AsyncWebServerRequest * request)
  { 
    request->send_P(200, "text/html", webpage);
    Serial.println("Welcome");
  });
  server.on("/initial_/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    request->send_P(200, "text/html", webpage);
    Serial.println("INITIAL ON");
    InitialState = "on";
    StandState = "off";
    ForwardState = "off";
    BackwardState = "off";
    LeftState = "off";
    RightState = "off";
    LeftTurnState = "off";
    RightTurnState = "off";
    header="INITIAL% ON ";
    initial();
  });
  server.on("/stand___/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    request->send_P(200, "text/html", webpage);
    Serial.println("STAND on");
    InitialState = "off";
    StandState = "on";
    ForwardState = "off";
    BackwardState = "off";
    LeftState = "off";
    RightState = "off";
    LeftTurnState = "off";
    RightTurnState = "off";
    header="STAND%%% ON ";
    stand();
  });
  server.on("/forward_/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    request->send_P(200, "text/html", webpage);
    Serial.println("FORWARD ON");
    InitialState = "off";
    StandState = "off";
    ForwardState = "on";
    BackwardState = "off";
    LeftState = "off";
    RightState = "off";
    LeftTurnState = "off";
    RightTurnState = "off";
    if (request->hasParam("times")) {
      String times_str = request->getParam("times")->value();      
      header="FORWARD% ON "+times_str;
      Serial.println(header);
    }
  });
  server.on("/backward/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    request->send_P(200, "text/html", webpage);
    Serial.println("BACKWARD ON");
    InitialState = "off";
    StandState = "off";
    ForwardState = "off";
    BackwardState = "on";
    LeftState = "off";
    RightState = "off";
    LeftTurnState = "off";
    RightTurnState = "off";
    if (request->hasParam("times")) {
      String times_str = request->getParam("times")->value();      
      header="BACKWARD ON "+times_str;
      Serial.println(header);
    }
  });
  server.on("/left____/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    request->send_P(200, "text/html", webpage);
    Serial.println("LEFT ON");
    InitialState = "off";
    StandState = "off";
    ForwardState = "off";
    BackwardState = "off";
    LeftState = "on";
    RightState = "off";
    LeftTurnState = "off";
    RightTurnState = "off";
    if (request->hasParam("times")) {
      String times_str = request->getParam("times")->value();      
      header="LEFT%%%% ON "+times_str;
      Serial.println(header);
    }
  });
  server.on("/right___/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    request->send_P(200, "text/html", webpage);
    Serial.println("RIGHT ON");
    InitialState = "off";
    StandState = "off";
    ForwardState = "off";
    BackwardState = "off";
    LeftState = "off";
    RightState = "on";
    LeftTurnState = "off";
    RightTurnState = "off";
    if (request->hasParam("times")) {
      String times_str = request->getParam("times")->value();      
      header="RIGHT%%% ON "+times_str;
      Serial.println(header);
    }
  });
  server.on("/leftturn/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    request->send_P(200, "text/html", webpage);
    Serial.println("LEFTTURN ON");
    InitialState = "off";
    StandState = "off";
    ForwardState = "off";
    BackwardState = "off";
    LeftState = "off";
    RightState = "off";
    LeftTurnState = "on";
    RightTurnState = "off";
    if (request->hasParam("times")) {
      String times_str = request->getParam("times")->value();      
      header="LEFTTURN ON "+times_str;
      Serial.println(header);
    }
  });
  server.on("/rigtturn/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    request->send_P(200, "text/html", webpage);
    Serial.println("RIGHTTURN ON");
    InitialState = "off";
    StandState = "off";
    ForwardState = "off";
    BackwardState = "off";
    LeftState = "off";
    RightState = "off";
    LeftTurnState = "off";
    RightTurnState = "on";
    if (request->hasParam("times")) {
      String times_str = request->getParam("times")->value();      
      header="RIGTTURN ON "+times_str;
      Serial.println(header);
    }
  });
  server.on("/grabhand/", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
    request->send_P(200, "text/html", webpage);
    if (request->hasParam("theta1")) {
      String theta1_str = request->getParam("theta1")->value();      
      theta1=theta1_str.toInt();
      Serial.println(theta1);
    }
    if (request->hasParam("theta2")) {
      String theta2_str = request->getParam("theta2")->value();      
      theta2=theta2_str.toInt();
    }
    if (request->hasParam("theta3")) {
      String theta3_str = request->getParam("theta3")->value();      
      theta3=theta3_str.toInt();
    }
    if (request->hasParam("theta4")) {
      String theta4_str = request->getParam("theta4")->value();      
      theta4=theta4_str.toInt();
    }
    if (request->hasParam("grableft")) {
      String leftgrab_str = request->getParam("grableft")->value();      
      leftgrab=leftgrab_str.toInt();
      Serial.println(leftgrab);
    }
    if (request->hasParam("grabright")) {
      String rightgrab_str = request->getParam("grabright")->value();      
      rightgrab=rightgrab_str.toInt();
    }
    hand_grab(theta1,theta2,leftgrab,theta3,theta4,rightgrab);
  });
  server.onNotFound(notFound);
  
  server.begin();
  
}


void loop() {
  // put your main code here, to run repeatedly:
  if (header.indexOf("FORWARD% ON")>=0){
    int times = int(header[12])-int('0');
    forward(times);
    Serial.println(header);
  }
  if (header.indexOf("BACKWARD ON")>=0){
    int times = int(header[12])-int('0');
    backward(times);
    Serial.println(header);
  }
  if (header.indexOf("LEFT%%%% ON")>=0){
    int times = int(header[12])-int('0');
    goleft(times);
    Serial.println(header);
  }
  if (header.indexOf("RIGHT%%% ON")>=0){
    int times = int(header[12])-int('0');
    goright(times);
    Serial.println(header);
  }
  if (header.indexOf("LEFTTURN ON")>=0){
    int times = int(header[12])-int('0');
    turnleft(times);
    Serial.println(header);
  }
  if (header.indexOf("RIGTTURN ON")>=0){
    int times = int(header[12])-int('0');
    turnright(times);
    Serial.println(header);
  }
  
}
int angle2pulse(int angle){
  int pulse ;
  if (angle <= 180){
    pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  }
  return pulse;
}

double cubicsp(double _thetai, double _thetaf,double _tf,double t){
      //a0 = 70;a1 = 0;a2 = 30 ;a3 = -10;
      //double thetai=45; double thetaf=135;
      double _theta0;
      a0 = _thetai;
      a1 = 0;
      a2 = 3/(pow(_tf,2))*(_thetaf - _thetai);
      a3 = -2/(pow(_tf,3))*(_thetaf - _thetai);
      _theta0 = a0 + a1*(t) +a2*(pow(t,2)) +a3*(pow(t,3));
    
      return _theta0; 
  }

void tryit(){
  pwm.setPWM(2,0,angle2pulse(60));   
  pwm.setPWM(8,0,angle2pulse(60)); 
  pwm.setPWM(14,0,angle2pulse(60)); 
  pwm.setPWM(5,0,angle2pulse(120));   
  pwm.setPWM(11,0,angle2pulse(120)); 
  pwm.setPWM(17,0,angle2pulse(120));
}
void stand(){

  pwm.setPWM(0,0,angle2pulse(90));
  pwm.setPWM(1,0,angle2pulse(75));
  pwm.setPWM(2,0,angle2pulse(0));

  pwm.setPWM(6,0,angle2pulse(90));
  pwm.setPWM(7,0,angle2pulse(75));
  pwm.setPWM(8,0,angle2pulse(0));

  pwm.setPWM(12,0,angle2pulse(90));
  pwm.setPWM(13,0,angle2pulse(75));
  pwm.setPWM(14,0,angle2pulse(0));

  pwm.setPWM(3,0,angle2pulse(90));
  pwm.setPWM(4,0,angle2pulse(75));
  pwm.setPWM(5,0,angle2pulse(0));

  pwm.setPWM(9,0,angle2pulse(90));
  pwm.setPWM(10,0,angle2pulse(75));
  pwm.setPWM(11,0,angle2pulse(0));

  pwm.setPWM(15,0,angle2pulse(90));
  pwm1.setPWM(0,0,angle2pulse(75));
  pwm1.setPWM(1,0,angle2pulse(0));

}

void backward(int times) {
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0)); 

  delay(1000);     
  double timestep = 0.01; //1
  double tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,90-deltatheta,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,45,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,90+deltatheta,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  for(int j=0;j<times;j++){
  timestep = 0.01; //2
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90-deltatheta,90-deltatheta,tf,t); //小腿1,3,5
    theta2 =cubicsp(45,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90+deltatheta,90+deltatheta,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //3
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90-deltatheta,90+deltatheta,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90+deltatheta,90-deltatheta,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,45,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //4
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90+deltatheta,90+deltatheta,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90-deltatheta,90-deltatheta,tf,t); //小腿2,4,6
    theta4 =cubicsp(45,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //5
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90+deltatheta,90-deltatheta,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,45,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90-deltatheta,90+deltatheta,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  }
  
}

void forward(int times) {
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0)); 

  delay(1000);   
  double timestep = 0.01; //1
  double tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,90-deltatheta,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,90+deltatheta,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,45,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  for(int j=0;j<times;j++){
  timestep = 0.01; //2
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90-deltatheta,90-deltatheta,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90+deltatheta,90+deltatheta,tf,t); //小腿2,4,6
    theta4 =cubicsp(45,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position

  }

  timestep = 0.01; //3
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90-deltatheta,90+deltatheta,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,45,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90+deltatheta,90-deltatheta,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //4
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90+deltatheta,90+deltatheta,tf,t); //小腿1,3,5
    theta2 =cubicsp(45,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90-deltatheta,90-deltatheta,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //5
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90+deltatheta,90-deltatheta,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90-deltatheta,90+deltatheta,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,45,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(15,0,angle2pulse(theta1)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  }
  
}

void initial(){
  pwm.setPWM(0,0,angle2pulse(90));
  pwm.setPWM(1,0,angle2pulse(135));
  pwm.setPWM(2,0,angle2pulse(0));

  pwm.setPWM(6,0,angle2pulse(90));
  pwm.setPWM(7,0,angle2pulse(135));
  pwm.setPWM(8,0,angle2pulse(0));

  pwm.setPWM(12,0,angle2pulse(90));
  pwm.setPWM(13,0,angle2pulse(135));
  pwm.setPWM(14,0,angle2pulse(0));

  pwm.setPWM(3,0,angle2pulse(90));
  pwm.setPWM(4,0,angle2pulse(135));
  pwm.setPWM(5,0,angle2pulse(0));

  pwm.setPWM(9,0,angle2pulse(90));
  pwm.setPWM(10,0,angle2pulse(135));
  pwm.setPWM(11,0,angle2pulse(0));

  pwm.setPWM(15,0,angle2pulse(90));
  pwm1.setPWM(0,0,angle2pulse(135));
  pwm1.setPWM(1,0,angle2pulse(0));

}

void turnleft(int times) {
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0)); 

  delay(1000);     
  double timestep = 0.01; //1
  double tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,45,tf,t);// 大腿2,4,6

    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  for(int j=0;j<times;j++){
  timestep = 0.01; //2
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(120,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(45,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //3
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(120,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,45,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //4
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(45,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(120,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //5
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(120,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,45,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  }
  
}

void turnright(int times) {
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0)); 

  delay(1000);     
  double timestep = 0.01; //1
  double tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,90+deltatheta,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,45,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,90-deltatheta,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6

    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  for(int j=0;j<times;j++){
  timestep = 0.01; //2
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(120,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(45,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //3
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(120,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,45,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //4
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,60,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,90,tf,t);// 大腿1,3,5
    theta3 = cubicsp(120,120,tf,t); //小腿2,4,6
    theta4 =cubicsp(45,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //5
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,120,tf,t); //小腿1,3,5
    theta2 =cubicsp(90,45,tf,t);// 大腿1,3,5
    theta3 = cubicsp(120,60,tf,t); //小腿2,4,6
    theta4 =cubicsp(90,90,tf,t);// 大腿2,4,6
    //末腿1,3,5
    pwm.setPWM(0,0,angle2pulse(theta1));   
    pwm.setPWM(6,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta1)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta2));   
    pwm.setPWM(7,0,angle2pulse(theta2)); 
    pwm.setPWM(13,0,angle2pulse(theta2)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(8,0,angle2pulse(0)); 
    pwm.setPWM(14,0,angle2pulse(0)); 
    //末腿2,4,6
    pwm.setPWM(3,0,angle2pulse(theta3));   
    pwm.setPWM(9,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta3)); 
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta4));   
    pwm.setPWM(10,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(0));   
    pwm.setPWM(11,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0));  

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  }
  
}
void goright(int times) {
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(30));   
  pwm.setPWM(8,0,angle2pulse(30)); 
  pwm.setPWM(14,0,angle2pulse(30)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(30));   
  pwm.setPWM(11,0,angle2pulse(30)); 
  pwm1.setPWM(1,0,angle2pulse(30)); 

  delay(1000);
  for(int j=0;j<times;j++){     
  double timestep = 0.01; //1
  double tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,90,tf,t); //小腿1,3,5
    theta4 =cubicsp(30,30,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,90,tf,t); //小腿1,3,5
    theta2 =cubicsp(30,30,tf,t);// 大腿1,3,5
    theta5 = cubicsp(90,45,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

    pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
 
  timestep = 0.01; //2
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,100,tf,t); //小腿1,3,5
    theta4 =cubicsp(30,0,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,100,tf,t); //小腿1,3,5
    theta2 =cubicsp(30,60,tf,t);// 大腿1,3,5
    theta5 = cubicsp(45,45,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);             // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //3
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(100,100,tf,t); //小腿1,3,5
    theta4 =cubicsp(0,0,tf,t);// 大腿1,3,5
    theta3 = cubicsp(100,100,tf,t); //小腿1,3,5
    theta2 =cubicsp(60,60,tf,t);// 大腿1,3,5
    theta5 = cubicsp(45,90,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                 // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //4
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(100,45,tf,t); //小腿1,3,5
    theta4 =cubicsp(0,0,tf,t);// 大腿1,3,5
    theta3 = cubicsp(100,45,tf,t); //小腿1,3,5
    theta2 =cubicsp(60,60,tf,t);// 大腿1,3,5
    theta5 = cubicsp(90,90,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                  // waits 15 ms for the servo to reach the position
  }
  timestep = 0.01; //4
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,90,tf,t); //小腿1,3,5
    theta4 =cubicsp(0,30,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,90,tf,t); //小腿1,3,5
    theta2 =cubicsp(60,0,tf,t);// 大腿1,3,5
    theta5 = cubicsp(90,90,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                  // waits 15 ms for the servo to reach the position
  }
  }
  
}
void goleft(int times) {
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(90));   
  pwm.setPWM(7,0,angle2pulse(90)); 
  pwm.setPWM(13,0,angle2pulse(90)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(30));   
  pwm.setPWM(8,0,angle2pulse(30)); 
  pwm.setPWM(14,0,angle2pulse(30)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(90));   
  pwm.setPWM(10,0,angle2pulse(90)); 
  pwm1.setPWM(0,0,angle2pulse(90)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(30));   
  pwm.setPWM(11,0,angle2pulse(30)); 
  pwm1.setPWM(1,0,angle2pulse(30)); 

  delay(1000);
  for(int j=0;j<times;j++){     
  double timestep = 0.01; //1
  double tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,90,tf,t); //小腿1,3,5
    theta2 =cubicsp(30,30,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,90,tf,t); //小腿1,3,5
    theta4 =cubicsp(30,30,tf,t);// 大腿1,3,5
    theta5 = cubicsp(90,45,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

    pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
 
  timestep = 0.01; //2
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(90,100,tf,t); //小腿1,3,5
    theta2 =cubicsp(30,0,tf,t);// 大腿1,3,5
    theta3 = cubicsp(90,100,tf,t); //小腿1,3,5
    theta4 =cubicsp(30,60,tf,t);// 大腿1,3,5
    theta5 = cubicsp(45,45,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);             // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //3
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(100,100,tf,t); //小腿1,3,5
    theta2 =cubicsp(0,0,tf,t);// 大腿1,3,5
    theta3 = cubicsp(100,100,tf,t); //小腿1,3,5
    theta4 =cubicsp(60,60,tf,t);// 大腿1,3,5
    theta5 = cubicsp(45,90,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                 // waits 15 ms for the servo to reach the position
  }

  timestep = 0.01; //4
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(100,45,tf,t); //小腿1,3,5
    theta2 =cubicsp(0,0,tf,t);// 大腿1,3,5
    theta3 = cubicsp(100,45,tf,t); //小腿1,3,5
    theta4 =cubicsp(60,60,tf,t);// 大腿1,3,5
    theta5 = cubicsp(90,90,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                  // waits 15 ms for the servo to reach the position
  }
  timestep = 0.01; //4
  tf = 0.4;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = cubicsp(60,90,tf,t); //小腿1,3,5
    theta2 =cubicsp(0,30,tf,t);// 大腿1,3,5
    theta3 = cubicsp(60,90,tf,t); //小腿1,3,5
    theta4 =cubicsp(60,0,tf,t);// 大腿1,3,5
    theta5 = cubicsp(90,90,tf,t); //小腿2,4,6
    theta6 =cubicsp(0,0,tf,t);// 大腿2,4,6

     pwm.setPWM(0,0,angle2pulse(90));   
    pwm.setPWM(6,0,angle2pulse(90)); 
    pwm.setPWM(12,0,angle2pulse(90)); 
    pwm.setPWM(3,0,angle2pulse(90));   
    pwm.setPWM(9,0,angle2pulse(90)); 
    pwm.setPWM(15,0,angle2pulse(90));
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta1));   
    pwm.setPWM(7,0,angle2pulse(theta1)); 
    pwm.setPWM(13,0,angle2pulse(theta3)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(theta2));   
    pwm.setPWM(8,0,angle2pulse(theta2)); 
    pwm.setPWM(14,0,angle2pulse(theta4)); 
     
    //小腿2,4,6
    pwm.setPWM(4,0,angle2pulse(theta5));   
    pwm.setPWM(10,0,angle2pulse(theta5)); 
    pwm1.setPWM(0,0,angle2pulse(theta5)); 
    //大腿2,4,6
    pwm.setPWM(5,0,angle2pulse(theta6));   
    pwm.setPWM(11,0,angle2pulse(theta6)); 
    pwm1.setPWM(1,0,angle2pulse(theta6)); 

    delay(timestep*1000);                  // waits 15 ms for the servo to reach the position
  }
  }
  
}
void niupipi(){
  pwm.setPWM(0,0,angle2pulse(90));   
  pwm.setPWM(6,0,angle2pulse(90)); 
  pwm.setPWM(12,0,angle2pulse(90)); 
  //小腿1,3,5
  pwm.setPWM(1,0,angle2pulse(75));   
  pwm.setPWM(7,0,angle2pulse(75)); 
  pwm.setPWM(13,0,angle2pulse(75)); 
  //大腿1,3,5
  pwm.setPWM(2,0,angle2pulse(0));   
  pwm.setPWM(8,0,angle2pulse(0)); 
  pwm.setPWM(14,0,angle2pulse(0)); 
  //末腿2,4,6
  pwm.setPWM(3,0,angle2pulse(90));   
  pwm.setPWM(9,0,angle2pulse(90)); 
  pwm.setPWM(15,0,angle2pulse(90)); 
  //小腿2,4,6
  pwm.setPWM(4,0,angle2pulse(75));   
  pwm.setPWM(10,0,angle2pulse(75)); 
  pwm1.setPWM(0,0,angle2pulse(75)); 
  //大腿2,4,6
  pwm.setPWM(5,0,angle2pulse(0));   
  pwm.setPWM(11,0,angle2pulse(0)); 
  pwm1.setPWM(1,0,angle2pulse(0)); 

  delay(1000); 
  for(int j=0;j<10;j++){    
  double timestep = 0.01; //1
  double tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = linearsp(90,105,tf,t); //小腿1,3,5
    theta2 = linearsp(90,75,tf,t); //小腿2,4,6
    theta3=linearsp(90,90,tf,t);
    theta4=linearsp(75,90,tf,t);

    pwm.setPWM(0,0,angle2pulse(theta1));
    pwm.setPWM(3,0,angle2pulse(theta3));    
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta2)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta4));  
    pwm.setPWM(4,0,angle2pulse(theta4));  
    pwm.setPWM(7,0,angle2pulse(theta4));
    pwm.setPWM(10,0,angle2pulse(theta4));  
    pwm.setPWM(13,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(5,0,angle2pulse(0)); 
    pwm.setPWM(8,0,angle2pulse(0));
    pwm.setPWM(11,0,angle2pulse(0));  
    pwm.setPWM(14,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 
    
    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  timestep = 0.01; //1
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = linearsp(105,90,tf,t); //小腿1,3,5
    theta2 = linearsp(75,90,tf,t); //小腿2,4,6
    theta3 =linearsp(90,90,tf,t);
    theta4 =linearsp(90,75,tf,t); //小腿2,4,6

    pwm.setPWM(0,0,angle2pulse(theta1));
    pwm.setPWM(3,0,angle2pulse(theta3));    
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta2)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta4));  
    pwm.setPWM(4,0,angle2pulse(theta4));  
    pwm.setPWM(7,0,angle2pulse(theta4));
    pwm.setPWM(10,0,angle2pulse(theta4));  
    pwm.setPWM(13,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(5,0,angle2pulse(0)); 
    pwm.setPWM(8,0,angle2pulse(0));
    pwm.setPWM(11,0,angle2pulse(0));  
    pwm.setPWM(14,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 
    
    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  timestep = 0.01; //1
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = linearsp(90,75,tf,t); //小腿1,3,5
    theta2 = linearsp(90,105,tf,t); //小腿2,4,6
    theta3 =linearsp(90,90,tf,t);
    theta4 =linearsp(75,90,tf,t); //小腿2,4,6

    pwm.setPWM(0,0,angle2pulse(theta1));
    pwm.setPWM(3,0,angle2pulse(theta3));    
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta2)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta4));  
    pwm.setPWM(4,0,angle2pulse(theta4));  
    pwm.setPWM(7,0,angle2pulse(theta4));
    pwm.setPWM(10,0,angle2pulse(theta4));  
    pwm.setPWM(13,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(5,0,angle2pulse(0)); 
    pwm.setPWM(8,0,angle2pulse(0));
    pwm.setPWM(11,0,angle2pulse(0));  
    pwm.setPWM(14,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 
    
    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  timestep = 0.01; //1
  tf = 0.2;
  for (double t = 0; t <= tf; t +=timestep) { 
    theta1 = linearsp(75,90,tf,t); //小腿1,3,5
    theta2 = linearsp(105,90,tf,t); //小腿2,4,6
    theta3 =linearsp(90,90,tf,t);
    theta4 =linearsp(90,75,tf,t); //小腿2,4,6

    pwm.setPWM(0,0,angle2pulse(theta1));
    pwm.setPWM(3,0,angle2pulse(theta3));    
    pwm.setPWM(6,0,angle2pulse(theta2)); 
    pwm.setPWM(9,0,angle2pulse(theta1)); 
    pwm.setPWM(12,0,angle2pulse(theta3)); 
    pwm.setPWM(15,0,angle2pulse(theta2)); 
    //小腿1,3,5
    pwm.setPWM(1,0,angle2pulse(theta4));  
    pwm.setPWM(4,0,angle2pulse(theta4));  
    pwm.setPWM(7,0,angle2pulse(theta4));
    pwm.setPWM(10,0,angle2pulse(theta4));  
    pwm.setPWM(13,0,angle2pulse(theta4)); 
    pwm1.setPWM(0,0,angle2pulse(theta4)); 
    //大腿1,3,5
    pwm.setPWM(2,0,angle2pulse(0));   
    pwm.setPWM(5,0,angle2pulse(0)); 
    pwm.setPWM(8,0,angle2pulse(0));
    pwm.setPWM(11,0,angle2pulse(0));  
    pwm.setPWM(14,0,angle2pulse(0)); 
    pwm1.setPWM(1,0,angle2pulse(0)); 
    
    delay(timestep*1000);                      // waits 15 ms for the servo to reach the position
  }
  }
}

void hand_grab(unsigned short theta1,unsigned short theta2,bool grab_left, unsigned short theta3, unsigned short theta4,bool grab_right){
  pwm1.setPWM(2,0,angle2pulse(theta1)); 
  Serial.println(theta1);
  pwm1.setPWM(3,0,angle2pulse(theta2)); 
  pwm1.setPWM(4,0,angle2pulse(int(grab_left)*90)); 
  pwm1.setPWM(2,0,angle2pulse(theta1)); 
  pwm1.setPWM(3,0,angle2pulse(180-theta2)); 
  pwm1.setPWM(4,0,angle2pulse(int(grab_left)*90)); 
}
double linearsp(double _thetai, double _thetaf,double _tf,double t){ 
      double theta;
      theta=_thetai+(_thetaf-_thetai)*t/_tf;
      return _thetaf; 
  }
