#include "procesos.h"

int main(int argc, char *argv[])
{
	//	flag para el argumento "-m"
	int mflag = 0;
	//	flag para el argumento "-h"
	int hflag = 0;
	//	variable para guardar el número de hijos a ser creado
	int hvalue = 0;	

	int c;

	//	Se asigna el valor 0 a opterr, de manera que los errores mostrados por pantalla sean los programados
	//	y no los que la función "getopt" muestra.
	opterr = 0;

	//	Se realiza un loop while para recorrer los argumentos entregados por terminal.
	while((c = getopt(argc, argv, "h:m")) != -1)
	{
		switch(c)
		{
			//	En caso de ser h, se toma el número de hijos y se guarda en la variable "hvalue" por medio de sscanf,
			//	además se cambia el valor de la variable "hflag" para indicar que el argumento fue ingresado.
			case 'h':
				//	Si los argumentos pasados por terminal son "-h -m", se indica que se debe entregar el 
				//	número de hijos.
				if(strcmp(optarg, "-m") == 0)
				{
					fprintf(stderr, "Se debe indicar el número de hijos junto al parametro -h.\n");
					return 1;
				}
				sscanf(optarg, "%d", &hvalue);
				hflag = 1;
				break;
			//	En caso de ser m, se cambia el valor de la variable "mflag" para indicar que el argumento se ingresó en la terminal.
			case 'm':
				mflag = 1;
				break;
			//	Si getopt detecta un error, devuelve el valor '?'. Por lo que es necesario agregar el caso.
			case '?':
				//	Si el error encontrado está en el argumento "-h", se indica que se debe ingresar el número de hijos.
				if(optopt == 'h')
				{
					fprintf(stderr, "Se debe indicar el número de hijos junto al parametro -%c.\n", optopt);
				}
				//	Si el argumento en el que se encontró el error se puede imprimir,
				//	se indica que la opción no existe.
				else if (isprint(optopt))
				{
					fprintf(stderr, "Opcion desconocida -%c.\n", optopt);
				}
				//	Si el argumento en el que se encuentra el error no se puede imprimir, 
				//	se indica que la opción tiene un caracter desconocido.
				else
				{
					fprintf(stderr, "Opcion con caracter desconocido `\\x%x'.\n", optopt);
				}
				return 1;
			default:
				abort();
		}
	}

	//	Se verifica si se ingreso el parámetro "-h". Si no existe, termina la ejecución.
	if(hflag == 0)
	{
		fprintf(stderr, "No se ingreso el parametro obligatorio '-h'. Terminando la ejecucion.\n");
		return 1;
	}
	//	Se verifica si el número de hijos es positivo. De no ser así, se termina la ejecución.
	if(hvalue < 0)
	{
		fprintf(stderr, "Se ha ingresado un número negativo de hijos. Terminando la ejecucion.\n");
		return 1;	
	}

	printf("mflag = %d, hflag = %d, hvalue = %d\n",
          mflag, hflag, hvalue);
	
	procesosHijos(hvalue);



	return 0;	
}