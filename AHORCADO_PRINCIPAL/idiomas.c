#include "idiomas.h"

//-------------------------------------------------------- Evaluar Parametros ------------------------------------------------------------------
//Evalua los parametros recibidos al ejecutar el juego para saber en que modo inicializar el idioma
void EvaluarParametros(Idioma *idioma, char *base, bool *debug, char *argv[], int argc)
{
	*debug = false; // Antes de evaluar parametros digo que debug esta desactivado
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
		char input[strlen(PASSWORD)];						// Para el input del usuario de la contraseña
		setColor(GRIS);
		printf("\nIngrese la contrasena del modo debug: "); // EL MODO DEBUG SIEMPRE SE EJECUTA EN ESPAÑOL
		scanf("%s", input);
		if (strcmp(PASSWORD, input) != 0)
		{
			fprintf(stderr, "\n\tXXXX CONTRASENA INCORRECTA XXXX\n"); // EL MODO DEBUG SIEMPRE SE EJECUTA EN ESPAÑOL
			exit(EXIT_FAILURE);
		}
		printf("\n\tMODO DEBUG CON IDIOMA DEFAULT ACTIVADO!\n\n"); // EL MODO DEBUG SIEMPRE SE EJECUTA EN ESPAÑOL
		setColor(BLANCO);
		system("pause");
	}
}

//-------------------------------------------------------- Contar Palabras ------------------------------------------------------------------
//Cuenta la cantidad de palabras introducidas en el archivo de idioma elegido
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

//------------------------------------------------------- Guardar Palabras --------------------------------------------------------------------
//Guarda las palabras del archivo de idioma elegido en un arreglo de palabras
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

//-------------------------------------------------------- Cargar Idioma --------------------------------------------------------------
//Carga el registro de idioma con todos los strings del archivo de idioma
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
		strtok(string, "\n");												   // Elimina el salto de línea del fgets utilizando strtok
		idioma->palabras = (Palabra *)malloc(*cantPalabras * sizeof(Palabra)); // Calculo el tamaño contando la cantidad de palabras
		GuardarPalabras(idioma, string);

		// Resto de strings
		// Por favor ingresa tu nombre:
		AsignarMemoria(&idioma->ingresaNom, fp);

		// Bienvenido
		AsignarMemoria(&idioma->bienvenido, fp);

		// ---- Reglas del juego ----
		AsignarMemoria(&idioma->reglasJuego, fp);

		// Tendras 6 intentos para adivinar una palabra, podes ingresar caracter a caracter o ingresar la palabra completa,
		AsignarMemoria(&idioma->reglas1, fp);

		// pero cuidado porque si fallas perdes automaticamente. Buena Suerte!
		AsignarMemoria(&idioma->reglas2, fp);

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

		// ----- RANKING DE JUGADORES -----
		AsignarMemoria(&idioma->rankingJugadores, fp);

		// intentos,
		AsignarMemoria(&idioma->intentosRanking, fp);

		// segundos
		AsignarMemoria(&idioma->segundosRanking, fp);

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

//------------------------------------------------------------- Asignar Memoria -------------------------------------------------------------
//Reserva la memoria suficiente para cada string del registro de idioma
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

//-------------------------------------------------------- Cargar Default A Base ------------------------------------------------------
//Carga el idioma del archivo default al string base
void CargarDefaultABase(char *base)
{
	// Busco el nombre del archivo default a leer en el archivo default
	FILE *dl = fopen("Idiomas/_default.dat", "r");
	
	// Error al abrir el archivo
	if (dl == NULL)
	{
		fprintf(stderr, "No se pudo abrir correctamente el archivo Idiomas/_default.dat\n");
		exit(EXIT_FAILURE);
	}

	fscanf(dl, "%[^\n]", base);
	fclose(dl);
}

//-------------------------------------------------------------- Guardar Idioma ------------------------------------------------------------
//Permite seleccionar el idioma por defecto del juego
void GuardarIdioma(Idioma *idioma)
{
	FILE *dl = fopen("Idiomas/_default.dat", "r"); // Abro archivo en modo lectura para leer el idioma que esta por defecto
	
	system("cls"); // Limpia la consola

	// Error al abrir el archivo
	if (dl == NULL)
	{
		fprintf(stderr, "No se pudo abrir correctamente el archivo Idiomas/_default.dat\n");
		exit(EXIT_FAILURE);
	}

	char porDefecto[TSTRCHICO];
	int OpcionIdiomas;										// Selector de opcion
	int tam = 0;								// Contador de archivos
	char **files = malloc(10 * sizeof(char *)); // reservamos memoria para el arreglo de nombres de archivos

	fgets(porDefecto, sizeof(porDefecto), dl);						  // Guardo el nombre del archivo que hay en el archivo default
	fclose(dl);	                                                      // Cierro el archivo abierto en modo lectura para despues abrirlo en modo escritura
	
	setColor(CIAN_CLARO);													  
	printf("\t---- %s %s ----\n", idioma->idiomaDefecto, porDefecto); // Idioma por defecto:
	setColor(BLANCO);

	LeerDirectorio(files, &tam);				  // Funcion de leer todos los archivos en el directorio de Idioma
	printf("\n%s\n", idioma->idiomasDisponibles); // Idiomas disponibles:

	// Imprimo los Idiomas del archivo
	for (int i = 0; i < tam; i++)
	{
		printf("%d. %s\n", (i + 1), files[i]);
	}

	do
	{
		printf("\n%s %d:\n", idioma->intercambiarIdioma, tam); // Elija un idioma del 1 al n
		scanf("%d", &OpcionIdiomas);

		if (OpcionIdiomas < 1 || OpcionIdiomas > tam)
		{
			printf("\t%s\n", idioma->invOP); // Opcion Invalida!
		}

	} while (OpcionIdiomas < 1 || OpcionIdiomas > tam);

	// Cargo el idioma con el nombre del archivo elegido
	dl = fopen("Idiomas/_default.dat", "w");					// Abro el archivo en modo escritura para limpiarlo del anterior

	// Error al abrir el archivo
	if (dl == NULL)
	{
		fprintf(stderr, "No se pudo abrir correctamente el archivo Idiomas/_default.dat\n");
		exit(EXIT_FAILURE);
	}

	fputs("Idiomas/", dl);										// Le agrego al contenido del archivo el directorio de los idiomas
	fputs(files[OpcionIdiomas - 1], dl);									// Le agrego al archivo el nombre del idioma
	fclose(dl);													// Cierro el archivo en modo escritura
	printf("\n%s %s\n\n", idioma->idiomaDefecto, files[OpcionIdiomas - 1]); // Idioma por defecto:

	// LIBERO MEMORIA DE CADA NOMBRE DE ARCHIVO DE DIRECTORIO Y DEL ARREGLO DE DIRECTORIOS
	for (int i = 0; i < tam; i++)
	{
		free(files[i]);
	}
	free(files);

	system("pause");
}

//-------------------------------------------------------- Leer Directorio ------------------------------------------------------
//Lee el directorio de idiomas y almacena los nombres de los archivos en un array de strings
void LeerDirectorio(char **files, int *tam)
{
	DIR *d = opendir("Idiomas"); // puntero a un flujo de directorio
	struct dirent *dir;			 // estructura para almacenar información de cada entrada

	if (d == NULL)
	{ // comprobamos si hubo algún error
		fprintf(stderr, "No se pudo abrir el directorio");
		exit(EXIT_FAILURE);
	}

	int max = 10; // tamaño inicial del arreglo (En caso de que existan mas que 10 idiomas se adecua la memoria para todos)

	if (files == NULL)
	{ // comprobamos si hubo algún error
		fprintf(stderr, "No se pudo reservar memoria");
		exit(EXIT_FAILURE);
	}

	while ((dir = readdir(d)) != NULL)
	{ // leemos cada entrada del directorio
		if (strcmp(dir->d_name, "_default.dat") != 0 && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
		{ // ignoramos los directorios "." , ".." y el del idioma default

			// Condicional para evaluar si hay que redimensionar la memoria reservada
			if (max == *tam)
			{												  // si el arreglo está lleno, lo redimensionamos
				max *= 2;									  // duplicamos el tamaño
				files = realloc(files, max * sizeof(char *)); // reasignamos memoria
				if (files == NULL)
				{ // comprobamos si hubo algún error
					fprintf(stderr, "No se pudo reasignar memoria");
					exit(EXIT_FAILURE);
				}
			}

			files[*tam] = malloc(strlen(dir->d_name) + 1); // reservamos memoria para cada cadena
			if (files[*tam] == NULL)
			{ // comprobamos si hubo algún error en esta asignacion de memoria
				fprintf(stderr, "No se pudo reservar memoria");
				exit(EXIT_FAILURE);
			}

			strcpy(files[*tam], dir->d_name); // copiamos el nombre del archivo
			(*tam)++;						  // incrementamos el contador
		}
	}
	closedir(d); // cerramos el flujo de directorio
}

//-------------------------------------------------------- Poner color en la terminal ------------------------------------------------------
//Permite setear el color del texto de la terminal a uno en especifico (parte visual)
void setColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}