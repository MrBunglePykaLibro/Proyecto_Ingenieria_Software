
// esta macro se usa para guardar el token de autenticación de la aplicación BLYNK que interactúa con la tarjeta ESP8266
#define BLYNK_AUTH_TOKEN "nIHV1KdtkVm8wGutlAN6wNo59D3Ag5Ci"

// esta macro se usa para definir la función de la librería BLYNK que imprime los resultados de sobre la interfaz de la aplicación
#define BLYNK_PRINT Serial

//aquí se manda implementa la librería de la comunicación wifi de la tarjeta ESP8266
#include <ESP8266WiFi.h>

//aquí se manda implementa la librería de la aplicación BLYNK que interactúa con la tarjeta
#include <BlynkSimpleEsp8266.h>

// aqui se definen los nombres de los pines y su numero ID

// este es el pin para la lectura del sensor de movimiento
#define pirPin 16

// este es el pin para la emisión de ondas del sensor ultrasónico
#define trigPin 5

// este es el pin para la lectura del sensor ultrasónico que captura las ondas que emite el sensor
#define echoPin 4

// este es el pin para la señal de salida del foco 1
#define foco1Pin 2

// este es el pin para la señal de salida del foco 2
#define foco2Pin 0

//aquí se definen el nombre de la red y su contraseña donde va a operar la comunicación
char ssid[] = "INFINITUM636F";
char pass[] = "rB4L4Zay8E";

// esta variable va a guardar la señal que leerá el sensor de movimiento
int pirValor;

// esta variable va a guardar el valor de la distancia que calcula el sensor ultrasónico
int distance;

// esta variable guarda el valor capturado el pin "echo" del sensor ultrasónico
long duration;

//aquí se inicializan el comportamiento de los pines y los métodos de inicio de las bibliotecas
void setup()
{
// establece la frecuencia de comunicación del puerto serial
  Serial.begin(115200);

//inicializa el servidor de la aplicación BLYNK
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

//aquí se establece cómo se comportará cada pin
  pinMode(pirPin,INPUT);
  pinMode(trigPin,INPUT);
  pinMode(echoPin,INPUT);
  pinMode(foco1Pin,OUTPUT);
  pinMode(foco2Pin,OUTPUT);

}

// este método obtiene la lectura del sensor de movimiento
void pirSensor()
{
// se guardan los datos de la lectura del PIN del sensor de movimiento con la función "digital read"
  pirValor = digitalRead(pirPin); 
  
// se escriben esos datos en el widget de la aplicación BLYNK por medio del PIN virtual v1
  Blynk.virtualWrite(V1, pirValor);

}

// este método obtiene la lectura del sensor ultrasónico
void distanceSensor(){
  
//aquí se apaga la señal al pin disparador en el sensor ultrasónico
  digitalWrite(trigPin, LOW);  

  //se hace un retardo de 2 microsegundos
  delayMicroseconds(2);

//aquí se enciende la señal al pin disparador en el sensor ultrasónico
  digitalWrite(trigPin, HIGH);  

  //se hace un retardo de 10 microsegundos
  delayMicroseconds(10);
  
//aquí se apaga la señal al pin disparador en el sensor ultrasónico
  digitalWrite(trigPin, LOW);   

  // se toma la lectura del pin "echo" en microsegundos
  duration = pulseIn(echoPin, HIGH);   
  
// se calcula la distancia
  distance = duration * 0.034 / 2;   

  //Serial.print("Distance = ");        //Output distance on arduino serial monitor
  //Serial.println(distance);

// se escriben esos datos en el widget de la aplicación BLYNK por medio del PIN virtual v0
  Blynk.virtualWrite(V0, distance);
}

//Este método corre todo el programa sobre la tarjeta.
void loop()
{
//Se ejecuta la interacción con la aplicación.
  Blynk.run();

//Se ejecuta el método del sensor de movimiento.
  pirSensor();

//Se ejecuta el método del sensor ultrasónico.
  distanceSensor();

// este if expresa que si se detecta movimiento por parte del sensor de movimiento, se mandará una señal al foco y para encenderse
  if (pirValor == HIGH)
  {
    digitalWrite(foco1Pin, HIGH);
  }
  else{
    digitalWrite(foco1Pin, LOW); 

  }


// este if expresa que si la distancia calculada por el sensor ultrasónico es menor a 15 cm, se mandará una señal al foco 2 para encenderse
  if(distance <= 15){
    digitalWrite(foco2Pin, HIGH);
  }
  else{
    digitalWrite(foco2Pin, LOW); 
  }

}

/* la principal limitante de la implementación es que por el momento 
   solo es posible establecerlo sobre una red local
*/

/* los principios que se cumplen de código limpio son "los nombres son importantes", ya que
   se especifica cada nombre lo más explícito posible y qué ejemplo es el nombre del método 
   que calcula la distancia.
   
   Otro principio es "Debes ser el verdadero autor del código", ya que las funciones solo 
   contienen el código necesario para funcionar correctamente y un ejemplo es la función 
   "pirSensor", que solo contiene 2 líneas de código
   
   El último principio que cumplen es "DRY (Don’t Repeat Yourself)", ya que no hay repetición de 
   funciones o variables que induzcan a ambigüedades que confundan al programador y su propia 
   ejecución.
*/
