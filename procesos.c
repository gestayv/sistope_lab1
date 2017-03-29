#include "procesos.h"

//	Función procesosHijos
//	Se encarga de crear un numero "n" de hijos. Donde "n" es ingresado por la terminal.
//	Parámetros:
//		- hijos: Entero, indica el número de hijos que deben ser creados.
//		- muestraHijos:	Entero, 0 o 1, indica si se debe mostrar el número de cada hijo
//						y su PID por terminal o no.
void procesosHijos(int hijos, int muestraHijos)
{
	pid_t pid_padre = getpid();
	pid_t pid_hijo = 0;

	int contador = 0;

	printf("PID padre: %d\n", (int)pid_padre);

	while(contador != hijos)
	{
		if(getpid() == pid_padre)
		{
			pid_hijo = fork();
			// Aquí agregar en una estructura de datos.
			/*---*/
		}
		contador++;
	}

	// Sleep para revisar htop y ver los procesos creados
	sleep(10000);
}