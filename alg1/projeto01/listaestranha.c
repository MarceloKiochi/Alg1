#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "listaestranha.h"

expr* cria_expressao(){

	expr* expressao = (expr*)malloc(sizeof(expr));

	assert(expressao!=NULL);

	expressao->elem = NULL;

	expressao->nelem = 0;

	expressao->erro = 0;

	return expressao;
}

void adiciona_elem(expr** expressao, char sinal, double valor){	//adiciona elementos sempre no fim da lista

	assert(((*expressao))!=NULL);
					
	(*expressao)->elem = (elementos*)realloc((*expressao)->elem, ((*expressao)->nelem+1)*sizeof(elementos));	

	(*expressao)->elem[(*expressao)->nelem].sinal = sinal; //pega o sinal e o valor passado

	(*expressao)->elem[(*expressao)->nelem].valor = valor;

	(*expressao)->nelem++;	
			
}
void retira_elem(int posicao, expr* expressao){ //retira um elemento de uma posicao especifica
												//sem pegar o valor (só exclui)

	assert(expressao!=NULL);

	int l;

	for(l = posicao; l < expressao->nelem-1; l++){
		expressao->elem[l].valor = expressao->elem[l+1].valor;
		expressao->elem[l].sinal = expressao->elem[l+1].sinal;
	}

	expressao->nelem--;
}

void its_been_a_long_day(expr* expressao){ //adeus memoria alocada, seja livre!

	assert(expressao!=NULL);

	free(expressao->elem);

	free(expressao); //#LiberdadeDeExpressao	
}