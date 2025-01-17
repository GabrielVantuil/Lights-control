
void handleRoot() {
  String argumento = server.arg(0);
  Serial.println(argumento);
  digitalWrite(led, 1);
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 400,

           "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

           hr, min % 60, sec % 60
          );
  server.send(200, "text/html", temp);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
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
  digitalWrite(led, 0);
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
          <p>NeoPixel set pixel: <a href=\"./setpixel?48=0,50,0\">setpixel?48=0,50,0</a></p>\
          <p>NeoPixel 0-48 = vermelho: <a href=\"./setpixelRange?0,48=255,0,0\">setpixelRange?0,48=255,0,0</a></p>\
          <p>NeoPixel animacao: <a href=\"./pixelAnimacao?0,48=C:0,250,0,D:10&48,0=C:0,0,250,D:10&0,48=C:250,0,0,D:10&48,0=C:0,0,0,D:10\">pixelAnimacao?0,48=C:0,250,0,D:10</a></p>\
        </body>\
      </html>");

  server.send(200, "text/html", temp);
}
