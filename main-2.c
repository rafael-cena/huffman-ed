#include <stdio.h>
#include <conio2.h>
#include <string.h>
#include <stdlib.h>

int n = 0;
int cod[100];
#include "tad.h"

char* exibePalavra (Lista *L, int cod) {
	while (L != NULL) {
		if (L->registro.codigo == cod) return L->registro.palavra;
		L = L->prox;
	}
}

void exibef(Tree *raiz, Lista *L) {
	if (raiz != NULL) {
		if (raiz->esq == NULL && raiz->dir == NULL) {
			printf("%s", exibePalavra(L, raiz->codigo));
			n++;
		}
		else 
			if (cod[n] == 0) { //ramificacao a esquerda
				n++;
				exibef(raiz->esq, L);
			}
			else {				//ramificacao a direita
				n++;
				exibef(raiz->dir, L);
			}
	}
}

void pVet() {
	FILE *ptr;
	int i;
	union byte bAux;
	
	ptr = fopen("frase.dat", "rb");
	i=0;
	fread(&n, sizeof(int), 1, ptr);
	fread(&bAux.num, sizeof(char), 1, ptr);
	while(!feof(ptr) && i<n) {
		cod[i] = bAux.bi.b0;
		cod[i+1] = bAux.bi.b1;
		cod[i+2] = bAux.bi.b2;
		cod[i+3] = bAux.bi.b3;
		cod[i+4] = bAux.bi.b4;
		cod[i+5] = bAux.bi.b5;
		cod[i+6] = bAux.bi.b6;
		cod[i+7] = bAux.bi.b7;
		i+=8;
		fread(&bAux.num, sizeof(char), 1, ptr);
	}
	fclose(ptr);
}

void refTree (Tree **raiz, Huffman item, int i) {
	if (item.codigoHuffman[i] != 2) {
		if (item.codigoHuffman[i] == 0) { //ramificacao a esquerda
			if ((*raiz)->esq == NULL)
				(*raiz)->esq = CriaNo(-1, 0);
			refTree(&(*raiz)->esq, item, i+1);
			if ((*raiz)->esq->esq == NULL) (*raiz)->esq->codigo = item.codigo;
		}
		else {
			if ((*raiz)->dir == NULL)
				(*raiz)->dir = CriaNo(-1, 0);
			refTree(&(*raiz)->dir, item, i+1);
			if ((*raiz)->dir->dir == NULL) (*raiz)->dir->codigo = item.codigo;
		}
	}
}

void Exec() {
	Huffman item;
	FILE *Ptr;
	Tree *tree;
	Lista *L;
	Floresta *F;
	int i;
	
	Init(&L);
	inicializar(&F);
	
	tree = CriaNo(-1,0);
	inserirArvore(&F, tree);
	
	Ptr = fopen("tabela.dat", "rb");
	fread(&item, sizeof(Huffman), 1, Ptr);
	while (!feof(Ptr)) {
		i=0;
		printf("%d\t|%s \t|", item.codigo, item.palavra);
		while (item.codigoHuffman[i] != 2) {		
			printf("%d", item.codigoHuffman[i]);
			i++;
		}
		printf("\n");
		i=0;
		refTree(&F->raiz, item, i);
		Push(&L, item);
		fread(&item, sizeof(Huffman), 1, Ptr);
	}
	fclose(Ptr);
	
	exibeh(F->raiz);
	pVet();
	i=n;
	n=0;
	while (n<i) exibef(F->raiz, L);
}

int main (void) {
	Exec();
	return 0;
}
