#include <conio.h>
#include <time.h>
#include <unistd.h>

// Librerias creadas
#include "ranking.h"

// Constantes
#define MINOP 1 // numero minimo de opcion para el menu
#define MAXOP 5 // numero maximo de opcion para el menu

// FUNCIONES
// Menues y parte grafica
void MenuInicio(Idioma *idioma, int *cantPalabras, char *base, bool *debug, Jugador *leaderboard);
void EmpezarJuego(Idioma *idioma, int *cantPalabras, char *base, bool *debug, Jugador *jugadorActual, Jugador *leaderboard);
void SaludoEInstrucciones(Idioma *idioma, Jugador *jugador);
void DibujarAhorcado(int intentos);
void MostrarLetrasProbadas(Idioma *idioma, char *letras, int intentos);

// Logica del juego
int CompararPalabras(char *palabraIngresada, char *palabraBuscada, int longitud, char *frase);
int AcertarLetra(char letra, char *palabra, int longitud, char *frase);
int JuegoGanado(Idioma *idioma, int *cantPalabras, char *frase, int longitud, char *base, bool *debug);
int JuegoPerdido(Idioma *idioma, int *cantPalabras, char *palabra, int intentos, char *base, bool *debug);
void OrdenamientoBurbuja(Palabra *arr, int *cantPalabras);
int busquedaSecuencial(char *arr, int longitud, char letra);

void ImpresionListaPalabras(Idioma *idioma, int *cantPalabras);


//---------------------------------------------------------- MAIN -----------------------------------------------------------------------
int main(int argc, char *argv[])
{
	srand(getpid());								   // Randomizacion
	Idioma *idioma = (Idioma *)malloc(sizeof(Idioma)); // Reservo memoria para el registro de idioma completo
	int *cantPalabras = malloc(sizeof(int));		   // Cantidad de palabras totales para adivinar
	bool *debug = malloc(sizeof(bool));				   // Booleano para saber si esta activado el modo DEBUG o no
	Jugador leaderboard[JUGADORES_MAX];				   // Creo el leaderboard de jugadores
	char base[TSTRCHICO];							   // Puntero para guardar el nombre del archivo del idioma

	LimpiezaLeaderboard(leaderboard); // Limpio el leaderboard por las dudas
	EvaluarParametros(idioma, base, debug, argv, argc);
	CargarIdioma(idioma, base, cantPalabras);
	CargarRankingDB(leaderboard);
	MenuInicio(idioma, cantPalabras, base, debug, leaderboard);

	return 0;
}

//------------------------------------------------------------- MenuInicio --------------------------------------------------------------
// La funcion muestra el menu del juego y permite seleccionar una opcion
void MenuInicio(Idioma *idioma, int *cantPalabras, char *base, bool *debug, Jugador *leaderboard)
{
	int op;
	Jugador *jugadorActual = malloc(sizeof(Jugador)); // Creo el registro del jugador actual

	system("cls"); // Limpia la consola antes de volver a escribir
	
	// DEBUG
	if (*debug)
	{
		setColor(MAGENTA);
		printf("\n\t\t\t\t %s", idioma->titulo); // JUEGO EL AHORCADO 
		setColor(GRIS);
		printf(" * DEBUG *\n\n"); // * DEBUG *
	}
	else
	{
		setColor(MAGENTA);
		printf("\t\t\t\t %s \n\n", idioma->titulo); // JUEGO EL AHORCADO
		setColor(GRIS);
	}
	setColor(MAGENTA);
	printf("\t%s\n", idioma->menu); //**** MENU ****
	setColor(CIAN);
	printf("%s\n", idioma->jugar);		// 1. JUGAR
	setColor(AMARILLO);
	printf("%s\n", idioma->ranking);	// 2. RANKING DE JUGADORES
	setColor(VERDE_CLARO);
	printf("%s\n", idioma->listPalOrd); // 3. MOSTRAR LISTA DE PALABRAS ORDENADA POR DIFICULTAD (LONGITUD)
	setColor(CIAN_CLARO);
	printf("%s\n", idioma->setIdi);		// 4. ELEGIR IDIOMA POR DEFECTO
	setColor(ROJO_CLARO);
	printf("%s\n\n", idioma->salir);	// 5. SALIR
	setColor(BLANCO);

	// Validacion del ingreso de la opcion
	do
	{
		setColor(BLANCO);
		printf(" %s ", idioma->selOP); // Seleccione una opcion:
		scanf("%i", &op);
		if (op < MINOP || op > MAXOP)
		{
			printf("X %s X\n", idioma->invOP); // Opcion Invalida
		}
	} while (op < MINOP || op > MAXOP);

	// Casos del menu
	switch (op)
	{
	case 1:
		SaludoEInstrucciones(idioma, jugadorActual);
		EmpezarJuego(idioma, cantPalabras, base, debug, jugadorActual, leaderboard);
		break;

	case 2:
		ImpresionRanking(leaderboard, idioma);
		// DEBUG
		if (*debug && strcmp(leaderboard[0].nombre, "") != 0)
		{ // Que no este vacio el leaderboard y este activado el debug
			DebugRanking(leaderboard);
			ImpresionRanking(leaderboard, idioma);
		}

		MenuInicio(idioma, cantPalabras, base, debug, leaderboard);
		break;

	case 3:
		OrdenamientoBurbuja(idioma->palabras, cantPalabras);
		ImpresionListaPalabras(idioma, cantPalabras);
		MenuInicio(idioma, cantPalabras, base, debug, leaderboard);
		break;

	case 4:
		GuardarIdioma(idioma);
		MenuInicio(idioma, cantPalabras, base, debug, leaderboard);
		break;

	case 5:
		setColor(CIAN_CLARO);
		printf("\n\n\t\t\t\t\t\t%s\n", idioma->despedida); // Gracias por jugar!!!
		setColor(BLANCO);
		printf("\n\t**** %s ****\n\n", idioma->creditos); // creditos
		system("pause");
		GuardarRankingDB(leaderboard); // Guardo el leadearboard en la DB
		exit(EXIT_SUCCESS);
		break;
	}
}

//--------------------------------------------------------------- EmpezarJuego ---------------------------------------------------------------
// Bucle del juego
void EmpezarJuego(Idioma *idioma, int *cantPalabras, char *base, bool *debug, Jugador *jugadorActual, Jugador *leaderboard)
{
	int espacios;
	int intentos = 0;
	int cantLetrasIng = 0;
	bool win = false;

	jugadorActual->tiempo = 0.0;		 // Inicializo el tiempo del jugador
	clock_t inicio_cronometro = clock(); // Inicio el cronometro

	int opcion = rand() % (*cantPalabras - 1); // Se genera el numero aleatorio de la palabra entre 1 y 9

	int longitud = idioma->palabras[opcion].longitud; // Se almacena el tamaño de la palabra
	char letrasProbadas[INTENTOSGB + longitud];
	char letras[longitud];

	char *palabraBuscada = strupr(idioma->palabras[opcion].string); // Se almacena la palabra Buscada (NUEVO)

	char frase[longitud];

	//*inicializacion de los arrays
	// limpieza del string de letrasProbadas
	for (int i = 0; i < INTENTOSGB + longitud; i++)
	{
		letrasProbadas[i] = ' ';
	}

	// se inicializa el array de caracteres frase _  para indicar la cantidad de letras a encontrar
	for (int i = 0; i < longitud; i++)
	{
		if (palabraBuscada[i] == ' ')
		{
			frase[i] = ' ';
		}
		else
		{
			frase[i] = '_';
		}
	}

	do
	{
		// muestra en pantalla la "interfaz" del juego
		system("cls");
		setColor(MAGENTA);
		printf("\n\t\t\t\t %s \n\n", idioma->titulo);
		setColor(CIAN);													// JUEGO EL AHORCADO
		printf(" %s %d \t\t", idioma->intentos, INTENTOSGB - intentos); // Intentos Disponibles:

		// MODO DEBUG
		if (*debug)
		{
			setColor(GRIS);
			printf("Palabra a buscar: %s", idioma->palabras[opcion].string); // EL MODO DEBUG SIEMPRE SE IMPRIME EN ESPAÑOL
			setColor(BLANCO);
		}

		if (cantLetrasIng > 0)
			MostrarLetrasProbadas(idioma, letrasProbadas, cantLetrasIng);

		DibujarAhorcado(intentos);

		// Muestra las letras descubiertas hasta el momento en pantalla
		printf("\n\n\n");
		for (int i = 0; i < longitud; i++)
		{
			printf(" %c ", frase[i]);
		}

		// Comprueba si se gano o se perdio (se hace en este momento para que te muestre el muñeco del ahorcado o la palabra completa en pantalla)
		if (JuegoGanado(idioma, cantPalabras, frase, longitud, base, debug) == 1)
		{
			win = true;
			jugadorActual->intentosTotales = intentos; // Guardo los intentos restantes del jugador para el ranking
			break;
		}
		else
		{
			if (JuegoPerdido(idioma, cantPalabras, palabraBuscada, intentos, base, debug) == 1)
			{
				break;
			}
		}

		
		setColor(BLANCO);
		printf("\n\n %s ", idioma->digiteLetra); // Digite una letra:
		scanf(" %[^\n]s", &letras); //Para que se puedan ingresar palabras con espacios
		strcpy(letras, strupr(letras)); // pasa las letras que estan en minuscula a mayuscula para que la comparacion sea efectiva

		// comprobacion de acierto de letra o palabra completa
		if (strlen(letras) > 1)
		{
			if (CompararPalabras(letras, palabraBuscada, longitud, frase) == 0)
			{
				intentos = INTENTOSGB;
			}
		}
		else
		{
			//guarda si la letra se encontro (1) o no (0) en la busqueda secuencial
			int busquedaCheck = busquedaSecuencial(letrasProbadas, cantLetrasIng, letras[0]);
			//si no se encontro en la busqueda secuncial lo agrega a letras probadas
			if (busquedaCheck == 0)
			{
				letrasProbadas[cantLetrasIng] = letras[0];
				cantLetrasIng++; // incrementa cuantos elementos tiene letras probadas
			}
			// si la letra no se encuentra en la palabra a buscar devuelve 0 o si se encontra en letras probadas
			if ((AcertarLetra(letras[0], palabraBuscada, longitud, frase) == 0) || (busquedaCheck == 1))
			{
				intentos++;
			}
		}

	} while (intentos <= INTENTOSGB);

	clock_t fin_cronometro = clock();														// Termino el cronometro
	jugadorActual->tiempo += (double)(fin_cronometro - inicio_cronometro) / CLOCKS_PER_SEC; // Calculo el tiempo y lo guardo

	system("pause"); // Pauso la consola para leer

	if (win) // Si gano evaluo para el ranking
	{
		bool esMejor = false; // Variable para saber si el jugador actual es mejor que el del leaderboard
		int posicionJugador = JugadorExistente(jugadorActual,leaderboard); //Variable de la posicion del jugador
		if (posicionJugador != -1) //El jugador ya exisita previamente (se encontro)
		{
			esMejor = LimpiarJugadorExistentePeor(jugadorActual,leaderboard,&posicionJugador); //Si el jugador actual es igual y tiene mejor puntaje
		}

		if (esMejor || posicionJugador == -1) // Si el jugador nuevo es mejor (se encontro y tiene mejor puntaje) o si el jugador no se encuentra registrado previamente
		{
			ActualizarRanking(jugadorActual, leaderboard); // Actualizo el ranking (si es posible)
		}
		
		ImpresionRanking(leaderboard, idioma);		   // Imprimo el ranking
	}

	MenuInicio(idioma, cantPalabras, base, debug, leaderboard); // Vuelvo a mostrar el menu
}

//--------------------------------------------------------- Juego Perdido -------------------------------------------------------------------
// comprobacion de condicion de finalizacion
int JuegoPerdido(Idioma *idioma, int *cantPalabras, char *palabra, int intentos, char *base, bool *debug)
{
	if (intentos == INTENTOSGB)
	{
		setColor(ROJO);
		printf("\n\n %s\n", idioma->perdiste);		   // PERDISTE!!
		printf(" %s %s\n\n", idioma->solEra, palabra); // LA SOLUCION ERA:
		setColor(BLANCO);
		return 1;
	}
	return 0;
}

//--------------------------------------------------------- Juego Ganado -------------------------------------------------------------------
// comprobacion de condicion para ganar
int JuegoGanado(Idioma *idioma, int *cantPalabras, char *frase, int longitud, char *base, bool *debug)
{
	int espacios = 0;

	for (int i = 0; i < longitud; i++)
	{
		if (frase[i] == '_')
			espacios++;
	}

	if (espacios == 0)
	{
		setColor(VERDE);
		printf("\n %s\n\n", idioma->ganaste); // FELICIDADES.. GANASTE!!
		setColor(BLANCO);
		return 1;
	}

	return 0;
}

//--------------------------------------------------------- Comparar Palabras -------------------------------------------------------------------
// funcion que se encarga de comparar la palabra ingresada y la guarda en el array de frase
int CompararPalabras(char *palabraIngresada, char *palabraBuscada, int longitud, char *frase)
{
	if (strcmp(palabraIngresada, palabraBuscada) == 0)
	{
		for (int j = 0; j < longitud; j++)
		{
			frase[j] = palabraIngresada[j];
		}
		return 1;
	}
	return 0;
}

//------------------------------------------------------------- Acertar Letra ----------------------------------------------------------------
// funcion que se encaarga de buscar si la letra ingresada se encuentra en la palabra y la guarda en esa posicion
int AcertarLetra(char letra, char *palabra, int longitud, char *frase)
{
	int aciertos = 0;
	for (int j = 0; j < longitud; j++)
	{
		if (letra == palabra[j])
		{
			frase[j] = letra;
			aciertos++;
		}
	}
	return aciertos;
}

//----------------------------------------------------------- Mostrar Letras Utilizadas -------------------------------------------------------------------
// Funcion que imprime en pantalla las letras que fueron probadas hasta el momento
void MostrarLetrasProbadas(Idioma *idioma, char *letras, int intentos)
{
	printf("\t %s ", idioma->letProb); // Letras probadas:
	for (int i = 0; i < intentos; i++)
	{
		printf("%c ", letras[i]);
	}
	printf("\n");
}

//--------------------------------------------------------- Dibujar Ahorcado --------------------------------------------------------------------
void DibujarAhorcado(int intentos)
{
	switch (intentos)
	{
	case 0:
		setColor(VERDE);
		printf("\n     _______\n    |       |\n    |\n    |\n    |\n    |\n    |\n ----------");
		setColor(BLANCO);
	break;
	case 1:
		setColor(VERDE);
		printf("\n     _______\n    |       |\n    |       0\n    |\n    |\n    |\n    |\n ----------");
		setColor(BLANCO);
	break;
	case 2:
		setColor(AMARILLO);
		printf("\n     _______\n    |       |\n    |       0\n    |       |\n    |\n    |\n    |\n ----------");
		setColor(BLANCO);
	break;
	case 3:
		setColor(AMARILLO);
		printf("\n     _______\n    |       |\n    |       0\n    |      /|\n    |\n    |\n    |\n ----------");
		setColor(BLANCO);
	break;
	case 4:
		setColor(ROJO);
		printf("\n     _______\n    |       |\n    |       0\n    |      /|");
		printf("\\");
		printf("\n");
		printf("    |\n    |\n    |\n ----------");
		setColor(BLANCO);	
	break;
	case 5:
		setColor(ROJO);
		printf("\n     _______\n    |       |\n    |       0\n    |      /|");
		printf("\\");
		printf("\n");
		printf("    |      /\n    |\n    |\n ----------");
		setColor(BLANCO);
	break;
	case 6:
		setColor(ROJO);
		printf("\n     _______\n    |       |\n    |       0\n    |      /|");
		printf("\\");
		printf("\n");
		printf("    |      / ");
		printf("\\");
		printf("\n");
		printf("    |\n    |\n ----------");
		setColor(BLANCO);
	break;
}
}

//-------------------------------------------------------- Impresion Lista de Palabras ------------------------------------------------------
void ImpresionListaPalabras(Idioma *idioma, int *cantPalabras)
{
	system("cls");
	setColor(VERDE_CLARO);
	printf("\t\t\t---- %s ----\n\n", idioma->palabrasOrdenadas); // Palabras Ordenadas por dificultad
	setColor(BLANCO);
	for (int i = 0; i < *cantPalabras; i++)
	{
		strupr(idioma->palabras[i].string);
		printf("%s\n", idioma->palabras[i].string);
	}

	printf("\n");
	system("pause");
}

//-------------------------------------------------------- Ordenamiento Burbuja (Palabras a adivinar) ------------------------------------------------------
void OrdenamientoBurbuja(Palabra *arr, int *cantPalabras)
{
	Palabra aux;
	// Ordeno las palabras por Burbuja
	for (int i = 0; i < (*cantPalabras) - 1; i++)
	{
		for (int j = 0; j < (*cantPalabras) - i - 1; j++)
		{
			if (arr[j].longitud > arr[j + 1].longitud)
			{
				aux = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = aux;
			}
		}
	}
}

//-------------------------------------------------------- Saludo e instrucciones ------------------------------------------------------
void SaludoEInstrucciones(Idioma *idioma, Jugador *jugador)
{
	system("cls");
	printf("%s ", idioma->ingresaNom); // Por favor ingresa tu nombre:
	scanf("%s", jugador->nombre);
	system("cls");
	setColor(CIAN_CLARO);
	printf("\n\t\t\t\t\t\t    %s %s!\n\n", idioma->bienvenido, jugador->nombre); // Bienvenido %s!
	setColor(AMARILLO);
	printf("\t\t\t\t\t\t%s\n\n", idioma->reglasJuego); //---- Reglas del juego ----
	printf("\t%s\n", idioma->reglas1);				   // Tendras 6 intentos para adivinar una palabra, podes ingresar caracter a caracter o ingresar la palabra completa,
	printf("\t\t%s\n\n\n", idioma->reglas2);		   // pero cuidado porque si fallas perdes automaticamente. Buena Suerte!
	setColor(BLANCO);
	system("pause");
}

//-------------------------------------------------------- Busqueda Secuencial (letras utilizadas repetidas) ------------------------------------------------------
int busquedaSecuencial(char *arr, int longitud, char letra)
{
	for (int i = 0; i < longitud; i++)
	{
		if (arr[i] == letra)
		{
			return 1; // Retorna el índice del elemento si se encuentra
		}
	}
	return 0; // Retorna 0 si el elemento no está en el array
}

