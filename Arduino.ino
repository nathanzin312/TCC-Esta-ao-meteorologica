#include <bufferfiller.h>
#include <enc28j60.h>
#include <EtherCard.h>
#include <net.h>
#include <stash.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

# define Hall sensor 2   
int contador;
float temperature ;
float humidity;
float pressure ;
#define ALTITUDE 0
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
byte Ethernet::buffer[700];
static uint32_t timer;

char mensagem[80];

const byte websiteip[] = {192,168,0,107};
const char website[] PROGMEM = "192.168.0.107";
static byte myip[]={192,168,0,113};
static byte gwip[]={192,168,0,1};

static void my_callback (byte status, word off, word len)
{
  Serial.println("-----------------------------------------------------");
  Serial.println("entrou no mycalllback");
  Serial.println("-----------------------------------------------------");
  Ethernet::buffer[off+len] = 0;    //mensagem do retorno do servidor
 // Serial.print((const char*) Ethernet::buffer + off);  //mostra todo o retorno
  Serial.print((const char*) Ethernet::buffer + off + 157); //mostra somente ok
}

// Const def biruta

int pin=0;  // A0 entrada do biruta *
float valor =0;
float Winddir =0;


// Constants definitions anem.
const float pi = 3.14159265;           // Numero pi
int period = 2500;               // Tempo de medida(miliseconds) 5000
int radius = 147;      // Raio do anemometro(mm)

// Variable definitions

unsigned int counter = 0; // magnet counter for sensor
unsigned int RPM = 0;          // Revolutions per minute
float speedwind = 0;         // Wind speed (Km/h)

Adafruit_BME280 bme; // I2C



void setup() {
  Serial.begin(9600);//serial
  //bme280
     bool status;
    status = bme.begin(0x76);  //The I2C address of the sensor I use is 0x76
    if (!status) {
        Serial.print("Sensor not working, check connections");
        while (1);
    }
    
   // Anemômetro 
  pinMode(2, INPUT);  // Entrada anemometro *
    digitalWrite(2, HIGH);  //internall pull-up active

      Serial.println("Obtendo IP:");
  if (ether.begin(sizeof Ethernet::buffer, mymac,53) == 0){
    Serial.println( "Falha ao acessar o controlador Ethernet");
  }
  ether.staticSetup(myip, gwip);
  ether.printIp("IP: ", ether.myip);
  ether.printIp("Gatway: ", ether.gwip);
   ether.printIp("Sevidor DNS: ", ether.dnsip);
   if (!ether.dnsLookup(website)){
    Serial.println("Falha no DNS");
  }
  ether.parseIp(ether.hisip, "192.168.0.107"); //IP do servidor manual
  ether.printIp("Servidor: ", ether.hisip);
  Serial.println(" ");
}

// Calculo da velocidade wind  *************
void windvelocity(){
  speedwind = 0;
  counter = 0;  
 attachInterrupt(0, addcount, RISING);
  unsigned long millis();       
  long startTime = millis();
  while(millis() < startTime + period) {
  }
}


void RPMcalc(){
  RPM=((counter)*60)/(period/500);  // Calculate revolutions per minute (RPM)
}


void SpeedWind(){
  speedwind = (((4 * pi * radius * RPM)/60) / 500)*3.6;  // Calculate wind speed on km/h
 
}

void addcount(){
  counter++;
} 
// Fim calculo veloc wind  *****************



void loop() {
  

 //anenometro
 windvelocity();
RPMcalc();
 SpeedWind();

//biruta
   valor = analogRead(pin)* (5.0 / 1023.0); 

if (valor <= 0.27) {
Winddir = 315;
}
else if (valor <= 0.32) { 
Winddir = 270;
}
else if (valor <= 0.38) { 
Winddir = 225;
}
else if (valor <= 0.45) { 
Winddir = 180;
}
else if (valor <= 0.57) { 
Winddir = 135;
}
else if (valor <= 0.75) { 
Winddir = 90;
}
else if (valor <= 1.25) {  
Winddir = 45;
}
else {  
Winddir = 000;
}
 
 //pluviometro
 
Serial.println(valor);//bme280
getPressure();
getHumidity();
getTemperature();
   char t[50];
   char h[50];
   char p[50];
   char w[50];
   char sw[50];
   dtostrf(pressure,1,2,p);
   dtostrf(Winddir,1,2,w);
   dtostrf(speedwind,1,2,sw);
   dtostrf(temperature,1,2,t);
   dtostrf(humidity,1,2,h);  
   //Printing Temperature
 String temperatureString = String(temperature,1);
 Serial.write("Temperatura:"); 
 Serial.print(temperatureString);
 Serial.write((char)223);
 Serial.write("C ");
 Serial.println("");
 
 //Printing Humidity
 String humidityString = String(humidity,0); 
 Serial.write("Umidade: ");
 Serial.print(humidityString);
 Serial.write("%");
 Serial.println("");
 
 //Printing Pressure
 Serial.write("Pressao: ");
 String pressureString = String(pressure,2);
 Serial.print(pressureString);
 Serial.write(" hPa");
 Serial.println("");

 Serial.print("Direcao: ");
 Serial.print(Winddir);
 Serial.print(" graus ");
 Serial.print("--Veloc ");
 Serial.print(speedwind);
 Serial.print(" Km/h ");
 Serial.println("");
  ether.packetLoop(ether.packetReceive());
 if (millis() > timer){
    timer = millis() + 300000;//60000;//14000;//4000
    ether.persistTcpConnection(true); 
    sprintf(mensagem, "?temperatura=%s&pressao=%s&umidade=%s&velocidade=%s&direcao=%s",t,p,h,sw,w);//diz que o valor é nulo,e nao insere no banco 
    ether.browseUrl(PSTR("/teste.php"),mensagem,  website, my_callback);  
 }
  delay(100);
}

float getTemperature()
{
  temperature = bme.readTemperature();
}

float getHumidity()
{
  humidity = bme.readHumidity();
}

float getPressure()
{
  pressure = bme.readPressure();
  pressure = bme.seaLevelForAltitude(ALTITUDE,pressure);
  pressure = pressure/100.0F;
}
