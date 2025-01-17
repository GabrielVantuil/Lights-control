
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


void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}
