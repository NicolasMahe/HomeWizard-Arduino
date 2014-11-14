#ifndef Sensor
#define Sensor


#define NULLEVAL 509
#define SENSIVITY 62

#include "Common.h"

#define nbrValueStats 200

int stats[6][nbrValueStats];
int statsCpt[6] = {0, 0, 0, 0, 0, 0};

/* prototype */
int getMaxValueFrom(int entre, int duration);
void sensorCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete);

/* implementation */

/* sensorCmd
 * controller qui permet de répondre au requete HTTP et de controler sensor
 */
void sensorCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];
  
  //Serial.println(url_tail);
  //Serial.println(tail_complete);
  
  server.httpSuccess();
  
  if (type == WebServer::HEAD)
    return;
    
    
  int duree = 1000;
  int sensivity = SENSIVITY;
  int valeurNulle = NULLEVAL;

  if (strlen(url_tail)) {
    int apikeyParamIsValid = 0;
    
    //check GET param
    while (strlen(url_tail)) {
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc != URLPARAM_EOS) {
        if(strcmp(name, "duration") == 0) {
          duree = atoi(value);
        }
        else if(strcmp(name, "sensivity") == 0) {
          sensivity = atoi(value);
        }
        else if(strcmp(name, "valNulle") == 0) {
          valeurNulle = atoi(value);
        }
      }
    }
  }
  //else {
  //  jsonSay(server, "error", "no get param");
  //}
    
    float valeur = getMaxValueFrom(I2, duree);
    float amplitude = (valeur - valeurNulle) / 1024 * 5 / sensivity * 1000000;
    int effective = amplitude / 1.414;
    
    float puissance = 230.0 * (float)effective / 1000.0;
    
    Serial.print(valeur);
    Serial.print("\t");
    Serial.print(amplitude);
    Serial.print("\t");
    Serial.print(effective);
    Serial.print("\t");
    Serial.println(puissance);
    
  

    jsonSayWithData(server, "success", String(puissance));

}

int getMaxValueFrom(int entre, int duration)
{
  int sensorValue;
  int sensorMax = 0;
  uint32_t start_time = millis();
  while((millis() - start_time) < duration) {
    sensorValue = analogRead(entre);
    if(sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }
  }
  return sensorMax;
}

void checkSensorValues()
{
  for(int i=0; i<6; i++) {
    if(statsCpt[i] >= nbrValueStats) {
      statsCpt[i] = 0;
    }
    stats[i][statsCpt[i]] = analogRead(SENSORS_PIN[i]);
    statsCpt[i]++;
  }
}

#endif
