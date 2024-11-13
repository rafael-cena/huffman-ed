#include <stdio.h>
#include <conio2.h>
#include <string.h>
#include <stdlib.h>

int n = 0;
#include "tad.h"

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
//		i=0;
//		printf("%d\t|%s \t|", item.codigo, item.palavra);
//		while (item.codigoHuffman[i] != 2) {		
//			printf("%d", item.codigoHuffman[i]);
//			i++;
//		}
//		printf("\n");
		i=0;
		refTree(&F->raiz, item, i);
		fread(&item, sizeof(Huffman), 1, Ptr);
	}
	fclose(Ptr);
	
	exibeh(F->raiz);
}

int main (void) {
	Exec();
	return 0;
}
