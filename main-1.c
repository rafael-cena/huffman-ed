#include <stdio.h>
#include <conio2.h>
#include <string.h>
#include <stdlib.h>
#define TF 40
int n = 0;
int cod[TF];
#include "tad.h"

void alterarItem(int codigo, Lista *lista) {
	Huffman item;
	int i;
	while (lista != NULL && lista->registro.codigo != codigo) lista = lista->prox;
	if (lista != NULL) {
		for (i=0; cod[i] != -1; i++) lista->registro.codigoHuffman[i] = cod[i];
		cod[i] = -1;
	}
}

void gerarHuffman (Tree *A, Lista *lista) {
	static int x=0;
	int i;
	
	if (A != NULL) {
		cod[x] = 0;
		x++;
		gerarHuffman (A->esq, lista);
		x--;
		cod[x] = 2;
		alterarItem(A->codigo, lista);
		
		cod[x] = 1;
		x++;
		gerarHuffman (A->dir, lista);
		x--;
		cod[x] = 2;
		alterarItem(A->codigo, lista);
	}
}

void exibeL (Lista *lista) {
	Lista *L = lista;
	printf("L");
	while (L != NULL) {
		printf(" -> (%s, %d)", L->registro.palavra, L->registro.codigo);
		L = L->prox;
	}
}

char equal (char *palavra, char *aux) {
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

char buscaPalavra (char *palavra, Lista *lista) {
	Lista *aux;
	aux=lista;
	while (aux!=NULL) {
		if (equal(aux->registro.palavra, palavra))
			return 1;
		aux=aux->prox;
	}
	return 0;
}

char* getPalavra (char frase[310], int j) {
	char *palavra;
	int i;
	palavra = (char*)malloc(50*sizeof(char));
	i=0;
	while (frase[j] != NULL && frase[j] != ' ' && frase[j] != '\0') {
		palavra[i] = frase[j];
		j++; i++;
	}
	palavra[i]='\0';
	return palavra;
}

void gerarBin (Lista *lista) {
	Huffman item;
	FILE *Ptr;
	int i;
	Lista *L = lista;
	moldUp();
	i=4;
	Ptr = fopen("tabela.dat", "wb");
	while (L != NULL) {
		item = L->registro;
		fwrite(&item, sizeof(Huffman), 1, Ptr);
		mold(i, item);
		i++;
		L = L->prox;
	}
	fclose(Ptr);
	moldLow(i);
}

void zerarVet(int i) {
	int j;
	for (j=i; j<TF; j++) cod[j] = 0;
}

Huffman getItem (Lista *L, char *palavra) {
	Huffman item;
	while (L != NULL && L->registro.codigoHuffman) {
		if (equal(palavra, L->registro.palavra))
			return L->registro;
		L = L->prox;
	}
	return item;
}

void fraseBin(Lista *lista) {
	char *frase, *palavra;
	Huffman item;
	int i, j, k, l, codigoFrase[100];
	Lista *L;
	union byte Byte, bAux;
	FILE *ptr;
	
	L = lista;	
	frase = "viver cada escolha fara crescer e amadurecer";
	k=0; j=0; i=0;
	
	palavra = "";
	palavra = getPalavra(frase, j);
	while (strlen(palavra) > 0) {
		L = lista;
		j += strlen(palavra)+1;
		item = getItem(L, palavra);
		for(k=0; item.codigoHuffman[k] != 2; k++, i++)
			codigoFrase[i] = item.codigoHuffman[k];
		codigoFrase[i] = 0;
		i++;
		palavra = getPalavra(frase, j);
	}
	zerarVet(i);
	
	ptr = fopen("frase.dat", "wb+");
	fwrite(&i, sizeof(int), 1, ptr);
	for (j=0; codigoFrase[j] != 2; j+=8) {
		Byte.bi.b0 = codigoFrase[j];
		Byte.bi.b1 = codigoFrase[j+1];
		Byte.bi.b2 = codigoFrase[j+2];
		Byte.bi.b3 = codigoFrase[j+3];
		Byte.bi.b4 = codigoFrase[j+4];
		Byte.bi.b5 = codigoFrase[j+5];
		Byte.bi.b6 = codigoFrase[j+6];
		Byte.bi.b7 = codigoFrase[j+7];
		fwrite(&Byte.num, sizeof(char), 1, ptr);
	}
	rewind(ptr);
	i=8;
	fread(&j, sizeof(int), 1, ptr);
	fread(&bAux.num, sizeof(char), 1, ptr);
	printf("\n\nFrase original: '%s'\n", frase);
	printf("Frase codificada: ");
	while(!feof(ptr) && i<j) {
		printf("%d", bAux.bi.b0);
		printf("%d", bAux.bi.b1);
		printf("%d", bAux.bi.b2);
		printf("%d", bAux.bi.b3);
		printf("%d", bAux.bi.b4);
		printf("%d", bAux.bi.b5);
		printf("%d", bAux.bi.b6);
		printf("%d ", bAux.bi.b7);
		i+=8;
		fread(&bAux.num, sizeof(char), 1, ptr);
	}
	fclose(ptr);
	printf("\n");
}

void Executar () {
	int i, j;
	char *frase, *fraseAux;
	char *palavra;
	char *aux;
	Lista *lista;
	Tree *tree;
	Huffman item;
	Floresta *floresta;

	frase = "amar e sonhar sonhar e viver viver e curtir curtir e amar cada um tera uma escolha cada um fara a escolha cada um escolhe a sua escolha levaremos um tempo para crescer levaremos um tempo para amadurecer levaremos um tempo para entender levaremos um tempo para envelhecer levaremos um tempo para morrer";
	Init(&lista);
	
	//contagem de frequencia para " " (espaco)
	item = newHuffman(" ");
	tree = CriaNo(n, 0);
	n+=1;
	for (i=0; frase[i] != '\0'; i++)
		if (frase[i] == ' ')
			tree->frequencia += 1;
	
	Push(&lista, item);
	inserirArvore(&floresta, tree);
	
	
	i=0;
	//contagem de frequencia para cada palavra, verificando se a palavra ja foi utilizada
	palavra="";
	palavra=getPalavra(frase, i);
	while (strlen(palavra) > 0) {
		i += strlen(palavra)+1;
		if (!buscaPalavra(palavra, lista)) {
			item = newHuffman(palavra);
			tree = CriaNo(n, 0);
			tree->frequencia+=1;
			n+=1;
			j=i;
			aux=getPalavra(frase, j);
			while (strlen(aux) > 0) {
				j += strlen(aux)+1;
				if (equal(palavra, aux))
					tree->frequencia += 1;
				aux=getPalavra(frase, j);
			}
			
			Push(&lista, item);
			inserirArvore(&floresta, tree);
		}
		palavra=getPalavra(frase, i);
	}
	montarArvore(&floresta);
	gerarHuffman(floresta->raiz, lista);
	gerarBin(lista);
	exibeh(floresta->raiz);
	fraseBin(lista);
}

int main (void) {
	Executar();
	return 0;
}
