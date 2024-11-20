struct bits {
	unsigned char b7:1;
	unsigned char b6:1;
	unsigned char b5:1;
	unsigned char b4:1;
	unsigned char b3:1;
	unsigned char b2:1;
	unsigned char b1:1;
	unsigned char b0:1;
};
union byte {
	struct bits bi;
	unsigned char num;
};

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

//	TADLista
void Init (Lista **L) {
	*L = NULL;
}

char IsEmpty (Lista *L) {
	return L == NULL;
}

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


//tabela
void moldUp () {
	int x, y;
	gotoxy(1,1);
	printf("%c", 201);
	gotoxy(50,1);
	printf("%c", 187);
	
	gotoxy(1,3);
	printf("%c", 204);
	gotoxy(50,3);
	printf("%c", 185);
	
	for (x=2, y=1; x<50; x++) {
		gotoxy(x,y);
		printf("%c", 205);
		gotoxy(x, y+2);
		printf("%c", 205);
	}
	gotoxy(12,1);
	printf("%c", 203);
	gotoxy(28,1);
	printf("%c", 203);
	
	gotoxy(12,3);
	printf("%c", 206);
	gotoxy(28,3);
	printf("%c", 206);
	
	gotoxy(1,2);
	printf("%c  Codigo  %c", 186, 186);
	gotoxy(17,2);
	printf("Palavra    %c", 186);
	gotoxy(31,2);
	printf("Codigo de Huffman  %c", 186);
}

void mold (int y, Huffman item) {
	int i;
	gotoxy(1,y); printf("%c", 186);
	gotoxy(12,y); printf("%c", 186);
	gotoxy(28,y); printf("%c", 186);
	gotoxy(50,y); printf("%c", 186);
	
	gotoxy(6, y);
	printf("%d", item.codigo);
	gotoxy(16, y);
	printf("%s", item.palavra);
	gotoxy(35,y);
	i=0;
	while (item.codigoHuffman[i] != 2) {		
		printf("%d", item.codigoHuffman[i]);
		i++;
	}
}

void moldLow (int y) {
	int x;
	gotoxy(1,y);
	printf("%c", 200);
	gotoxy(50,y);
	printf("%c", 188);
	
	for (x=2; x<50; x++) {
		gotoxy(x,y);
		printf("%c", 205);
	}
	
	
	gotoxy(12,y);
	printf("%c", 202);
	gotoxy(28,y);
	printf("%c\n\n", 202);
}




