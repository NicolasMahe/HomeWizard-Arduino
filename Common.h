#ifndef Common
#define Common

// Analog In/Out
#define I0 A0
#define I1 A1
#define I2 A2
#define I3 A3
#define I4 A4
#define I5 A5

#define O0 11
#define O1 10
#define O2 9
#define O3 6
#define O4 5
#define O5 3

/* max length of GET param nam<e & value */
#define NAMELEN 32
#define VALUELEN 32

/* jsonSay
 * retourne dans la réponse HTTP un status et une erreur en JSON
 */
void jsonSay(WebServer &server, char *status, char *error)
{
  server.print("{");

  server.print("\"status\":\"");
  server.print(status);
  server.print("\"");

  server.print(",");
  server.print("\"error\":\"");
  server.print(error);
  server.print("\"");

  server.print("}");
}
/* jsonSayWithData
 * retourne dans la réponse HTTP un status et des data en JSON
 */
void jsonSayWithData(WebServer &server, char *status, String data)
{
  server.print("{");

  server.print("\"status\":\"");
  server.print(status);
  server.print("\"");

  server.print(",");
  server.print("\"data\":\"");
  server.print(data);
  server.print("\"");

  server.print("}");
}

/* splitStringToInt
 * divise un char* de type 1,2,3 en un int*
 */
void splitStringToInt(int *array, char *stringToSplit)
{
  char *p;
  int i = 0;
  do {
    if(i==0) {
      p = strtok(stringToSplit, ",");
    }
    else {
      p = strtok(NULL, ",");
    }
    
    if(p) {
      array[i] = atoi(p);
    }
    i++;
  }
  while(p);
}

#endif
