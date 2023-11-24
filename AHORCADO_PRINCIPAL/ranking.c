#include "ranking.h"

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