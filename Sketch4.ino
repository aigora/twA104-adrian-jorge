

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
		
		static char contrasenaantigua[] = "    "; //contraseña antigua, se guardara cuando cambie la contraseña
		static char contrasena[] = "0000";  //aquí escribimos la contraseña de 4 dígitos
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
				Serial.print("\nCONTRASEÑA ANTIGUA: ");
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
					Serial.print("\nCONTRASEÑA NUEVA: ");
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
