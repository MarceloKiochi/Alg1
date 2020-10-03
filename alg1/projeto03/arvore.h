typedef struct nodo nodo;
typedef struct arvore arv;

arv* cria_arvore();

int inserir(arv* tri, int info);

int busca_sucessor(arv* tri, int info, int* res);

int busca_predecessor(arv* tri, int info, int* res);

int busca_max(arv* tri, int* res);

int busca_min(arv* tri, int* res);

void impressao_pre_ordem(arv* tri);

void impressao_em_ordem(arv* tri);

void impressao_pos_ordem(arv* tri);

void finaliza(arv* tri);