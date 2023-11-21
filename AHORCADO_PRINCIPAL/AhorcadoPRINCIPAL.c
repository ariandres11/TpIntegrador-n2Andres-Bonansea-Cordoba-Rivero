#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <dirent.h>

// DEBUG por codigo
// #define DEBUGPRINTFPROBADAS
// #define DEBUGTRGISTRO
// #define DEBUGMOSTRARLONGITUDPALABRAS

// Constantes
#define INTENTOSGB 6	 // cantidad de intentos en el juego (intentos GloBales)
#define MINOP 1			 // numero minimo de opcion para el menu
#define MAXOP 5			 // numero minimo de opcion para el menu
#define TSTRCHICO 50	 // Tamaño string chico para cargar idioma por defecto
#define TSTRGRANDE 10000 // Tamaño string grande para la mayoria de los reservar memoria
#define PASSWORD "123"	 // Contraseña para el modo DEBUG

// REGISTROS
//Palabra
typedef struct
{
	char string[TSTRGRANDE];
	int longitud;
} Palabra;

//Idioma
typedef struct
{
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
	char *volverAJugar;
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

//Jugador
typedef struct 
{
    char *nombre;
    int intentos;
    double tiempo;
}Jugador;

// PROTOTIPOS DE LAS FUNCIONES
// Menues y parte grafica
void MenuInicio(Idioma *idioma, int *cantPalabras, char *base, bool *debug);
void EmpezarJuego(Idioma *idioma, int *cantPalabras, char *base, bool *debug);
void SaludoEInstrucciones(Idioma *idioma, Jugador *jugador);
void DibujarAhorcado(int intentos);
void MostrarLetrasProbadas(Idioma *idioma, char *letras, int intentos);


// Logica del juego
int CompararPalabras(char *palabraIngresada, char *palabraBuscada, int longitud, char *frase);
int AcertarLetra(char letra, char *palabra, int longitud, char *frase);
int JuegoGanado(Idioma *idioma, int *cantPalabras, char *frase, int longitud, char *base, bool *debug);
int JuegoPerdido(Idioma *idioma, int *cantPalabras, char *palabra, int intentos, char *base, bool *debug);
void VolverAJugar(Idioma *idioma, int *cantPalabras, char *base, bool *debug);
int TieneEspacios(char *opcion);
void OrdenamientoBurbuja(Palabra *arr, int *cantPalabras);

// Funciones de la carga del Idioma
void EvaluarParametros(Idioma *idioma,char *base, bool *debug, char *argv[], int argc);
int ContarPalabras(char *string);
void GuardarPalabras(Idioma *idioma, char *string);
void CargarIdioma(Idioma *idioma, char *nombreArchivo, int *cantPalabras);
void AsignarMemoria(char **puntero, FILE *fp);
void ImpresionListaPalabras(Idioma *idioma, int *cantPalabras);
void GuardarIdioma(Idioma *idioma);
void CargarDefaultABase(char *base);
void LeerDirectorio(char **files, int *tam);

//Ranking de jugadores


//---------------------------------------------------------- MAIN -----------------------------------------------------------------------
// FUNCION PRINCIPAL MAIN
int main(int argc, char *argv[])
{
	Idioma *idioma = (Idioma *)malloc(sizeof(Idioma)); // Reservo memoria para el registro de idioma completo
	int *cantPalabras = malloc(sizeof(int));           // Cantidad de palabras totales para adivinar
	bool *debug = malloc(sizeof(bool));                //Booleano para saber si esta activado el modo DEBUG o no

	char base[TSTRCHICO]; // Puntero para guardar el nombre del archivo del idioma

	EvaluarParametros(idioma,base,debug,argv,argc);

	CargarIdioma(idioma, base, cantPalabras);

	MenuInicio(idioma, cantPalabras, base, debug);

	return 0;
}

//------------------------------------------------------------- MenuInicio --------------------------------------------------------------
// La funcion muestra el menu del juego y permite seleccionar una opcion
void MenuInicio(Idioma *idioma, int *cantPalabras, char *base, bool *debug)
{
	int op;
	Jugador *jugadorActual;

	do
	{
		system("cls");								  // Limpia la consola antes de volver a escribir
		printf("\n\t\t\t\t %s \n\n", idioma->titulo); // JUEGO EL AHORCADO
		printf("\t%s\n\n", idioma->menu);			  //**** MENU ****
		printf(" %s\n", idioma->jugar);				  // 1. JUGAR
		printf(" %s\n", idioma->ranking);			  // 2. RANKING DE JUGADORES
		printf(" %s\n", idioma->listPalOrd);		  // 3. MOSTRAR LISTA DE PALABRAS ORDENADA POR DIFICULTAD (LONGITUD)
		printf(" %s\n", idioma->setIdi);			  // 4. GUARDAR IDIOMA ACTUAL COMO POR DEFECTO
		printf(" %s\n\n", idioma->salir);			  // 5. SALIR

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

		switch (op)
		{
		case 1:
			EmpezarJuego(idioma, cantPalabras, base, debug);
			break;

		case 2:
			/* code */
			break;

		case 3:
		 	OrdenamientoBurbuja(idioma->palabras, cantPalabras);
			ImpresionListaPalabras(idioma, cantPalabras);
			MenuInicio(idioma, cantPalabras, base, debug);
			break;

		case 4:
			GuardarIdioma(idioma);
			MenuInicio(idioma, cantPalabras, base, debug);
			break;

		case 5:
			printf("\n\n\t\t\t\t\t\t%s\n", idioma->despedida); // Gracias por jugar!!!
			printf("\n\t**** %s ****\n\n", idioma->creditos);  // creditos
			system("pause");
			exit(EXIT_SUCCESS);
			break;

		default:
			break;
		}

	} while (op != MAXOP);
}

//--------------------------------------------------------------- EmpezarJuego ---------------------------------------------------------------
// Bucle del juego
void EmpezarJuego(Idioma *idioma, int *cantPalabras, char *base, bool *debug)
{
	int opcion, longitud, espacios;
	int intentos = 0;
	int cantLetrasIng = 0;
	char letrasProbadas[INTENTOSGB + 10];

	srand(time(NULL)); // Randomizacion

	opcion = rand() % (*cantPalabras - 1);		  // Se genera el numero aleatorio de la palabra entre 1 y 9
	longitud = idioma->palabras[opcion].longitud; // Se almacena el tamaño de la palabra
	char letras[longitud];

	char *palabraBuscada = strupr(idioma->palabras[opcion].string); // Se almacena la palabra Buscada (NUEVO)

	char frase[longitud];

	//*inicializacion de los arrays
	// limpieza del string de letrasProbadas
	for (int i = 0; i < INTENTOSGB + 10; i++)
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
		printf("\n\t\t\t\t%s\n\n", idioma->titulo);						// JUEGO EL AHORCADO
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
		{
			letrasProbadas[cantLetrasIng] = letras[0];
			cantLetrasIng++;
			if (AcertarLetra(letras[0], palabraBuscada, longitud, frase) == 0)
			{
				intentos++;
			}
		}

		// comprobacion de finalizacion del juego

	} while (intentos <= INTENTOSGB);
	VolverAJugar(idioma, cantPalabras, base, debug);
	printf("\n\n");
}

//--------------------------------------------------------- VolverAJugar -------------------------------------------------------------------
void VolverAJugar(Idioma *idioma, int *cantPalabras, char *base, bool *debug)
{
	printf(" %s", idioma->volverAJugar); // Presiona una tecla para volver a jugar..
	getch();
	MenuInicio(idioma, cantPalabras, base, debug);
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
#ifdef DEBUGPRINTF
		printf("\ni: %d intentos: %d\n", i, intentos);
#endif
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

//-------------------------------------------------------- Evaluar Parametros ------------------------------------------------------------------
void EvaluarParametros(Idioma *idioma, char *base, bool *debug, char *argv[], int argc){
	*debug = false; //Antes de evaluar parametros digo que debug esta desactivado
	if (argc < 2)
	{ // Si no tiene parametros
		CargarDefaultABase(base);
	}
	else if (argc == 2)
	{ // Si tiene 2 parametros
		if (strcmp(argv[1], "DEBUG") == 0)
		{ 
			*debug = true; // Si el segundo parametro es DEBUG se inicia modo DEBUG CON EL IDIOMA POR DEFAULT
			CargarDefaultABase(base);
		}
		else
		{						   // Si el parametro no es DEBUG
			strcpy(base, argv[1]); // Utilizo base en ambos casos para prevenir errores
		}
	}
	else if (argc > 2)
	{ // Cuando se pasa de parametros al inicializar el juego
		fprintf(stderr, "\nUSAGE: %s \"MODE/LANGUAGE\"\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// MODO DEBUG
	if (*debug)
	{
		char input[strlen(PASSWORD)];					  // Para el input del usuario de la contraseña
		printf("Ingrese la contrasena del modo debug: "); // EL MODO DEBUG SIEMPRE SE EJECUTA EN ESPAÑOL
		scanf("%s", input);
		if (strcmp(PASSWORD, input) != 0)
		{
			fprintf(stderr, "\n\tXXXX CONTRASENA INCORRECTA XXXX\n"); // EL MODO DEBUG SIEMPRE SE EJECUTA EN ESPAÑOL
			exit(EXIT_FAILURE);
		}
		printf("\tMODO DEBUG CON IDIOMA DEFAULT ACTIVADO!\n\n"); // EL MODO DEBUG SIEMPRE SE EJECUTA EN ESPAÑOL
		system("pause");
	}
}


//-------------------------------------------------------- ContarPalabras ------------------------------------------------------------------
int ContarPalabras(char *string)
{
	int cont = 0;
	char *token = strtok(string, ",");
	while (token != NULL)
	{
		token = strtok(NULL, ",");
		cont++;
	}
	return cont;
}

//------------------------------------------------------- GuardarPalabras --------------------------------------------------------------------
void GuardarPalabras(Idioma *idioma, char *string)
{
	int cont = 0;
	char *token = strtok(string, ",");
	while (token != NULL)
	{
		strcpy(idioma->palabras[cont].string, token);	 // Guardo en registro el string
		idioma->palabras[cont].longitud = strlen(token); // Guardo en registro la longitud del string
		token = strtok(NULL, ",");						 // Divido
		cont++;											 // Incremento contador para la posicion del arreglo de palabras
	}
}

//-------------------------------------------------------- CargarIdioma --------------------------------------------------------------
void CargarIdioma(Idioma *idioma, char *nombreArchivo, int *cantPalabras)
{
	FILE *fp;
	char string[TSTRGRANDE]; // Para hacer la separacion de las palabras
	char aux[TSTRGRANDE];

	fp = fopen(nombreArchivo, "r");

	// Error al abrir el archivo
	if (fp == NULL)
	{
		fprintf(stderr, "No se pudo abrir correctamente el archivo en: %s\n", nombreArchivo);
		exit(EXIT_FAILURE);
	}

	// El archivo se encuentra vacio
	if (fgets(string, sizeof(string), fp) == NULL)
	{
		printf("\n\t%s %s\n", nombreArchivo, idioma->archivoVacio); // se encuentra vacio!

		// El archivo tiene algo
	}
	else
	{
		// Reinicio el cursor por lo de arriba
		rewind(fp);

		// Palabras
		fgets(string, sizeof(string), fp);
		strcpy(aux, string);
		*cantPalabras = ContarPalabras(aux);
		strtok(string, "\n");	  // Elimina el salto de línea del fgets utilizando strtok
		idioma->palabras = (Palabra *)malloc(*cantPalabras * sizeof(Palabra)); // Calculo el tamaño contando la cantidad de palabras
		GuardarPalabras(idioma, string);

		// Resto de strings
		// JUEGO EL AHORCADO
		AsignarMemoria(&idioma->titulo, fp);

		//**** MENU ****
		AsignarMemoria(&idioma->menu, fp);

		// 1. JUGAR
		AsignarMemoria(&idioma->jugar, fp);

		// 2. RANKING DE JUGADORES
		AsignarMemoria(&idioma->ranking, fp);

		// 3. MOSTRAR LISTA DE PALABRAS ORDENADA POR DIFICULTAD (LONGITUD)
		AsignarMemoria(&idioma->listPalOrd, fp);

		// 4. GUARDAR IDIOMA ACTUAL COMO POR DEFECTO
		AsignarMemoria(&idioma->setIdi, fp);

		// 5. SALIR
		AsignarMemoria(&idioma->salir, fp);

		// Ingresa una opcion:
		AsignarMemoria(&idioma->selOP, fp);

		// Opcion Invalida!
		AsignarMemoria(&idioma->invOP, fp);

		// Intentos Disponibles:
		AsignarMemoria(&idioma->intentos, fp);

		// PERDISTE!!
		AsignarMemoria(&idioma->perdiste, fp);

		// LA SOLUCION ERA:
		AsignarMemoria(&idioma->solEra, fp);

		// FELICIDADES.. GANASTE!!
		AsignarMemoria(&idioma->ganaste, fp);

		// Presiona una tecla para volver al menu..
		AsignarMemoria(&idioma->volverAJugar, fp);

		// Digite una letra:
		AsignarMemoria(&idioma->digiteLetra, fp);

		// Letras probadas:
		AsignarMemoria(&idioma->letProb, fp);

		// Gracias por jugar!!!
		AsignarMemoria(&idioma->despedida, fp);

		// Se encuentra vacio
		AsignarMemoria(&idioma->archivoVacio, fp);

		// Idioma por defecto:
		AsignarMemoria(&idioma->idiomaDefecto, fp);

		// Idioma actual:
		AsignarMemoria(&idioma->idiomasDisponibles, fp);

		// Desea intercambiar el idioma por defecto por el idioma actual? (1.Si/2.No)
		AsignarMemoria(&idioma->intercambiarIdioma, fp);

		//---- Palabras Ordenadas por dificultad ----
		AsignarMemoria(&idioma->palabrasOrdenadas, fp);

		//**** Creditos: Andres Ariel Sebastian, Bonansea Mariano Nicolas, Cordoba Luis Tahiel, Rivero Lucia Jazmin ****
		AsignarMemoria(&idioma->creditos, fp);
	}

	// Cierro el archivo de los idiomas pq no lo necesito mas por ahora
	fclose(fp);
}

//------------------------------------------------------------- AsignarMemoria -------------------------------------------------------------
void AsignarMemoria(char **puntero, FILE *fp)
{
	char string[TSTRGRANDE];
	fgets(string, sizeof(string), fp);
	*puntero = (char *)malloc((strlen(string) + 1) * sizeof(char)); // Reservo la memoria suficiente para el string
	// Para prevenir errores
	if (*puntero == NULL)
	{
		fprintf(stderr, "EL PUNTERO ES NULL\n");
		exit(EXIT_FAILURE);
	}
	strtok(string, "\n");	  // Elimina el salto de línea del fgets utilizando strtok
	strcpy(*puntero, string); // Finalmente copio el string al registro
}

//-------------------------------------------------------- CargarDefaultABase ------------------------------------------------------
void CargarDefaultABase(char *base)
{
	// Busco el nombre del archivo default a leer en el archivo default
	FILE *dl = fopen("Idiomas/_default.dat", "r");
	fscanf(dl, "%[^\n]", base);
	fclose(dl);
}

//-------------------------------------------------------------- GuardarIdioma ------------------------------------------------------------
void GuardarIdioma(Idioma *idioma)
{
	FILE *dl = fopen("Idiomas/_default.dat", "r"); //Abro archivo en modo lectura para leer el idioma que esta por defecto
	char porDefecto[TSTRCHICO];
	int o; //Selector de opcion
	int tam=0; //Contador de archivos
	char **files= malloc(10 * sizeof(char *)); // reservamos memoria para el arreglo de nombres de archivos

	system("cls"); // Limpio pantalla

	fgets(porDefecto, sizeof(porDefecto), dl);			  // Guardo el nombre del archivo que hay en el archivo default
	fclose(dl); // Cierro el archivo abierto en modo lectura para despues abrirlo en modo escritura
	printf("\t---- %s %s ----\n", idioma->idiomaDefecto, porDefecto); // Idioma por defecto:

	LeerDirectorio(files,&tam); //Funcion de leer todos los archivos en el directorio de Idioma
	printf("\n%s\n", idioma->idiomasDisponibles);		  // Idiomas disponibles:

	//Imprimo los Idiomas del archivo
	for (int i = 0; i < tam; i++)
	{
		printf("%d. %s\n",(i+1),files[i]);
	}
	
	do
	{
		printf("\n%s %d:\n", idioma->intercambiarIdioma, tam); // Elija un idioma del 1 al n
		scanf("%d", &o);

		if (o < 1 || o > tam)
		{
			printf("\t%s\n", idioma->invOP); // Opcion Invalida!
		}

	} while (o < 1 || o > tam);

	//Cargo el idioma con el nombre del archivo elegido
	dl = fopen("Idiomas/_default.dat","w"); //Abro el archivo en modo escritura para limpiarlo del anterior
	fputs("Idiomas/", dl); //Le agrego al contenido del archivo el directorio de los idiomas
	fputs(files[o-1], dl); //Le agrego al archivo el nombre del idioma
	fclose(dl); // Cierro el archivo en modo escritura
	printf("\n%s %s\n", idioma->idiomaDefecto, files[o-1]); // Idioma por defecto:

	//LIBERO MEMORIA DE CADA NOMBRE DE ARCHIVO DE DIRECTORIO Y DEL ARREGLO DE DIRECTORIOS
	for (int i = 0; i < tam; i++)
	{
		free(files[i]);
	}
	free(files);

	system("pause");
}

//-------------------------------------------------------- LEER DIRECTORIO ------------------------------------------------------
void LeerDirectorio(char **files, int *tam){
	DIR *d = opendir("Idiomas"); // puntero a un flujo de directorio
	struct dirent *dir; // estructura para almacenar información de cada entrada
	
	if (d == NULL) { // comprobamos si hubo algún error
		fprintf(stderr,"No se pudo abrir el directorio");
		exit(EXIT_FAILURE);
  	}
	
  	int max = 10; // tamaño inicial del arreglo (En caso de que existan mas que 10 idiomas se adecua la memoria para todos)
	
  	if (files == NULL) { // comprobamos si hubo algún error
    	fprintf(stderr,"No se pudo reservar memoria");
    	exit(EXIT_FAILURE);
  	}

	while ((dir = readdir(d)) != NULL) { // leemos cada entrada del directorio
		if (strcmp(dir->d_name, "_default.dat") != 0 && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {// ignoramos los directorios "." , ".." y el del idioma default
			
			//Condicional para evaluar si hay que redimensionar la memoria reservada
			if (max == *tam) { // si el arreglo está lleno, lo redimensionamos
				max *= 2; // duplicamos el tamaño
				files = realloc(files, max * sizeof(char *)); // reasignamos memoria
				if (files == NULL) { // comprobamos si hubo algún error
					fprintf(stderr,"No se pudo reasignar memoria");
					exit(EXIT_FAILURE);
				}
			}

			files[*tam] = malloc(strlen(dir->d_name) + 1); // reservamos memoria para cada cadena
			if (files[*tam] == NULL) { // comprobamos si hubo algún error en esta asignacion de memoria
				fprintf(stderr,"No se pudo reservar memoria");
				exit(EXIT_FAILURE);
			}

			strcpy(files[*tam], dir->d_name); // copiamos el nombre del archivo
			(*tam)++; // incrementamos el contador
		}
	}
	closedir(d); // cerramos el flujo de directorio
}

//-------------------------------------------------------- ImpresionListaPalabras ------------------------------------------------------
void ImpresionListaPalabras(Idioma *idioma, int *cantPalabras)
{
	system("cls");
	printf("\t\t\t---- %s ----\n", idioma->palabrasOrdenadas); // Palabras Ordenadas por dificultad
	for (int i = 0; i < *cantPalabras; i++)
	{
		printf("%s\n", idioma->palabras[i].string);
		#ifdef DEBUGMOSTRARLONGITUDPALABRAS
		printf("\tlongitud: %d \n",idioma->palabras[i].longitud);
		#endif
	}
	system("pause");
}
//-------------------------------------------------------- ComprobarEspacios ------------------------------------------------------
void ComprobarEspacios(Idioma *idioma, char *opcion)
{

	if (TieneEspacios(opcion))
	{
		// Para cada espacio en vez de _ imprimir " " y marcar no adivinable
	}
}
int TieneEspacios(char *opcion)
{
	int tam = strlen(opcion);
	for (int i = 0; i < tam; i++)
	{
		if (opcion[i] == ' ')
		{
			printf(" "); // Caracter !adivinable
			// strcpy();
			return 1;
		}
		else
		{
			printf("_"); // Caracter adivinable
		}
	}
	return 0;
}

void OrdenamientoBurbuja(Palabra *arr, int *cantPalabras)
{
	Palabra aux;
	//Ordeno las palabras por Burbuja
	for (int i = 0; i < (*cantPalabras)-1; i++)
	{
		for (int j = 0; j < (*cantPalabras)-i-1; j++)
		{
			if (arr[j].longitud>arr[j+1].longitud)
			{
				aux=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=aux;
			}
		}
	}
}

void SaludoEInstrucciones(Idioma *idioma, Jugador *jugador){
	printf("Por favor ingresa tu nombre: ");
	scanf("%d",jugador->nombre);
}