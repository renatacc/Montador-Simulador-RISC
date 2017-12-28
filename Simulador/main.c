/* -------------------------------
 * File:   main.c
 * Autora: Renata Caroline Cunha
 * Matrícula : 0022429
 * Data: 11/12/2016
---------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include "simulador.h"

int main(int argc, char ** argv)
{
	  if(argc == 2)
	  {
	    simulador(argv[1]);
	    return 0;
	  }
	  else
	  {
	      printf("Erro!");
	  }

  	return(-1);
}
