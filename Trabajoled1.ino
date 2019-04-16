
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Key.h>
#define longcontra 4
#define ledrojo 9
#define ledverde 10
void setup() {
	pinMode(ledrojo, OUTPUT);
	pinMode(ledverde, OUTPUT); // Configurado el pin 10 como salida
}
void color(int valorrojo, int valorverde)
{
	analogWrite(ledrojo, valorrojo);
	analogWrite(ledverde, valorverde);
}

// the loop function runs over and over again until power down or reset
void loop() {
	int i, v1[longcontra];
	char contra[longcontra] = { '1','2','3','4' };
	char teclas[longcontra];
	if (contra[0] != teclas[0]) {
		color(255, 0);
		exit(-1);
	}
	else
		color(0, 255);
	if (contra[1] != teclas[1]) {
		color(255, 0);
		exit(-1);
	}
	else
		color(0, 255);
	if (contra[2] != teclas[2]) {
		color(255, 0);
		exit(-1);
	}
	else
		color(0, 255);
	if (contra[3] != teclas[3]) {
		color(255, 0);
	}
	else
		color(0, 255);
}