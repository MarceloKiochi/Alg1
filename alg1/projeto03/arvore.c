#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#define OK 1
#define VERMELHO 1
#define PRETO 0

struct nodo{
	int info;
	nodo* esq;
	nodo* dir;
	nodo* pai;
	int cor;	/*1 para vermelho e 0 para preto*/
};

struct arvore{
	nodo* raiz;
};

arv* cria_arvore(){/*inicia uma arvore rubro-negra*/
	arv* tri = (arv*)malloc(sizeof(arv));
	tri->raiz = NULL;
	return tri;
}

nodo* cria_nodo(int info){/*aloca um nodo, zera as variaveis e retorna o ponteiro*/
	nodo* novo = (nodo*)malloc(sizeof(nodo));
	novo->info = info;
	novo->esq = NULL;
	novo->dir = NULL;
	novo->pai = NULL;
	novo->cor = VERMELHO;
	return novo;
}

void inverte_cor(nodo* raiz){/*inverte as cores nodo passado e dos seus filhos*/
	if(raiz==NULL) return;

	raiz->cor = !raiz->cor;

	if(raiz->esq!=NULL){
		raiz->esq->cor = !raiz->esq->cor;
	}
	if(raiz->dir!=NULL){
		raiz->dir->cor = !raiz->dir->cor;
	}
}

nodo* rotEE(nodo* raiz){	/*rotacao simples para a esquerda*/
	nodo* aux;
	aux = raiz->dir;
	aux->pai = raiz->pai;

	if(aux->esq!=NULL)
		aux->esq->pai = raiz;

	raiz->dir = aux->esq;
	raiz->pai = aux;
	aux->esq = raiz;
	aux->cor = raiz->cor;
	raiz->cor = VERMELHO;
	return aux;
}

nodo* rotDD(nodo* raiz){	/*rotacao simples para a direita*/
	nodo* aux;
	aux = raiz->esq;
	aux->pai = raiz->pai;

	if(aux->dir!=NULL)
		aux->dir->pai = raiz;

	raiz->esq = aux->dir;
	raiz->pai = aux;
	aux->dir = raiz;
	aux->cor = raiz->cor;
	raiz->cor = VERMELHO;

	return aux;
}

int cor(nodo* raiz){	/*retorna a cor do nodo passado*/
	if(raiz==NULL) return PRETO;/*nodos nulos sao pretos*/

	return raiz->cor;
}

int inserir_rec(nodo** raiz, int* info, int* flag){	/*insercao de um nodo de forma recursiva*/
	if(*raiz==NULL){
		*raiz = cria_nodo(*info);
		*flag = 1;	/*indica que um novo nodo foi inserido e precisa verificar se eh necessario fazer alguma alteracao*/
		return OK;
	}
	else if(*info < (*raiz)->info){	/*insere a esquerda*/
		if(inserir_rec(&(*raiz)->esq, info, flag) != OK) return !OK;	/*se retornar !OK eh pq o elemento ja existe*/
		if(*flag){
			(*raiz)->esq->pai = (*raiz);	/*atualiza o pai do filho, que talvez nao esteja certo*/
			if((*raiz)->esq->info == *info){	/*se o ondo inserido eh o filho desse, eh preciso voltar ao avo para ver mais coisas*/
				*flag = 1;	/*flag igual a 1 indica que o nodo foi inserido a esquerda*/
			}
			else{/*se o nodo atual for o avo do inserido, entra aqui*/
				if((*raiz)->cor == PRETO && cor((*raiz)->esq) == VERMELHO && cor((*raiz)->dir) == VERMELHO){
					/*avo eh preto, pai e tio vermelhos*/
					inverte_cor((*raiz));
					if(cor((*raiz)->pai) == PRETO) *flag = 0;	/*o avo vai ficar vermelho, se o api dele for preto, ok*/
					else{	/*mas se for vermelho, eh como se o avo estivesse sendo inserido agora*/
						*info = (*raiz)->info;	/*agora a informacao que foi inserida eh a do avo*/
						if((*raiz)->pai!=NULL && (*raiz)->pai->esq == (*raiz))	/*ve se o avo eh filho esquerdo ou direito para mudar a flag*/
							*flag = 1;
						else
							*flag = 2;
					}
				}
				else if((*raiz)->cor == PRETO && cor((*raiz)->dir)==PRETO && cor((*raiz)->esq)==VERMELHO){
					/*avo eh preto tio eh preto e pai eh vermelho*/
					if(*flag == 1){	/*se o nodo foi inserido na esquerda, rotacao simples*/
						(*raiz) = rotDD((*raiz));
					}

					else if(*flag == 2){	/*se nao, rotacao dupla*/
						(*raiz)->esq = rotEE((*raiz)->esq);
						(*raiz) = rotDD((*raiz));
					}
					*flag = 0;						
				}
				else *flag = 0;
			}
		}
		return OK;
	}
	else if(*info > (*raiz)->info){/*mesma coisa pra a direita*/
		if(inserir_rec(&(*raiz)->dir, info, flag) != OK) return !OK;
		if(*flag){
			(*raiz)->dir->pai = (*raiz);
			if((*raiz)->dir->info == *info){
				*flag = 2;							
			}
			else{
				if((*raiz)->cor == PRETO && cor((*raiz)->esq) == VERMELHO && cor((*raiz)->dir) == VERMELHO){
					inverte_cor((*raiz));
					if(cor((*raiz)->pai) == PRETO) *flag = 0;
					else{
						*info = (*raiz)->info;
						if((*raiz)->pai!=NULL && (*raiz)->pai->dir == (*raiz))
							*flag = 2;
						else{
							*flag = 1;
						}
					}
				}
				else if((*raiz)->cor == PRETO && cor((*raiz)->dir)==VERMELHO && cor((*raiz)->esq)==PRETO){
					if(*flag == 2){
						(*raiz) = rotEE((*raiz));
					}

					else if(*flag == 1){
						(*raiz)->dir = rotDD((*raiz)->dir);
						(*raiz) = rotEE((*raiz));

					}
					*flag = 0;	
				}
				else *flag = 0;										
			}

		}
		return OK;
	}
	return !OK;
}

int inserir(arv* tri, int info){	/*chama a recursao para a insercao*/
	int flag = 0;
	if(tri==NULL) return !OK;

	if(inserir_rec(&tri->raiz, &info, &flag) != OK) return !OK;

	if(tri->raiz!=NULL) tri->raiz->cor = PRETO; /*sempre muda a raiz para preto*/
	return OK;
}

void finaliza_nodo(nodo* raiz){	/*funcao que libera os nodos de forma recursiva*/
	if(raiz!=NULL){
		finaliza_nodo(raiz->esq);
		finaliza_nodo(raiz->dir);
		free(raiz);
		raiz = NULL;
	}
}

void finaliza(arv* tri){	/*chama a recursao para finalizacao da arvore*/
	if(tri!=NULL){
		finaliza_nodo(tri->raiz);
		free(tri);
		tri = NULL;
	}
}

nodo* busca_nodo(nodo* raiz, int info){	/*busca o ponteiro de um elemento passado*/
	if(raiz==NULL) return NULL;

	if(info > raiz->info) return busca_nodo(raiz->dir, info);

	if(info < raiz->info) return busca_nodo(raiz->esq, info);

	return raiz;
}

int busca_predecessor(arv* tri, int info, int* res){
	nodo* aux;
	aux = busca_nodo(tri->raiz, info);	/*pega o ponteiro do elemento*/

	if(aux==NULL) return !OK;
	*res = info;

	if(aux->esq != NULL){	/*se tiver filho esquerdo, esta embaixo*/
		aux = aux->esq;
		while(aux->dir!=NULL){
			aux = aux->dir;
		}
	}		

	else{	/*se nao, esta em cima*/
		while(aux->pai!=NULL && aux->info >= info){
			aux = aux->pai;
		}
	}	

	*res = aux->info;

	if(aux->info >= info) return !OK;

	return OK;
}

int busca_sucessor(arv* tri, int info, int* res){/*igual ao predecessor*/
	nodo* aux;
	aux = busca_nodo(tri->raiz, info);
	if(aux==NULL) return !OK;
	*res = info;

	if(aux->dir != NULL){
		aux = aux->dir;
		while(aux->esq!=NULL)
			aux = aux->esq;	
	}

	else{
		while(aux->pai!=NULL && aux->info <= info)
			aux = aux->pai;
	}
	*res = aux->info;

	if(*res <= info) return !OK;

	return OK;
}

int busca_max(arv* tri, int* res){

	nodo* aux;

	if(tri==NULL) return !OK;

	if(tri->raiz == NULL) return !OK;

	aux = tri->raiz;
	while(aux->dir != NULL){/*anda o max para a direita*/
		aux = aux->dir;
	}

	*res = aux->info;

	return OK;
}

int busca_min(arv* tri, int* res){

	nodo* aux;

	if(tri==NULL) return !OK;

	if(tri->raiz == NULL) return !OK;

	aux = tri->raiz;
	while(aux->esq != NULL){/*anda o max para a esquerda*/
		aux = aux->esq;
	}

	*res = aux->info;

	return OK;
}

void pre_ordem_rec(nodo* raiz){
	if(raiz==NULL) return;
	printf("%d ", raiz->info);
	pre_ordem_rec(raiz->esq);
	pre_ordem_rec(raiz->dir);
}

void impressao_pre_ordem(arv* tri){
	if(tri==NULL) return;
	pre_ordem_rec(tri->raiz);
	printf("\n");
}

void em_ordem_rec(nodo* raiz){
	if(raiz==NULL) return;
	em_ordem_rec(raiz->esq);
	printf("%d ", raiz->info);
	em_ordem_rec(raiz->dir);
}

void impressao_em_ordem(arv* tri){
	if(tri==NULL) return;
	em_ordem_rec(tri->raiz);
	printf("\n");
}

void pos_ordem_rec(nodo* raiz){
	if(raiz==NULL) return;
	pos_ordem_rec(raiz->esq);
	pos_ordem_rec(raiz->dir);
	printf("%d ", raiz->info);
}

void impressao_pos_ordem(arv* tri){
	if(tri==NULL) return;
	pos_ordem_rec(tri->raiz);
	printf("\n");
}