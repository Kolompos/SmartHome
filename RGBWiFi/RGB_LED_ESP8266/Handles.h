#include <FS.h>

// ------------------------------------------------------------------------------------------ FUNCTIONS
String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  // TODO: add compressed formats as well. readmore: https://tttapa.github.io/ESP8266/Chap11%20-%20SPIFFS.html
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path, bool hasArgs)                        // send the right file to the client (if it exists)
{
  if(lastCommandEpochTick + RETAIN_COMMAND_FOR > timeClient.getEpochTime())
    isCommandValid = true;
  else
    isCommandValid = false;
    
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/"))
  {
    path += "index.html";                               // If a folder is requested, send the index file
  }
  
  // this redirects us to the parametered index page if the data is valid and there are no args already
  if(isCommandValid && !hasArgs && path.endsWith("index.html"))
  {
    // TODO: this will be unbearable later on, do something with this... make a getUrlCode() function or someting
    String url = "/index.html?LR=";
    url += stripLogo.getColor(R);
    url += "&LG=";
    url += stripLogo.getColor(G);
    url += "&LB=";
    url += stripLogo.getColor(B);
    url += "&LE=";
    url += stripLogo.getEffect();
    
    server.sendHeader("Location", url, true);
    server.send ( 302, "text/plain", "");
    return true;
  }
  
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path))                              // If the file exists
  {                            
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
  // save last time data was sent to ESP
  timeClient.update();
  EEPROMWrite_uint32_t(LAST_COMMAND_EPOCH_ADDRESS,timeClient.getEpochTime());

  // TODO: make a function in strip class that gets the args and sets itself
  // TODO: also make an API or at least make a little bit fool proof the command syntax and parameter order
  stripLogo.setColor(server.arg(0).toInt(),server.arg(1).toInt(),server.arg(2).toInt());
  stripLogo.setEffect(server.arg(3).toInt());
  
  EEPROM.commit();
  
  String message = "<html><head></head><body style='font-family: sans-serif; font-size: 12px' onload=\"onLoadFunction()\">";

  message += "Redirecting to previous page in 2 sec.";
  // this is a html code that auto executes on load of the page and calls another function delayed by  V this value, that redirects us to the previous page.
  message += "<script>function onLoadFunction(){setTimeout(function(){ window.location.href=\"/\";}, 2000); } </script>";
  
  message += "</body></html>";
  server.send(200, "text/html", message);
}

void handleNotFound() {
  if (!handleFileRead(server.uri(), server.args()?true:false))
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
