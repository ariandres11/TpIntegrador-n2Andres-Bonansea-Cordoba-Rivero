//JUEGO REALIZADO EN LENGUAJE C
//MAS DETALLES EN MI PAGINA WEB PROGRAMADOREC.COM

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <conio.h>

//PROTOTIPOS DE LAS FUNCIONES
void categorias();
void ingresoCategoria (int op);
void empezarJuego (char palabras[][15], char nombre[]);
void dibujo (int intentos);


//FUNCION PRINCIPAL MAIN
int main(){
	system("mode con cols=80 lines=25");
	categorias();
	system("pause");
	return 0;
}

//FUNCION CATEGORIAS, IMPRIME EN PANTALLA LAS CATEGORIAS Y PERMITE ELEGIR UNA
void categorias(){
	int op;
	do{
		system("cls");
		printf("\n\t\t\t\tJUEGO EL AHORCADO\n\n");
		printf(" CATEGORIAS\n\n");
		printf(" 1. Frutas\n");
		printf(" 2. Animales\n");
		printf(" 3. Paises\n");
		printf(" 4. Objetos\n\n");
		printf(" Ingresa una opcion: ");
		scanf("%i",&op);		
	}while(op<1 || op>4);

	if (op==1) ingresoCategoria(op);
	if (op==2) ingresoCategoria(op);
	if (op==3) ingresoCategoria(op);
	if (op==4) ingresoCategoria(op);
}


//ESTA FUNCION RECIBE UN VALOR, Y DEPENDIENDO DEL VALOR RECIBIDO LLAMA A OTRA FUNCION PARA EMPEZAR EL JUEGO
void ingresoCategoria (int op){
	char nombrecat[4][15] = {"Frutas","Animales","Paises","Objetos"};
	char frutas [10][15] = {"MELON","PAPAYA","SANDIA","MANZANA","PERA","NARANJA","UVA","CEREZA","CIRUELA","KIWI"};
	char animales [10][15] = {"PERRO","GATO","CABALLO","GALLINA","JIRAFA","MONO","VACA","CONEJO","TORTUGA","LOBO"};
	char paises [10][15] = {"PERU","COLOMBIA","ARGENTINA","NICARAGUA","ITALIA","MEXICO","CANADA","VENEZUELA","ECUADOR","BRASIL"};
	char objetos [10][15] = {"MOCHILA","RELOJ","ZAPATILLA","MUEBLE","CUADERNO","SILLA","MESA","CELULAR","PUERTA","AURICULARES"};
	
	switch(op){
		case 1:
			empezarJuego(frutas,nombrecat[op-1]);
			break;
		case 2:
			empezarJuego(animales,nombrecat[op-1]);
			break;
		case 3:
			empezarJuego(paises,nombrecat[op-1]);
			break;
		case 4:
			empezarJuego(objetos,nombrecat[op-1]);
			break;	
	}	
}


//FUNCION QUE CONTIENE EL ALGORITMO DEL JUEGO
void empezarJuego (char palabras[][15], char nombre[]){
	int opcion,i,j,k,longitud,espacios,puntos=1200;
	char letra;
	int aciertos = 0;
	int intentos = 0;
	int ganar = 0;
	srand(time(NULL));
	
	opcion = rand() % 10; //SE GENERA UN NUMERO ALEATORIO COMPRENDIDO ENTRE 0 Y 9
	longitud = strlen(palabras[opcion]); //SE ALMACENA LA LONGITUD DE LA PALABRA 
	char frase[longitud];
	
	//SE COLOCAN GUIONES BAJOS EN EL ARRAY DE CARACTERES FRASE
	for(i=0; i < longitud; i++){
		frase[i] = '_';
	}
	
	
	do{
		aciertos = 0;
		system("cls");
		printf("\n\t\t\t\tJUEGO EL AHORCADO\n\n");
		printf(" CATEGORIA: %s\n\n",nombre);
		printf(" Intentos Disponibles: %i\t\t\t\tPuntuacion: %i\n\n",6-intentos,puntos);
		dibujo(intentos);
		
		//IMPRIME EL ARRAY DE CARACTERES FRASE
		printf("\n\n\n");
		for(i=0; i < longitud; i++){
			printf(" %c ",frase[i]);
		}

		
		if (intentos == 6){
			printf("\n\n PERDISTE!!\n");
			printf(" LA SOLUCION ERA: %s\n\n",palabras[opcion]);
			printf(" Presiona una tecla para volver a jugar..");
			getch();
			categorias();	
		}
		
		
		//PROCESO QUE COMPRUEBA SI SE HA ADIVINADO LA PALABRA
		espacios=0;
		
		for (i = 0; i < longitud; i++){
			if (frase[i] == '_')
				espacios++;
		}
		
		
		if (espacios == 0){
			printf("\n\n FELICIDADES.. GANASTE!!\n\n");
			printf(" Presiona una tecla para volver a jugar..");
			getch();
			categorias();		
		}
		
		
		printf("\n\n Digite una letra: ");
		scanf(" %c",&letra);
		
		//PROCESO QUE VERIFICA SI LA LETRA INGRESADA EXISTE EN LA PALABRA, SI ESTO ES VERDADERO, SE REEMPLAZA EL CARACTER GUION BAJO POR LA LETRA INGRESADA 
		for (j = 0; j < longitud; j++){
			if (letra == palabras[opcion][j]){
				frase[j] = letra;
				aciertos++;
			}	
		}
		
		if (aciertos == 0){
			intentos++;	
			puntos -= 200;
		}
		
		
			
	}while(intentos != 7);
	
	
	printf("\n\n");		
}

//FUNCION QUE REALIZA EL DIBUJO DE EL AHORCADO, RECIBE EL NUMERO DE INTENTOS Y CON ESE DATO REALIZA EL DIBUJO
void dibujo (int intentos){
	switch(intentos){
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


