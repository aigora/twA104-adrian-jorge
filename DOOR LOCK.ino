#include <Servo.h>
Servo servoMotor;
#include <LiquidCrystal_I2C.h> //Actualizacion de libreria LiquidCrystal
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Keypad.h> //Necesaria descarga libreria Keypad.h
#include <Key.h>

#define DELAY 400 
#define FILAS 4		//Filas del teclado matricial
#define COLUMNAS 3  //Columnas del teclado matricial

//Pines a los que estan conectados los componentes
#define buzzer 12
#define ledverde 9
#define ledrojo 10

// the setup function runs once when you press reset or power the board
void setup() {

	Serial.begin(9600);

	servoMotor.attach(11); //Pin del servomotor
	pinMode(buzzer, OUTPUT);
	pinMode(ledverde, OUTPUT);
	pinMode(ledrojo, OUTPUT);

	servoMotor.write(0);  //Posicion inicial del servomotor
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
	static int cont = 0, activar = 0, cerrar = 0, intentos = 0, incorrecto = 0, cambio = 0, cursor = 0, RESTART = 0, dim = 1, i = 6, error = 0;
	static char contrasenaantigua[] = "    ", *pcontrasenaantigua; //contraseña antigua, se guardara cuando cambie la contraseña
	static char contrasena[] = "0000";  //aquí escribimos la contraseña de 4 dígitos
	static char codigo[] = "    ";            //Cadena donde se guardaran los caracteres de las teclas presionadas
	static char aceptar[] = " ";//Cadena aceptar

	pcontrasenaantigua = &contrasenaantigua[0];

	char teclas = keypad.getKey(); //se guarda en la variable teclas el caracter de la tecla presionada

	if (teclas)
	{
		if (cambio == 0 && teclas == '#') //Continuar con pin inicial
		{
			cambio = 8;
		}
		if (cambio == 0 && teclas == '*') //Cambiar pin
		{
			lcd.clear();

			cursor = 0;
			cambio = cambio + 1;
		}
		if (cambio < 5 && cambio > 0) //Cambiar la contraseña, pide contraseña antigua para establecer la nueva
		{
			lcd.display();
			lcd.setCursor(2, 0);
			lcd.print("PIN ANTIGUO");

			if (teclas == contrasena[cursor] && teclas != '#' && teclas != '*')
			{
				contrasenaantigua[cursor] = contrasena[cursor];

				lcd.setCursor(i, 1);
				lcd.print(teclas);
			}
			if (teclas != contrasena[cursor] && teclas != '#' && teclas != '*')
			{
				contrasenaantigua[cursor] = teclas;

				lcd.setCursor(i, 1);
				lcd.print(teclas);

				error = error + 1;
			}

			i = i + 1;
			cursor = cursor + 1;
			cambio = cambio + 1;

			if (teclas == '*' || teclas == '#')
			{
				clearLCD();

				i = 6;
				cursor = 0;
				cambio = 1;
			}
		}
		if (cambio == 5)
		{
			if (cursor == 4)
			{
				if (error == 0) //Contraseña antigua correcta
				{
					if (teclas == '#')
					{
						clearLCD();

						i = 6;
						cursor = 0;
						cambio = cambio + 1;

					}
				}
				if (error != 0) //Contraseña antigua incorrecta, vuelve a pedirla
				{
					if (teclas == '#')
					{
						clearLCD();

						i = 6;
						cursor = 0;
						cambio = 1;
						error = 0;
					}
				}
			}
		}

		if (cambio == 6) //Pedir nueva contraseña
		{
			lcd.display();
			lcd.setCursor(2, 0);
			lcd.print(" PIN  NUEVO ");

			if (teclas == '#' || teclas == '*')
			{
				i = 6;
				cursor = 0;

				clearLCD();
			}
			else
			{
				contrasena[cursor] = teclas;

				lcd.setCursor(i, 1);
				lcd.print(teclas);

				i = i + 1;
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
		if (cambio == 7) //Confirmar nueva contraseña para empezar a usarla
		{
			if (teclas == '#')
			{
				cambio = cambio + 1;
				i = 6;
				cont = 0;
				activar = 0;
				incorrecto = 0;
			}
			if (teclas == '*' && intentos != 3) //Volver a cambiar contraseña
			{
				lcd.clear();
				lcd.display();
				lcd.setCursor(4, 0);
				lcd.print("CAMBIAR");
				lcd.setCursor(7, 1);
				lcd.print("PIN");

				cursor = 0;
				cambio = 0;
				cont = 0;
				activar = 0;
				incorrecto = 0;
				error = 0;
				i = 6;
			}
		}
		//Abrir el cerrojo
		if (cambio == 8)
		{
			if (cont < 4) //Pide el pin
			{
				lcd.clear();
				IntroducePin();

				if (teclas == contrasena[cont] && teclas != '#' && teclas != '*')
				{
					codigo[cont] = contrasena[cont];

					lcd.setCursor(i, 1);
					lcd.print(teclas);

					activar = activar + 1;
				}
				if (teclas != contrasena[cont] && teclas != '#' && teclas != '*')
				{
					codigo[cont] = teclas;

					lcd.setCursor(i, 1);
					lcd.print(teclas);

					incorrecto = incorrecto + 1;
				}

				i = i + 1;
				cont = cont + 1;

				if (teclas == '#') //Borrar pin puesto, vuele a pedirlo
				{
					i = 6;
					cont = 0;
					incorrecto = 0;
				}
			}
			if (teclas == '*' && cont == 0) //Cambiar contraseña
			{
				lcd.clear();
				lcd.display();
				lcd.setCursor(4, 0);
				lcd.print("CAMBIAR");
				lcd.setCursor(7, 1);
				lcd.print("PIN");

				cursor = 0;
				cambio = 1;
				cont = 0;
				incorrecto = 0;
				activar = 0;
				error = 0;
				i = 6;
			}
		}
		if (cont == 4) //Una vez puesta la contraseña
		{
			if (teclas == '#') //Confirmar eleccion
			{
				if (activar == 4) //C0ntraseña correcta
				{
					aceptar[0] = teclas;

					cont = cont + 1;
				}
				if (incorrecto <= 4 && incorrecto > 0) //COntraseña incorrecta, vuelve a pedirla
				{
					LedRojo1();
					Apagarled();
					clearLCD();

					cont = 0;
					activar = 0;
					incorrecto = 0;
					i = 6;
					intentos = intentos + 1;
				}
			}
			if (teclas == '*') //Cambiar contraseña
			{
				lcd.clear();
				lcd.display();
				lcd.setCursor(4, 0);
				lcd.print("CAMBIAR");
				lcd.setCursor(7, 1);
				lcd.print("PIN");

				cursor = 0;
				cambio = 1;
				cont = 0;
				incorrecto = 0;
				activar = 0;
				error = 0;
				i = 6;
			}
		}
	}
	//Contraseña correcta, abre el cerrojo
	if (aceptar[0] == '#'&&codigo[0] == contrasena[0] && codigo[1] == contrasena[1] && codigo[2] == contrasena[2] && codigo[3] == contrasena[3])
	{
		LedVerde();
		lcd.clear();
		lcd.display();
		lcd.setCursor(3, 0);
		lcd.print("CONFIRMADO");

		abrir();

		if (teclas == '*') //Cierra el cerrojo
		{
			Apagarled();
			cierra();
			RESTART = 1;
			aceptar[0] = teclas;
		}
	}
	//Contraseña incorrecta 3 veces, suena alarma
	if (intentos == 3)
	{
		lcd.clear();
		lcd.setCursor(4, 0);
		lcd.print("DENEGADO");

		tone(buzzer, 900);
		delay(350);
		noTone(buzzer);
		LedRojo();
		delay(DELAY);
		cierra();
		if (teclas == '*')
		{
			Apagarled();
			RESTART = 1;
		}
	}
	if (RESTART == 1) //Cerrar y pedir pin
	{
		cursor = 0;
		error = 0;
		cont = 0;
		cambio = 8;
		activar = 0;
		incorrecto = 0;
		intentos = 0;
		i = 6;
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
	lcd.setCursor(0, 1);
	lcd.print("                ");
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
	delay(450);
	digitalWrite(ledrojo, LOW);
}
void LedVerde()
{
	digitalWrite(ledverde, HIGH);
	delay(1000);
	color(0, 255);
}
void Apagarled()
{
	digitalWrite(ledrojo, LOW);
	digitalWrite(ledverde, LOW);
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
