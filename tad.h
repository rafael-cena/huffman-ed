struct huffman {
	int codigo, codigoHuffman[40];
	char palavra[25];
};
typedef struct huffman Huffman;

struct tree {
	int frequencia, codigo;
	struct tree *esq, *dir;
};
typedef struct tree Tree;

struct floresta {
	Tree *raiz;
	struct floresta *prox;
};
typedef struct floresta Floresta;

struct lista {
	Huffman registro;
	struct lista *prox;
};
typedef struct lista Lista;

Tree *CriaNo (int cod, int freq) {
	Tree *no = (Tree*)malloc(sizeof(Tree));
	no->codigo = cod;
	no->frequencia = freq;
	no->esq=NULL;
	no->dir=NULL;
	return no;
}

Floresta *CriaArvore (Tree *no) {
	Floresta *raiz = (Floresta*)malloc(sizeof(Floresta));
	raiz->raiz = no;
	raiz->prox = NULL;
	return raiz;
}

Huffman newHuffman (char *palavra) {
	Huffman novo;
	novo.codigo = n;
	strcpy(novo.palavra, palavra);
	return novo;
}

void exibeh (Tree *A) {
	static int x = -1;
	int i;
	if (A != NULL) {
		x++;
		exibeh (A->dir);
		for (i=0; i<5*x; i++) printf(" ");
		if (A->codigo == -1) printf("(-, %d) \n",A->frequencia);
		else printf("(%d, %d) \n",A->codigo,A->frequencia);
		exibeh (A->esq);
		x--;
	}
}

//	TADPilha
struct pilha {
	Tree *info;
	struct pilha *prox;
};
typedef struct pilha Pilha;

void init (Pilha **P) {
	*P = NULL;
}

char isEmpty (Pilha *P) {
	return P == NULL;
}

Tree *top (Pilha *P) {
	if (!isEmpty(P))
		return P->info;
	return NULL;
}

void push (Pilha **P, Tree *no) {
	Pilha *nova;
	nova = (Pilha*)malloc(sizeof(Pilha));
	nova->info = no;
	nova->prox = *P;
	*P = nova;
}

void pop (Pilha **P, Tree *no) {
	Pilha *aux;
	if (!isEmpty(*P)) {
		no = (*P)->info;
		aux = *P;
		*P = (*P)->prox;
		free(aux);
	}
	else {
		no = NULL;
	}
}


//	TADLista
void Init (Lista **L) {
	*L = NULL;
}

char IsEmpty (Lista *L) {
	return L == NULL;
}

/*
void Push (Lista **L, Tree *no) {
	Lista *nova, *aux, *ant;
	nova = (Lista*)malloc(sizeof(Lista));
	nova->no = no;
	nova->prox = NULL;
	
	if (IsEmpty(*L))
		*L = nova;
	else {
		aux = ant = *L;
		while (aux->prox != NULL && aux->no->registro.frequencia <= nova->no->registro.frequencia) {
			ant = aux;
			aux = aux->prox;
		}
		if (aux->no->registro.frequencia < nova->no->registro.frequencia) {
			nova->prox = aux->prox;
			aux->prox = nova;
		}
		else {
			nova->prox = aux;
			if (aux == ant) *L = nova;
			else ant->prox = nova;
		}
	}
}
*/

void Push(Lista **L, Huffman item) {
	Lista *nova;
	nova = (Lista*)malloc(sizeof(Lista));
	nova->registro = item;
	nova->prox = NULL;
	
	if (IsEmpty(*L))
		*L = nova;
	else {
		nova->prox = *L;
		(*L) = nova;
	}
}

Huffman Pop (Lista **L) {
	Lista *aux;
	Huffman item;
	if (!IsEmpty(*L)) {
		item = (*L)->registro;
		aux = *L;
		*L = (*L)->prox;
		free(aux);
	}
	return item;
}

//	TADFloresta
void inicializar(Floresta **F) {
	*F = NULL;
}

char vazia(Floresta *F) {
	return F == NULL;
}

void inserirArvore(Floresta **F, Tree *no) {
	Floresta *aux, *ant, *nova;
	
	nova = (Floresta*)malloc(sizeof(Floresta));
	nova->raiz = no;
	nova->prox = NULL;
	
	if (vazia(*F)) *F = nova;
	else {
		if ((*F)->raiz->frequencia > nova->raiz->frequencia) {
			nova->prox = *F;
			*F = nova;
		}
		else {
			ant=*F;
			aux=ant->prox;
			if (aux != NULL) {
				while (aux->prox!=NULL && aux->raiz->frequencia <= nova->raiz->frequencia) {
					ant = aux;
					aux = aux->prox;
				}
				if (aux->raiz->frequencia < nova->raiz->frequencia) {
					nova->prox = aux->prox;
					ant->prox = nova;
				}
				else {
					nova->prox=aux;
					ant->prox = nova;
				}
			}
			else {
				if (ant->raiz->frequencia < nova->raiz->frequencia)
					ant->prox = nova;
				else {
					nova->prox=ant;
					*F = nova;
				}
			}
		}
	}
}

Tree *removerArvore (Floresta **F) {
	Floresta *aux;
	Tree *tree;
	if (!vazia(*F)) {
		tree = (*F)->raiz;
		aux = *F;
		*F = (*F)->prox;
		free(aux);
		return tree;
	}
	return NULL;
}

void montarArvore(Floresta **F) {
	Tree *ant, *aux, *nova;
	
	while ((*F)->prox != NULL) {
		ant = removerArvore(&*F);
		aux = removerArvore(&*F);
		
		nova = CriaNo(-1, ant->frequencia+aux->frequencia);
		nova->esq = ant;
		nova->dir = aux;
		
		inserirArvore(&*F, nova);
	}
}
