#ifndef Actuator
#define Actuator

#include "Common.h"

//desactivation asynchrone
unsigned long deactiveAsyncTime[7] = { 0, 0, 0, 0, 0, 0, 0 };
int deactiveAsyncArray[7] = { -1, -1, -1, -1, -1, -1, -1 };

/* prototype */
void parsedCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete);
void piloteRelais(int noCommande, int duree, int deactivate);
void deactiveAsync();

/* implementation */

/* parsedCmd
 * controller qui permet de répondre au requete HTTP et de controler l'activation/desactivation des relais
 */
void parsedCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];
  
  //Serial.println(url_tail);
  //Serial.println(tail_complete);
  
  server.httpSuccess();
  
  if (type == WebServer::HEAD)
    return;

  if (strlen(url_tail)) {
    int activateRelaiArray[6] = { -1, -1, -1, -1, -1, -1 };
    int desactivateRelaiArray[6] = { -1, -1, -1, -1, -1, -1 };
    int duree = 1000; //default 1sec
    int success = 0;
    int apikeyParamIsValid = 0;

    //check GET param
    while (strlen(url_tail)) {
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc != URLPARAM_EOS) {
        if(strcmp(name, "actuate") == 0) {
          splitStringToInt(activateRelaiArray, value);
        }
        else if(strcmp(name, "duration") == 0) {
          duree = atoi(value);
        }
        else if(strcmp(name, "deactivate") == 0) {
          splitStringToInt(desactivateRelaiArray, value);
        }
        else if(strcmp(name, "apikey") == 0) {
          if(strcmp(value, APIKEY) == 0)
          {
            apikeyParamIsValid = 1;
          }
        }
      }
    }
    
    //check apikey
    if(apikeyParamIsValid != 1)
    {
      jsonSay(server, "error", "api key not good");
      return;
    }

    //deactivate
    for(int j=0; j<6; j++) {
      if(desactivateRelaiArray[j] != -1) {
        if(!(desactivateRelaiArray[j] > 0 && desactivateRelaiArray[j] < 7)) {
          jsonSay(server, "error", "value of get param deactivate is not between 1 and 6 included");
          return;
        }
        else {
          //execute
          piloteRelais(desactivateRelaiArray[j], 0, 1);
          success = 1;
        }
      }
    }

    //activate
    for(int j=0; j<6; j++) {
      if(activateRelaiArray[j] != -1) {
        if(!(activateRelaiArray[j] > 0 && activateRelaiArray[j] < 7)) {
          jsonSay(server, "error", "value of get param actuate is not between 1 and 6 included");
          return;
        }
        else if(!(duree >= 0)) {
          jsonSay(server, "error", "value of get param duration is not a number");
          return;
        }
        else {
          //execute
          piloteRelais(activateRelaiArray[j], duree, 0);
          success = 1;
        }
      }
    }

    if(success) {
      jsonSay(server, "success", "");
    }
  }
  else {
    jsonSay(server, "error", "no get param");
  }

}

/* piloteRelais
 * fonction qui permet d'activer ou désactiver un relai.
 * si une durée est indiqué, alors on enregistre la désactivation du relai dans le tableau asunchrone
 */
void piloteRelais(int noCommande, int duree, int deactivate)
{
  if(deactivate==0) {
    Serial.print("allumer relai ");
    Serial.print(noCommande);
    Serial.println();
    digitalWrite(RELAIS_PIN[noCommande], HIGH);

    if(duree > 0) {
      //ajout de l'arret en asynchrone
      deactiveAsyncTime[noCommande] = millis() + duree;
      deactiveAsyncArray[noCommande] = noCommande;
    }
  }
  else {
    Serial.print("eteindre relai ");
    Serial.print(noCommande);
    Serial.println();
    digitalWrite(RELAIS_PIN[noCommande], LOW);
    deactiveAsyncTime[noCommande] = 0; //raz async
    deactiveAsyncArray[noCommande] = -1; //raz async
  }
}


/* deactiveAsync
 * fonction qui permet de désactiver les relais en asynchrone
 */
void deactiveAsync()
{
  unsigned long timeNow = millis();
  
  for(int j=0; j<7; j++) {
    if(deactiveAsyncTime[j] > 0 && timeNow > deactiveAsyncTime[j]) {
    //execute
    deactiveAsyncTime[j] = 0;
    
      if(deactiveAsyncArray[j] > 0) {
        piloteRelais(deactiveAsyncArray[j], 0, 1);
      }
    }
  }
}

#endif
