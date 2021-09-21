#include <DHT.h>
#include <LiquidCrystal_I2C.h>    //Libreria que controla el I2C

// DHT22 config.-
#define DHTPIN 12                 //Defino sensor humedad y temperatura           D6
#define DHTTYPE DHT22
#define ENCENDER_SENSOR_DHT   dht.begin();                //inicia sensor dht
#define LEER_HUMEDAD          dht.readHumidity();    //lee humedad ambiente
#define LEER_TEMPERATURA      dht.readTemperature(); //lee temperatura ambiente
float humidity, temperature;

//sensorHumedadSuelo config.-
#define SENSORPIN A0              //defino Pin SCL LCD                            A0
#define CONFIGURAR_SENSORPIN  pinMode(SENSORPIN,INPUT);   //entrada de sensor suelo.
#define LEER_SENSOR           analogRead(SENSORPIN); // lee Humedad de suelo
int humedad;

//Cooler config.-
#define COOLER 15                  //defino cooler                               D10
#define CONFIGURAR_COOLER  pinMode(COOLER,OUTPUT);   //entrada de sensor suelo.
#define ACTIVAR_COOLER       digitalWrite(COOLER,HIGH);//Activa el buffer
#define DESACTIVAR_COOLER    digitalWrite(COOLER,LOW);//desactiva el buffer


LiquidCrystal_I2C lcd(0x27,16,2); //Crea  objeto lcd  dirección  0x3F y 16 columnas x 2 filas
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  IniciarLCD();
  ENCENDER_SENSOR_DHT;
  CONFIGURAR_SENSORPIN;
  CONFIGURAR_COOLER;
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  LeerTempHum();
  ActualizarLCD();
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  
}

//Presentacion de inicio de programa.-
void IniciarLCD(){
  lcd.init();               // Inicializar el LCD
  lcd.backlight();          // Activar luz de fondo
  lcd.clear();              // Borrar LCD
  lcd.setCursor(3,0);       // coordenadas LCD (x,y)
  lcd.print("INICIANDO");   // Mensaje de inicio 
  lcd.setCursor(6,1);       // coordenadas LCD (x,y)
  lcd.print("...");         // Mensaje de inicio
  delay(2000);              //retardo 2000  millis
  
  lcd.clear();              // Borrar LCD
  lcd.setCursor(3,0);       // coordenadas LCD (x,y)
  lcd.print("");            // Mensaje de inicio 
  lcd.setCursor(5,0);       // coordenadas LCD (x,y)
  lcd.print("FINCAR");      // Mensaje de inicio
  delay(1500);              //Retardo de 1500 millis
  
  lcd.clear();  
  }

//lectura de valores a controlar.-
void LeerTempHum(){
  humidity = LEER_HUMEDAD;
  temperature = LEER_TEMPERATURA;
  humedad = LEER_SENSOR;
  }

//muestra por display los valores.-
void ActualizarLCD(){
  static unsigned long tiempo_ant = 0;
  
  if(millis()-tiempo_ant < 1000) return;
  tiempo_ant = millis();
  
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");
  lcd.setCursor(12, 0);
  lcd.print(temperature);
  lcd.setCursor(0, 1);
  lcd.print("Humedad:");
  lcd.setCursor(8, 1);
  lcd.print(humedad);
  lcd.setCursor(12, 1);
  lcd.print(humidity);
  Serial.print("Temperatura:");
 
  Serial.print(temperature);
  
  Serial.print("Humedad:");
  
  Serial.print(humedad);
 
  Serial.print(humidity);
  Serial.print("\n");

}

 
