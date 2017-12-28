/* -------------------------------
 * File:   simulador.c
 * Autora: Renata Caroline Cunha
 * Matrícula : 0022429
 * Data: 11/12/2016
--------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "simulador.h"

/* --------------- Estruturas --------------- */
struct campo1
{
	unsigned int rc:8;
	unsigned int rb:8;
	unsigned int ra:8;
	unsigned int opcode:8;
};

struct campo2
{
	unsigned int rc:8;
	unsigned int end:16; /* Endereço de memoria */
	unsigned int opcode:8;
};

struct campo3
{
	unsigned int end:24; /* Endereço de memoria */
	unsigned int opcode:8;
};
/* ------------------------------------------- */

typedef struct campo1 *Tipo1;
typedef struct campo2 *Tipo2;
typedef struct campo3 *Tipo3;

/* --------- Declaração das funções --------- */
void instructionDecode(int instrucao);
void leInstrucao(char *arqEntrada);
long int instructionFetch(void);
void executaInstrucoes(void);
int flagNeg(int rc);
int flagZero(int rc);
int flagOverflowAdd(int ra, int rb, int rc);
int flagOverflowSub(int ra, int rb, int rc);
int flagOverflowPot(int ra, int rb, int rc);
int flagOverflowMultDiv(int ra, int rb, int rc);
int flagCarry(int ra, int rb, int rc);
int verificaAddress(char *instrucao);
void mostraFlags();
/* ------------------------------------------ */

/* ----- Declaração das variáveis globais ----*/
int memoria[65536];
int registradores[32];
int IR, Address = 0;
int PC=0, contMemoria = 0;
int neg, zero, carry,overflow;
/* ------------------------------------------ */

void simulador(char *arqEntrada)
{
	  leInstrucao(arqEntrada); /* Lê instruções e carrega na memória */
		executaInstrucoes(); /* Executa as instruções */
}

void executaInstrucoes(void) /* Executa as instruções */
{
		Tipo1 instrucaoTipo1;

		IR = instructionFetch(); /* Busca na memória a próxima instrução a ser executada e armazena no registrador de instruções */
		instrucaoTipo1= (Tipo1) &IR;

		/* Executa instruções até que seja encontrado um halt */
		while(instrucaoTipo1->opcode != 255)
		{
			 instructionDecode(IR); /* Decodifica a instrução e os operandos são buscados no banco de registradores */
			 IR = instructionFetch();
			 instrucaoTipo1= (Tipo1) &IR;
		}

		instructionDecode(IR);
}

int verificaAddress(char *instrucao) /* Verifica se a instrução tem um address */
{
   int i;
	 char address[20];

   for(i=0;isalpha(instrucao[i]);i++)
   {
      address[i] = instrucao[i];
   }

   address[i]= '\0';

	 if(strcmp(address,"address")==0)
	 {
		 return(1);
	 }

   return(0);
}

void leInstrucao(char *arqEntrada) /* Lê instruções e carrega na memória */
{
  	FILE *arq;
		char linha[1000];
		char endAddress[15];

		arq=fopen(arqEntrada,"r");

	  while(fgets(linha, sizeof(linha),arq)) /* Lê instruções */
	  {
				if(verificaAddress(linha)) /* Se tiver um address, inicia-se o carregamento a partir do endereço de memória */
				{
				  	sscanf(linha,"address %s", endAddress);
						printf("address %s\n", endAddress);
						Address = strtol(endAddress,NULL,2);
						contMemoria = Address;
						PC = Address;
				}
				else
				{
					 memoria[contMemoria]=strtol(linha,NULL,2); /* Armazena instrução na memória */
					 contMemoria++;
				}
	  }
}

long int instructionFetch(void) /* Busca na memória a próxima instrução a ser executada */
{
		int PCanterior;

		PCanterior = PC;
		PC++;

		return(memoria[PCanterior]);
}

int flagNeg(int rc) /* Verifica flag neg */
{
		if(registradores[rc] < 0) /* Retorna 1 se for negativo e 0 se não for */
		{
			 return(1);
		}

		return(0);
}

int flagZero(int rc) /* Verifica flag zero */
{
		if(registradores[rc] == 0) /* Retorna 1 se for igual a 0 e 0 se não for */
		{
			 return(1);
		}

		return(0);
}

int flagOverflowAdd(int ra, int rb, int rc) /* Verifica flag overflow instrução add */
{
		if((registradores[ra] > 0 && registradores[rb] > 0 && registradores[rc] < 0) ||
		(registradores[ra] < 0 && registradores[rb] < 0 && registradores[rc] > 0))
		{
			 return(1);
		}

		return(0);
}

int flagOverflowSub(int ra, int rb, int rc) /* Verifica flag overflow instrução sub */
{
	if((registradores[ra] > 0 && registradores[rb] < 0 && registradores[rc] < 0) ||
	(registradores[ra] < 0 && registradores[rb] > 0 && registradores[rc] > 0))
	{
		 return(1);
	}

	return(0);
}

int flagOverflowMultDiv(int ra, int rb, int rc) /* Verifica flag overflow instrução mult e div */
{
		if((registradores[ra] > 0 && registradores[rb] > 0 && registradores[rc] < 0) ||
		(registradores[ra] < 0 && registradores[rb] < 0 && registradores[rc] < 0) ||
	  (registradores[ra] > 0 && registradores[rb] < 0 && registradores[rc] > 0) ||
	  (registradores[ra] < 0 && registradores[rb] > 0 && registradores[rc] > 0))
		{
			 return(1);
		}

		return(0);
}

int flagCarry(int ra, int rb, int rc) /* Verifica flag carry */
{
		if( (registradores[ra] >= 0 && registradores[rb] < 0 && registradores[rc] >= 0) ||
	  (registradores[ra] < 0 && registradores[rb] >= 0 && registradores[rc] >= 0) ||
	  (registradores[ra] < 0 && registradores[rb] < 0) )
		{
			 return(1);
		}

		return(0);
}

int flagOverflowPot(int ra, int rb, int rc) /* Verifica flag overflow instrução pot */
{
	if((registradores[rb] < 0) || (registradores[ra] > 0 && registradores[rc] <= 0 ) ||
  (registradores[ra] < 0 && (registradores[rb] % 2) == 0 && registradores[rb] > 0 && registradores[rc] < 0 ) ||
  (registradores[ra] < 0 && (registradores[rb] % 2) != 0 && registradores[rb] > 0 && registradores[rc] > 0 ))
	{
		 return(1);
	}

	return(0);
}

void mostraFlags() /* Mostra flags */
{
	  printf("neg = %d  zero = %d  overflow = %d  carry = %d\n\n", neg, zero,overflow,carry);

		return;
}

void instructionDecode(int instrucao) /* Decodifica a instrução e os operandos são buscados no banco de registradores */
{
		int rc, rb, ra, numero;
		Tipo1 instrucaoTipo1;
	  Tipo2 instrucaoTipo2;
		Tipo3 instrucaoTipo3;

		instrucaoTipo1= (Tipo1) &instrucao;
		instrucaoTipo2= (Tipo2) &instrucao;
		instrucaoTipo3= (Tipo3) &instrucao;

		/* Decodifica instrução e escreve resultados no registrador */
		switch(instrucaoTipo1->opcode)
		{
					 case 0: //NOP
							printf("nop\n");
					 break;
					 case 1: //ADD
							ra = instrucaoTipo1->ra;
							rb = instrucaoTipo1->rb;
							rc = instrucaoTipo1->rc;

							registradores[rc] = registradores[ra] + registradores[rb];

							/* Flags */
							neg = flagNeg(rc);
							zero = flagZero(rc);
							overflow = flagOverflowAdd(ra,rb,rc);
							carry = flagCarry(ra,rb,rc);

							printf("add r%d = %d\n", rc, registradores[rc]);
							mostraFlags();
					 break;
					 case 2: //SUB
							ra = instrucaoTipo1->ra;
							rb = instrucaoTipo1->rb;
							rc = instrucaoTipo1->rc;

							registradores[rc] = registradores[ra] - registradores[rb];

							/* Flags */
							neg = flagNeg(rc);
							zero = flagZero(rc);
							overflow = flagOverflowSub(ra,rb,rc);
							carry = flagCarry(ra,rb,rc);

							printf("sub r%d = %d\n", rc, registradores[rc]);
							mostraFlags();
					 break;
					 case 3: //ZERA
							rc = instrucaoTipo1->rc;
							registradores[rc] = 0;

							neg = 0;
							zero = 1;
							carry = 0;
							overflow = 0;

							printf("zeros r%d\n",rc);
							mostraFlags();
					 break;
					 case 4: //XOR
							ra = instrucaoTipo1->ra;
							rb = instrucaoTipo1->rb;
							rc = instrucaoTipo1->rc;

							registradores[rc] = registradores[ra] ^ registradores[rb];

							/* Flags */
							neg = flagNeg(rc);
							zero = flagZero(rc);
							overflow = 0;
							carry = 0;

							printf("xor r%d = %d\n", rc, registradores[rc]);
							mostraFlags();
					 break;
					 case 5: //OR
							 ra = instrucaoTipo1->ra;
							 rb = instrucaoTipo1->rb;
							 rc = instrucaoTipo1->rc;

							 registradores[rc] = registradores[ra] | registradores[rb];

							 /* Flags */
							 neg = flagNeg(rc);
							 zero = flagZero(rc);
							 overflow = 0;
							 carry = 0;

							 printf("or r%d = %d\n", rc,registradores[rc]);
							 mostraFlags();
					 break;
					 case 6: //NOT
							 ra = instrucaoTipo1->ra;
							 rc = instrucaoTipo1->rc;

							 registradores[rc] = ~registradores[ra];

							 /* Flags */
							 neg = flagNeg(rc);
							 zero = flagZero(rc);
							 overflow = 0;
							 carry = 0;

							 printf("not r%d = %d\n", rc,registradores[rc]);
							 mostraFlags();
					 break;
					 case 7: //AND
							 ra = instrucaoTipo1->ra;
							 rb = instrucaoTipo1->rb;
							 rc = instrucaoTipo1->rc;

							 registradores[rc] = registradores[ra] & registradores[rb];

							 /* Flags */
							 neg = flagNeg(rc);
							 zero = flagZero(rc);
							 overflow = 0;
							 carry = 0;

							 printf("and r%d = %d\n", rc,registradores[rc]);
							 mostraFlags();
					 break;
					 case 8: //SHIFT ARIT ESQUERDA
							 ra = instrucaoTipo1->ra;
							 rb = instrucaoTipo1->rb;
							 rc = instrucaoTipo1->rc;

							 registradores[rc] = registradores[ra] << registradores[rb];

							 /* Flags */
							 neg = flagNeg(rc);
							 zero = flagZero(rc);
							 overflow = 0;
							 carry = 0;

							 printf("asl r%d = %d\n", rc,registradores[rc]);
							 mostraFlags();
					 break;
					 case 9: //SHIFT ARIT DIREITA
							 ra = instrucaoTipo1->ra;
							 rb = instrucaoTipo1->rb;
							 rc = instrucaoTipo1->rc;

							 registradores[rc] = registradores[ra] >> registradores[rb];

							 /* Flags */
							 neg = flagNeg(rc);
							 zero = flagZero(rc);
							 overflow = 0;
							 carry = 0;

							 printf("asr r%d = %d\n", rc,registradores[rc]);
							 mostraFlags();
					 break;
					 case 10: //SHIFT LOGICO ESQUERDA
							 ra = instrucaoTipo1->ra;
							 rb = instrucaoTipo1->rb;
							 rc = instrucaoTipo1->rc;

							 registradores[rc] = registradores[ra] << registradores[rb];

							 /* Flags */
							 neg = flagNeg(rc);
							 zero = flagZero(rc);
							 overflow = 0;
							 carry = 0;

							 printf("lsl r%d = %d\n", rc,registradores[rc]);
							 mostraFlags();
					 break;
					 case 11: //SHIFT LOGICO DIREITA
							 ra = instrucaoTipo1->ra;
							 rb = instrucaoTipo1->rb;
							 rc = instrucaoTipo1->rc;

							 registradores[rc] = registradores[ra] >> registradores[rb];

							 /* Flags */
							 neg = flagNeg(rc);
							 zero = flagZero(rc);
							 overflow = 0;
							 carry = 0;

							 printf("lsr r%d = %d\n", rc,registradores[rc]);
							 mostraFlags();
					 break;
					 case 12: //COPIA
							ra = instrucaoTipo1->ra;
							rc = instrucaoTipo1->rc;

							registradores[rc] = registradores[ra];

							/* Flags */
							neg = flagNeg(rc);
							zero = flagZero(rc);
							overflow = 0;
							carry = 0;

							printf("passa r%d = %d\n", rc,registradores[rc]);
							mostraFlags();
					 break;
					 case 14: //LOAD CONSTANTE MAIS SIGNIFICATIVOS
							 rc = instrucaoTipo2->rc;
							 numero = instrucaoTipo2->end; //Mudar para const16

							 registradores[rc] =(numero << 16) | (rc & 0x0000ffff);

							 /* Nenhuma Flag */

							 printf("lch r%d = %d\n", rc, registradores[rc]);
							 mostraFlags();
					 break;
					 case 15: //LOAD CONSTANTE MENOS SIGNIFICATIVOS
							rc = instrucaoTipo2->rc;
							numero = instrucaoTipo2->end;

							registradores[rc] = numero | (rc & 0xffff0000);

							/* Nenhuma Flag */

							printf("lcl r%d = %d\n", rc, registradores[rc]);
							mostraFlags();
					 break;
					 case 16: //LOAD
							ra = instrucaoTipo1->ra;
							rc = instrucaoTipo1->rc;

							registradores[rc] = memoria[registradores[ra]];

							/* Nenhuma Flag */

							printf("LOAD r%d = m%d que possui %d\n", rc, registradores[ra], memoria[registradores[ra]]);
							mostraFlags();
					 break;
					 case 17: //STORE
							ra = instrucaoTipo1->ra;
							rc = instrucaoTipo1->rc;

							memoria[registradores[rc]] = registradores[ra];

							/* Nenhuma Flag */

							printf("STORE m%d = r%d que possui %d\n",registradores[rc],ra,registradores[ra]);
							mostraFlags();
					 break;
					 case 18: //MULT
							 ra = instrucaoTipo1->ra;
							 rb = instrucaoTipo1->rb;
							 rc = instrucaoTipo1->rc;

							 registradores[rc] = registradores[ra] * registradores[rb];

						   /* Flags */
 							 neg = flagNeg(rc);
 							 zero = flagZero(rc);
 						   overflow = flagOverflowMultDiv(ra,rb,rc);
 							 carry = 0;

							 printf("mult r%d = %d\n", rc, registradores[rc]);
							 mostraFlags();
					 break;
					 case 19: //DIV
							 ra = instrucaoTipo1->ra;
							 rb = instrucaoTipo1->rb;
							 rc = instrucaoTipo1->rc;

							 registradores[rc] = registradores[ra] / registradores[rb];

							 /* Flags */
 							neg = flagNeg(rc);
 							zero = flagZero(rc);
 							overflow = flagOverflowMultDiv(ra,rb,rc);
 							carry = 0;

							 printf("div r%d = %d\n", rc, registradores[rc]);
							 mostraFlags();
					 break;
					 case 20: //POT
							 ra = instrucaoTipo1->ra; // base
							 rb = instrucaoTipo1->rb; // expoente
							 rc = instrucaoTipo1->rc;

							 registradores[rc] = pow(registradores[ra],registradores[rb]); //compilar -lm

								 /* Flags */
	 							neg = flagNeg(rc);
	 							zero = flagZero(rc);
	 							overflow = flagOverflowPot(ra,rb,rc);
	 							carry = 0;

								printf("pot r%d = %d\n", rc, registradores[rc]);
								mostraFlags();
					 break;
					 case 21: //MOD
							 ra = instrucaoTipo1->ra; // base
							 rb = instrucaoTipo1->rb; // expoente
							 rc = instrucaoTipo1->rc;

							 registradores[rc] = registradores[ra] % registradores[rb];

							 /* Nenhuma Flag */
							 neg = 0;
							 zero = flagZero(rc);
							 overflow = 0;
							 carry = 0;

							 printf("mod r%d = %d\n", rc, registradores[rc]);
							 mostraFlags();
					 break;
					 case 22: //ADDI
							ra = instrucaoTipo1->ra;
							rb = instrucaoTipo1->rb;
							rc = instrucaoTipo1->rc;

							registradores[rc] = registradores[ra] + rb;

							/* Flags */
							neg = flagNeg(rc);
							zero = flagZero(rc);
							overflow = flagOverflowAdd(ra,rb,rc);
							carry = flagCarry(ra,rb,rc);

							printf("addi r%d = %d\n", rc, registradores[rc]);
							mostraFlags();
					 break;
					 case 23:	//INC
							 ra = instrucaoTipo1->ra;
							 rc = instrucaoTipo1->rc;

							 registradores[ra]++;

							 registradores[rc] = registradores[ra];

							 /* Flags */
							 neg = flagNeg(rc);
							 zero = flagZero(rc);
							 if((registradores[ra] > 0 && registradores[rc] < 0) ||
					 		 (registradores[ra] < 0 && registradores[rc] > 0))
					 		 {
					 		 	 overflow = 1;
					 	   }
							 else
							 {
								 overflow = 0;
							 }

							 carry = 0;

							 printf("inc r%d = %d\n", rc, registradores[rc]);
							 mostraFlags();
					 break;
					 case 24: //DEC
							ra = instrucaoTipo1->ra;
							rc = instrucaoTipo1->rc;

							registradores[ra]--;

							registradores[rc] = registradores[ra];

							/* Flags */
							neg = flagNeg(rc);
							zero = flagZero(rc);
							if((registradores[ra] > 0 && registradores[rc] < 0) ||
							(registradores[ra] <= 0 && registradores[rc] > 0))
							{
								 overflow = 1;
							}
							else
							{
								overflow = 0;
							}
							carry = 0;

							printf("dec r%d = %d\n", rc, registradores[rc]);
							mostraFlags();
					 break;
					 case 25: //LI
							 ra = instrucaoTipo1->ra;
							 rc = instrucaoTipo1->rc;

						  registradores[rc] = ra;

						 /* Nenhuma Flag */

						 printf("li r%d = %d\n", rc, registradores[rc]);
						 mostraFlags();
					 break;
					 case 26:  //Branch greater than == bgt
							 ra = instrucaoTipo1->ra;
							 rb = instrucaoTipo1->rb;
							 numero = instrucaoTipo1->rc;

							 printf("bgt if %d > %d\n",registradores[ra],registradores[rb]);

 							if(registradores[ra] > registradores[rb])
 							{
 									PC = numero + Address;
 									printf(" then PC = %d\n", PC);
 							}
 							else
 							{
 							  	printf("nada\n");
 							}

 							/* Nenhuma Flag */
 							mostraFlags();
					 break;
					 case 32: //JUMP AND LINK
							numero = instrucaoTipo3->end;
							registradores[31] = PC;
							PC = numero + Address;

							/* Nenhuma Flag */

							printf("jal r31 = %d e PC = %d\n",registradores[31], PC);
							mostraFlags();
					 break;
					 case 33: //JUMP REGISTER
						 PC = registradores[31] + Address;

						 /* Nenhuma Flag */

						 printf("jr PC = %d\n", PC);
						 mostraFlags();
					 break;
					 case 34: //JUMP SE IGUAL
							ra = instrucaoTipo1->ra;
							rb = instrucaoTipo1->rb;
							numero = instrucaoTipo1->rc;

							printf("beq if %d==%d\n",registradores[ra],registradores[rb]);

							if(registradores[ra] == registradores[rb])
							{
									PC = numero + Address;
									printf(" then PC = %d\n", PC);
							}
							else
							{
							  	printf("nada\n");
							}

							/* Nenhuma Flag */
							mostraFlags();
					 break;
					 case 35: //JUMP SE DIFERENTE
							 ra = instrucaoTipo1->ra;
							 rb = instrucaoTipo1->rb;
							 numero = instrucaoTipo1->rc;

							 printf("bne if %d!=%d\n",registradores[ra],registradores[rb]);

							 if(registradores[ra] != registradores[rb])
							 {
									 PC = numero + Address;
									 printf(" then PC = %d", PC);
							 }
							 else
							 {
								   printf("nada\n");
							 }

							 /* Nenhuma Flag */
							 mostraFlags();
					 break;
					 case 36: //JUMP INCONDICIONAL
							numero = instrucaoTipo3->end;
							PC = numero + Address;

							/* Nenhuma Flag */

							printf("j PC = %d\n", PC);
							mostraFlags();
					 break;
					 case 255:
							printf("halt\n");
							exit(0);
					 break;
				}
}
