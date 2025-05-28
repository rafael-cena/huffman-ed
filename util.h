Tree *CriaNo (int cod, int freq) {
	Tree *no = (Tree*)malloc(sizeof(Tree));
	no->codigo = cod;
	no->freq = freq;
	no->esq=NULL;
	no->dir=NULL;
	return no;
}

Huffman NewHuffman (int n, char *palavra) {
	Huffman novo;
	novo.codigo = n;
	strcpy(novo.palavra, palavra);
	return novo;
}

void ExibeH (Tree *A) {
	static int x = -1;
	int i;
	if (A != NULL) {
		x++;
		ExibeH(A->dir);
		for (i=0; i<5*x; i++) printf(" ");
		if (A->codigo == -1) printf("(-, %d) \n",A->freq);
		else printf("(%d, %d) \n",A->codigo,A->freq);
		ExibeH(A->esq);
		x--;
	}
}

// compara as strings, letra por letra
char iguais (char *palavra, char *aux) {
	int i;
	if (strlen(palavra) == strlen(aux)) {
		for (i=0; palavra[i] != '\0' && aux[i] != '\0'; i++)
			if (palavra[i] != aux[i])
				return 0;
		if (palavra[i] == '\0' && aux[i] == '\0')
			return 1;
	}
	return 0;
}

Huffman getItem (Lista *L, char *palavra) {
	Huffman item;
	while (L != NULL && L->registro.codigoHuffman) {
		if (iguais(palavra, L->registro.palavra))
			return L->registro;
		L = L->prox;
	}
	return item;
}

//	TADLista
char IsEmpty (Lista *L) {
	return L == NULL;
}

void Enqueue(Lista **L, Huffman item) {
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

Huffman Dequeue (Lista **L) {
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
char Vazia(Floresta *F) {
	return F == NULL;
}

void InserirArvore(Floresta **F, Tree *no) {
	Floresta *aux, *ant, *nova;
	
	nova = (Floresta*)malloc(sizeof(Floresta));
	nova->raiz = no;
	nova->prox = NULL;
	
	if (Vazia(*F)) 
		*F = nova;
	else {
		if ((*F)->raiz->freq > nova->raiz->freq) {
			nova->prox = *F;
			*F = nova;
		}
		else {
			ant=*F;
			aux=ant->prox;
			if (aux != NULL) {
				while (aux->prox!=NULL && aux->raiz->freq <= nova->raiz->freq) {
					ant = aux;
					aux = aux->prox;
				}
				if (aux->raiz->freq < nova->raiz->freq) {
					nova->prox = aux->prox;
					ant->prox = nova;
				}
				else {
					nova->prox=aux;
					ant->prox = nova;
				}
			}
			else {
				if (ant->raiz->freq < nova->raiz->freq)
					ant->prox = nova;
				else {
					nova->prox=ant;
					*F = nova;
				}
			}
		}
	}
}

Tree *RemoverArvore (Floresta **F) {
	Floresta *aux;
	Tree *tree;
	tree = NULL;
	if (!Vazia(*F)) {
		tree = (*F)->raiz;
		aux = *F;
		*F = (*F)->prox;
		free(aux);
	}
	return tree;
}

void MontarArvore(Floresta **F) {
	Tree *ant, *aux, *nova;
	
	while ((*F)->prox != NULL) {
		ant = RemoverArvore(&*F);
		aux = RemoverArvore(&*F);
		
		nova = CriaNo(-1, ant->freq+aux->freq);
		nova->esq = ant;
		nova->dir = aux;
		
		InserirArvore(&*F, nova);
	}
}


//tabela
void MolduraSuperior () {
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

void MolduraItem (int y, Huffman item) {
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
	while (item.codigoHuffman[i] != -1) {		
		printf("%d", item.codigoHuffman[i]);
		i++;
	}
}

void MolduraInferior (int y) {
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
