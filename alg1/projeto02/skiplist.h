typedef struct skip skiplist;

typedef struct pala palavra;

skiplist* cria_lista();

int insere(skiplist* l, char* word, char* def);

int impressao(skiplist* l, char let);

void fri(skiplist* l);

char* buscadef(skiplist*l, char* word);

int altera(skiplist* l, char* word, char* def);

int remover(skiplist* l, char* word);