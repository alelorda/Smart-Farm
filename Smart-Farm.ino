#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <DHT.h>
#include <LiquidCrystal_I2C.h>    //Libreria que controla el I2C

// DHT22 config.-
#define DHTPIN 12                 //Defino sensor humedad y temperatura           D6
#define DHTTYPE DHT22
#define ENCENDER_SENSOR_DHT   dht.begin();                //inicia sensor dht
#define LEER_HUMEDAD          dht.readHumidity();    //lee humedad ambiente
#define LEER_TEMPERATURA      dht.readTemperature(); //lee temperatura ambiente
float humidity, temperature;  
int valor_referencia_temp = 26;

//sensorHumedadSuelo config.-
#define SENSORPIN A0              //defino Pin SCL LCD                            A0
#define CONFIGURAR_SENSORPIN  pinMode(SENSORPIN,INPUT);   //entrada de sensor suelo.
#define LEER_SENSOR           analogRead(SENSORPIN); // lee Humedad de suelo
int humedad, valor_referencia_hum=700;                  
String humSuelo=" ";
                              
//Cooler config.-             
#define COOLER 15                  //defino cooler                               D10
#define CONFIGURAR_COOLER     pinMode(COOLER,OUTPUT);   //entrada de sensor suelo.
#define ACTIVAR_COOLER        digitalWrite(COOLER,HIGH);//Activa el buffer
#define DESACTIVAR_COOLER     digitalWrite(COOLER,LOW);//desactiva el buffer

//Luces config.-
#define LUCES 13                   //Defino Led Verde                             D7
#define CONFIGURAR_LUCES     pinMode(LUCES,OUTPUT);   //entrada de sensor suelo.
#define ACTIVAR_LUCES         digitalWrite(LUCES,55); //Activa led verde 
#define DESACTIVAR_LUCES      digitalWrite(LUCES,LOW); //desactiva led verde 

//BombaAgua config.-
#define BOMBA_AGUA 14               //Defino Bomba de agua                         D5
#define CONFIGURAR_BOMBA_AGUA pinMode(BOMBA_AGUA,OUTPUT);       //salida de led rojo
#define ACTIVAR_BOMBA_AGUA    digitalWrite(BOMBA_AGUA,HIGH); //Activa led verde 
#define DESACTIVAR_BOMBA_AGUA digitalWrite(BOMBA_AGUA,LOW); //desactiva led verde



LiquidCrystal_I2C lcd(0x27,16,2); //Crea  objeto lcd  dirección  0x3F y 16 columnas x 2 filas
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Alumnos";
const char* password = "";
const char* mqtt_server = "172.23.3.198";//"192.168.5.101";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
float value = 0;

int manual = 0;
//int incomingByte = 0;
void setup() {
   Serial.begin(9600);
   
   EEPROM.begin(4);                     //Inicia a EEPROM com tamanho de 4 Bytes (minimo).
  
  LeerEEPROM();
  IniciarLCD();
  ENCENDER_SENSOR_DHT;
  CONFIGURAR_SENSORPIN;
  CONFIGURAR_COOLER;
  CONFIGURAR_LUCES;
  CONFIGURAR_BOMBA_AGUA;
  
  pinMode(LED_BUILTIN, OUTPUT);

 // Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1884);  //setea mqtt server en puerto 1884.-
  client.setCallback(callback);         //función a ejecutar al recibir un mensaje.-
 
}

void setup_wifi() {
 
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());  //imprime ip asignada.-
}
 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido[");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  if ((char)payload[0] == '1') {        // Si el primer caracter es == 1.-
    digitalWrite(BUILTIN_LED, LOW);
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  
  }
 
}
 
void reconnect() {
  
  while (!client.connected()) {
    Serial.print("Intentando conectar a MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("conectado");
      char temp[6];
      dtostrf(temperature,2,2,temp);
      client.publish("outTopic",temp);            //envia temperatura ambiente.-
      client.subscribe("inTopic");
    } else {
      Serial.print("error, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando en 5 segundos");
      delay(5000);
    }
  }
}

void loop() {
  LeerTempHum();
  Controlador();
  ActualizarLCD();   
  EncenderManual();
  
 if (!client.connected()) {
   reconnect();
  }
  
  client.loop();

  
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
  Blink();
  tiempo_ant = millis();

  if(humedad<valor_referencia_hum){
    humSuelo="OK  ";
    }
  else{
    humSuelo="Baja ";
    }
    
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(5, 0);
  lcd.print(temperature);
  lcd.setCursor(9, 0);
  lcd.print((char)223);
  lcd.setCursor(11,0);
  lcd.print(humidity);
  lcd.setCursor(15, 0);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Hum.Suelo:"+ humSuelo);
  }

void Blink(){
  static boolean estado = 1;
  digitalWrite(LED_BUILTIN, estado);   // turn the LED on (HIGH is the voltage level)
  estado = !estado;
}

void Controlador(){
  if(manual == 1) return;
  if(temperature > valor_referencia_temp){
    ACTIVAR_COOLER
    DESACTIVAR_LUCES
  }else{ 
    DESACTIVAR_COOLER
    ACTIVAR_LUCES
  }
  
  if(humedad<700){
    
    ACTIVAR_BOMBA_AGUA
    ActualizarLCD;
    }
  else{
  
      DESACTIVAR_BOMBA_AGUA
      ActualizarLCD;
    }
  }
void LeerEEPROM(){

   Serial.print("Reinicio Numero ");
   //Mostrar en monitor antes de grabar 
   int contador=EEPROM.read(0);
   contador ++;
   Serial.println(contador);
   EEPROM.write(0, contador);
   
   EEPROM.commit();//Salva los datos en EEPROM.
   EEPROM.end();//Fecha a EEPROM.
}

void CambiarParametro(){
    Serial.println("Ingrese un valor de referencia");
    do{
      
    }while(Serial.available() <= 0);
    
    valor_referencia_temp = Serial.parseInt();
    Serial.print("Nuevo valor de referencia: ");
    Serial.println(valor_referencia_temp);
  
}
void EncenderManual(){
  if (Serial.available() > 0) {
    int valor = Serial.parseInt();
    switch(valor){
      case 1:
          ACTIVAR_BOMBA_AGUA;
          manual = 1;
          Serial.println("Bomba Desactivada");
          break;
      case 2: 
          DESACTIVAR_BOMBA_AGUA;
          manual = 1;
          Serial.println("Bomba Activada");
          break;
      case 3:
          DESACTIVAR_COOLER;
          manual = 1;
          Serial.println("Cooler Desactivada");
          break;
      case 4: 
          ACTIVAR_COOLER; 
          manual = 1;
          Serial.println("Cooler Activad0");
          break;
      case 5:
          DESACTIVAR_LUCES;
          manual = 1;
          Serial.println("Luces Desactivada");
          break;
      case 6: 
          ACTIVAR_LUCES; 
          manual = 1;
          Serial.println("Luces Activada");
          break;
      case 7: 
          CambiarParametro(); 
          break;        
      case 10:
          Serial.println("Modo Automoatico Activado");
          manual = 0;
          break;
      default:
          Serial.println("Comando no reconocido \n 1:Bomba Desactivada\n 2:Bomba Activada\n 3:Cooler Desactivada\n 4:Cooler Activado\n 5:Luces Desactivada\n 6:Luces Activada\n 7:Cambiar Parametros\n 10:Modo Automatico");
          break;
      }
    }
  } 
 
