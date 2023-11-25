#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <windows.h>

// Defines
#define TSTRCHICO 50	 // Tamaño string chico para cargar idioma por defecto
#define TSTRGRANDE 10000 // Tamaño string grande para la mayoria de los reservar memoria
#define PASSWORD "123"	 // Contraseña para el modo DEBUG

//Paleta de colores
#define AZUL 1
#define VERDE 2
#define CIAN 3
#define ROJO 4
#define MAGENTA 5
#define AMARILLO 6
#define BLANCO 7
#define GRIS 8
#define AZUL_CLARO 9
#define VERDE_CLARO 10
#define CIAN_CLARO 11
#define ROJO_CLARO 12
#define AMARILLO_CLARO 13
#define BLANCO_BLANCO 14

// Registros
// Palabra
typedef struct
{
	char string[TSTRGRANDE];
	int longitud;
} Palabra;

// Idioma
typedef struct
{
	char *ingresaNom;
	char *bienvenido;
	char *reglasJuego;
	char *reglas1;
	char *reglas2;
	char *titulo;
	char *menu;
	char *jugar;
	char *ranking;
	char *listPalOrd;
	char *setIdi;
	char *salir;
	char *selOP;
	char *invOP;
	char *intentos;
	char *perdiste;
	char *solEra;
	char *ganaste;
	char *rankingJugadores;
	char *intentosRanking;
	char *segundosRanking;
	char *digiteLetra;
	char *letProb;
	char *despedida;
	char *archivoVacio;
	char *idiomaDefecto;
	char *idiomasDisponibles;
	char *intercambiarIdioma;
	char *palabrasOrdenadas;
	char *creditos;
	Palabra *palabras;
} Idioma;

// Funciones de la carga del Idioma
void EvaluarParametros(Idioma *idioma, char *base, bool *debug, char *argv[], int argc);
int ContarPalabras(char *string);
void GuardarPalabras(Idioma *idioma, char *string);
void CargarIdioma(Idioma *idioma, char *nombreArchivo, int *cantPalabras);
void AsignarMemoria(char **puntero, FILE *fp);
void GuardarIdioma(Idioma *idioma);
void CargarDefaultABase(char *base);
void LeerDirectorio(char **files, int *tam);

// Colores
void setColor(int color);