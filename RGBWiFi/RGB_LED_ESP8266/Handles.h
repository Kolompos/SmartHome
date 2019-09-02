#include <FS.h>

// ------------------------------------------------------------------------------------------ FUNCTIONS
String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}

// ------------------------------------------------------------------------------------------ HANDLES
void handleCommand() {
  String message = "<html><head></head><body style='font-family: sans-serif; font-size: 12px'>Following functions are available for the logo logo:<br><br>";
  message += "<a href='/logo/rainbow?fade=3000'>/rainbow</a> a rainbow animation on the logo.<br>";
  message += "<a href='/logo/wave?r=255&g=255&b=255&fade=5000'>/wave</a> a slow wave animation on LED on base color specified by arguments: r=<0..255> g=<0..255> b=<0..255><br>";
  message += "<a href='/logo/setleds?r=255&g=255&b=255&fade=1000'>/setleds</a> sets LEDs to the color from arguments: r=<0..255> g=<0..255> b=<0..255><br>";
  message += "<a href='/logo/off?fade=500'>/off.</a> turn the logo's LEDs off.<br>";
  message += "All functions support the argument 'fade' which specifies the milliseconds it takes to fade to the new specified state.<br>";
  message += "<br>Syntax is as follows: http://&ltip>/&ltcommand>?&ltargument1>=&ltvalue1>&&ltargument2>=&ltvalue2>&...<br>";
  message += "You can click on each link to see an example.<br><br>";
  message += "Go back to <a href='/'>root</a>.<br></body></html>";
  server.send(200, "text/html", message);
}

void handleNotFound() {
  if (!handleFileRead(server.uri()))
  {
    String message = "Command Not Found\n\n";
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
  }
}
