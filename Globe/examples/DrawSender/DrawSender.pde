/**
 * Draw HTTP server.
 * Requires an Ethernet shield
 */

#include <Ethernet.h>
#include <SPI.h>
#include <Pin.h>
#include <BusMaster.h>
#include "WebServer.h"

// MAC address of the server
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x00, 0xEA, 0xF9
};


// Bus master
BusMaster *bus = BusMaster::get();
// Web server
WebServer server(80);

WebResponse listenToRequests(WebRequest &request) {
  if (request.resource == "sendData") {
    uint8_t digits = 3;
    uint8_t value = 255;
    bus->send((char*) &value, 1);
    value = 0;
    while (request.bodyAvailable()) {
      char letter = request.bodyRead();
      if (letter >= 48 && letter <= 57) {
        digits--;
        value+= (letter - 48) * pow(10, digits);
        if (digits == 0) {
          bus->send((char*) &value, 1);
          value = 0;
          digits = 3;
        }
      }
    }
    // Send an OK response
    WebResponse response = WebResponse::createText("OK");
    response.setAllowAllOrigins();
    return response;
  }
  return WebResponse::createNull();
}

void setup() {
  Serial.begin(9600);
  // Bus setup
  bus->begin();
  // Server setup
  server.registerServeMethod(listenToRequests);
  server.begin(mac);
  Serial.println(server.localIP());
}

void loop() {
  server.serve();
}