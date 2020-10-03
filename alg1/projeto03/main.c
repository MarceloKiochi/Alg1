/*
	Marcelo Kiochi Hatanaka
	10295645
	Trabalho 3 de Algortimos e Estruturas de Dados - Arvore Rubro-Negra classica
*/

#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#define OK 1

int main(){

	char string[10];
	int op;
	int info;
	int res;
	int erro;
	arv* tri;

	scanf("%s", string);

	tri = cria_arvore();

	while(!feof(stdin)){
		erro = 0;
		scanf("%d", &op);	/*operacao*/

		if(op==1){
			scanf("%d", &info);
			if(inserir(tri, info) != OK){
				erro = 1;	/*indica que deve imprimir erro*/
			}			
		}
		else if(op==2){
			scanf("%d", &info);
			if(busca_sucessor(tri, info, &res) == OK){
				printf("%d\n", res);
			}
			else{
				erro = 1;
			}
		}
		else if(op==3){
			scanf("%d", &info);
			if(busca_predecessor(tri, info, &res) == OK){
				printf("%d\n", res);
			}
			else{
				erro = 1;
			}
		}
		else if(op==4){
			if(busca_max(tri, &res) == OK){
				printf("%d\n", res);
			}
			else{
				erro = 1;
			}
		}
		else if(op==5){
			if(busca_min(tri, &res) == OK){
				printf("%d\n", res);
			}
			else{
				erro = 1;
			}
		}
		else if(op==6){
			impressao_pre_ordem(tri);
		}
		else if(op==7){
			impressao_em_ordem(tri);
		}
		else if(op==8){
			impressao_pos_ordem(tri);
		}
		if(erro == 1){
			printf("erro\n");
		}
	}

	finaliza(tri);


	return 0;
}