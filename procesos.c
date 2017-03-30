#include "procesos.h"

//	Función procesosHijos
//	Se encarga de crear un numero "n" de hijos. Donde "n" es ingresado por la terminal.
//	Parámetros:
//		- hijos: Entero, indica el número de hijos que deben ser creados.
//		- muestraHijos:	Entero, 0 o 1, indica si se debe mostrar el número de cada hijo
//						y su PID por terminal o no.
int procesosHijos(int hijos, int muestraHijos)
{
	pid_t pid_padre = getpid();
	pid_t pid_hijo = 0;

	int contador = 0;

	int i = 0, j = 0;
	int **arreglo;

	arreglo = malloc(sizeof(int*)*hijos);
	for(i = 0; i < hijos; i++)
	{
		arreglo[i] = malloc(sizeof(int)*2);
	}

	for (i = 0; i < hijos; ++i)
	{
		for (j = 0; j < 2; ++j)
		{
			arreglo[i][j] = 0;
		}
	}

	printf("PID padre: %d\n", (int)pid_padre);

	while(contador < hijos)
	{
		if(getpid() == pid_padre)
		{
			pid_hijo = fork();
			arreglo[contador][0] = (int)pid_hijo;
		}
		contador++;
	}


	if(muestraHijos == 1 && getpid() == pid_padre)
	{
		for(i = 0; i < hijos; i++)
		{
			printf("Numero: %d, pid: %d \n", i + 1, arreglo[i][0]);
		}	
	}

	if(getpid() != pid_padre)
	{
		while(1);
	}


	envioSignal(arreglo, hijos);

	return 0;
}

void envioSignal(int **arregloHijos, int numHijos)
{
	int X = 0, Y = 0;
	while(1)
	{
		printf("Ingresar numero de hijo y senal a enviar (X - Y):\n");
		scanf("%d - %d", &X, &Y);
		printf("%d - %d \n", X, Y);
	}
}