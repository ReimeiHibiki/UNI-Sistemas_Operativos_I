#include <iostream>
#include <stdlib.h>
#include <windows.h>

FILE *archivo;

using namespace std;

int main(int argc, char *argv[])
{
	//................Variables.........................
	unsigned int valor;
	unsigned char buff[512];
	int pos;

	FILE *salida;
	salida = fopen("ARQ_mbr.bin", "rb");
	fread(buff, sizeof(buff), 1, salida);

	//...........Operaciones y movimientos de los bytes
	for (pos = 446; pos < 511; pos = pos + 16)
	{
		cout << "\nAnalizando la posicion " << pos << endl;

		valor = buff[pos + 0];
		if (valor == 128)
		{
			printf("\nEl sistema esta empezando...\n");
			printf("\nLa tabla es booteable!");
		}
		else if (valor == 0)
		{
			printf("La tabla no es booteable.\n");
		}
		else
		{
			printf("ERROR");
		}

		// byte 01
		printf("\nByte 01\n");
		valor = (unsigned int)buff[pos + 1];
		printf("Cabeza inicial = %d\n", valor);

		// byte 02
		printf("\nByte 02\n");
		valor = (unsigned int)buff[pos + 2];
		valor = valor << 2;
		valor = valor >> 2;
		cout << valor << endl;

		// byte 03
		printf("\nByte 03\n");
		WORD valor2;
		valor2 = (unsigned int)buff[pos + 3];
		valor = valor >> 6;
		WORD valor3;
		valor3 = (unsigned int)valor;
		valor3 = valor3 << 8;
		unsigned int valor4;
		valor4 = (unsigned int)valor2;
		unsigned int valor5;
		valor5 = valor3 | valor4;
		cout << valor5 << endl;
		cout << ".....Cilindro de partida....." << endl;
		valor2 = 0;
		valor3 = 0;
		valor4 = 0;
		valor5 = 0;

		// byte 04
		printf("\nByte 04\n");
		valor = (unsigned int)buff[pos + 4];
		printf("Denominacion del sistema operativo\n");
		cout << valor << endl;

		// byte 05
		printf("\nByte 05\n");
		valor = (unsigned int)buff[pos + 5];
		cout << valor << endl;

		// byte 06
		printf("\nByte 06\n");
		valor = (unsigned int)buff[pos + 6];
		valor = valor << 2;
		valor = valor >> 2;
		cout << valor << endl;

		// byte 07
		printf("\nByte 07\n");
		// WORD valor2;
		valor2 = (unsigned int)buff[pos + 7];
		valor = valor >> 6;
		// WORD valor3;
		valor3 = (unsigned int)valor;
		valor3 = valor3 << 8;
		// unsigned int valor4;
		valor4 = (unsigned int)valor2;
		// unsigned int valor5;
		valor5 = valor3 | valor4;
		cout << valor5 << endl;
		cout << ".....Cilindro de partida....." << endl;

		// byte 08 (DWORD - 4 bytes)
		printf("\nByte 08\n");
		unsigned long int v1;
		valor = (unsigned int)buff[pos + 8];
		v1 = valor;
		cout << v1 << endl;

		// byte 12 (DWORD - 4 bytes)
		printf("\nByte 12\n");
		unsigned long int v2;
		valor = (unsigned int)buff[pos + 12];
		v2 = valor;
		cout << v2 << endl;
		cout << "--------------------------------------------------" << endl;
	}
	return 0;
}
