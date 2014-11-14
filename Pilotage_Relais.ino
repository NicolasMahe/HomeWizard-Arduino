#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <util.h>


/* 
 Logiciel Arduino pour piloter des relais à partir d'un serveur Http
 V1 - By Nicolas Mahé - nicolas@mahe.me
*/

/*
 DOC WebServer: https://github.com/sirleech/Webduino
 Version 1.7
 
 Attention à la taille du buffer: il limite la longeur de l'url
 */

#define WEBDUINO_FAIL_MESSAGE "Request Failed"
#include "SPI.h"
//#include "avr/pgmspace.h"
//#include "Ethernet.h"
#include "WebServer.h"

/* fichier perso */
#include "Config.h"
#include "Actuator.h"
#include "Sensor.h"
#include "Common.h"


/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
WebServer webserver("", 80);

/* HTTP fail response */
void my_failCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  /* this line sends the "HTTP 400 - Bad Request" headers back to the browser */
  server.httpFail();

  /* if we're handling a GET or POST, we can output our data here.
   For a HEAD request, we just stop after outputting headers. */
  if (type == WebServer::HEAD)
    return;
    
  //server.println("Unknown page");
}

void setup()
{
  //Pin mode
  pinMode(O0, OUTPUT);
  pinMode(O1, OUTPUT);
  pinMode(O2, OUTPUT);
  pinMode(O3, OUTPUT);
  pinMode(O4, OUTPUT);
  pinMode(O5, OUTPUT);
  
  Serial.begin(9600);
  
  Ethernet.begin(mac, ip);
  webserver.setDefaultCommand(&parsedCmd);
  webserver.setFailureCommand(&my_failCmd);
  webserver.addCommand("sensor", &sensorCmd);
  webserver.begin();
  
}

int incomingByte = 0;

void loop()
{
  char buff[128]; //correspond au nombre max de caractère dans url
  int len = 128; //correspond au nombre max de caractère dans url
  webserver.processConnection(buff, &len);
  
  //check asynchrone deactivation of relais
  deactiveAsync();
  
  //check sensor values
  //checkSensorValues();
}
