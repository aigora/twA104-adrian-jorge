/*
 Name:		Sketch3.ino
 Created:	16/04/2019 11:16:22
 Author:	Adri
*/

#include <Wire.h>
#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 20, 4);


#define FILAS 4
#define COLUMNAS 3
Servo servomotor;
char contrasenaantigua[] = "0000";
char contrasena[] = "0000";  //aquí escribimos la contraseña de 4 dígitos
char codigo[4];            //Cadena donde se guardaran los caracteres de las teclas presionadas
char aceptar[] = " ";	  //Cadena aceptar
int cont = 0, activar = 1, cerrar = 0, intentos = 0, incorrecto = 0;

char teclado[FILAS][COLUMNAS] = {
{ '1','2','3' },
{ '4','5','6' },
{ '7','8','9' },
{ '*','0','#' }
};
byte pinColumnas[COLUMNAS] = { 2,3,4 };
byte pinFilas[FILAS] = { 5,6,7,8 };
Keypad keypad = Keypad{ makeKeymap(teclado), pinFilas, pinColumnas, FILAS, COLUMNAS };

void loop()
{
char teclas = keypad.getKey(); //se guarda en la variable teclas el caracter de la tecla presionada
if (teclas)
{
	if (cont < 4)
	{
		if (teclas == contrasena[cont])
		{
			codigo[cont] = contrasena[cont];
			Serial.print(codigo);
			activar = activar + 1;
		}
		else
		{
			codigo[cont] = teclas;
			Serial.print(codigo);
			incorrecto = incorrecto + 1;
		}
	}
}

