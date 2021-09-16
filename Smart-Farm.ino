#include <DHT.h>
#include <LiquidCrystal_I2C.h>    //Libreria que controla el I2C

// DHT22 config.-
#define DHTPIN 12                 //Defino sensor humedad y temperatura           D6
#define DHTTYPE DHT22
#define ENCENDER_SENSOR_DHT   dht.begin();                //inicia sensor dht
#define LEER_HUMEDAD          dht.readHumidity();    //lee humedad ambiente
#define LEER_TEMPERATURA      dht.readTemperature(); //lee temperatura ambiente



LiquidCrystal_I2C lcd(0x27,16,2); //Crea  objeto lcd  dirección  0x3F y 16 columnas x 2 filas
DHT dht(DHTPIN, DHTTYPE);

//Defino variables Sensor Humedad & Temperatura
float humidity, temperature;

void setup() {
  IniciarLCD();
  ENCENDER_SENSOR_DHT;
}

void loop() {
  LeerTempHum();
  ActualizarLCD();

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

void LeerTempHum(){
  humidity = LEER_HUMEDAD;
  temperature = LEER_TEMPERATURA;
  }

void ActualizarLCD(){
  static unsigned long tiempo_ant = 0;
  
  if(millis()-tiempo_ant < 1000) return;
  tiempo_ant = millis();
  //Opcion 1
  lcd.setCursor(12, 0);
  lcd.print(temperature);
  lcd.setCursor(12, 1);
  lcd.print(humidity);
  lcd.print(" ");
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");
  lcd.setCursor(0, 1);
  lcd.print("Humedad:");
}

 
