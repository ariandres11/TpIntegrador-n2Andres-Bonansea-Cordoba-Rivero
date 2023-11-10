#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#define INTENTOSGB 7 // cantidad de intentos de errores en el juego
#define DEBUG

//REGISTROS
typedef struct{
    char string[10000];
    int longitud;
}Palabra;

typedef struct{
    char *titulo;
    char *menu;
    char *jugar;
    char *ranking;
    char *setIdi;
    char *salir;
    char *selOP;
	char *intentos;
	char *puntuacion;
	char *perdiste;
	char *solEra;
	char *ganaste;
	char *volverAJugar;
	char *digiteLetra;
	char *letProb;
    Palabra *palabras;
}Idioma;

// PROTOTIPOS DE LAS FUNCIONES
void categorias(Idioma *idioma);
void ingresoCategoria(Idioma *idioma, int op);
void empezarJuego(Idioma *idioma, char palabras[][15], char nombre[]);
void dibujo(Idioma *idioma, int intentos);
void MostrarLetrasProbadas(Idioma *idioma, char letras[6], int intentos);

//Funciones de la carga del Idioma
int contarPalabras(char *string);
void guardarPalabras(Idioma *idioma,char *string);
int cargarIdioma(Idioma *idioma,char *nombreArchivo);
void asignarMemoria(char **puntero,FILE *fp);
void impresionDebugRegistro(Idioma *idioma,int cant);


// FUNCION PRINCIPAL MAIN
int main(int argc, char *argv[])
{
    Idioma *idioma = (Idioma *)malloc(sizeof(Idioma)); //Reservo memoria para el registro de idioma completo
    int cantPalabras;
	
    char base[20];
    if (argc < 2) {
		//Busco el nombre del archivo default a leer en el archivo default
		FILE *dl = fopen("_default.dat","r");
		fscanf(dl, "%[^\n]", base); 
		fclose(dl);
    }else{
		strcpy(base,argv[1]); //Utilizo base en ambos casos para prevenir errores
	}
    cantPalabras = cargarIdioma(idioma,base);
		
	//Impresion DEBUG del registro
	#ifdef DEBUG
		impresionDebugRegistro(idioma,cantPalabras);
	#endif

	categorias(idioma);
	system("pause");
	return 0;
}

// FUNCION CATEGORIAS, IMPRIME EN PANTALLA LAS CATEGORIAS Y PERMITE ELEGIR UNA
void categorias(Idioma *idioma)
{
	int op;
	do
	{
		system("cls");
		printf("\n\t\t\t\t %s \n\n",idioma->titulo);
		printf( "\t%s\n\n",idioma->menu);
		printf(" %s\n",idioma->jugar);
		printf(" %s\n",idioma->ranking);
		printf(" %s\n",idioma->setIdi);
		printf(" %s\n\n",idioma->salir);
		printf(" %s ",idioma->selOP);
		scanf("%i", &op);
	} while (op < 1 || op > 4);

	if (op == 1)
		ingresoCategoria(idioma, op);
	if (op == 2)
		ingresoCategoria(idioma, op);
	if (op == 3)
		ingresoCategoria(idioma, op);
	if (op == 4)
		ingresoCategoria(idioma, op);
}

// ESTA FUNCION RECIBE UN VALOR, Y DEPENDIENDO DEL VALOR RECIBIDO LLAMA A OTRA FUNCION PARA EMPEZAR EL JUEGO
void ingresoCategoria(Idioma *idioma, int op)
{
	char nombrecat[4][15] = {"Frutas", "Animales", "Paises", "Objetos"};
	char frutas[10][15] = {"MELON", "PAPAYA", "SANDIA", "MANZANA", "PERA", "NARANJA", "UVA", "CEREZA", "CIRUELA", "KIWI"};
	char animales[10][15] = {"PERRO", "GATO", "CABALLO", "GALLINA", "JIRAFA", "MONO", "VACA", "CONEJO", "TORTUGA", "LOBO"};
	char paises[10][15] = {"PERU", "COLOMBIA", "ARGENTINA", "NICARAGUA", "ITALIA", "MEXICO", "CANADA", "VENEZUELA", "ECUADOR", "BRASIL"};
	char objetos[10][15] = {"MOCHILA", "RELOJ", "ZAPATILLA", "MUEBLE", "CUADERNO", "SILLA", "MESA", "CELULAR", "PUERTA", "AURICULARES"};

	switch (op)
	{
	case 1:
		empezarJuego(idioma, frutas, nombrecat[op - 1]);
		break;
	case 2:
		empezarJuego(idioma, animales, nombrecat[op - 1]);
		break;
	case 3:
		empezarJuego(idioma, paises, nombrecat[op - 1]);
		break;
	case 4:
		empezarJuego(idioma, objetos, nombrecat[op - 1]);
		break;
	}
}

// FUNCION QUE CONTIENE EL ALGORITMO DEL JUEGO
void empezarJuego(Idioma *idioma, char palabras[][15], char nombre[])
{
	int opcion, i, j, k, longitud, espacios, puntos = 1200;
	char letra;
	int aciertos = 0;
	int intentos = 0;
	int ganar = 0;
	int letraEncontrada; // funciona como booleano para decir si esta la letra o no en la palabra
	char letrasProbadas[INTENTOSGB];
	for (int i = 0; i < INTENTOSGB; i++)
	{
		letrasProbadas[i] = ' ';
	}
	srand(time(NULL));

	opcion = rand() % 10;				 // SE GENERA UN NUMERO ALEATORIO COMPRENDIDO ENTRE 0 Y 9
	longitud = strlen(palabras[opcion]); // SE ALMACENA LA LONGITUD DE LA PALABRA
	char frase[longitud];

	// SE COLOCAN GUIONES BAJOS EN EL ARRAY DE CARACTERES FRASE
	for (i = 0; i < longitud; i++)
	{
		frase[i] = '_';
	}

	do
	{
		letraEncontrada = 0;
		aciertos = 0;
		system("cls");
		printf("\n\t\t\t\t%s\n\n",idioma->titulo);
		printf(" %s %s\n\n",idioma->menu,nombre);
		printf(" %s %i\t\t\t\t%s %i\n\n",idioma->intentos,6-intentos,idioma->puntuacion,puntos);

		if (intentos > 0)
			MostrarLetrasProbadas(idioma,letrasProbadas, intentos);
		dibujo(idioma,intentos);

		// IMPRIME EL ARRAY DE CARACTERES FRASE
		printf("\n\n\n");
		for (i = 0; i < longitud; i++)
		{
			printf(" %c ", frase[i]);
		}

		if (intentos == 6)
		{
			printf("\n\n %s\n",idioma->perdiste);
			printf(" %s %s\n\n",idioma->solEra,palabras[opcion]);
			printf(" %s",idioma->volverAJugar);
			getch();
			categorias(idioma);
		}

		// PROCESO QUE COMPRUEBA SI SE HA ADIVINADO LA PALABRA
		espacios = 0;

		for (i = 0; i < longitud; i++)
		{
			if (frase[i] == '_')
				espacios++;
		}

		if (espacios == 0)
		{
			printf("\n\n %s\n\n",idioma->ganaste);
			printf(" %s",idioma->volverAJugar);
			getch();
			categorias(idioma);
		}

		printf("\n\n %s: ",idioma->digiteLetra);
		scanf(" %c", &letra);

		// PROCESO QUE VERIFICA SI LA LETRA INGRESADA EXISTE EN LA PALABRA, SI ESTO ES VERDADERO, SE REEMPLAZA EL CARACTER GUION BAJO POR LA LETRA INGRESADA
		for (j = 0; j < longitud; j++)
		{
			if (letra == palabras[opcion][j])
			{
				frase[j] = letra;
				aciertos++;
				letraEncontrada = 1;
			}
		}

		if (letraEncontrada == 0)
		{
			letrasProbadas[intentos] = letra;
		}

		if (aciertos == 0)
		{
			intentos++;
			puntos -= 200;
		}

	} while (intentos != INTENTOSGB);

	printf("\n\n");
}

// FUNCION QUE REALIZA EL DIBUJO DE EL AHORCADO, RECIBE EL NUMERO DE INTENTOS Y CON ESE DATO REALIZA EL DIBUJO
void dibujo(Idioma *idioma, int intentos)
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

void MostrarLetrasProbadas(Idioma *idioma, char letras[INTENTOSGB], int intentos)
{
	printf("%s ",idioma->letProb);
	for (int i = 0; i <= intentos; i++)
	{
		printf("%c ", letras[i]);
	}
}

int contarPalabras(char *string){
    int cont = 0;
    char *token = strtok(string,",");
    while (token != NULL) {
        token = strtok(NULL,",");
        cont++;
    }
    return cont;
}

void guardarPalabras(Idioma *idioma, char *string){
    int cont = 0;
    char *token = strtok(string,",");
    while (token != NULL) {
		strcpy(idioma->palabras[cont].string,token); //Guardo en registro
		token = strtok(NULL,","); //Divido
        cont++; //Incremento contador para la posicion del arreglo de palabras
    }
}

int cargarIdioma(Idioma *idioma,char *nombreArchivo){
	FILE *fp;
    char string[10000]; //Para hacer la separacion de las palabras
    char aux[10000];
	int cantPalabras;

    fp = fopen(nombreArchivo, "r");

    //Error al abrir el archivo
    if (fp == NULL) {
		fprintf(stderr,"No se pudo abrir correctamente el archivo en: %s\n", nombreArchivo);
        exit(EXIT_FAILURE);
    }
    //El archivo se abre correctamente
    printf("El archivo se abrio correctamente! %s\n", nombreArchivo);
    
    //El archivo se encuentra vacio
    if (fgets(string,sizeof(string),fp)==NULL)
    {
        printf("\n\tEl archivo se encuentra vacio!\n");
    
    //El archivo tiene algo
    }else{
        //Reinicio el cursor por lo de arriba
        rewind(fp);

		//Palabras
		fgets(string,sizeof(string),fp);
		strcpy(aux,string);
		cantPalabras = contarPalabras(aux);
		idioma->palabras = (Palabra *) malloc(cantPalabras * sizeof(Palabra)); //Calculo el tamaño contando la cantidad de palabras
		guardarPalabras(idioma,string);

		//Resto de strings
			//Titulo
			asignarMemoria(&idioma->titulo,fp);

			//Menu
			asignarMemoria(&idioma->menu,fp);

			//Jugar
			asignarMemoria(&idioma->jugar,fp);

			//Ranking
			asignarMemoria(&idioma->ranking,fp);

			//Setear Idioma por defecto
			asignarMemoria(&idioma->setIdi,fp);

			//Salir
			asignarMemoria(&idioma->salir,fp);

        	//Ingrese opcion
			asignarMemoria(&idioma->selOP,fp);

			//Intentos Disponibles
			asignarMemoria(&idioma->intentos,fp);

			//Puntuacion
			asignarMemoria(&idioma->puntuacion,fp);

        	//Perdiste
			asignarMemoria(&idioma->perdiste,fp);

        	//La sol era:
			asignarMemoria(&idioma->solEra,fp);

        	//Ganaste:
			asignarMemoria(&idioma->ganaste,fp);

			//Volver a jugar:
			asignarMemoria(&idioma->volverAJugar,fp);

			//Digite Letra:
			asignarMemoria(&idioma->digiteLetra,fp);

			//Letras probadas:
			asignarMemoria(&idioma->letProb,fp);
    }

    //Cierro el archivo de los idiomas pq no lo necesito mas por ahora
    fclose(fp);
	//Devuelvo la cantidad de palabras
	return cantPalabras;
}

void asignarMemoria(char **puntero,FILE *fp){
	char string[100];
	fgets(string,sizeof(string),fp);
	*puntero = (char *)malloc((strlen(string)+1)*sizeof(char));//Aca esta el problema en ambos casos (DEPENDE DE SI ABRO CON IDIOMA POR DEFAULT O PASANDO IDIOMA)
	
	if(*puntero == NULL){
		fprintf(stderr,"EL PUNTERO ES NULL\n");
		exit(EXIT_FAILURE);
	}
	strcpy(*puntero,string);
}

void impresionDebugRegistro(Idioma *idioma,int cant){
	printf("idioma->titulo: %s\n",idioma->titulo);
	printf("idioma->menu: %s\n",idioma->menu);
	printf("idioma->jugar: %s\n",idioma->jugar);
	printf("idioma->ranking: %s\n",idioma->ranking);
	printf("idioma->setIdi: %s\n",idioma->setIdi);
	printf("idioma->salir: %s\n",idioma->salir);
	printf("idioma->selOP: %s\n",idioma->selOP);
	printf("idioma->intentos: %s\n",idioma->intentos);
	printf("idioma->puntuacion: %s\n",idioma->puntuacion);
	printf("idioma->solEra: %s\n",idioma->solEra);
	printf("idioma->ganaste: %s\n",idioma->ganaste);
	printf("idioma->volverAJugar: %s\n",idioma->volverAJugar);
	printf("idioma->digiteLetra: %s\n",idioma->digiteLetra);
	printf("Palabras:\n");
	for (int i = 0; i < cant; i++)
	{
		printf("%s\n",idioma->palabras[i].string);
	}	
}