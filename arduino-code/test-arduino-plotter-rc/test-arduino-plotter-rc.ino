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