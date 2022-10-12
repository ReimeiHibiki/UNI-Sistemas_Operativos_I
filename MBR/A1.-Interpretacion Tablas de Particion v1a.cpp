#include <iostream>
#include <stdlib.h>
#include <windows.h>

FILE*archivo;

using namespace std;

int main(int argc, char *argv[]) {
	//................variables.........................
	unsigned int valor;
	unsigned char buff[512];
    int pos;

    //...........lectura del archivo .bin ...............
    /*FILE* file_;
    file_ = fopen("ARQ_mbr.BIN", "rb");
    while (!feof(file_)) // para leer el archivo
    {
        // funcion para leer el contenido del archivo
        fread(buff, sizeof(buff), 1, file_);
        //cout << buff;
    }*/

    FILE *salida;
    salida = fopen("C:\\Users\\USERNAME\\Documents\\Code Projects\\Sistemas Operativos I\\ARQ_mbr.bin","rb");
    fread(buff, sizeof(buff ), 1, salida);

//...........operaciones y movimientos de los bytes
	for (pos = 446; pos < 511; pos = pos + 16) {
		cout <<"\nAnalizando la posicion " <<pos <<endl;
		//pos = x;
		valor = buff [pos + 0];
		if (valor == 128) {
			printf ("\nEl sistema esta empezando...\n");
	    	printf ("\nLa tabla es booteable!");
		}
		else if (valor == 0) {
			printf("La tabla no es booteable.\n");
		}
		else {
			printf ("ERROR");
		}

		//byte 01
		printf ("\nByte 01\n");
		valor = (unsigned int) buff [pos+1];
		printf ("Cabeza inicial = %d\n", valor);

		//byte 02
	    printf ("\nByte 02\n");
	    valor = (unsigned int) buff [pos+2];
		valor = valor << 2;
		valor = valor >> 2;
		cout<<valor<<endl;

	    //byte 03
	    printf ("\nByte 03\n");
	    WORD valor2;
	    valor2 = (unsigned int) buff [pos +3];
	    valor = valor >> 6;
	    WORD valor3;
	    valor3 = (unsigned int) valor;
	    valor3 = valor3 << 8;
	    unsigned int valor4;
	    valor4 = (unsigned int) valor2;
	    unsigned int valor5;
	    valor5 = valor3 | valor4;
	    cout <<valor5 <<endl;
	    cout <<".....Cilindro de partida....." <<endl;
	    valor2 = 0;
	    valor3 = 0;
	    valor4 = 0;
	    valor5 = 0;

	    //byte 04
	    printf ("\nByte 04\n");
	    valor = (unsigned int) buff [pos+4];
	    printf ("Denominacion del sistema operativo\n");
	    cout <<valor <<endl;

	    //byte 05
	    printf ("\nByte 05\n");
	    valor = (unsigned int) buff [pos+5];
	    cout <<valor <<endl;

	    //byte 06
	    printf ("\nByte 06\n");
	    valor = (unsigned int) buff [pos+6];
		valor = valor << 2;
		valor = valor >> 2;
		cout<<valor<<endl;

	    //byte 07
	    printf ("\nByte 07\n");
	    //WORD valor2;
	    valor2 = (unsigned int) buff [pos +7];
	    valor = valor >> 6;
	    //WORD valor3;
	    valor3 = (unsigned int) valor;
	    valor3 = valor3 << 8;
	    //unsigned int valor4;
	    valor4 = (unsigned int) valor2;
	    //unsigned int valor5;
	    valor5 = valor3 | valor4;
	    cout <<valor5 <<endl;
	    cout <<".....Cilindro de partida....." <<endl;

	    //byte 08
	    printf ("\nByte 08\n");
	    unsigned long int v;
		valor = (unsigned int) buff [pos + 8];
		v = valor;
		cout <<v <<endl;

	    //byte 09
	    printf ("\nByte 09\n");
	    valor = (unsigned int) buff [pos + 9];
	    cout <<valor <<endl;

	    //byte 10
	    printf ("\nByte 10\n");
	    valor = (unsigned int) buff [pos + 10];
	    cout <<valor <<endl;

	    //byte 11
	    printf ("\nByte 11\n");
	    valor = (unsigned int) buff [pos + 11];
	    cout <<valor <<endl;

	    //byte 12
	    printf ("\nByte 12\n");
	    unsigned long int v2;
		valor = (unsigned int) buff [pos + 12];
		v2 = valor;
		cout <<v2 <<endl;

		//byte 13
	    printf ("\nByte 13\n");
	    valor = (unsigned int) buff [pos + 13];
	    cout <<valor <<endl;

	    //byte 14
	    printf ("\nByte 14\n");
	    valor = (unsigned int) buff [pos + 14];
	    cout <<valor <<endl;

	    //byte 15
	    printf ("\nByte 15\n");
	    valor = (unsigned int) buff [pos + 15];
	    cout <<valor <<endl;

	    //byte 16
	    printf ("\nByte 16\n");
	    valor = (unsigned int) buff [pos + 16];
	    cout <<valor <<endl;


	    cout <<"--------------------------------------------------" <<endl;
	}
	return 0;
}
