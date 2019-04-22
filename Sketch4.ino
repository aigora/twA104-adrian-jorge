/*
 Name:		Sketch4.ino
 Created:	4/16/2019 2:44:07 PM
 Author:	Adri
*/
// Inclu�mos la librer�a para poder controlar el servo

#include <Keypad.h>
#include <Key.h>
void setup() {

	Serial.begin(9600);

}



void loop() {
	void loop() {

		//Definir el teclado
		static char teclado[FILAS][COLUMNAS] = {
		{ '1','2','3' },
		{ '4','5','6' },
		{ '7','8','9' },
		{ '*','0','#' }
		};
		static byte pinColumnas[COLUMNAS] = { 4,3,2 };
		static byte pinFilas[FILAS] = { 8,7,6,5 };
		static Keypad keypad = Keypad{ makeKeymap(teclado), pinFilas, pinColumnas, FILAS, COLUMNAS }; //rastreo del teclado

		//variables
		
		static char contrasenaantigua[] = "    "; //contrase�a antigua, se guardara cuando cambie la contrase�a
		static char contrasena[] = "0000";  //aqu� escribimos la contrase�a de 4 d�gitos
		static int cambio = 0, cursor = 0;
		
		char teclas = keypad.getKey(); //se guarda en la variable teclas el caracter de la tecla presionada
		if (teclas)
		{
			if (cambio < 4 && teclas == '#')
			{
				cambio = 5;
			}
			if (cambio < 4 && teclas == '*')
			{
				Serial.print("\nCONTRASE�A ANTIGUA: ");
				contrasenaantigua[cambio] = contrasena[cambio];
				Serial.print(contrasenaantigua);

				cambio = cambio + 1;
			}
			if (cambio == 4)
			{
				if (teclas == '*')
				{
					cursor = 0;
				}
				else
				{
					Serial.print("\nCONTRASE�A NUEVA: ");
					contrasena[cursor] = teclas;
					Serial.print(contrasena);

					cursor = cursor + 1;
				}
				if (cursor == 4)
				{
					cambio = cambio + 1;
				}
			}

}
