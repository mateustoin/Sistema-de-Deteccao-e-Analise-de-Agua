#include <Arduino.h>
#include <SPIFFS.h>

#define pinReadSignal 34
#define pinPwmOutput 13 
#define intervalCollect 100 // Intervalo de tempo entre mudanças de tensão em milissegundos (ms)

int signal_value = 0;

const int pwm_frequency = 31500; // Define frequência de saída do PWM (Hz)
const int pwm_channel = 0;		 // Define canal do PWM para a ESP32
const int pwm_resolution = 8;	 // Define uma resolução de 8 bits para PWM (0-255)

void pwmConfig(){
	ledcSetup(pwm_channel, pwm_frequency, pwm_resolution);
	ledcAttachPin(pinPwmOutput, pwm_channel);

	analogReadResolution(10); // Altera resolução da leitura analógica para 10 bits (0-1023)
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void startVoltammetry(int time_interval, float min_voltage, float max_voltage){
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

	int voltageValue = 0;
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
}

void loop(){
}