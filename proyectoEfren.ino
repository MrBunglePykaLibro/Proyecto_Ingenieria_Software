
// esta macro se usa para guardar el token de auttenticacion de la aplicaciòn BLYNK que interactua con la tarjeta ESP8266
#define BLYNK_AUTH_TOKEN "nIHV1KdtkVm8wGutlAN6wNo59D3Ag5Ci"

// esta macro se usa para definir la funcion de la libreria BLYNK que imprime los resultados de sobre la interfaz de la aplicación
#define BLYNK_PRINT Serial

// aqui se manda implementa la libreria de la comunicacion wifi de la tarjeta ESP8266
#include <ESP8266WiFi.h>

// aqui se manda implementa la libreria de la aplicacion BLYNK que interactua con la tarjeta
#include <BlynkSimpleEsp8266.h>

// aqui se definen los nombres de los pines y su numero ID

// este es el pin para la lectura del sensor de movimiento
#define pirPin 16

// este es el pin para la emision de ondas del sensor ultrasonico
#define trigPin 5

// este es el pin para la lectura del sensor ultrasonico que captura las ondas que emite el sensor
#define echoPin 4

// este es el pin para la señal de salida del foco 1
#define foco1Pin 2

// este es el pin para la señal de salida del foco 2
#define foco2Pin 0

// aqui se definen el nombre de la red y su contraseña donde va operar la comunicacion 
char ssid[] = "INFINITUM636F";
char pass[] = "rB4L4Zay8E";

// esta variable va a guardar la señal que leera el sensor de movimiento
int pirValor;

// esta variable va a guardar el valor de la distancia que calcula el sensor ultrasonico
int distance;

// esta variable guarda el valor capturado el pin "echo" del sensor ultrasonico
long duration;

//aqui se inicalizan el comportamiento de los pines y los metodos de inicio de las bibliotecas
void setup()
{
  // establece la frecuencia de comunicaion del puerto serial
  Serial.begin(115200);

  // inicaliza el servidor de la aplicacion BLYNK
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // aqui se establece como se comportara cada pin 
  pinMode(pirPin,INPUT);
  pinMode(trigPin,INPUT);
  pinMode(echoPin,INPUT);
  pinMode(foco1Pin,OUTPUT);
  pinMode(foco2Pin,OUTPUT);

}

// este metodo obtiene la lectura del sensor de movimiento
void pirSensor()
{
  // se guardan los datos de la lectura del pin del sensor de movimiento con la funcion "digitalRead"
  pirValor = digitalRead(pirPin); 
  
  // se escriben esos datos en el widget de la aplicacion BLYNK por medio del pin virtual v1
  Blynk.virtualWrite(V1, pirValor);

}

// este metodo obtiene la lectura del sensor ultrasonico
void distanceSensor(){
  
  // aqui se apaga la señal al pin disparador en el sensor ultrasonico
  digitalWrite(trigPin, LOW);  

  //se hace un retardo de 2 microsegundos
  delayMicroseconds(2);

  // aqui se enciende la señal al pin disparador en el sensor ultrasonico
  digitalWrite(trigPin, HIGH);  

  //se hace un retardo de 10 microsegundos
  delayMicroseconds(10);
  
  // aqui se apaga la señal al pin disparador en el sensor ultrasonico
  digitalWrite(trigPin, LOW);   

  // se toma la lectura del pin "echo" en microsegundos
  duration = pulseIn(echoPin, HIGH);   
  
  // se calcula la distaniad
  distance = duration * 0.034 / 2;   

  //Serial.print("Distance = ");        //Output distance on arduino serial monitor
  //Serial.println(distance);

  // se escriben esos datos en el widget de la aplicacion BLYNK por medio del pin virtual v0
  Blynk.virtualWrite(V0, distance);
}

// este metodo corre todo el programa sobre la tarjeta.
void loop()
{
  // se ejecuta la interaccion con la aplicacion.
  Blynk.run();

  // se ejecuta el metodo del sensor de movimiento. 
  pirSensor();

  // se ejecuta el metodo del sensor ultrasonico.
  distanceSensor();

  // este if expresa que si se detecta movimiento por parte del sensor de movimiento se mandara una señal al foco i para encenderse
  if (pirValor == HIGH)
  {
    digitalWrite(foco1Pin, HIGH);
  }
  else{
    digitalWrite(foco1Pin, LOW); 

  }


  // este if expresa que si la distancia calculada por el sensor ultrasonico es menor a 15cm, se mandara una señal al foco 2 para encenderse
  if(distance <= 15){
    digitalWrite(foco2Pin, HIGH);
  }
  else{
    digitalWrite(foco2Pin, LOW); 
  }

}

/* la principal limitante de la implementacio es que por el momento 
   solo es posible establecerlo sobre una red local
*/

/* los pricipios que se cumplen de codigo limpio son "Los nombres son importantes" ya que
   se especifica cada nombre lo mas explicito posible y sue ejemplo es el nombre del metodo 
   que calcula la distancia.
   
   Otro prinipio es "Debes ser el verdadero autor del código" ya que las funciones solo 
   contienen el codigo necesario para funcionar correctamente y un ejemplo es la funcion 
   "pirSensor" que solo contiene 2 lineas de codigo
   
   El ultimo principio que cumplen es "DRY (Don’t Repeat Yourself)" ya que no hay repetion de 
   funciones o variables que induscan a ambiguedades que confundan al programador y su propia 
   ejecucion.
*/
