/*procesos.c: Contiene todas las relacionadas con la creacion de hijos y el llamado de señales a estas. */

/*Inclusion de procesos.h, que contiene "los include" de bibliotecas (como <stdio.h> o <signal.h> y cabeceras de las funciones utilizadas
 en este archivo*/
#include "procesos.h"

//Declaracion de variables globales.
int contadorSignal16;	//Contador que indica cuantas veces se llamo la señal 16 para cada hijo.
int *pid_inicial;		//Entero que indica cual es el pid del padre de todos los procesos

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

	pid_inicial = malloc(sizeof(int));
	
	*pid_inicial = pid_padre;
	contadorSignal16 = 0;
	
	//Arreglo que contendra el PID de todos los hijos creados.
	arreglo = malloc(sizeof(int)*hijos);

	for (i = 0; i < hijos; ++i)
	{
		arreglo[i] = 0;
	}	

	//Creacion de Hijos, el proceso padre crea tantos hijos como fueron indicados por terminal con el comando -h
	for(contador = 0; contador < hijos; contador++)
	{
		if(getpid() == pid_padre)
		{
			//Se agrega a arreglo el PID de cada hijo creado.
			arreglo[contador] = (int)fork();
		}
	}

	//Se dejan todos los procesos hijos esperando
	if(getpid() != pid_padre)
	{
		while(1)
		{
			pause();
		};
	}

	//Si se recibio el comando "-m", se muestra el menu que tiene el PID de cada proceso hijo.
	if(muestraHijos == 1)
	{
		for(i = 0; i < hijos; i++)
		{
			printf("Numero: %d, pid: %d \n", i + 1, arreglo[i]);
		}	
	}

	//Se llama al proceso envioSignal.
	envioSignal(arreglo, hijos);

	return 0;
}

/* Funcion envioSignal: Procedimiento que pregunta al usuario la señal y el proceso hijo que recibira tal señal.
						Utiliza la funcion 
	Entrada: El arreglo que contiene los PID de los procesos Hijos
			 El numero de hijos generados.
	Salida: 
	*/
void envioSignal(int *arreglo, int numHijos)
{
	int hijo = 0, senal = 0, pid_hijo = 0;
	int status;
	while(1)
	{
		//Se pregunta al usuario los datos requeridos.
		printf("Ingresar numero de hijo y senal a enviar (X - Y):\n");
		scanf("%d - %d", &hijo, &senal);
		if(hijo > numHijos)
		{
			printf("El hijo %d no existe. \n", hijo);
		}
		//En caso	 de ser un valor correcto
		else
		{	//Se encuentra el hijo correspondiente al dato ingresado por el usuario
			pid_hijo = arreglo[hijo-1];
			//Se avisa al usuario
			printf("La senal %d sera enviada al hijo %d de pid %d \n", senal, hijo, pid_hijo);
			//Se envia la señal, usando la funcion kill.
			switch(senal)
			{
				case 15:
					kill(pid_hijo, SIGTERM);
					waitpid(pid_hijo, &status, 0);
					break;
				case 16:
					kill(pid_hijo, SIGUSR1);
					break;
				case 17:
					kill(pid_hijo, SIGUSR2);
					break;
			}
		}
	}
}

/* Funcion userSignal: Funcion que reemplaza a la señal 16 (SIGUSR1)
	Aumenta en uno el contadorSignal16 (constante de cada proceso).
	Tambien lo muestra por pantalla la cantidad de veces que se han llamado a esta señal.
*/
void userSignal(int value)
{
	pid_t pid_proceso = getpid();
	int i = 0;
	//Aumenta en 1 el contadorSignal16
	contadorSignal16++;
	//Muestra el mensaje por pantalla
	printf("<PID: %d y he recibido esta llamada %d veces >\n", (int)pid_proceso, contadorSignal16);
}

/* Funcion userSignal2: Funcion que reemplaza a la señal 17 (SIGUSR2)
	Hace que un proceso determinado cree un hijo.*/
void userSignal2(int value)
{
	//Se realiza fork para crear un hijo nuevo
	pid_t pid_hijo = fork();
}

/* Funcion signalOverride: Funcion que reemplaza a la señal 2 (SIGINT) o cuando se presiona Ctrl+C 
	En primera instancia, procede a que todos los procesos hijos muestren un mensaje de estado
	Ademas, vuelve a asignar la funcion original a la señal 2 (matar todos los procesos) para las siguientes
	llamadas. */
void signalOverride(int value)
{
	pid_t pid_proceso = getpid();
	//Si es un proceso hijo...
	if(pid_proceso != *pid_inicial)
	{
		//... entonces imprime un mensaje con su estado.
		printf("<Soy el hijo con PID: %d y estoy vivo aun>\n", pid_proceso); 
	}
	//Setea la funcion default de SIGINT. Matar a todos los procesos.
	signal(SIGINT, SIG_DFL);
}