#include <Arduino.h>
#include <SPIFFS.h>
#include "FileIO.h"

// Includes para funcionamento do Firebase
#include <WiFi.h>
#include <FirebaseESP32.h>

// Informações referentes ao Realtime Database
#define FIREBASE_HOST "https://cyclic-voltammetry-fe847-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "hQPiV5ps5XlkKQdjtbAKXVxTGPEt9B1McePULPUf"
#define WIFI_SSID "24Apt102"
#define WIFI_PASSWORD "J606165249"

FirebaseData firebaseData;

// Nome do arquivo que armazenará os dados da voltametria cíclica
#define fileName "/coletas.txt"
#define nodeVoltage "/coletas/voltage"
#define nodeCurrent "/coletas/current"

// Definição dos pinos de entrada e saída
#define pinReadSignal 34
#define pinPwmOutput 13 
#define intervalCollect 100 // Intervalo de tempo entre mudanças de tensão em milissegundos (ms)

int signal_value = 0;

const int pwm_frequency = 31500; // Define frequência de saída do PWM (Hz)
const int pwm_channel = 0;		 // Define canal do PWM para a ESP32
const int pwm_resolution = 8;	 // Define uma resolução de 8 bits para PWM (0-255)

void FirebaseConfig(){
	// Conecta ao WiFi
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	Serial.print("Connecting to Wi-Fi");
	while (WiFi.status() != WL_CONNECTED){
		Serial.print(".");
		delay(300);
	}

	Serial.println();
	Serial.print("Connected with IP: ");
	Serial.println(WiFi.localIP());
	Serial.println();

	// Inicia conexão com o Firebase
	Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

	// Reconexão automática com a internet
	Firebase.reconnectWiFi(true);
}

void pwmConfig(){
	/*
		Configuração do pino de saída PWM para possuir
		as mesmas características do que foi testado e
		validado no Arduino Nano.
	*/
	ledcSetup(pwm_channel, pwm_frequency, pwm_resolution);
	ledcAttachPin(pinPwmOutput, pwm_channel);

	analogReadResolution(10); // Altera resolução da leitura analógica para 10 bits (0-1023)
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void startVoltammetry(int time_interval, float min_voltage, float max_voltage){
	// Verifica se os parâmetros passados para variação de tensão condizem com a realidade
	if (min_voltage < 0){
		Serial.println("Min Voltage must be 0!");
		return;
	}else if(min_voltage > 3){
		Serial.println("Min Voltage must be lesser than 3!");
		return;
	}

	if (max_voltage > 3.3){
		Serial.println("Max Voltage must be 3.3!");
		return;
	}else if(max_voltage < 0.3){
		Serial.println("Max Voltage must be higher than 0.3!");
		return;
	}

	// Cria arquivo na memória interna para armazenar todos os dados da coleta
	if (verifyFile(SPIFFS, fileName)){
		return;
	}

	// Determina os valores de duty cycle para atender a tensão de variação de saída requerida
	int min_duty_cycle = map(min_voltage, 0.0, 3.3, 0.0, 255.0);
	int max_duty_cycle = map(max_voltage, 0.0, 3.3, 0.0, 255.0);

	double voltageValue = 0;
	double currentValue = 0.0;

	/*
	// Cria contadores para determinar o fim da coleta de dados
	int current_counter = 0;
	int counter_limit = max_duty_cycle - min_duty_cycle;

	Lógica do millis pode ser utilizada para criar uma função não bloqueante futuramente

	int init_time = millis();
	int end_time = init_time;
	*/

	// Realiza variação de tensão positivamente na saída e coleta dados
	for (int i = min_duty_cycle; i <= max_duty_cycle; i++){
		ledcWrite(pwm_channel, i);
		delay(intervalCollect);
		// Coleta dados de corrente e tensão de acordo com tabela verdade da corrente
		voltageValue = analogRead(pinReadSignal);
		if (voltageValue > 511){
			currentValue = mapfloat(voltageValue, 512, 1023, 0.0, 0.0002);
		}else{
			currentValue = (-1)*mapfloat(voltageValue, 0, 511, 0.0, 0.0002);
		}

		voltageValue = mapfloat(voltageValue, 0, 1023, 0.0, 3.3);

		// Salvando dados na memória interna no arquivo refernete ao fileName
		appendFileVoltammetry(SPIFFS, fileName, String(voltageValue).c_str(), String(currentValue).c_str());
	}

	// Realiza variação de tensão negativamente na saída e coleta dados
	for (int i = max_duty_cycle; i >= min_duty_cycle; i--){
		ledcWrite(pwm_channel, i);
		delay(intervalCollect);
		// Coleta dados de corrente e tensão de acordo com tabela verdade da corrente
		voltageValue = analogRead(pinReadSignal);
		if (voltageValue > 511){
			currentValue = mapfloat(voltageValue, 512, 1023, 0.0, 0.0002);
		}else{
			currentValue = (-1)*mapfloat(voltageValue, 0, 511, 0.0, 0.0002);
		}

		voltageValue = mapfloat(voltageValue, 0, 1023, 0.0, 3.3);

		// Salvando dados na memória interna no arquivo refernete ao fileName
		appendFileVoltammetry(SPIFFS, fileName, String(voltageValue).c_str(), String(currentValue).c_str());
	}
}

void testVoltammetry(int time_interval, float min_voltage, float max_voltage){
	/*
		Função criada para gerar dados aleatórios de voltametria para validar criação de gráfico
	*/
	deleteFile(SPIFFS, fileName);
	// Verifica se os parâmetros passados para variação de tensão condizem com a realidade
	if (min_voltage < 0){
		Serial.println("Min Voltage must be 0!");
		return;
	}else if(min_voltage > 3){
		Serial.println("Min Voltage must be lesser than 3!");
		return;
	}

	if (max_voltage > 3.3){
		Serial.println("Max Voltage must be 3.3!");
		return;
	}else if(max_voltage < 0.3){
		Serial.println("Max Voltage must be higher than 0.3!");
		return;
	}
	
	// Determina os valores de duty cycle para atender a tensão de variação de saída requerida
	int min_duty_cycle = map(min_voltage, 0.0, 3.3, 0.0, 255.0);
	int max_duty_cycle = map(max_voltage, 0.0, 3.3, 0.0, 255.0);

	double voltageValue = 0;
	double currentValue = 0.0;

	// Realiza variação de tensão positivamente na saída e coleta dados
	for (int i = min_duty_cycle; i <= max_duty_cycle; i++){
		ledcWrite(pwm_channel, i);
		delay(intervalCollect);
		// Coleta dados de corrente e tensão para testes
		voltageValue = random(0, 330)/100;
		currentValue = (random(0, 400) - 200)/100;

		// Salvando dados na memória interna no arquivo refernete ao fileName
		appendFileVoltammetry(SPIFFS, fileName, String(voltageValue).c_str(), String(currentValue).c_str());
	}

	// Realiza variação de tensão negativamente na saída e coleta dados
	for (int i = max_duty_cycle; i >= min_duty_cycle; i--){
		ledcWrite(pwm_channel, i);
		delay(intervalCollect);

		// Coleta dados de corrente e tensão para testes
		voltageValue = float(random(0, 330))/100.0;
		currentValue = float((random(0, 400) - 200))/100.0;

		// Salvando dados na memória interna no arquivo refernete ao fileName
		appendFileVoltammetry(SPIFFS, fileName, String(voltageValue).c_str(), String(currentValue).c_str());
	}
}

void sendDataToFirebase(fs::FS &fs, const char* path){
	/*
		Esta função é responsável por fazer uma varredura no arquivo de coletas e enviar os dados para o firebase
	*/
	Serial.printf("Reading file: %s\r\n", path);
	String leitura = "";
	bool voltageUp = true;

    File file = fs.open(path);

    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
		String valor = file.readStringUntil('\n');
		//Serial.println(valor);

		if (voltageUp){
			 if(Firebase.pushString(firebaseData, nodeVoltage, valor)){
				//Success
				//Serial.println("Set String data success");

			}else{
				//Failed?, get the error reason from firebaseData

				Serial.print("Error in setInt, ");
				Serial.println(firebaseData.errorReason());
			}

			//Serial.println("Voltage: " + valor + "V");
			voltageUp = false;
		}else{
			if(Firebase.pushString(firebaseData, nodeCurrent, valor)){
				//Success
				//Serial.println("Set String data success");

			}else{
				//Failed?, get the error reason from firebaseData

				Serial.print("Error in setInt, ");
				Serial.println(firebaseData.errorReason());
			}

			//Serial.println("Current: " + valor + "A");
			voltageUp = true;
		}
    }
}

void setup(){
	// Serial port for debugging purposes
	Serial.begin(115200);

	// Configura PWM para o filtro RC
	pwmConfig();

	// Initialize SPIFFS
	if (!SPIFFS.begin())
	{
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}

	FirebaseConfig();

	//testVoltammetry(1, 1.5, 2.5);
	listDir(SPIFFS, "/", 0);
	//readFile(SPIFFS, fileName);
	sendDataToFirebase(SPIFFS, fileName);
}

void loop(){

}