#include <windows.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
//Librerias creadas
#include "idiomas.h"

// Constantes
#define INTENTOSGB 6	 // cantidad de intentos en el juego (intentos GloBales)
#define MINOP 1			 // numero minimo de opcion para el menu
#define MAXOP 5			 // numero minimo de opcion para el menu
#define JUGADORES_MAX 10 // cantidad de jugadores del ranking

// REGISTROS
// Jugador
typedef struct
{
	char nombre[TSTRCHICO];
	int intentosTotales;
	double tiempo;
} Jugador;

// PROTOTIPOS DE LAS FUNCIONES
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

// Ranking de jugadores
void LimpiezaLeaderboard(Jugador *leaderboard);
void ActualizarRanking(Jugador *jugador, Jugador *leaderboard);
void ImpresionRanking(Jugador *leaderboard, Idioma *idioma);
void CargarRankingDB(Jugador *leaderboard);
void GuardarRankingDB(Jugador *leaderboard);

// Funcion para DEBUG
void DebugRanking(Jugador *leaderboard);

//---------------------------------------------------------- MAIN -----------------------------------------------------------------------
// FUNCION PRINCIPAL MAIN
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
		printf("\n\t\t\t\t %s * DEBUG *\n\n", idioma->titulo); // JUEGO EL AHORCADO * DEBUG *
	}
	else
	{
		printf("\t\t\t\t %s \n\n", idioma->titulo); // JUEGO EL AHORCADO
	}

	printf("\t%s\n", idioma->menu); //**** MENU ****
	printf("%s\n", idioma->jugar);		// 1. JUGAR
	printf("%s\n", idioma->ranking);		// 2. RANKING DE JUGADORES
	printf("%s\n", idioma->listPalOrd);	// 3. MOSTRAR LISTA DE PALABRAS ORDENADA POR DIFICULTAD (LONGITUD)
	printf("%s\n", idioma->setIdi);		// 4. ELEGIR IDIOMA POR DEFECTO
	printf("%s\n\n", idioma->salir);		// 5. SALIR

	// Validacion del ingreso de la opcion
	do
	{
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
		printf("\n\n\t\t\t\t\t\t%s\n", idioma->despedida); // Gracias por jugar!!!
		printf("\n\t**** %s ****\n\n", idioma->creditos);  // creditos
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
	int opcion, longitud, espacios;
	int intentos = 0;
	int cantLetrasIng = 0;
	bool win = false;

	jugadorActual->tiempo = 0.0;		 // Inicializo el tiempo del jugador
	clock_t inicio_cronometro = clock(); // Inicio el cronometro

	opcion = rand() % (*cantPalabras - 1);		  // Se genera el numero aleatorio de la palabra entre 1 y 9
	longitud = idioma->palabras[opcion].longitud; // Se almacena el tamaño de la palabra
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
		frase[i] = '_';
	}

	do
	{
		// muestra en pantalla la "interfaz" del juego
		system("cls");
		printf("\n\t\t\t\t %s \n\n", idioma->titulo); // JUEGO EL AHORCADO
		printf(" %s %d \t\t", idioma->intentos, INTENTOSGB - intentos); // Intentos Disponibles:

		// MODO DEBUG
		if (*debug)
		{
			printf("Palabra a buscar: %s", idioma->palabras[opcion].string); // EL MODO DEBUG SIEMPRE SE IMPRIME EN ESPAÑOL
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

		printf("\n\n %s ", idioma->digiteLetra); // Digite una letra:
		scanf("%s", &letras);
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
		{ int aux = busquedaSecuencial(letrasProbadas, cantLetrasIng, letras[0]);
			if (aux == 0)
			{
				letrasProbadas[cantLetrasIng] = letras[0];
				cantLetrasIng++;
			}else if(aux == 1){
				intentos++;
			}
			if (AcertarLetra(letras[0], palabraBuscada, longitud, frase) == 0)
			{
				intentos++;
			}
		}

		// comprobacion de finalizacion del juego

	} while (intentos <= INTENTOSGB);

	clock_t fin_cronometro = clock();														// Termino el cronometro
	jugadorActual->tiempo += (double)(fin_cronometro - inicio_cronometro) / CLOCKS_PER_SEC; // Calculo el tiempo y lo guardo

	system("pause"); // Pauso la consola para leer

	if (win) // Si no perdio evaluo para el ranking
	{
		ActualizarRanking(jugadorActual, leaderboard); // Actualizo el ranking (si es posible)
		ImpresionRanking(leaderboard, idioma);		   // Imprimo el ranking
	}

	MenuInicio(idioma, cantPalabras, base, debug, leaderboard); // Vuelvo a mostrar el menu
}

//--------------------------------------------------------- JuegoPerdido -------------------------------------------------------------------
// comprobacion de condicion de finalizacion
int JuegoPerdido(Idioma *idioma, int *cantPalabras, char *palabra, int intentos, char *base, bool *debug)
{
	if (intentos == INTENTOSGB)
	{
		printf("\n\n %s\n", idioma->perdiste);		   // PERDISTE!!
		printf(" %s %s\n\n", idioma->solEra, palabra); // LA SOLUCION ERA:
		return 1;
	}
	return 0;
}

//--------------------------------------------------------- JuegoGanado -------------------------------------------------------------------
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
		printf("\n %s\n\n", idioma->ganaste); // FELICIDADES.. GANASTE!!
		return 1;
	}

	return 0;
}

//--------------------------------------------------------- CompararPalabras -------------------------------------------------------------------
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

//------------------------------------------------------------- AcertarLetra ----------------------------------------------------------------
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

//----------------------------------------------------------- MostrarLetrasProbadas -------------------------------------------------------------------
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

//--------------------------------------------------------- DibujarAhorcado --------------------------------------------------------------------
// FUNCION QUE REALIZA EL DIBUJO DE EL AHORCADO, RECIBE EL NUMERO DE INTENTOS Y CON ESE DATO REALIZA EL DIBUJO
void DibujarAhorcado(int intentos)
{
	switch (intentos)
	{
	case 0:
		printf("\n     _______\n    |       |\n    |\n    |\n    |\n    |\n    |\n ----------");
		break;
	case 1:
		printf("\n     _______\n    |       |\n    |       0\n    |\n    |\n    |\n    |\n ----------");
		break;
	case 2:
		printf("\n     _______\n    |       |\n    |       0\n    |       |\n    |\n    |\n    |\n ----------");
		break;
	case 3:
		printf("\n     _______\n    |       |\n    |       0\n    |      /|\n    |\n    |\n    |\n ----------");
		break;
	case 4:
		printf("\n     _______\n    |       |\n    |       0\n    |      /|");
		printf("\\");
		printf("\n");
		printf("    |\n    |\n    |\n ----------");
		break;
	case 5:
		printf("\n     _______\n    |       |\n    |       0\n    |      /|");
		printf("\\");
		printf("\n");
		printf("    |      /\n    |\n    |\n ----------");
		break;
	case 6:
		printf("\n     _______\n    |       |\n    |       0\n    |      /|");
		printf("\\");
		printf("\n");
		printf("    |      / ");
		printf("\\");
		printf("\n");
		printf("    |\n    |\n ----------");
		break;
	}
}

//-------------------------------------------------------- ImpresionListaPalabras ------------------------------------------------------
void ImpresionListaPalabras(Idioma *idioma, int *cantPalabras)
{
	system("cls");
	printf("\t\t\t---- %s ----\n\n", idioma->palabrasOrdenadas); // Palabras Ordenadas por dificultad

	for (int i = 0; i < *cantPalabras; i++)
	{
		strupr(idioma->palabras[i].string);
		printf("%s\n", idioma->palabras[i].string);
	}

	printf("\n");
	system("pause");
}

//-------------------------------------------------------- Ordenamiento Burbuja ------------------------------------------------------
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
	printf("\n\t\t\t\t\t\t    %s %s!\n\n", idioma->bienvenido, jugador->nombre); // Bienvenido %s!
	printf("\t\t\t\t\t\t%s\n\n", idioma->reglasJuego);							 //---- Reglas del juego ----
	printf("\t%s\n", idioma->reglas1);											 // Tendras 6 intentos para adivinar una palabra, podes ingresar caracter a caracter o ingresar la palabra completa,
	printf("\t\t%s\n\n\n", idioma->reglas2);									 // pero cuidado porque si fallas perdes automaticamente. Buena Suerte!
	system("pause");
}

//-------------------------------------------------------- Limpieza leaderboard ------------------------------------------------------
void LimpiezaLeaderboard(Jugador *leaderboard)
{
	for (int i = 0; i < JUGADORES_MAX; i++)
	{
		strcpy(leaderboard[i].nombre, "");
		leaderboard[i].intentosTotales = INTENTOSGB;
		leaderboard[i].tiempo = 0.0;
	}
}

//-------------------------------- Impresion Ranking ----------------------------
void ImpresionRanking(Jugador *leaderboard, Idioma *idioma)
{
	system("cls"); // Limpio pantalla

	printf("\n\t\t%s\n\n", idioma->rankingJugadores); //----- RANKING DE JUGADORES -----

	for (int i = 0; i < JUGADORES_MAX; i++)
	{
		if (strcmp(leaderboard[i].nombre, "") != 0)
		{
			printf("%i* - %s (%i %s %.1f %s)\n", (i + 1), leaderboard[i].nombre, (leaderboard[i].intentosTotales + 1), idioma->intentosRanking, leaderboard[i].tiempo, idioma->segundosRanking); // intentos, segundos
		}
	}

	printf("\n\n");
	system("pause"); // Espero input para continuar
}

//-------------------------------- Actualizar Ranking ----------------------------
void ActualizarRanking(Jugador *jugador, Jugador *leaderboard)
{
	int i;

	// Busco la posicion del ranking en donde debo colocar en base a intentos
	for (i = 0; i < JUGADORES_MAX && jugador->intentosTotales > leaderboard[i].intentosTotales; i++)
		;

	// Si se encuentra dentro de las primeras 10 posiciones en base a intentos analizo en base a tiempo
	if (i < JUGADORES_MAX)
	{
		// Mientras tengan la misma cantidad de intentos y el tiempo sea mayor
		while (jugador->intentosTotales == leaderboard[i].intentosTotales && jugador->tiempo > leaderboard[i].tiempo)
		{
			// Incremento la posicion
			i++;
		}

		// Si se encuentra dentro de las primeras 10 posiciones en base a tiempo acomodo para colocarlo
		if (i < JUGADORES_MAX)
		{
			// Desplazamiento a la derecha
			for (int j = JUGADORES_MAX - 1; j > i; j--)
			{
				leaderboard[j].intentosTotales = leaderboard[j - 1].intentosTotales;
				strcpy(leaderboard[j].nombre, leaderboard[j - 1].nombre);
				leaderboard[j].tiempo = leaderboard[j - 1].tiempo;
			}

			// Coloco el jugador en la posicion que quedo
			leaderboard[i].intentosTotales = jugador->intentosTotales;
			strcpy(leaderboard[i].nombre, jugador->nombre);
			leaderboard[i].tiempo = jugador->tiempo;
		}
	}
}

//-------------------------------- Cargar RankingDB ----------------------------
void CargarRankingDB(Jugador *leaderboard)
{
	FILE *DB = fopen("_RankingDB.dat", "r");

	char *descargar = malloc(sizeof(char *)); // String para obtener una linea de 1 jugador

	char *nombre = malloc(sizeof(char *)); // String auxiliar para guardar el nombre del jugador en el registro

	int i = 0;

	if (fgets(descargar, sizeof(descargar), DB) != NULL) // El archivo tiene algo
	{
		rewind(DB); // Reinicio el cursor
		while (!(feof(DB)))
		{
			fgets(descargar, TSTRCHICO, DB);																	// Obtengo el string completo de la Base de datos sin separar
			sscanf(descargar, "%[^;];%d;%lf", nombre, &leaderboard[i].intentosTotales, &leaderboard[i].tiempo); // Guardo nombre;intentos;tiempo en sus respectivas variables
			strcpy(leaderboard[i].nombre, nombre);																// Copio el nombre por separado pq no se le puede asignar directamente el string
			i++;																								// Incremento el contador para desplazarme a la siguiente posicion del leaderboard
		}
	}

	fclose(DB); // Cierro el archivo
}

//-------------------------------- Guardar RankingDB ----------------------------
void GuardarRankingDB(Jugador *leaderboard)
{
	FILE *DB = fopen("_RankingDB.dat", "w");

	char *cadenaAux = malloc(sizeof(char)); // Cadena auxiliar para las conversiones de entero y flotante

	// Recorro el arreglo de leaderboard completo y guardo a todos los jugadores SI EXISTEN EN ESA POSICION
	for (int i = 0; i < JUGADORES_MAX && strcmp(leaderboard[i].nombre, "") != 0; i++)
	{
		char cargar[TSTRCHICO] = ""; // Declaro el string para cargar limpio de basura

		strcpy(cargar, leaderboard[i].nombre);					  // Copio el nombre
		strcat(cargar, ";");									  // Concateno el ;
		sprintf(cadenaAux, "%d", leaderboard[i].intentosTotales); // Convierto los intentos del registro a string
		strcat(cargar, cadenaAux);								  // Concateno los intentos
		strcat(cargar, ";");									  // Concateno el ;
		sprintf(cadenaAux, "%.2f", leaderboard[i].tiempo);		  // Convierto el tiempo del registro a string
		strcat(cargar, cadenaAux);								  // Concateno el tiempo

		// Para que no cargue un salto de linea en el ultimo jugador (cualquiera sea su posicion)
		if (i < (JUGADORES_MAX - 1) && strcmp(leaderboard[i + 1].nombre, "") != 0) // Miro tambien el siguiente a ver si tiene algo cargado
		{
			strcat(cargar, "\n"); // Si no es el ultimo y el siguiente tiene algo cargado pongo un salto de linea
		}

		fputs(cargar, DB); // Guardo la cadena en el archivo
	}

	fclose(DB); // Cierro archivo de jugadores
}

//-------------------------------- Debug Ranking ----------------------------
void DebugRanking(Jugador *leaderboard)
{
	int o;
	do
	{
		printf("\n\n* DEBUG: desea borrar el ranking de jugadores (registro) (1.Si/2.No): ");
		scanf("%d", &o);
		if (o < 1 || o > 2)
		{
			printf("\t\tXX Ingrese una opcion valida! XX\n");
		}
	} while (o < 1 || o > 2);

	if (o == 1)
	{
		LimpiezaLeaderboard(leaderboard);
	}
}

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