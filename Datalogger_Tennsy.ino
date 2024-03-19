//librerias
  
    /* 
    este archivo es un datalogger para analizar 3 termocuplas 1 sensor de humedad y temperatura 
    luego las ingresa en una tarjeta SD
    */
  #include <SD.h>
    /* 
    la lireria SD es una libreria de arduino que permite acceder y crear archivos
    en una SD
    */
  #include <TimeLib.h>
    /* 
    la libreria TimeLib facilita setear y manejar el tiempo con fnciones mas amigables
    */
  #include <max6675.h>
    /*
    Libreria que ayuda a utilizar la termocupla 
    */
  #include "DFRobot_SHT20.h"
    /*
      libreria que permite leer los dartos entregados por el sensor SHT10
      en este caso la probeta de temperatura y humedad
    */

  #include<Wire.h>
    /*
    libreria que permite las comunicaciones entre teensy y arduino 
    tambien libreria base de todas las comunicaciones I2C
    */

//variables
  File Datos;
  const int chipSelect = BUILTIN_SDCARD; 
  float Termocupla1  = 0 ;
  float Termocupla2  = 0 ;
  float Termocupla3  = 0 ;

//pins
  //pins Termoculpa 1
    #define soPin1   4  // SO=Serial Out
    #define csPin1   10 // CS = chip select CS pin
    #define sckPin1   13 // SCK = Serial Clock pin
  //pins Termoculpa 2
    #define soPin2    6  // SO=Ser ial Out
    #define csPin2    0  // CS = chip select CS pin
    #define sckPin2   16 // SCK = Serial Clock pin
    //pins Termoculpa 3
    #define soPin3    8 // SO=Serial Out
    #define csPin3    37 // CS = chip select CS pin
    #define sckPin3   24 // SCK = Serial Clock pin

  //setups
  MAX6675 TC1(sckPin1, csPin1, soPin1);
  MAX6675 TC2(sckPin2, csPin2, soPin2);
  MAX6675 TC3(sckPin3, csPin3, soPin3);
void setup() {
  Serial.begin(9600);
  //fijar el tiempo  
    setTime(12, 6, 0, 21, 12, 2023);

  //iniciar Tarjeta SD
    Serial.print("Buscando Tarjeta SD...");
    // revisa la conexion con la Tarjeta SD
    if (!SD.begin(chipSelect)) {
      Serial.println(" SD no encontrada");
      while (1);
    }
    Serial.println("SD conectada correctamente");

  //set headears
    Datos = SD.open("Datalogger.CSV", FILE_WRITE); 
    Datos.print("Tiempo");
    Datos.print(",");
    Datos.print("Fecha");
    Datos.print(",");
    Datos.print("Termocupla1[°C]");
    Datos.print(",");
    Datos.print("Termocupla2[°C]");
    Datos.print(",");
    Datos.println("Termocupla3[°C]");

  } 


void loop() {
  for(int i = 0; i<1000; i++){
  Datos = SD.open("Datalogger.CSV", FILE_WRITE); 
  Fecha();
  Temperaturas();
  //escribir los datos en la Tarjeta SD  
    Datos.print(hour());
    Datos.print(":");
    Datos.print(minute());
    Datos.print(":");
    Datos.print(second());
    Datos.print(",");
    Datos.print(day());
    Datos.print("/");
    Datos.print(month());
    Datos.print("/");
    Datos.print(year()); 
    Datos.print(","); 
    Datos.print(Termocupla1);
    Datos.print(",");
    Datos.print(Termocupla2);
    Datos.print(",");
    Datos.println(Termocupla3);
    delay(1000);
  }



}
//Conjunto de funciones que revisan la fecha y escriben la fechaen la SD
  void Fecha(){

    // revisa si el archivo se abre
    if(!Datos) {
    Serial.println("error opening Fecha.txt");
    }
  // digital clock display of the time
  if (Datos) {

  } 
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  }
  void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Datos.print(":");
  if(digits < 10)
    Datos.print('0');
  Datos.print(digits);}
//  funcion de tempertaturas
  void Temperaturas(){
    // leer los sensores

    Termocupla1  = TC1.readCelsius();
    Termocupla2  = TC2.readCelsius();
    Termocupla3  = TC3.readCelsius();

    // Print the values to the serial port
    Serial.print("temp1= " );
    Serial.print(Termocupla1);
    Serial.println("  °C");
    Serial.print("temp2= " );
    Serial.print(Termocupla2);
    Serial.println("  °C");
    Serial.print("temp3= " );
    Serial.print(Termocupla3);
    Serial.println("  °C");

    }