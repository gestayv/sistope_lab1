/*procesos.h: Header que contiene las cabeceras de las funciones y las bibliotecas a utilizar.*/

#ifndef PROCESOS_H
#define PROCESOS_H


#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int procesosHijos(int hijos, int muestraHijos);
void envioSignal(int *arreglo, int numHijos);
void userSignal(int signal);
void userSignal2(int signal);
void signalOverride(int signal);

#endif