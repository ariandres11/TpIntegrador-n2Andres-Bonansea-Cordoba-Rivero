#include "idiomas.h"

//Defines
#define JUGADORES_MAX 10 // cantidad de jugadores del ranking
#define INTENTOSGB 6	 // cantidad de intentos en el juego (intentos Globales)

//Registros
//Jugador
typedef struct
{
	char nombre[TSTRCHICO];
	int intentosTotales;
	double tiempo;
} Jugador;

// Funciones del Ranking de jugadores
void LimpiezaLeaderboard(Jugador *leaderboard);
void ActualizarRanking(Jugador *jugador, Jugador *leaderboard);
void ImpresionRanking(Jugador *leaderboard, Idioma *idioma);
void CargarRankingDB(Jugador *leaderboard);
void GuardarRankingDB(Jugador *leaderboard);
int JugadorExistente(Jugador *jugador, Jugador *leaderboard);
bool LimpiarJugadorExistentePeor(Jugador *jugador, Jugador *leaderboard, int *posicionJugador);
void AcomodarJugadoresExistentes(Jugador *jugador, Jugador *leaderboard, int *posicionJugador);

// Funcion para DEBUG
void DebugRanking(Jugador *leaderboard);