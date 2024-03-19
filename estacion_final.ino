

// Librerias
  #include <SPI.h>
  #include <SD.h>
    /* 
    la lireria SD es una libreria de arduino que permite acceder y crear archivos
    en una SD
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

//  variables SD
  const int chipSelect = 4; 
  //morado sck pin 13
File myFile;


//pins probe temperatura
  #define probeSDA  A4 //verde
  #define probeSCL  A5 // amarillo
  DFRobot_SHT20 probe(&Wire, SHT20_I2C_ADDR);
// variables anemometro
  long int avereageSpeed = 0;
  const int interruptPin = 2;// negro
  const int analogPin = A1;// rojo 
  volatile int Dir = 0;
  volatile int INT = 0;
  int rotations=0;
  int x=0;
  float previousMillis2=0;
  unsigned long currentMillis = 0;
  int Direccion=0;
// Variables de tiempo
  const int intervalo = 1000; // intervalo de timepo para mediciones en ms
  unsigned long previousMillis = 0; // variable que guarda cuanto tiempo a pasado desde la ultima medicion
//Variables piranómetro 
  int pyr = 0;
  #define pyrPin A0 

// variables de array
const int numSamples = 10; // Number of samples to collect before writing to SD card
float tempExterior[numSamples];
float Humedad[numSamples];
float DirViento[numSamples];
float velpromedio[numSamples];
float piranometro[numSamples];
float velinstantnea[numSamples];
int index=0;




void setup() {
  pinMode(interruptPin, INPUT_PULLUP); // Set interrupt pin as input with internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(interruptPin), Anemometro, CHANGE); // Attach interrupt to the pin
  Serial.begin(9600);
  Serial.print("Buscando Tarjeta SD...");
    // revisa la conexion con la Tarjeta SD
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
     /*Datos.print("Fecha");
  Datos.print(",");*/
  myFile.print("LargoDeOndaCorta[W/m^2]");
  myFile.print(",");
  myFile.print("% Humedad");
  myFile.print(",");
  myFile.print("Temperatura[C]");
  myFile.print(",");
  myFile.print("VelocidadVientoInstantanea[m/s]");
  myFile.print(",");
  myFile.println("DireccionViento[]");
  myFile.println();
  

    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } 
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening file.txt");

  }

 


  //iniciar el sensor de temperatura y humedad
    probe.initSHT20();

}

void loop() {
 
  currentMillis = millis(); // revisa cuanto tiempo ha pasado

    if (currentMillis - previousMillis >= intervalo) {  // revisa si ha pasado el intervalo de tiempo
  
      saveDataToArray();
      
      previousMillis = currentMillis; // guarda el tiempo actual
      
    // Increment the index for the next sample
    index++;

    }
  if (index==numSamples){
    myFile = SD.open("test.txt", FILE_WRITE);

    if (myFile) {
      for (int i = 0; i < numSamples; i++) {
        myFile.print(piranometro[i]);
        myFile.print(",");
        myFile.print(Humedad[i]);
        myFile.print(",");
        myFile.print(tempExterior[i]);
        myFile.print(",");
        myFile.print(velinstantnea[i]);
        myFile.print(",");
        myFile.print(DirViento[i]); 
        myFile.println(); // Move to the next line for the next set of data
        delay(100);
      }
   
  
 
    Serial.print("done");
    }
    Serial.println(myFile);
    myFile.close();
    //delay(5000);
    index=0;
  }

  
}






void Anemometro() {


  // This function will be called when there is a change on the interrupt pin
  INT = digitalRead(interruptPin);
  // Print the values to the serial monitor
  if (INT == 0){
   rotations++;
  }
}


void saveDataToArray() {
  // Collect data into arrays
    tempExterior[index] = probe.readTemperature();
    Humedad[index] = probe.readHumidity();
    Serial.print("Temperatura Exterior: ");    
    Serial.print(tempExterior[index]);
    Serial.println("°C  ");
    Serial.print("Humedad ");
    Serial.print(Humedad[index]);
    Serial.println("% ");
// VIENTO 
  Dir = analogRead(analogPin);
  DirViento[index]=map(Dir,0,1025,0,364);
  Serial.println(DirViento[index]);

    // guarda el tiempo actual
    Serial.print("Velocidad Instantanea: ");
    Serial.println(rotations);

    velinstantnea[index]=rotations;
    avereageSpeed = avereageSpeed+rotations;
    rotations = 0;
  
 if (currentMillis - previousMillis2 >= 5000){
     previousMillis2 = currentMillis;
     Serial.print("Velocidad Promedio: ");
     Serial.println(avereageSpeed/5);
     velpromedio[index]=avereageSpeed/5;
     avereageSpeed=0;
    }
  else{
    velpromedio[index]=-1;
    
  }
    
  //PYR
     pyr=analogRead(pyrPin); // lee los datos del piranómetro
    piranometro[index]=0.8 * pyr;// convierte la lectura del sensor en largo de onda WL = Wave Length;
    Serial.print(piranometro[index]); // imprime el largo de onda en W/m^2
    Serial.println(" W/m^2");

}