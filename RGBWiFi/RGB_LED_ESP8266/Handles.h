#include <FS.h>

// ------------------------------------------------------------------------------------------ DEFINES



// ------------------------------------------------------------------------------------------ FUNCTIONS
String getContentType(String filename)              // convert the file extension to the MIME type
{ 
  if (filename.endsWith(".html")) return "text/html";
  // TODO: add compressed formats as well. readmore: https://tttapa.github.io/ESP8266/Chap11%20-%20SPIFFS.html
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String _path, bool _hasArgs)      // send the right file to the client (if it exists)
{
  if(lastCommandEpochTick + RETAIN_COMMAND_FOR > timeClient.getEpochTime())
    isCommandValid = true;
  else
    isCommandValid = false;
    
  #ifdef VERBOSE_MODE
    Serial.println("handleFileRead: " + _path);
  #endif
  
  if (_path.endsWith("/"))
  {
    _path += "index.html";                            // If a folder is requested, send the index file
  }
  
  // this redirects us to the parametered index page if the data is valid and there are no args already
  if(isCommandValid && !_hasArgs && _path.endsWith("index.html"))
  {
    String url = "/index.html?";
    for(uint8_t index = 0; index < NUMBER_OF_STRIPS; index++)
    {
      url += strips[index].getConfigAsURL();
      if( (index + 1) < NUMBER_OF_STRIPS)
        url += "&";
    }
    
    server.sendHeader("Location", url, true);
    server.send ( 302, "text/plain", "");

    #ifdef VERBOSE_MODE
      Serial.println("Redirected to parametered index.html");
    #endif
    
    return true;
  }
  
  String contentType = getContentType(_path);            // Get the MIME type
  if (SPIFFS.exists(_path))                              // If the file exists
  {                            
    File file = SPIFFS.open(_path, "r");                 // Open it
    #ifdef VERBOSE_MODE
      Serial.print("File ");
      Serial.print(_path);
      Serial.println(" opened");
    #endif
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    #ifdef VERBOSE_MODE
      Serial.println("File closed");
    #endif
    return true;
  }
  
  #ifdef VERBOSE_MODE
    Serial.println("\tFile Not Found");
  #endif
  
  return false;                                         // If the file doesn't exist, return false
}

// ------------------------------------------------------------------------------------------ HANDLES
void handleCommand() {
  // save last time data was sent to ESP
  timeClient.update();
  EEPROMWrite_uint32_t(LAST_COMMAND_EPOCH_ADDRESS,timeClient.getEpochTime());
  
  for (uint8_t index = 0; index < server.args(); index++)
  {
    //strip number is coded in the offset 9 / 10 == 0              ||| setConfigTo(config number as defined, config value)
    strips[server.argName(index).toInt() / URL_OFFSET_TO_NEXT_STRIP].setConfigTo(server.argName(index).toInt() % URL_OFFSET_TO_NEXT_STRIP,server.arg(index).toInt());
  }

  for(uint8_t index = 0; index < NUMBER_OF_STRIPS; index++)
  {
    strips[index].saveConfigToEEPROM();
  }
  
  String message = "<html><head></head><body style='font-family: sans-serif; font-size: 12px' onload=\"onLoadFunction()\">";

  message += "Redirecting to previous page...";
  // this is a html code that auto executes on load of the page and calls another function delayed by  V this value, that redirects us to the previous page.
  message += "<script>function onLoadFunction(){setTimeout(function(){ window.location.href=\"/\";}, 100); } </script>";
  
  message += "</body></html>";
  server.send(200, "text/html", message);
}

void handleNotFound() {
  if (!handleFileRead(server.uri(), server.args()?true:false))
  {
    #ifdef VERBOSE_MODE
      Serial.println("handleFileRead returned false");
    #endif
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
    #ifdef VERBOSE_MODE
      Serial.println("Sent debug message to HTTP client");
    #endif
  }
}
