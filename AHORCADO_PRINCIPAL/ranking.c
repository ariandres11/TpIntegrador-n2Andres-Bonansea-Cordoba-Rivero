#include "ranking.h"

//-------------------------------------------------------- Limpieza leaderboard ------------------------------------------------------
//Limpia el arreglo leaderboard (de Jugadores) poniendoles valores base
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
//Realiza un impresion de los jugadores cargados en el registro de ranking
void ImpresionRanking(Jugador *leaderboard, Idioma *idioma)
{
	system("cls"); // Limpio pantalla
	setColor(AMARILLO); 

	printf("\n\t\t%s\n\n", idioma->rankingJugadores); //----- RANKING DE JUGADORES -----

	setColor(BLANCO);

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
//Actualiza el ranking de jugadores cuando un nuevo jugador gana
void ActualizarRanking(Jugador *jugador, Jugador *leaderboard)
{
	int i;

	// Busco la posicion del ranking en donde debo colocar en base a intentos
	for (i = 0; i < JUGADORES_MAX && jugador->intentosTotales > leaderboard[i].intentosTotales; i++);

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
//Carga el leaderboard desde un archivo (carga la base de datos)
void CargarRankingDB(Jugador *leaderboard)
{
	FILE *DB = fopen("_RankingDB.dat", "r");

	// Error al abrir el archivo
	if (DB == NULL)
	{
		fprintf(stderr, "No se pudo abrir correctamente el archivo _RankingDB.dat\n");
		exit(EXIT_FAILURE);
	}

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
//Almacena el leaderboard en un archivo con el formato requerido (guarda en la base de datos)
void GuardarRankingDB(Jugador *leaderboard)
{
	FILE *DB = fopen("_RankingDB.dat", "w");

	// Error al abrir el archivo
	if (DB == NULL)
	{
		fprintf(stderr, "No se pudo abrir correctamente el archivo _RankingDB.dat\n");
		exit(EXIT_FAILURE);
	}

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

//-------------------------------- Jugador Existente ----------------------------
//Chequea si el nombre del jugador ya se encuentra dentro del leaderboard (case sensitive)
int JugadorExistente(Jugador *jugador, Jugador *leaderboard)
{
	for (int i = 0; i < JUGADORES_MAX; i++)
	{
		if (strcmp(jugador->nombre, leaderboard[i].nombre)==0)
		{
			return i; // Retorna la posicion del jugador si se encuentra
		}
	}
	return -1; // Retorna -1 si el nombre del jugador no se encuentra

}

//-------------------------------- Actualizar Jugador Existente ----------------------------
//Funcion que limpia o no las estadisticas del jugador en el leaderboard en caso de que se encuentre y las mismas sean inferiores
bool LimpiarJugadorExistentePeor(Jugador *jugador, Jugador *leaderboard, int *posicionJugador){ 

	if (*posicionJugador != -1)//El jugador ya se encontraba en el leaderboard
	{
		if (jugador->intentosTotales<leaderboard[*posicionJugador].intentosTotales) //Si la cantidad de intentos nueva es menor
		{
			//limpio el jugador del leaderboard
			strcpy(leaderboard[*posicionJugador].nombre, "");
			leaderboard[*posicionJugador].intentosTotales = INTENTOSGB;
			leaderboard[*posicionJugador].tiempo = 0.0;
			return true; //Devuelvo que el jugador fue modificado

		}else if(jugador->intentosTotales == leaderboard[*posicionJugador].intentosTotales && jugador->tiempo<leaderboard[*posicionJugador].tiempo){ //Si la cantidad de intentos es igual pero el tiempo es menor
			//limpio el jugador del leaderboard
			strcpy(leaderboard[*posicionJugador].nombre, "");
			leaderboard[*posicionJugador].intentosTotales = INTENTOSGB;
			leaderboard[*posicionJugador].tiempo = 0.0;
			return true; //Devuelvo que el jugador fue modificado

		}
	}
	return false; //Devuelvo que el jugador NO fue modificado
}

//-------------------------------- Debug Ranking ----------------------------
//Funcion especial cundo se encuentra activo el modo debug para poder eliminar el leaderboard
void DebugRanking(Jugador *leaderboard)
{
	int opcionDebug;

	setColor(GRIS);
	do
	{
		printf("\n\n* DEBUG: desea borrar el ranking de jugadores (registro) (1.Si/2.No): ");
		scanf("%d", &opcionDebug);
		if (opcionDebug < 1 || opcionDebug > 2)
		{
			printf("\t\tXX Ingrese una opcion valida! XX\n");
		}
	} while (opcionDebug < 1 || opcionDebug > 2);
	
	setColor(BLANCO);

	if (opcionDebug == 1)
	{
		LimpiezaLeaderboard(leaderboard);
	}
}
