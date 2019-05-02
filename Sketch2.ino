/*
 Name:		CERROJO.ino
 Created:	11/04/2019 12:06:19
 Author:	Jorge
*/



#include <Servo.h>
Servo servoMotor; //Va con la libreria
#include <LiquidCrystal_I2C.h> //Actualizacion de libreria LiquidCrystal
LiquidCrystal_I2C lcd(0x27, 16, 2);//Va con la libreria
#include <Keypad.h> //Necesaria descarga libreria Keypad.h
#include <Key.h>
#define DELAY 400 
#define FILAS 4		//Filas del teclado matricial
#define COLUMNAS 3  //Columnas del teclado matricial

//Pines a los que estan conectados los componentes
#define buzzer 12
#define ledverde 9
#define ledrojo 10


typedef struct {
	char contrasenantigua[5];
}contrasenas;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	servoMotor.attach(11); //Pin del servomotor
	servoMotor.write(0);  //Posicion inicial del servomotor
	pinMode(buzzer, OUTPUT);
	pinMode(ledverde, OUTPUT);
	pinMode(ledrojo, OUTPUT);
	IniciarLCD();
}

// the loop function runs over and over again until power down or reset
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
	static int cont = 0, activar = 0, cerrar = 0, intentos = 0, incorrecto = 0, cambio = 0, cursor = 0, RESTART = 0, dim = 0, i = 0;
	static contrasenas *contant;
	static char contrasenaantigua[] = "    ", *pcontrasenaantigua; //contraseña antigua, se guardara cuando cambie la contraseña
	static char contrasena[] = "0000";  //aquí escribimos la contraseña de 4 dígitos
	static char codigo[] = "    ";            //Cadena donde se guardaran los caracteres de las teclas presionadas
	static char aceptar[] = " ";//Cadena aceptar
	pcontrasenaantigua = &contrasenaantigua[0];

	contant = (contrasenas*)malloc(sizeof(contrasenas));
	contant = (contrasenas*)calloc(5, sizeof(contrasenas));

	char teclas = keypad.getKey(); //se guarda en la variable teclas el caracter de la tecla presionada
	if (teclas)
	{
		if (cambio < 4 && teclas == '#')
		{
			cambio = 5;
		}
		if (cambio == 0 && teclas == '*')
		{
			lcd.clear();
			lcd.display();
			lcd.setCursor(4, 0);
			lcd.print("CAMBIAR");
			lcd.setCursor(7, 1);
			lcd.print("PIN");

		}
		if (cambio < 4 && teclas == '*')
		{
			contrasenaantigua[cambio] = contrasena[cambio];
			lcd.display();
			lcd.setCursor(2, 0);
			lcd.print("PIN ANTIGUO");
			lcd.setCursor(6, 1);
			lcd.print(contrasenaantigua);


			cambio = cambio + 1;
		}
		//Arreglar
		if (contrasenaantigua == contrasena)
		{
			//Arreglar
			*contant[dim].contrasenantigua = *pcontrasenaantigua;
			contant = contant + 1;
			dim++;
			contant = (contrasenas*)realloc(contant, dim * sizeof(contrasenas));
			Serial.print(contant->contrasenantigua);
		}
		//
		if (cambio == 4)
		{
			lcd.display();
			lcd.setCursor(2, 0);
			lcd.print(" PIN  NUEVO ");
			if (teclas == '*')
			{
				cursor = 0;
			}
			else
			{

				contrasena[cursor] = teclas;
				lcd.setCursor(6, 1);
				lcd.print(contrasena);

				cursor = cursor + 1;
			}
			if (cursor == 4)
			{
				lcd.clear();
				lcd.display();
				lcd.setCursor(3, 0);
				lcd.print("CONFIRMAR");
				lcd.setCursor(6, 1);
				lcd.print(contrasena);
				LedVerde();
				Apagarled();
				cambio = cambio + 1;
			}


		}
		if (teclas == '#' && cambio == 5)
		{
			cambio = cambio + 1;

			cont = 0;
			activar = 0;
			incorrecto = 0;

		}
		else if (teclas == '*' && cambio == 5)
		{
			lcd.clear();
			lcd.display();
			lcd.setCursor(4, 0);
			lcd.print("CAMBIAR");
			lcd.setCursor(7, 1);
			lcd.print("PIN");
			cambio = 0;
			cont = 0;
			activar = 0;
			incorrecto = 0;
		}

		if (cont < 4 && cambio == 6)
		{
			lcd.clear();
			IntroducePin();

			if (teclas == contrasena[cont] && teclas != '#' && teclas != '*')
			{
				codigo[cont] = contrasena[cont];

				lcd.setCursor(6, 1);
				lcd.print(codigo);

				activar = activar + 1;
			}
			if (teclas != contrasena[cont] && teclas != '#' && teclas != '*')
			{
				codigo[cont] = teclas;

				lcd.setCursor(6, 1);
				lcd.print(codigo);

				incorrecto = incorrecto + 1;
			}
			cont = cont + 1;
			if (teclas == '#')
			{
				cont = 0;
				incorrecto = 0;
			}
			if (teclas == '*'&&intentos != 3)
			{
				lcd.clear();
				lcd.display();
				lcd.setCursor(4, 0);
				lcd.print("CAMBIAR");
				lcd.setCursor(7, 1);
				lcd.print("PIN");
				cambio = 0;
				cont = 0;
				incorrecto = 0;
				activar = 0;
			}
		}

		if (cont == 4)
		{
			if (teclas == '#')
			{
				if (activar == 4)
				{
					aceptar[0] = teclas;
					cont = cont + 1;
				}
				if (incorrecto <= 4 && incorrecto > 0)
				{
					LedRojo1();
					Apagarled();
					clearLCD();
					cont = 0;
					activar = 0;
					incorrecto = 0;
					intentos = intentos + 1;
				}
			}
		}
	}
	if (aceptar[0] == '#'&&codigo[0] == contrasena[0] && codigo[1] == contrasena[1] && codigo[2] == contrasena[2] && codigo[3] == contrasena[3])
	{
		LedVerde();
		lcd.clear();
		lcd.display();
		lcd.setCursor(3, 0);
		lcd.print("CONFIRMADO");

		abrir();
		if (teclas == '*')
		{
			Apagarled();
			cierra();
			RESTART = 1;
			aceptar[0] = teclas;
		}
	}
	if (intentos == 3)
	{
		tone(buzzer, 1000);
		delay(350);
		noTone(buzzer);
		delay(150);
		LedRojo();
		lcd.clear();
		lcd.setCursor(4, 0);
		lcd.print("DENEGADO");

		cierra();
		if (teclas == '*')
		{
			Apagarled();
			RESTART = 1;
		}
	}
	if (RESTART == 1)
	{
		cont = 0;
		cambio = 6;
		activar = 0;
		incorrecto = 0;
		intentos = 0;
		i = 0;
		lcd.clear();
		IntroducePin();
	}
	RESTART = 0;

}
void IniciarLCD()
{
	lcd.init();
	lcd.backlight();
	lcd.clear();
	lcd.display();
	lcd.setCursor(2, 0);
	lcd.print("PIN  INICIAL");
	lcd.setCursor(6, 1);
	lcd.print("0000");
}
void IntroducePin()
{
	lcd.setCursor(1, 0);
	lcd.print("INTRODUCE  PIN");
	lcd.setCursor(6, 1);
	lcd.print("----");

}
void clearLCD()
{
	lcd.setCursor(6, 1);
	lcd.print("       ");
	lcd.display();
}
void LedRojo1()
{
	digitalWrite(ledrojo, HIGH);
	delay(1000);
	color(255, 0);

}
void LedRojo()
{
	digitalWrite(ledrojo, HIGH);
	color(255, 0);
	delay(200);
	digitalWrite(ledrojo, LOW);

}
void Apagarled()
{
	digitalWrite(ledrojo, LOW);
	digitalWrite(ledverde, LOW);
}
void LedVerde()
{
	digitalWrite(ledverde, HIGH);
	color(0, 255);
}
void color(int valorrojo, int valorverde)
{
	digitalWrite(ledrojo, valorrojo);
	digitalWrite(ledverde, valorverde);
}
void abrir()
{
	servoMotor.write(90);
}
void cierra()
{
	servoMotor.write(0);
}