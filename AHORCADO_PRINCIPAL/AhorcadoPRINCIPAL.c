#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <time.h>

// DEBUG
// #define DEBUGPRINTFPROBADAS
#define DEBUGPALABRA // palabra a encontrar
#define DEBUGTRGISTRO

// Constantes
#define INTENTOSGB 6 // cantidad de intentos en el juego (intentos GloBales)


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
    char *listPalOrd;
    char *setIdi;
    char *salir;
    char *selOP;
    char *invOP;
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
//Menues y parte grafica
void MenuInicio(Idioma *idioma, int cantPalabras);
void EmpezarJuego(Idioma *idioma, int cantPalabras);
void DibujarAhorcado(int intentos);
void MostrarLetrasProbadas(Idioma *idioma, char letras[6], int intentos);

//Logica del juego
void CompararPalabras(char *palabraIngresada, char *palabraBuscada, int longitud, char *frase);
int AcertarLetra(char letra, char *palabra, int longitud, char *frase);
void JuegoGanado(Idioma *idioma, int cantPalabras, char *frase, int longitud);
void JuegoPerdido(Idioma *idioma, int cantPalabras, char *palabra, int intentos);
void VolverAJugar(Idioma *idioma, int cantPalabras);

//Funciones de la carga del Idioma
int ContarPalabras(char *string);
void GuardarPalabras(Idioma *idioma,char *string);
int CargarIdioma(Idioma *idioma,char *nombreArchivo);
void AsignarMemoria(char **puntero,FILE *fp);
void ImpresionDebugRegistro(Idioma *idioma, int cant);
void GuardarIdioma();

//---------------------------------------------------------- MAIN -----------------------------------------------------------------------
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
    cantPalabras = CargarIdioma(idioma,base);
		
	//Impresion DEBUG del registro
	#ifdef DEBUGTRGISTRO
		ImpresionDebugRegistro(idioma,cantPalabras);
	#endif

	MenuInicio(idioma,cantPalabras);
	system("pause");
	return 0;
}

//------------------------------------------------------------- MenuInicio --------------------------------------------------------------
// La funcion muestra el menu del juego y permite seleccionar una opcion
void MenuInicio(Idioma *idioma, int cantPalabras)
{
	int op;
	do
	{
		system("cls"); // Limpia la consola antes de volver a escribir
		printf("\n\t\t\t\t %s \n\n",idioma->titulo); // JUEGO EL AHORCADO
		printf( "\t%s\n\n",idioma->menu); //**** MENU ****
		printf(" %s\n",idioma->jugar); //1. JUGAR
		printf(" %s\n",idioma->ranking); //2. RANKING DE JUGADORES
		printf(" %s\n",idioma->listPalOrd); //3. MOSTRAR LISTA DE PALABRAS ORDENADA POR DIFICULTAD (LONGITUD)
		printf(" %s\n",idioma->setIdi); //4. GUARDAR IDIOMA ACTUAL COMO POR DEFECTO
		printf(" %s\n\n",idioma->salir); //5. SALIR

		//Validacion del ingreso de la opcion
		do
		{
			printf(" %s ",idioma->selOP); //Seleccione una opcion:
			scanf("%i", &op);
			if (op < 1 || op > 4)
			{
				printf("X %s X\n",idioma->invOP); //Opcion Invalida
			}
		} while (op < 1 || op > 5);
		
		switch (op)
		{
		case 1:
			EmpezarJuego(idioma,cantPalabras);
			break;

		case 2:
			/* code */
			break;

		case 3:
			/* code */
			break;

		case 4:
			GuardarIdioma();
			break;

		case 5:
			/* code */
			break;

		default:
			break;
		}

	} while (op != 5);
}



//--------------------------------------------------------------- EmpezarJuego ---------------------------------------------------------------
// Bucle del juego
void EmpezarJuego(Idioma *idioma,int cantPalabras)
{
	int opcion, longitud, espacios, puntos = 1200;
	char letras[15];
	int intentos = 0;
	int cantLetrasIng = 0;
	char letrasProbadas[INTENTOSGB + 10];

	srand(time(NULL)); //Randomizacion

	opcion = rand() % (cantPalabras-1);				 // Se genera el numero aleatorio de la palabra entre 1 y 9
	longitud = idioma->palabras[opcion].longitud; // Se almacena el tamaño de la palabra

	char *palabraBuscada = idioma->palabras[opcion].string; //Se almacena la palabra Buscada (NUEVO)

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
		printf("\n\t\t\t\t%s\n\n",idioma->titulo); //JUEGO EL AHORCADO
		printf(" %s %d\n %s %d\n\n",idioma->intentos, 6 - intentos,idioma->puntuacion, puntos); //Intentos Disponibles: , Puntuacion:

	//POR ACA TA EL PROBLEMA XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	#ifdef DEBUGPALABRA
		printf("palabra a buscar: %s", idioma->palabras[opcion]);
	#endif

		if (cantLetrasIng > 0)
			MostrarLetrasProbadas(idioma,letrasProbadas, cantLetrasIng);
		
		DibujarAhorcado(intentos);

		// Muestra las letras descubiertas hasta el momento en pantalla
		printf("\n\n\n");
		for (int i = 0; i < longitud; i++)
		{
			printf(" %c ", frase[i]);
		}

		printf("\n\n %s ",idioma->digiteLetra); //Digite una letra:
		scanf("%s", &letras);
		strcpy(letras, strupr(letras)); // pasa las letras que estan en minuscula a mayuscula para que la comparacion sea efectiva

		// comprobacion de acierto de letra o palabra completa
		if (strlen(letras) > 1)
		{
			CompararPalabras(letras, palabraBuscada, longitud, frase);
			intentos = INTENTOSGB;
			puntos = 0;
		}
		else
		{
			letrasProbadas[cantLetrasIng] = letras[0];
			cantLetrasIng++;
			if (AcertarLetra(letras[0], palabraBuscada, longitud, frase) == 0)
			{
				intentos++;
				puntos -= 200;
			}
		}

		// comprobacion de finalizacion del juego
		JuegoGanado(idioma, cantPalabras, frase, longitud);
		JuegoPerdido(idioma, cantPalabras, palabraBuscada, intentos);
	} while (intentos <= INTENTOSGB);

	printf("\n\n");
}

//--------------------------------------------------------- VolverAJugar -------------------------------------------------------------------
void VolverAJugar(Idioma *idioma, int cantPalabras)
{
	printf(" %s",idioma->volverAJugar); //Presiona una tecla para volver a jugar..
	getch();
	MenuInicio(idioma,cantPalabras);
}

//--------------------------------------------------------- JuegoPerdido -------------------------------------------------------------------
// comprobacion de condicion de finalizacion
void JuegoPerdido(Idioma *idioma, int cantPalabras, char *palabra, int intentos)
{
	if (intentos == INTENTOSGB)
	{
		printf("\n\n %s\n",idioma->perdiste); //PERDISTE!!
		printf(" %s %s\n\n",idioma->solEra ,palabra); //LA SOLUCION ERA:
		VolverAJugar(idioma, cantPalabras);
	}
}

//--------------------------------------------------------- JuegoGanado -------------------------------------------------------------------
// comprobacion de condicion para ganar
void JuegoGanado(Idioma *idioma, int cantPalabras, char *frase, int longitud) 
{
	int espacios = 0;

	for (int i = 0; i < longitud; i++)
	{
		if (frase[i] == '_')
			espacios++;
	}

	if (espacios == 0)
	{
		printf("\n %s\n\n",idioma->ganaste); //FELICIDADES.. GANASTE!!
		VolverAJugar(idioma, cantPalabras);
	}
}

//--------------------------------------------------------- CompararPalabras -------------------------------------------------------------------
//funcion que se encarga de comparar la palabra ingresada y la guarda en el array de frase
void CompararPalabras(char *palabraIngresada, char *palabraBuscada, int longitud, char *frase)
{
	if (strcmp(palabraIngresada, palabraBuscada) == 0)
	{
		for (int j = 0; j < longitud; j++)
		{
			frase[j] = palabraIngresada[j];
		}
	}
	else
	{
		printf("la palabra ingresada es erronea\n");
		getchar();
		getchar();
	}
}

//------------------------------------------------------------- AcertarLetra ----------------------------------------------------------------
//funcion que se encaarga de buscar si la letra ingresada se encuentra en la palabra y la guarda en esa posicion
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
void MostrarLetrasProbadas(Idioma *idioma, char letras[INTENTOSGB + 10], int intentos)
{
	printf("\t Letras probadas: ");
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


//-------------------------------------------------------- ContarPalabras ------------------------------------------------------------------
int ContarPalabras(char *string){
    int cont = 0;
    char *token = strtok(string,",");
    while (token != NULL) {
        token = strtok(NULL,",");
        cont++;
    }
    return cont;
}

//------------------------------------------------------- GuardarPalabras --------------------------------------------------------------------
void GuardarPalabras(Idioma *idioma, char *string){
    int cont = 0;
    char *token = strtok(string,",");
    while (token != NULL) {
		strcpy(idioma->palabras[cont].string,token); //Guardo en registro el string
		idioma->palabras[cont].longitud = strlen(token); //Guardo en registro la longitud del string
		token = strtok(NULL,","); //Divido
        cont++; //Incremento contador para la posicion del arreglo de palabras
    }
}

//----------------------------------------------------------- CargarIdioma --------------------------------------------------------------
int CargarIdioma(Idioma *idioma,char *nombreArchivo){
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
		cantPalabras = ContarPalabras(aux);
		idioma->palabras = (Palabra *) malloc(cantPalabras * sizeof(Palabra)); //Calculo el tamaño contando la cantidad de palabras
		GuardarPalabras(idioma,string);

		//Resto de strings
			//Titulo
			AsignarMemoria(&idioma->titulo,fp);

			//Menu
			AsignarMemoria(&idioma->menu,fp);

			//Jugar
			AsignarMemoria(&idioma->jugar,fp);

			//Ranking
			AsignarMemoria(&idioma->ranking,fp);
			
			//Mostrar Lista de palabras ordenadas por dificultad
			AsignarMemoria(&idioma->listPalOrd,fp);

			//Setear Idioma por defecto
			AsignarMemoria(&idioma->setIdi,fp);

			//Salir
			AsignarMemoria(&idioma->salir,fp);

        	//Ingrese opcion
			AsignarMemoria(&idioma->selOP,fp);

        	//Opcion Invalida
			AsignarMemoria(&idioma->invOP,fp);

			//Intentos Disponibles
			AsignarMemoria(&idioma->intentos,fp);

			//Puntuacion
			AsignarMemoria(&idioma->puntuacion,fp);

        	//Perdiste
			AsignarMemoria(&idioma->perdiste,fp);

        	//La sol era:
			AsignarMemoria(&idioma->solEra,fp);

        	//Ganaste:
			AsignarMemoria(&idioma->ganaste,fp);

			//Volver a jugar:
			AsignarMemoria(&idioma->volverAJugar,fp);

			//Digite Letra:
			AsignarMemoria(&idioma->digiteLetra,fp);

			//Letras probadas:
			AsignarMemoria(&idioma->letProb,fp);
    }

    //Cierro el archivo de los idiomas pq no lo necesito mas por ahora
    fclose(fp);
	//Devuelvo la cantidad de palabras
	return cantPalabras;
}

//------------------------------------------------------------- AsignarMemoria -------------------------------------------------------------
void AsignarMemoria(char **puntero,FILE *fp){
	char string[100];
	fgets(string,sizeof(string),fp);
	*puntero = (char *)malloc((strlen(string)+1)*sizeof(char));//Aca esta el problema en ambos casos (DEPENDE DE SI ABRO CON IDIOMA POR DEFAULT O PASANDO IDIOMA)
	
	if(*puntero == NULL){
		fprintf(stderr,"EL PUNTERO ES NULL\n");
		exit(EXIT_FAILURE);
	}
	strcpy(*puntero,string);
}

//-------------------------------------------------------- ImpresionDebugRegistro ------------------------------------------------------
void ImpresionDebugRegistro(Idioma *idioma,int cant){
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

//-------------------------------------------------------------- GuardarIdioma ------------------------------------------------------------
void GuardarIdioma(){

}