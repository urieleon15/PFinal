/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion. 
	(c) Ponga su nombre y numero de cuenta aqui.
	
	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/


#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "corrector.h"

// Constantes y definiciones
#define TAMTOKEN 50
#define NUMPALABRAS 100

// Funciones publicas del proyecto
/*************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
**************/
void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos) {
	FILE* fp;
	iNumElementos = 0;
	char array[TAMTOKEN];
	const char* punnt = " \t\n\r.,;()";

	for (int i = 0; i < NUMPALABRAS; i++) {
		iEstadisticas[i] = 0;
	}

	fopen_s(&fp, szNombre, "r");
	if (fp == NULL) {
		return;
	}

	while (scanf(fp, "%s", array, TAMTOKEN) != EOF) {
		// Convertir a minúsculas
		for (int i = 0; array[i] != '\0'; i++) {
			array[i] = tolower(array[i]);
		}

		// Verificar si la palabra ya existe en el diccionario
		bool found = true;
		for (int i = 0; i < iNumElementos; i++) {
			if (strcmp(szPalabras[i], array) == 0) {
				iEstadisticas[i]++;
				found = false;
				break;
			}
		}

		// Si no se encontró, agregar la palabra al diccionario
		if (!found) {
			strcpy_s(szPalabras[iNumElementos], TAMTOKEN, array);
			iEstadisticas[iNumElementos++]++;
		}
	}

	fclose(fp);

	// Ordenar las palabras y sus estadísticas
	for (int i = 0; i < iNumElementos - 1; i++) {
		for (int j = 0; j < iNumElementos - i - 1; j++) {
			if (strcmp(szPalabras[j], szPalabras[j + 1]) > 0) {
				char temp[TAMTOKEN];
				strcpy_s(temp, TAMTOKEN, szPalabras[j]);
				strcpy_s(szPalabras[j], TAMTOKEN, szPalabras[j + 1]);
				strcpy_s(szPalabras[j + 1], TAMTOKEN, temp);

				int tempStat = iEstadisticas[j];
				iEstadisticas[j] = iEstadisticas[j + 1];
				iEstadisticas[j + 1] = tempStat;
			}
		}
	}
}

/*************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
**************/
void ListaCandidatas(
	char szPalabrasSugeridas[][TAMTOKEN],
	int iNumSugeridas,
	char szPalabras[][TAMTOKEN],
	int iEstadisticas[],
	int iNumElementos,
	char szListaFinal[][TAMTOKEN],
	int iPeso[],
	int& iNumLista)
{
	iNumLista = 0;

	for (int i = 0; i < iNumSugeridas; ++i) {
		int encont = 0;
		int duplicado = 0;

		// Verificar si la palabra sugerida está en el arreglo de palabras
		for (int j = 0; j < iNumElementos; ++j) {
			if (strcmp(szPalabrasSugeridas[i], szPalabras[j]) == 0) {
				encontrado = 1;

				// Verificar si la palabra sugerida ya está en la lista final
				for (int k = 0; k < iNumLista; ++k) {
					if (strcmp(szListaFinal[k], szPalabrasSugeridas[i]) == 0) {
						duplicado = 1;
						break;
					}
				}

				if (encontrado && !duplicado) {
					strcpy(szListaFinal[iNumLista], szPalabrasSugeridas[i]);
					iPeso[iNumLista++] = iEstadisticas[j];
				}

				break;
			}
		}
	}


	// Ordenar la lista final por peso usando el algoritmo de selección
	for (int i = 0; i < iNumLista - 1; i++) {
		int InMaxi = i;

		for (int j = i + 1; j < iNumLista; j++) {
			if (iPeso[j] > iPeso[InMaxi]) {
				InMaxi = j;
			}
		}

		if (InMaxi != i) {
			int tempPeso = iPeso[i];
			iPeso[i] = iPeso[InMaxi];
			iPeso[InMaxi] = tempPeso;

			char tempPalabra[TAMTOKEN];
			strcpy_s(tempPalabra, TAMTOKEN, szListaFinal[i]);
			strcpy_s(szListaFinal[i], TAMTOKEN, szListaFinal[InMaxi]);
			strcpy_s(szListaFinal[InMaxi], TAMTOKEN, tempPalabra);
		}
	}
}

/*************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
**************/
void ClonaPalabras(
	char* szPalabraLeida,
	char szPalabrasSugeridas[][TAMTOKEN],
	int& iNumSugeridas)
{
	// Asignación de una sola palabra sugerida
	strcpy_s(szPalabrasSugeridas[0], TAMTOKEN, szPalabraLeida);
	iNumSugeridas = 1;

	// Definición del abecedario
	const char* abecedario = "abcdefghijklmnopqrstuvwxyzáéíóú";
	int longi = strlen(abecedario);

	// Copiar la palabra leída a la lista de palabras sugeridas
	if (iNumSugeridas < TAMTOKEN) {
		strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, szPalabraLeida);
	}

	// Generar palabras sugeridas
	int lpalabral = strlen(szPalabraLeida);

	for (int i = 0; i < lpalabral; ++i) {
		for (int j = 0; j < longi; ++j) {
			char varian[TAMTOKEN];
			int k = 1;

			for (int m = 0; m < lpalabral; ++m) {
				if (m != i) {
					varian[k++] = szPalabraLeida[m]; // Copiar caracteres distintos al de la posición i
				}
				else {
					varian[k++] = abecedario[j]; // Reemplazar el carácter en la posición i con uno del abecedario
				}
			}

			varian[k] = '\0'; // Asegurar la terminación de la cadena

			// Verificar si la palabra sugerida ya está en la lista
			int sifound = 0;
			int l = 0;

			while (l < iNumSugeridas && strcmp(varian, szPalabrasSugeridas[l]) != 0) {
				l++;
			}

			if (l < iNumSugeridas) {
				sifound = 1;
			}


			// Si la palabra sugerida no está ya en la lista, agrégala
			if (!sifound) {
				strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, varian);
			}
		}
	}
}
