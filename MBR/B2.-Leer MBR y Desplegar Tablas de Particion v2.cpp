#include <windows.h>
#include <winioctl.h> // Desarrollo y control de controladores DDK
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#define BufferLength 1024

// Convierte el valor almacenado en cuatro bytes consecutivos a tipo int
uint32_t transtoint(unsigned char a[])
{
	uint32_t sum = 0UL;
	uint32_t temp = 0UL;

	for (int i = 3; i >= 0; i--)
	{
		temp = a[i]; // Asignar primero
		temp = temp << (8 * (3 - i));
		sum = sum | temp;
	}
	return sum;
}
// salida hexadecimal
bool HexOutput(char *buf, size_t len, bool ismbr, ULONGLONG *baseaddr, ULONGLONG *nextaddr, int EBRnum)
{
	bool mbrflag = 1; // Al leer el MBR, determine si la entrada es una partición primaria o una entrada de partición extendida
	unsigned char a = buf[0];
	if (ismbr)
		printf("-----------------------------------------------------");
	else
		printf("--------------------------% D EBR -------------------", EBRnum);
	// printf ("El primer byte es:% x \ n \ n", a);
	if (ismbr)
		printf("Primera parte (MBR): \n \n");
	else
		printf("Primera parte (EBR): \n \n");
	int flag = 0;
	for (size_t i = 0; i < len; ++i)
	{
		unsigned char c = buf[i]; // se debe usar unsigned char para imprimir valor>128
		flag++;
		if (c < 16)
			printf("0%x ", c);
		else
			printf("%x ", c);
		if (i == 445)
		{
			flag = 0;
			printf("\n \n Parte II (Tabla de particiones): \n");
		}
		if (i == 509)
		{
			flag = 0;
			printf("\n \n La tercera parte (marca final): \n");
		}
		if ((flag) % 16 == 0)
			printf("\n");
	}
	printf("\n \n<------------------- Analisis de informacion de la tabla de particiones -------------------> \n");
	printf("\n \nLa direccion y el tamano de la particion son: \n \n");
	int limit = ismbr ? 509 : 477; // Si ya sabe que es una EBR de partición extendida, no necesita leer tanto, rank = 1 es la tabla de particiones, y rank = 2 es la siguiente.
	for (int m = 445, rank = 1; m < limit && rank <= 4; m += 16, rank++)
	{
		unsigned char fifth = buf[m + 5]; // Obtenga el quinto bit del indicador del sistema de archivos, 05H o 0FH hexadecimal es la partición extendida
		if (fifth == 0x5 || fifth == 0xf) // Es una entrada de partición extendida, no es necesario leerla más tarde
		{
			printf("Esta es una particion extendida!\n");
			mbrflag = 0;
			rank = 4;
		}				// No mbr es una partición extendida EBR
		if (fifth < 16) // Ajusta el formato de salida
			printf("El indicador de tabla de particion% d es: 0% x \n", rank, fifth);
		else
			printf("El indicador de la tabla de particion% d es: % x \n", rank, fifth);
		if (fifth == 0x00) // Cuando el quinto bit (bandera) es 00, significa que la información de la tabla de particiones está vacía y no hay partición
		{
			printf("La tabla de particiones esta vacia \n \n"); // Ya no es necesario leerlo
		}
		else
		{ // Elemento de partición
			unsigned char offsetadd[4] = {0};
			// Leer al revés, el byte alto está en la dirección alta
			for (int n = m + 12, t = 0; n > m + 8, t < 4; n--, t++)
			{
				unsigned char temp = buf[n];
				if (temp < 16)
					printf("  0%x  ", temp);
				else
					printf("  %x  ", temp);
				offsetadd[t] = buf[n];
			}
			// Calcular dirección y convertir a número de sector decimal LBA
			printf("\n");
			uint32_t tempadd = transtoint(offsetadd);
			printf("\nLa direccion de inicio es:% I64x", (ULONGLONG)tempadd * (ULONGLONG)512 + *baseaddr);

			if (ismbr && !mbrflag) // si esta en la MBR y tiene entrada extendida, el EBR esta en relsecor+nowbase(0)
			{
				*baseaddr = (ULONGLONG)tempadd * (ULONGLONG)512 + *baseaddr; // only change once
				*nextaddr = (ULONGLONG)0UL;
			}
			else if (!mbrflag) // si es la entrada extendida
			{
				*nextaddr = (ULONGLONG)tempadd * (ULONGLONG)512;
			}
			printf("\n\n");
			printf("Talla:");
			for (int p = m + 16, w = 0; p > m + 12, w < 4; p--, w++)
			{
				unsigned char temp1 = buf[p];
				if (temp1 < 16)
					printf("  0%x  ", temp1);
				else
					printf("  %x  ", temp1);
				offsetadd[w] = buf[p];
			}
			// Calcular tamaño y convertir a GB
			printf("\n");
			uint32_t tempsize = transtoint(offsetadd);

			if (ismbr && !mbrflag)
				printf("\nEl tamano total del disco de expansion es:% lu sector =% lf GB \n", tempsize, ((double)tempsize / 2 / 1024.0 / 1024.0));
			else if (!ismbr)
			{
				printf("\nEl tamano del disco logico de% d es:% lu sector =% lf GB \n", EBRnum, tempsize, ((double)tempsize / 2 / 1024.0 / 1024.0));
			}
			else
				printf("\nEl tamano del disco es:% lu sector =% lf GB \n", tempsize, ((double)tempsize / 2 / 1024.0 / 1024.0));
		}
	}
	printf("\n\n");
	return (mbrflag);
}

// Función: Analiza la tabla de partición principal para obtener la dirección de desplazamiento y el tamaño de partición de cada partición
BOOL GetDriveGeometry(DISK_GEOMETRY *pdg, int addr)
{
	HANDLE hDevice; // manejador del dispositivo
	BOOL bResult;	// bandera resultados
	DWORD junk;		// desechar resultados cc
	char lpBuffer[BufferLength] = {0};

	// Obtenga el identificador del dispositivo a través del API de Windows CreateFile
	hDevice = CreateFile(TEXT("\\\\.\\PhysicalDrive0"),		 // Nombre del dispositivo, aquí se refiere al primer disco duro
						 GENERIC_READ,						 // sin acceso al disco
						 FILE_SHARE_READ | FILE_SHARE_WRITE, // modo compartir
						 NULL,								 // atributos de seguridad predeterminados
						 OPEN_EXISTING,						 // disposicion
						 0,									 // atributos del archivo
						 NULL);								 // no copiar atributos del archivo
	if (hDevice == INVALID_HANDLE_VALUE)					 // no se puede abrir el disco
	{
		printf("Createfile error! Revisar permisos! ERROR_ACCESS_DENIED！\n");
		return (FALSE);
	}

	// Use la función DeviceIoControl para realizar IO con el dispositivo
	bResult = DeviceIoControl(hDevice,						 // Manejo del dispositivo
							  IOCTL_DISK_GET_DRIVE_GEOMETRY, // Código de control, que indica el tipo de dispositivo
							  NULL,
							  0, // sin buffer de entrada
							  pdg,
							  sizeof(*pdg),
							  &junk,			   // # bytes regresados
							  (LPOVERLAPPED)NULL); // I/O sincrono

	LARGE_INTEGER offset;												   // long long signed
	offset.QuadPart = (ULONGLONG)addr * (ULONGLONG)512;					   // 0
	SetFilePointer(hDevice, offset.LowPart, &offset.HighPart, FILE_BEGIN); // Leer MBR de 0
	if (GetLastError())
		printf("Codigo de tipo de error:% ld \n \n", GetLastError()); // Si algo va mal

	DWORD dwCB;
	// Comienza a leer desde esta posición
	BOOL bRet = ReadFile(hDevice, lpBuffer, 512, &dwCB, NULL);
	// Si no es MBR,
	bool finished = 0;
	int EBRnum = 0;
	ULONGLONG *baseaddr = new ULONGLONG, *nextaddr = new ULONGLONG; // Dirección de inicio de partición extendida, dirección EBR
	*baseaddr = (ULONGLONG)0;
	*nextaddr = (ULONGLONG)0;
	finished = HexOutput(lpBuffer, 512, true, baseaddr, nextaddr, EBRnum); // Primero lee el MBR

	if (finished)
		CloseHandle(hDevice);
	else
	{
		// Sigue leyendo
		do
		{
			EBRnum++;
			memset(lpBuffer, 0, sizeof(lpBuffer));
			offset.QuadPart = (ULONGLONG)(*baseaddr + *nextaddr);				   // Encontrar EBR
			SetFilePointer(hDevice, offset.LowPart, &offset.HighPart, FILE_BEGIN); // Leer EBR
			if (GetLastError())
				printf("Codigo de tipo de error:% ld \n \n", GetLastError()); // Si algo va mal

			ReadFile(hDevice, lpBuffer, 512, &dwCB, NULL);

		} while (!HexOutput(lpBuffer, 512, false, baseaddr, nextaddr, EBRnum));
		CloseHandle(hDevice);
	}

	delete baseaddr;
	delete nextaddr;
	return bResult;
}
extern int add[20];
extern int disknum;
int main()
{
	DISK_GEOMETRY pdg;	// Estructura para guardar los parámetros del disco
	BOOL bResult;		// bandera de resultados genericos
	ULONGLONG DiskSize; // tamaño del disco, en bytes
	printf("<----------------- Bienvenido a usar el lector de particiones -----------------> \n \n");
	bResult = GetDriveGeometry(&pdg, 0);

	if (bResult)
	{
		DiskSize = pdg.Cylinders.QuadPart * (ULONG)pdg.TracksPerCylinder *
				   (ULONG)pdg.SectorsPerTrack * (ULONG)pdg.BytesPerSector;

		printf("Tamano total del disco =% I64d (Bytes) =% I64d (Gb) \n", DiskSize, DiskSize / (1024 * 1024 * 1024));
	}
	else
	{
		printf("GetDriveGeometry fallo. Error %ld.\n", GetLastError());
	}
	return ((int)bResult);
}
