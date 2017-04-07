/*procesos.c: Contiene el segmento de código relacionado a la creación de hijos y 
			  el llamado de señales. */

/*Inclusión de procesos.h, que contiene "los include" de bibliotecas (como <stdio.h> o <signal.h> y cabeceras de las funciones utilizadas
 en este archivo*/
#include "procesos.h"

//	Declaración de variables globales, estas variables se crean de manera que las señales
//	puedan usar datos que normalmente no podrían recibir.

int contadorSignal16;	//	Contador que indica cuantas veces se llamo la señal 16 para cada hijo.
int pid_inicial;		//	Entero que almacenará el pid del padre de todos los procesos
int numHijos;			//	Entero en el que se almacenará el número de hijos a ser creados.
int *catchSigint;		//	Arreglo en el que se almacenarán los pids de todos los hijos

//	Función procesosHijos
//	Se encarga de crear un número "n" de hijos. Donde "n" es ingresado por la terminal.
//	Parámetros:
//		- hijos: Entero, indica el número de hijos que deben ser creados.
//		- muestraHijos:	Entero, 0 o 1, indica si se debe mostrar el número de cada hijo
//						y su PID por terminal.
int procesosHijos(int hijos, int muestraHijos)
{
	//	Se crea una variable con el pid del proceso padre.
	pid_t pid_padre = getpid();

	//	Se crea un arreglo para guardar los pids de los procesos hijos.
	int *arreglo;
	
	//	Se crean un contador para recorrer los arreglos que contienen los pid de los hijos.
	int i = 0;

	//	Se setea el valor de las variables globales.
	pid_inicial = pid_padre;
	contadorSignal16 = 0;
	numHijos = hijos;
	
	//	Se reserva memoria para los arreglos que contendrán pid de hijos y se inicializan.
	arreglo = malloc(sizeof(int)*hijos);
	catchSigint = malloc(sizeof(int)*hijos);

	for (i = 0; i < hijos; ++i)
	{
		arreglo[i] = 0;
		catchSigint[i] = 0;
	}	

	//	Creacion de Hijos:
	//	El proceso padre crea tantos hijos como fueron indicados por terminal con el argumento -h
	for(i = 0; i < hijos; i++)
	{
		if(getpid() == pid_padre)
		{
			//	Se agrega a cada arreglo (global y local) el PID de cada hijo creado.
			arreglo[i] = (int)fork();
			catchSigint[i] = arreglo[i];
		}
	}

	//	Se dejan todos los procesos hijos esperando una señal.
	if(getpid() != pid_padre)
	{
		while(1)
		{
			pause();
		};
	}

	//	Si se recibio el comando "-m", se muestran los hijos con su respectivo PID.
	if(muestraHijos == 1)
	{
		for(i = 0; i < hijos; i++)
		{
			printf("Numero: %d, pid: %d \n", i + 1, arreglo[i]);
		}	
	}

	//	Se llama al procedimiento envioSignal.
	envioSignal(arreglo, hijos);

	return 0;
}

/* Función envioSignal: Procedimiento que pregunta al usuario la señal y el proceso hijo que recibira tal señal.
						Utiliza la funcion 
	Entrada: 
			- arreglo: 	Arreglo de enteros, contiene los PID de los procesos Hijos
			- numHijos: Entero, número de hijos generados.
	Salida: 
	*/
void envioSignal(int *arreglo, int numHijos)
{
	//	Se crean variables locales que contendrán:
	//	El hijo que recibe la señal, su pid y el número de la señal enviada respectivamente. 
	int hijo = 0, pid_hijo = 0, senal = 0;

	//	Se crea una variable para guardar el status devuelto por la función waitpid.
	int status = 0;

	//	Se entra en un loop para pedir que se envíen señales.
	while(1)
	{
		printf("Ingresar numero de hijo y senal a enviar (X - Y):\n");
		scanf("%d - %d", &hijo, &senal);
		//	Si se ingresa un hijo inexistente, se muestra por la terminal.
		if(hijo > numHijos)
		{
			printf("El hijo %d no existe. \n", hijo);
		}
		//	En caso de ser un valor correcto
		else
		{	//	Se encuentra el hijo correspondiente al dato ingresado por el usuario
			pid_hijo = arreglo[hijo-1];
			//	Se avisa al usuario la señal que se envía y el proceso que la recibe.
			printf("La senal %d sera enviada al hijo %d de pid %d \n", senal, hijo, pid_hijo);
			//	Se envia la señal, usando la funcion kill.
			switch(senal)
			{
				//	De ser la señal 15 se agrega una espera luego de enviar la señal
				//	de esta manera se identifica un cambio de estado en el proceso hijo
				//	evitando que quede como un proceso "zombie".
				case 15:
					kill(pid_hijo, SIGTERM);
					waitpid(pid_hijo, &status, 0);
					//	Se revisa el estado, si el hijo terminó por el envío de la señal...
					if(WIFSIGNALED(status))
					{
						//	... se modifica el arreglo global, de manera que se sepa que el proceso finalizó.
						catchSigint[hijo-1] = 0;	
					}
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

/* 
	Función userSignal: 
	Handler de la señal 16 (SIGUSR1)
	Aumenta en uno el contadorSignal16 (constante de cada proceso), de manera que se pueda
	mostrar por terminal la cantidad de veces que ha recibido la señal.
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

/* 
	Función userSignal2: 
	Handler de la señal 17 (SIGUSR2)
	Hace que un proceso determinado cree un hijo.
*/
void userSignal2(int value)
{
	//Se realiza un fork para crear un hijo nuevo
	pid_t pid_hijo = fork();
}

/* 
	Funcion signalOverride: 
	Handler de la señal 2 (SIGINT) o cuando se presiona Ctrl+C. 
	En primera instancia, procede a que todos los procesos hijos muestren un mensaje de estado
	Ademas, vuelve a asignar la funcion original a la señal 2 (matar todos los procesos) para las siguientes
	llamadas. 
*/
void signalOverride(int value)
{
	pid_t pid_proceso = getpid();
	int i = 0;
	//Si el proceso que recibe la señal es el padre...
	if(pid_proceso == pid_inicial)
	{
		//... entonces se recorre el arreglo con los pids de cada hijo...
		for (int i = 0; i < numHijos; ++i)
		{
			// ... y se imprime un mensaje por cada uno de ellos que aún esté vivo.
			if(catchSigint[i] != 0)
			printf("<Soy el hijo con PID: %d y estoy vivo aun>\n", catchSigint[i]);
		}	
	}
	//Setea la funcion default de SIGINT. Matar a todos los procesos.
	signal(SIGINT, SIG_DFL);
}