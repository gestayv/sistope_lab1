#include "procesos.h"

int *hijosG;
int contadorSignal16;
int *pid_inicial;

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

	int *arreglo;

	hijosG = malloc(sizeof(int));
	pid_inicial = malloc(sizeof(int));
	//contadorSignal16 = malloc(sizeof(int));

	*hijosG = hijos;	
	*pid_inicial = pid_padre;
	contadorSignal16 = 0;
	
	arreglo = malloc(sizeof(int)*hijos);

	for (i = 0; i < hijos; ++i)
	{
		arreglo[i] = 0;
	}	

	while(contador < hijos)
	{
		if(getpid() == pid_padre)
		{
			pid_hijo = fork();
			arreglo[contador] = (int)pid_hijo;
		}
		contador++;
	}

	if(muestraHijos == 1 && getpid() == pid_padre)
	{
		for(i = 0; i < hijos; i++)
		{
			printf("Numero: %d, pid: %d \n", i + 1, arreglo[i]);
		}	
	}

	if(getpid() != pid_padre)
	{
		while(1);
	}

	envioSignal(arreglo, hijos);

	return 0;
}

void envioSignal(int *arreglo, int numHijos)
{
	int hijo = 0, senal = 0, pid_hijo = 0;
	while(1)
	{
		printf("Ingresar numero de hijo y senal a enviar (X - Y):\n");
		scanf("%d - %d", &hijo, &senal);
		if(hijo > numHijos)
		{
			printf("El hijo %d no existe. \n", hijo);
		}
		else
		{
			pid_hijo = arreglo[hijo-1];
			printf("La senal %d sera enviada al hijo %d de pid %d \n", senal, hijo, pid_hijo);
			kill(pid_hijo, senal);
				
		}
	}
}

void userSignal(int value)
{
	pid_t pid_proceso = getpid();
	int i = 0;
	contadorSignal16++;
	printf("<PID: %d y he recibido esta llamada %d veces >\n", (int)pid_proceso, contadorSignal16);
}

void userSignal2(int value)
{
	pid_t pid_proceso = getpid();
	pid_t pid_hijo = fork();
}

void signalOverride(int value)
{
	pid_t pid_proceso = getpid();
	if(pid_proceso != *pid_inicial)
	{
		printf("<Soy el hijo con PID: %d y estoy vivo aun>\n", pid_proceso); 
	}
	signal(2, SIG_DFL);
}