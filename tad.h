struct huffman {
	int codigo, frequencia;
	char *palavra, *codigoHuffman;
};
typedef struct huffman Huffman;

struct tree {
	struct tree *esq, *dir;
	Huffman registro;
};
typedef struct tree Tree;

struct lista {
	Tree *no;
	struct lista *prox;
};
typedef struct lista Lista;

Tree *CriaNo (Huffman reg) {
	Tree *no = (Tree*)malloc(sizeof(Tree));
	no->registro = reg;
	no->esq=NULL;
	no->dir=NULL;
	return no;
}

Huffman newHuffman (char *palavra) {
	Huffman novo;
	novo.codigo = n;
	novo.frequencia = 0;
	novo.palavra = palavra;
	novo.codigoHuffman = "";
	return novo;
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

Tree *Top (Lista *L) {
	if (!IsEmpty(L))
		return L->no;
	return NULL;
}

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

void Pop (Lista **L, Tree *no) {
	Lista *aux;
	if (!IsEmpty(*L)) {
		no = (*L)->no;
		aux = *L;
		*L = (*L)->prox;
		free(aux);
	}
	else {
		no = NULL;
	}
}
