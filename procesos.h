#ifndef PROCESOS_H
#define PROCESOS_H

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int procesosHijos(int hijos, int muestraHijos);
void envioSignal(int **arregloHijos, int numHijos);
void userSignal(int signal);
void userSignal2(int signal);
void signalOverride(int signal);

#endif