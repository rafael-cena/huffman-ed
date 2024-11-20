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

void exibef(Tree *raiz, Lista *L, char frase[]) {
	if (raiz != NULL) {
		if (raiz->esq == NULL && raiz->dir == NULL) {
			strcat(frase, exibePalavra(L, raiz->codigo));
		}
		else 
			if (cod[n] == 0) { //ramificacao a esquerda
				n++;
				exibef(raiz->esq, L, frase);
			}
			else {				//ramificacao a direita
				n++;
				exibef(raiz->dir, L, frase);
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
		if (item.codigoHuffman[i] == 0) {
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
	char frase[100];
	
	Init(&L);
	inicializar(&F);
	
	tree = CriaNo(-1,0);
	inserirArvore(&F, tree);
	moldUp();
	i=4;
	Ptr = fopen("tabela.dat", "rb");
	fread(&item, sizeof(Huffman), 1, Ptr);
	while (!feof(Ptr)) {
		mold(i, item);
		i++;
		refTree(&F->raiz, item, 0);
		Push(&L, item);
		fread(&item, sizeof(Huffman), 1, Ptr);
	}
	fclose(Ptr);
	moldLow(i);
	
	exibeh(F->raiz);
	pVet();
	i=n;
	n=0;
	while (n<i) 
		exibef(F->raiz, L, frase);
	printf("\nFrase decodificada: '%s'\n", frase);
}

int main (void) {
	Exec();
	return 0;
}
