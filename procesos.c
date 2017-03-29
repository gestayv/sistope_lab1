#include "procesos.h"

void procesosHijos(int hijos)
{
	pid_t pid_padre = getpid();

	int contador = 0;

	printf("PID padre: %d\n", (int)pid_padre);

	while(contador != hijos)
	{
		if(getpid() == pid_padre)
		{
			fork();
		}
		contador++;
	}

	sleep(10000);
}