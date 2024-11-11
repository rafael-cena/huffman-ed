#include <stdio.h>
#include <conio2.h>
#include <string.h>
#include <stdlib.h>

int n = 0;
#include "tad.h"

void Exec() {
	Huffman item;
	FILE *Ptr;
	Tree *tree;
	Lista *L;
	
	Init(&L);
	Ptr = fopen("tabela.dat", "rb");
	while (!feof(Ptr)) {
		fread(&item, sizeof(Huffman), 1, Ptr);
		printf("%d\t|%s \t|%d\t|%s\n", item.codigo, item.palavra, item.frequencia, item.codigoHuffman);
		tree = CriaNo(item);
		Push(&L, tree);
	}
	fclose(Ptr);
//	exibeL(L);
}

int main (void) {
	Exec();
	return 0;
}
