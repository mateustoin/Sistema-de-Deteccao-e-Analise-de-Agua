# Sistema de Deteccao e Analise de Soluções Salinas na Água

Elaboração de um projeto capaz de detectar e analisar soluções com a presença de Sal, a fim de utilizar a técnica de Voltametria Cíclica e coletar dados.

# Índice
+ [Sobre](#sobre)
+ [Circuito](#circuito)
+ [Placa de Circuito Impresso](#pcb)
	- [Parte 1](#circuito-pt1)
	- [Parte 2](#circuito-pt2)
	- [Parte 3](#circuito-pt3)
	- [Parte 4](#circuito-pt4)
+ [Features Bônus](#bonus)
    - [Feature Bônus 1](#bonus1)
	- [Feature Bônus 2](#bonus2)
	- [Feature Bônus 3](#bonus3)
	- [Feature Bônus 4](#bonus4)
+ [Planos Futuros](#futuro)
+ [Referências Bibliográficas](#referencias)

<h1 id="sobre">Sobre</h2>

O objetivo desse projeto é realizar a detecção de Sal na água e realizar uma técnica chamada Voltametria Cíclica. A voltametria cíclica consiste em aplicar um potencial sobre o eletrodo de trabalho e medir a corrente elétrica originada pela variação desse potencial. Para isso, foi criado um potenciostato simples, que é um instrumento capaz de realizar a medição de corrente a partir do potencial, mencionado anteriormente. 

O artigo entitulado "Building a Microcontroller Based Potentiostat: A Inexpensive and Versatile Platform for Teaching Electrochemistry and Instrumentation" [1] foi indispensável para entender o processo de funcionamento e ter as primeiras noções de como o circuito funciona. Segundo Meloni, potenciostatos são dispositivos simples que dependem de amplificadores operacionais para manter a diferença de potencial necessária entre os dois eletrodos (eletrodo de trabalho e referÊncia), imersos na solução enquanto todos os dados de corrente elétricas são gravados. 

O circuito costruído para este experimento, que pode ser visto na figura a seguir, possui adaptações feitas pelo autor desse trabalho para simplificar o processo baseado na saída do Arduino e também no trabalho "Building an Arduino based potentiostat and Instrumentation for Cyclic Voltammetry" [2], que deixa um pouco mais claro o papel dos eletrodos no sistema.

<div align='center'>
    <img src="img/circuito-final-arduino.BMP">
    <p>Figura 1. Versão inicial de testes funcional</p>
</div>

Os testes iniciais de validação foram feitos com um Arduino Nano, a fim de validar a saída da placa como sendo funcional para um potenciostato. Para a versão final do circuito, foi selecionada a placa NodeMCU ESP32, visto que para aplicações com coleta de dados, integração com internet, consumo de energia e capacidade de processamento são extremamente superiores as do Arduino. Além disso, algumas de suas características suprem certas necessidades que serão discutidas na seção a seguir.

<h1 id="circuito">Esquemático do Circuito</h2>

Para a montagem e simulação do circuito foi utilizado o Software [4]. O uso de um software pago se deu por problemas técnicos e tempo para desenvolvimento. 

Como foi dito, a versão final do circuito envolve o uso de uma NodeMCU com ESP32, como pode ser visto no esquemático a seguir. Devido a falta de esquemáticos específicos para a versão requerida, foi utilizado um modelo de versão anterior, mas na prática o modelo utilizado é o mostrado na outra figura.

<div align='center'>
    <img src="img/circuit-nodemcu-bitmap.bmp">
    <p>Figura 2. Versão final do circuito com ESP32</p>
</div>

<div align='center'>
    <img width=300 src="img/esp32.jpg">
    <p>Figura 3. Placa com ESP32 idealizada para a versão real</p>
</div>

O circuito pode ser dividido em partes:

- Parte 1: Preparo do sinal da placa para o potenciostato
- Parte 2: Mudança no offset do nível de tensão para se adequar a diferença de potencial necessária
- Parte 3: Transformar a corrente gerada em tensão

<h2 id='circuito-pt1'><b>Parte 1: Preparo do sinal</b></h3>

Naturalmente uma variação de tensão na saída de uma placa de desenvolvimento se dá através do PWM (do inglês, Modulação por largura de pulso). O PWM gera um sinal com ondas quadradas onde ocorre uma variação apenas no tempo em que a onda passa em nível lógico 1 (conhecido como Duty Cicle). Essa variação, como o próprio nome indica, altera a largura do pulso dado um período de tempo. Um exemplo de sinal com diferentes larguras pode ser visto a seguir:

<div align='center'>
    <img src="img/pwm.png">
    <p>Figura 4. PWM de uma placa</p>
</div>

Esse sinal precisa ser convertido em uma tensão constante, de acordo com as variações do PWM, para que a medição seja precisa e ocorra normalmente, já que é necessário que o comportamento da corrente em relação a diferença de potencial seja estável. Para esse teste foi utilizado um Arduino real para o teste, com valores de resistor e capacitor iguais ao do simulador. O experimento consiste em mostrar que uma onda quadrada pode ter como saída uma triangular, dada uma variação de PWM, com o objetivo de se obter uma tensão DC correspondente ao PWM. 

<div align="center">
    <p float="left">
        <img width=250 src="img/foto_circuito.jpg" alt="descrição da imagem">
        <img width=250 src="img/pt1-simul/onda-dc.gif" alt="descrição da imagem">
    <p>Figura 5. (a) Circuito real para teste do filtro passa-baixo (b) Saída da tensão gerada pelo próprio Arduino</p>
    </p>
</div>

Para a obtenção desse resultado foi utilizado um filtro passa-baixo, que deixa apenas as frequências mais baixas do sinal passarem. Além disso foi necessária uma pequena modificação no código do Arduino para alterar a frequência de saída do sinal PWM, pois por padrão a frequência é baixa. Foi aumentada para 31 kHz. Como dito, a versão final deste projeto utilizaria uma ESP32, que não teria esse problema, pois além de ter uma frequência ampla e configurável, possui também seu próprio circuito conversor digital analógico, facilitando o trabalho. Ainda assim essa parte do circuito é importante para manter as possibilidades de projeto e aplicações.

Na figura 5(b) pode ser observado que os valores vão de quase 0 até acima de 900. Isso ocorre pois o conversor Analógico Digital do Arduino (ADC, do inglês Analogic Digital Converter) possui 10 bits de resolução, ou seja, de acordo com a tensão de entrada (0-5V), captura valores de 0 até 1023. Portanto, os resultados condizem com a saída de PWM.

<div align='center'>
    <img src="img/pt1-simul/circuito-arduino-nano-passa-baixo-test-A0.png">
    <p>Figura 6. Circuito com filtro passa-baixo</p>
</div>

Para a decisão da frequência de corte a fórmula a seguir foi utilizada:

> Fc = 1/(2 * pi * R * C)

O código utilizado para essa validação consiste em variar o PWM do 0 até o máximo, para obter a tensão mínima e máxima de saída, em forma de onda triangular. A mudança na frequência pode ser consultada em [3], pois outras modificações de frequência também podem ser utilizadas dependendo do caso.

```cpp
// Código criado para ler resultado do filtro passa-baixo e plotar utilizando Serial Plotter
#define PWM_OUTPUT 10
#define analogico A0

void setup() {
  // Inicia porta Serial
  Serial.begin(9600);

  // Configura pinos
  pinMode(PWM_OUTPUT, OUTPUT);
  pinMode(analogico, INPUT);
  
  TCCR1B = TCCR1B & B11111000 | B00000001; //Muda a frequência PWM da porta 10 do Arduino para 31 kHz
}

void loop() {
  // Altera Duty Cicle do PWM de 0% até 100%
  for (int i = 0; i <= 255; i++){
    analogWrite(PWM_OUTPUT, i);
    Serial.println(analogRead(analogico));
    Serial.print(" ");
  }

  // Altera Duty Cicle do PWM de 100% até 0%
  for (int i = 255; i >= 0; i--){
    analogWrite(PWM_OUTPUT, i);
    Serial.println(analogRead(analogico));
    Serial.print(" ");
  }
}
```

<h2 id='circuito-pt2'><b>Parte 2: Mudança do offset</b></h3>

A mudança no offset é necessária para o funcionamento do potenciostato, pois tensões negativas são necessárias para que as variações de um ciclo possam ir do pico negativo até o pico positivo de corrente. Essa saída vai diretamente para os eletrodos de trabalho (WE, do inglês Working Electrode) e eletrodo de referência (RE, do inglês Reference Electrode). 

Dito isso, o circuito responsável pelo offset pode ser visto a seguir:

<div align='center'>
    <img src="img/pt2-simul/Circuito22.png">
    <p>Figura 7. Alteração de offset e saída de eletrodos</p>
</div>

Esses eletrodos serão os responsáveis por gerar a diferença de potencial na solução. Segundo Joshi [2], o Buffer após a correção de offset é necessário para garantir que o potencial nos dois eletrodos (WE e RE) se mantenham iguais e estáveis, nos valores necessários. Essa é uma das principais e qualidades requeridas para que a análise de Voltametria Cíclica seja devidamente realizada.

O circuito de offset possui um potênciometro para que possam ocorrer ajustes na tensão. O trabalho dele é basicamente aumentar e diminuir o nível de tensão para se ajustar ao necessário. Por exemplo, se a entrada baseada na tensão DC que vem do filtro for de 0 até 2V, o circuito de offset deve ser capaz de transformar essa tensão em -1V até 1V.

<div align="center">
    <p float="left">
        <img width=250 src="img/pt2-simul/saida-onda-triangular.png" alt="descrição da imagem">
        <img width=250 src="img/pt2-simul/saida-onda-triangular-offset.png" alt="descrição da imagem">
    <p>Figura 8. Offset para alteração dos intervalos de tensão</p>
    </p>
</div>

<h2 id='circuito-pt3'><b>Parte 3: Transformação de corrente em tensão</b></h3>

Essa fase implica em utilizar um terceiro eletrodo (CE, do inglês Couter Electrode) para captar toda a corrente elétrica gerada pela diferença de potencial dos eletrodos anteriores. Essa corrente, que vai de negativa até positiva, é transformada em tensão para que a placa possa identificar os resultados e gerar gráficos, para assim finalizar a análise do processo.

<div align='center'>
    <img src="img/pt3-simul/circuito.png">
    <p>Figura 9. Amplificador de transresistência ou conversor corrente-tensão</p>
</div>

Os cálculos foram feitos para que, de acordo com a tensão de entrada e resistores, a saída máxima de tensão seja de 3,3V. Isso foi decidido para que tanto Arduinos quanto ESP's, que só trabalham a um nível de tensão de 3,3V, possam ser utilizados para medir o que é necessário para o projeto. A tabela verdade para o circuito pode ser vista a seguir:


<div align='center'>
    <div>
    <table>
    <tbody>
    <tr>
    <td>Entrada</td>
    <td>Sa&iacute;da</td>
    </tr>
    <tr>
    <td>-200uA</td>
    <td>0V</td>
    </tr>
    <tr>
    <td>200uA</td>
    <td>3.3V</td>
    </tr>
    </tbody>
    </table>
    </div>

<p>Tabela 1. Tabela verdade para valores de corrente</p>
</div>

Obs.: Os valores de corrente simulados para essa situação foram baseados no trabalho de [1], visto que, de acordo com a solução testada, as variações de tensão entre -1V e 1V foram utilizadas, já que eram o suficiente para a análise dos resultados pretendidos. Visto que a situação simulada não seria diferente, foram utilizados os mesmos valores como parâmetros para cálculos e testes de simulação.

<h2 id='circuito-pt4'><b>Lista de Materiais</b></h3>

<div align='center'>
    <div>
    <table>
<tbody>
<tr>
<td>Qnt.</td>
<td>Produto</td>
</tr>

<tr>
<td>1</td>
<td>*Arduino Nano</td>
</tr>

<tr>
<td>1</td>
<td>*NodeMCU ESP32</td>
</tr>

<tr>
<td>4</td>
<td>LM239</td>
</tr>

<tr>
<td>3</td>
<td>
<p>Resistor 4.7kΩ</p>
</td>
</tr>

<tr>
<td>2</td>
<td>Resistor 8.2kΩ</td>
</tr>

<tr>
<td>1</td>
<td>
<p>Pot&ecirc;nciometro 30kΩ</p>
</td>
</tr>

<tr>
<td>1</td>
<td>Capacitor 10uF</td>
</tr>

<tr>
<td>2</td>
<td>Pin Header 2.54mm Single Row Female</td>
</tr>

<tr>
<td>2</td>
<td>Conector T Block 3 pins</td>
</tr>

<tr>
<td>1</td>
<td>Conector T Block 2 pins</td>
</tr>

</tbody>
</table>
    </div>

<p>Tabela 2. Lista de materiais utilizados no projeto</p>
</div>

<p><i>* Pode ser uma ou outra placa</i></p>

<h2 id="pcb">Placa de Circuito Impresso</h2>

<div align="center">
    <p float="left">
        <img width=250 src="img/pcb/pcb_schema.png" alt="descrição da imagem">
        <img width=350 src="img/pcb/final-pcb-nodemcu-front.png">
    <p>Figura 9. Design e placa de circuito impresso</p>
    </p>
</div>

<h1 id='bonus'>Features Bônus</h3>

Diversas abordagens podem ser tomadas para o desenvolvimento dessas features. Desde bancos de dados NoSQL até SQL, com diagramas ER (Entidade-Relacionamento) para exemplificar. Entretanto, devido a limitação de tempo Foram utilizadas tecnologias um pouco mais rápidas, mas também muito poderosas. Para a construção do Banco de Dados para armazenamento dos arquivos e coleta de dados para criação de tabelas CSV e gráficos, foi escolhido o *Real Time Database* do *Firebase*, que é uma plataforma de serviços do google que pode ser diretamente integrado com a ESP32.

Para o desenvolvimento de uma versão final do produto foi idealizado a construção de uma API, que seria o intermediário perfeito para uma aplicação desktop ou Web para controle e gerenciamento das coletas de dados, além da requisição de dados para exibição e organização. 

<h2 id='bonus1'><b>Permitir que o operador do hardware possa utilizar a técnica de voltametria
cíclica para análise da substância aquosa</b></h2>

Para esta feature, a coleta de dados é realizada diretamente da ESP32 e armazenada em sua memória interna. A memória interna possui espaço suficiente para muitas coletas, então memso que não haja conexão com internet ou com algum computador, é possível realizar as coletas necessárias e recuperá-las depois. A seguinte função foi desenvolvida para realizar a análise e armazenamento:

```cpp
void startVoltammetry(int time_interval, float min_voltage, float max_voltage, String fileName);
```

O tempo de intervalo (*time_interval*) pode variar para se atingir melhores resultados, visto que, dependendo deste intervalo, a tensão de saída pode variar por causa do filtro RC passa-baixo. O tempo de subida e descida da tensão podem variar conforme o intervalo, melhorando ou piorando a qualidade da coleta. Já os parâmetros *min_voltage* e *max_voltage* dizem respeito a variação de tensão que se deseja obter como entrada na solução, para que os eletrodos gerem a diferença de potencial necessária. Por último o *fileName* é responsável por nomear o arquivo que serão guardados todos os dados da coleta.

```cpp
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

		// Salvando dados na memória interna no arquivo refernete ao fileName
		appendFileVoltammetry(SPIFFS, fileName, String(voltageValue).c_str(), String(currentValue).c_str());
	}
}
```

O resto do código pode ser visto <a src="src/main.cpp">aqui</a>. Foi criada uma função de teste de Voltametria Cíclica, apenas para gerar valores teste para que possam ser utilizadas para integrar com o *Real Time Database* (RDB).

A função a seguir acessa diretamente a memória interna da ESP para pegar os dados coletados e enviar para o banco de dados online. Desta forma é possível conectar os outros serviços para geração de *tabelas CSV* e *Gráficos*.

```cpp
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
```

Com isso, assim que a varredura da memória é ativada, todos os dados de *Tensão* e *Corrente* resultantes da análise de voltametria cíclica já ficam visíveis na web. Não há necessidade de nenhuma configuração por parte do banco de dados, pois quando há a identificação de novas coletas os nós de dados são criados automaticamente no RDB.

<div align="center">
    <img width=250 src="img/RDB.gif" alt="descrição da imagem">
    <p>Figura 10. Banco de Dados sendo atualizado em tempo real com dados de Corrente e Tensão</p>
    </p>
</div>

<h2 id='bonus2'>
<b>Exportar em forma de gráficos os dados de leitura da técnica de
voltametria cíclica</b>
</h2>

A geração de gráficos e tabelas CSV ficariam inclusos na Interface Gráfica. Devido ao tempo limitado, foram desenvolvidos como serviços separados em um Script em python, que substitui a API que integraria todas as partes do sistema. Dito isso, o acesso ao Firebase é feito diretamente do código, com a configuração do projeto e acesso aos nós de dados. As bibliotecas utilizadas para o acesso e geração de gráficos foram *pyrebase* e *matplotlib*.

```python
# Importa bibliotecas
import pyrebase
import matplotlib.pyplot as plt

# Todas as informações de configuração para acessar o Real Time Database
firebaseConfig = {
    "apiKey": "AIzaSyBEHvCZQpcogdq1o_9BEPW3xgh9Tq0vdB4",
    "authDomain": "cyclic-voltammetry-fe847.firebaseapp.com",
    "databaseURL": "https://cyclic-voltammetry-fe847-default-rtdb.firebaseio.com",
    "projectId": "cyclic-voltammetry-fe847",
    "storageBucket": "cyclic-voltammetry-fe847.appspot.com",
    "messagingSenderId": "183536016835",
    "appId": "1:183536016835:web:cdec2eb19b14ed6b8e7176"
}

# Inicializa instância do Firebase e cria um objeto do Banco de Dados
firebase=pyrebase.initialize_app(firebaseConfig)

db=firebase.database()

# Acessa os nós do Banco de Dados que contém os dados enviados pela ESP
result_voltage = db.child("coletas").child("voltage").get()
result_current = db.child("coletas").child("current").get()

voltage_data = []
current_data = []

for value in result_voltage.each():
    voltage_data.append(value.val())

for value in result_current.each():
    current_data.append(value.val())

# Configura gráfico para exibir os dados
plt.title("Gráfico teste Voltametria Cíclica")

#plt.scatter(voltage_data, current_data, color='darkblue', marker='x')
plt.plot(voltage_data, current_data)

plt.xlabel("Voltage (V)")
plt.ylabel("Current (A)")

plt.grid(True)

plt.show()
```

Para este exemplo de geração de gráficos, dados aleatórios foram gerados pela ESP e enviados ao Banco de Dados para testes. Portanto **os valores a seguir não correspondem a uma análise de voltametria cíclica**, servem apenas para mostrar que após a análise a obtenção dos dados e exibição dos mesmos em gráficos é totalmente possível. A figura pode ser vista a seguir:

<div align="center">
    <img src="img/voltametria-teste-rdb.png" alt="descrição da imagem">
    <p>Figura 11. Dados aleatórios gerados pela ESP para fins de simulação do envio de dados e obtenção no RDB</p>
    </p>
</div>

<h2 id='bonus3'>
<b>Exportar em forma de arquivo CSV os dados de leitura da técnica de
voltametria cíclica</b>
</h2>

A geração da tabela segue os mesmos princípios de funcionamento dos gráficos. Seria um sistema integrado, mas foi implementado separadamente para demonstrar o funcionamento extraindo dados direto do RDB. Esses dados que por sua vez foram gerados na ESP, que seriam pegos diretamente do resultado de uma análise voltamétrica. A seguir pode ser visto o código, parecido com o anterior, mas com a geração de tabelas CSV.

```python
# Importa bibliotecas
import pyrebase
import matplotlib.pyplot as plt

# Todas as informações de configuração para acessar o Real Time Database
firebaseConfig = {
    "apiKey": "AIzaSyBEHvCZQpcogdq1o_9BEPW3xgh9Tq0vdB4",
    "authDomain": "cyclic-voltammetry-fe847.firebaseapp.com",
    "databaseURL": "https://cyclic-voltammetry-fe847-default-rtdb.firebaseio.com",
    "projectId": "cyclic-voltammetry-fe847",
    "storageBucket": "cyclic-voltammetry-fe847.appspot.com",
    "messagingSenderId": "183536016835",
    "appId": "1:183536016835:web:cdec2eb19b14ed6b8e7176"
}

# Inicializa instância do Firebase e cria um objeto do Banco de Dados
firebase=pyrebase.initialize_app(firebaseConfig)

db=firebase.database()

# Acessa os nós do Banco de Dados que contém os dados enviados pela ESP
result_voltage = db.child("coletas").child("voltage").get()
result_current = db.child("coletas").child("current").get()

voltage_data = []
current_data = []

for value in result_voltage.each():
    voltage_data.append(value.val())

for value in result_current.each():
    current_data.append(value.val())

# Cria colunas com os dados extraídos do RDB
df = pd.DataFrame({'Coleta1-tensão':voltage_data,
                   'Coleta1-corrente':current_data})
                  
# Salva tabela
df.to_csv('voltametria.csv')
```

Um gif foi gerado para mostrar como é rápido e simples para a formação de tabelas.

<div align="center">
    <img width=250 src="img/firebase-to-csv.gif" alt="descrição da imagem">
    <p>Figura 12. Geração de tabela CSV a partir do RDB</p>
    </p>
</div>

<h2 id='bonus4'>
<b>Interface gráfica para operação e visualização do resultado da análise</b>
</h2>

Infelizmente não foi possível finalizar a interface gráfica, porém ela foi idealizada e pode ser explicada. O sistema final iria possuir uma API capaz de oferecer os serviços de geração de dados para gráficos, tabelas CSV e quaisquer outros que pudessem ser requeridos. Esses serviços seriam fornecidos no formato de microsserviços, a fim de fornecer uma maior versatilidade na construção do produto e evolução, ajudando também na manutenção do sistema.

A interface gráfica poderia ser feita utilizando frameworks multiplataforma como Qt, Electron, entre outros, porém a decisão escolhida para o projeto e para o tempo limitado foi uma página Web fornecida diretamente em um WebServer com a ESP32. Essa página Web seria simples, com um gráfico para exibição dos resultados da análise de voltametria, com botões para iniciar análise, parar, atualizar gráfico e alterar nome da análise. A página começou a ser feita, tinha capacidade para receber dados e exibir um gráfico em tempo real, porém não foram implementados os botões e suas funcionalidades. Uma figura do protótipo funcional ainda em desenvolvimento pode ser visto a seguir:

<div align="center">
    <img src="img/pagina-web-server.png" alt="descrição da imagem">
    <p>Figura 13. Página Web para interface gráfica em estágio inicial de desenvolvimento</p>
    </p>
</div>

<h1 id="futuro">Planos Futuros</h1>

Além do desenvolvimento da API para oferecer o projeto através de microsserviços, uma outra possibilidade de melhoria seria utilizar RTOS (Real Time Operational System) para Microcontroladores, suportado pela ESP32. A ESP32 é Dual Core, isso significa que possui paralelismo real de processamento e além de barata, poderia ser utilizada em diversas situações, dependendo de como o sistema será aplicado na prática. Desde conexão local a conexão remota, ela seria capaz de processar os dados, se conectar com a web, enquanto fornece outros serviços locais. É uma placa poderosa, com baixo consumo de energia e pode ser utilizada para diversos fins. Além do WiFi possui também Bluetooth, permitindo que também possa ser conectada a aplicativos offline.

Algumas melhorias também podem ser feitas no circuito, visto que a entrada de tensão para alimentação do ICVS (parte responsável por transformar corrente em tensão) pode ser reaproveitada do próprio Circuito ou ESP. Além disso, algumas melhorias também podem ser feitas em relação ao Off-Set da tensão de saída do Filtro, deixando-o mais variável para alterações em tempo real.

<h1 id="referencias">Referências Bibliográficas</h1> 

[1] Meloni, Gabriel N. “Building a Microcontroller Based Potentiostat: A Inexpensive and Versatile Platform for Teaching Electrochemistry and Instrumentation”. Journal of Chemical Education, vol. 93, no 7, julho de 2016, p. 1320–22. DOI.org (Crossref), doi:10.1021/acs.jchemed.5b00961.

[2] Joshi, Peetri, e S. Sutrave. “Building an Arduino based potentiostat and Instrumentation for Cyclic Voltammetry”. Journal of Applied Science and Computations, vol. V, no XII, Dezembro de 2018, p. 163–67.

[3] Arduino Playground - TimerPWMCheatsheet. https://playground.arduino.cc/Main/TimerPWMCheatsheet/. Acessado 20 de dezembro de 2020.

[4] Proteus. 8.6, Labcenter.