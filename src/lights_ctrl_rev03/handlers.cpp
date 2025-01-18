#include "handlers.h"

void digitalOut() {
  String arguments = "Comando(s) aceito(s): \n";
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "change") {
      digitalWrite(server.arg(i).toInt(), !digitalRead(server.arg(i).toInt()));
      arguments += String(i) + ") Status da porta " + server.arg(i).toInt() + " alterado para: " + ((digitalRead(server.arg(i).toInt()) == 0) ? "LOW" : "HIGH") + "\n";
    }
    else {
      arguments += String(i) + ") Enviando para porta " + server.argName(i) + ": " + ((server.arg(i).toInt() == 0) ? "LOW" : "HIGH") + "\n";
      digitalWrite(server.argName(i).toInt(), server.arg(i).toInt() == 0 ? LOW : HIGH);
    }
  }
  Serial.println(arguments);

  server.send(404, "text/plain", arguments);
}
void analogOutX(uint8_t out) {
  uint8_t pwmVal = 255;
  if (server.args() > 0) {
    pwmVal = server.arg(0).toInt();
    if (pwmVal == PWM_out[out]){
      out_d[out] = !out_d[out];
    }
    PWM_out[out] = pwmVal;
  }
  else {
    PWM_out[out] = 255;
    out_d[out] = !out_d[out];
  }
  toggleNpWhite(pwmVal);
}
void RGB_handle(uint8_t rgb) {
  if (server.args() > 0) {
    uint8_t arg = server.arg(0).toInt();
    RGBW[rgb] = (RGBW[rgb] != arg) * arg;
  }
  else    RGBW[rgb] = (RGBW[rgb] != 255) * 255;
  npAllPixels();
}

void d_Out1() {
  analogOutX(0);
  server.send(200, "text/plain", "Command accepted");
}
void d_Out2() {
  analogOutX(1);
  server.send(200, "text/plain", "Command accepted");
}
void d_Out3() {
  analogOutX(2);
  server.send(200, "text/plain", "Command accepted");
}
void d_Out4() {
  analogOutX(3);
  server.send(200, "text/plain", "Command accepted");
}

void np_R() {
  RGB_handle(0);
  server.send(200, "text/plain", "Command accepted");
}
void np_G() {
  RGB_handle(1);
  server.send(200, "text/plain", "Command accepted");
}
void np_B() {
  RGB_handle(2);
  server.send(200, "text/plain", "Command accepted");
}


void clearLeds() {
  for (uint8_t i = 0; i < 4; i++) {
    RGBW[i] = 0;
    bt_prev[i] = false;
    is_dimming[i] = false;
    bt_is_pressed[i] = false;
    out_d[i] = false;
    PWM_out[i] = 255;
    ch_presets[i] = presets_last_index;
  }
  ledQueue = "";
  shiftQueue = "";
  loopQueue = "";
  timesToShift = 0;
  isShift = false;
  pixels->clear();
  pixels->show();

  server.send(200, "text/plain", "Command accepted");
}
void genericOut_handler() {
  for (uint8_t a = 0; a < server.args(); a++) {
    for (uint8_t out = 0; out < 4; out++) {
      if (server.argName(a).toInt() == out) {
        analogWrite(OUT[out], server.arg(a).toInt());
      }
    }
  }
}
String strMac() {
  unsigned char mac[6];
  WiFi.macAddress(mac);
  String result = "";
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  result.toUpperCase();
  return result;
}

void handleRoot() {
  String page = "";
  page += "<!DOCTYPE html>";
  page += "<html>";
  page += "<link id='favicon' rel='shortcut icon' href='./favicon.png'>";
  page += "<body style='margin: 0;'>";
  page += "<p id='demo'>Failed to reach page</p>";
  page += "<script>";
  page += "let url = 'https://gabrielvantuil.github.io/projects/lightsControl/';";
  page += "const scriptPromise = new Promise((resolve, reject) => {";
  page += "  const script = document.createElement('script');";
  page += "  document.body.appendChild(script);";
  page += "  script.onload = resolve;";
  page += "  script.onerror = reject;";
  page += "  script.async = true;";
  page += "  script.src = url+'/script.js';";
  page += "});";
  page += "var link = document.getElementById('favicon');";
  page += "link.href = url+'/favicon.png';";
  page += "fetch (url)";
  page += ".then(x => x.text())";
  page += ".then(y => document.body.innerHTML = y);";
  page += "</script>";
  page += "</body>";
  page += "</html>";
  server.send(200, "text/html", page);
}
void info() {
  String IP_ADDR = WiFi.localIP().toString();
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int day =  hr / 24;
  char clockStr[30];
  sprintf(clockStr, "%d:%02d:%02d:%02d", day,  hr % 24,  min % 60, sec % 60);


  String temp = "<html>";
  temp += "<head>";
  temp += "<title>ESP8266 Demo</title>";
  temp += "<style>";
  temp += "body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }";
  temp += "</style>";
  temp += "</head>";
  temp += "<body>";
  temp += "<p>Uptime: ";
  temp += String(clockStr);//String(day) + ":" + String(hr % 24) + ":" + String(min % 60) + ":" + String(sec % 60);
  temp += "</p><br>";
  temp += "<p>IP address: " + IP_ADDR + " </p>";
  temp += "<p>MAC address: " + MAC_ADDR + "</p>";
  temp += "</body>";
  temp += "</html>";

  server.send(200, "text/html", temp);
}

void handleNotFound() {
  digitalWrite(AUXILIAR_LED, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(AUXILIAR_LED, 0);
}

void ping() {
  server.send(404, "text/plain", "pong");
}

void help() {

  char temp[1500];

  snprintf(temp, 1500,
           "<html>\
        <head>\
          <meta http-equiv='refresh' content='5'/>\
          <title>Help</title>\
          <style>\
            body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
          </style>\
        </head>\
        <body>\
          <h1>Help</h1>\
          <p>Ligar luz: <a href=\"./digitalOut?5=0\">digitalOut?5=0</a></p>\
          <p>Desligar luz: <a href=\"./digitalOut?5=1\">digitalOut?5=1</a></p>\
          <p>Ping: <a href=\"./ping\">ping</a></p>\
          <p>NeoPixel set pixel: <a href=\"./setPixels?48=0,50,0\">setPixels?48=0,50,0,0</a></p>\
          <p>NeoPixel 0-48 = vermelho: <a href=\"./setPixels?0,48=255,0,0\">setPixels?0,48=255,0,0</a></p>\
          <p>NeoPixel animacao: <a href=\"./pixelAnimacao?0,48=C:0,250,0,D:10&48,0=C:0,0,250,D:10&0,48=C:250,0,0,D:10&48,0=C:0,0,0,D:10\">pixelAnimacao?0,48=C:0,250,0,D:10</a></p>\
        </body>\
      </html>");

  server.send(200, "text/html", temp);
}
