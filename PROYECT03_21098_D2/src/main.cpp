//***************************************************************************************************************************************
/* Karen Leonor Córdova López
 * Proyecto 3- Sensor de temperatura LM75
 * UART TIVA C - ESP 32- PANTALLA TFT-NEOPIXEL
 * IE3027: Electrónica Digital 2 - 2023
 */
//***************************************************************************************************************************************

//***************************************************************************************************************************************
// Librerías 
//***************************************************************************************************************************************

#include <Arduino.h>
#include <Temperature_LM75_Derived.h>
#include <Adafruit_NeoPixel.h>
//***************************************************************************************************************************************
// Definición de pines y varaibles globales
//***************************************************************************************************************************************
// Pin al que están conectados los LEDs
#define LED_PIN 23
#define RXp2 16
#define TXp2 17

Generic_LM75 LM75;
// Número de LEDs en el anillo
#define NUM_LEDS 12
// Brillo de los LEDs (0 a 255)
#define BRIGHTNESS 75
// Declaración del objeto NeoPixel
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Variable para indicar si el efecto debe estar activo
bool neopixelEffectActive1 = false;
bool neopixelEffectActive2 = false;

//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************

void sensor();
void neore();
void setupNeopixel();
void loopNeopixel1();
void loopNeopixel2();
void processCommand(const String &command);
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  Serial.begin(115200);    // Comunicacion UART COMPUTADORA
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2);   // UART CON TIVA C 
  Wire.begin();

  // Inicializar el Neopixel
  setupNeopixel();
}

//***************************************************************************************************************************************
// Loop infinito
//***************************************************************************************************************************************
void loop() {
  sensor();
  neore();
  // Comando del Neopixel solo si el efecto está activo
  if (neopixelEffectActive1) {
    loopNeopixel1();
  }
  else{ 
     loopNeopixel2();
  }
}

//***************************************************************************************************************************************
// Función de  la lectura del sensor LM75
//***************************************************************************************************************************************

void sensor() {
  float temperature = LM75.readTemperatureC();  // Lee la temperatura del sensor

  // Envía la temperatura por Serial2 a la TIVA C
  Serial2.print("Temperature=");
  Serial2.println(temperature);
  Serial2.println(" C");

  delay(5000);
}

//***************************************************************************************************************************************
// Función de activación de los efectos 1 y 2 (estados de los botones)
//***************************************************************************************************************************************

void processCommand(const String &command) {
  // Procesar el comando recibido del botón 1
  if (command == "1") {
    neopixelEffectActive1 = true;
    Serial.println("Neopixel effect activated");
  } else if (command == "0") {
    neopixelEffectActive1 = false;
    Serial.println("Neopixel effect deactivated");
  }
  else if(command =="2"){  //Procesar el comando recibido del boton 2
    neopixelEffectActive2 = true;
    Serial.println("Dato guardado en la SD");
   
  }
}

//***************************************************************************************************************************************
// Inicialización del neopixel 
//***************************************************************************************************************************************

void setupNeopixel() {
  // Inicializar el objeto NeoPixel
  strip.begin();
  strip.clear();
  strip.setBrightness(BRIGHTNESS);

  Serial.println("Setup del Neopixel completado");
}

//***************************************************************************************************************************************
// Efecto1 del neopixel
//***************************************************************************************************************************************

// La lógica del efecto NeoPixel aquí
void loopNeopixel1() {
  uint8_t purple = 255;

  // Si el efecto está activo
  if (neopixelEffectActive1) {
    // Hacer que los colores cambien mientras recorremos el círculo
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, purple, 0, purple);
      strip.show();
      delay(40);
    }

    // Limpiar los LEDs del círculo
    strip.clear();
    strip.show();

    // Reiniciar neopixelEffectActive para permitir que el efecto se ejecute nuevamente
    neopixelEffectActive1 = false;
  }
}

//***************************************************************************************************************************************
// Efecto2 del neopixel
//***************************************************************************************************************************************
void loopNeopixel2() {
  uint8_t blue = 255;

  // Si el efecto está activo
  if (neopixelEffectActive2) {
    // Hacer que los colores cambien mientras recorremos el círculo
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, 0, 0, 200 );
      strip.show();
      delay(40);
    }

    // Limpiar los LEDs del círculo
    strip.clear();
    strip.show();

    // Reiniciar neopixelEffectActive para permitir que el efecto se ejecute nuevamente
    neopixelEffectActive2 = false;
  }
}

//***************************************************************************************************************************************
// UART CON LA TIVA C- recibe mandatos
//***************************************************************************************************************************************
void neore(){
   // Verificar si hay comandos disponibles en la UART
  while (Serial2.available() > 0) {
    char commandChar = Serial2.read();
    // Convertir el comando a String para facilitar su procesamiento
    String command = String(commandChar);
    processCommand(command);

    String message= Serial2.readStringUntil('\n');  // Recibe mensajes recibidos de la TIVA C 
    Serial.println(message);
  }
}