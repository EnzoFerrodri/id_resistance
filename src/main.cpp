#include <Arduino.h>
#include <BluetoothSerial.h>

#define PIN_VOUT 34          // Defina o pino ADC
#define VREF 3.3             // Defina a tensão de referência do ESP32
#define R_Fixo 1000.0        // Defina o valor do resistor fixo (em ohms)
#define resolucao 4095.0     // Resolução do ADC
#define PERIODO 500000       // Defina o período em microssegundos (500ms)

uint64_t t_atual = 0;
uint64_t t_anterior = 0;
uint64_t t_decorrido = 0;

BluetoothSerial SerialBT;    // Instancia do BluetoothSerial

void setup() {
  // Inicializa a comunicação serial por USB e Bluetooth
  Serial.begin(115200);
  SerialBT.begin("ESP32_Bluetooth"); // Define o nome do dispositivo Bluetooth

  pinMode(PIN_VOUT, INPUT);  // Define o pino ADC como entrada

  Serial.println("O Bluetooth está pronto. Emparelhe o dispositivo.");
}

void loop() {
    // Pausa antes da próxima leitura
  t_atual = micros();
  t_decorrido = t_atual - t_anterior;
  
   if (t_decorrido >= PERIODO) {
        t_anterior = t_atual;
        
        // Ler a tensão do pino ADC
        int valor_adc = analogRead(PIN_VOUT);

        // Converter o valor lido
        float Vout = valor_adc * (VREF / resolucao);

        // Calcular a resistência do potenciômetro usando a fórmula do divisor de tensão
        float Rpot = (R_Fixo * (VREF - Vout)) / Vout;

        // Imprimir o valor da resistência no monitor serial
        Serial.print("Resistencia do Potenciometro: ");
        Serial.print(Rpot);
        Serial.println(" ohms");

        // Enviar o valor da resistência via Bluetooth, se houver cliente conectado
        if (SerialBT.hasClient()) {
            SerialBT.print("Resistencia do Potenciometro: ");
            SerialBT.print(Rpot);
            SerialBT.println(" ohms");
        }
   }
}
