#include <stdio.h>
#include <conio2.h>
#include <string.h>
#include <stdlib.h>

#define TF 100

#include "tad.h"
#include "util.h"

void gerarArvore (Tree **raiz, Huffman H, int i) {
	// se == -1, indica fim do codigo Huffman
	if (H.codigoHuffman[i] != -1) { 
		// se == 0, caminha para esquerda na arvore
		if (H.codigoHuffman[i] == 0) {
			// se NULL, cria uma nova caixa na esquerda
			if ((*raiz)->esq == NULL)
				(*raiz)->esq = CriaNo(-1, 0);
				
			// chama recursivamente
			gerarArvore(&(*raiz)->esq, H, i+1);
			
			// se ja voltar ->esq->esq == NULL, quer dizer que chegou no lugar do item H
			if ((*raiz)->esq->esq == NULL)
				(*raiz)->esq->codigo = H.codigo;
		}
		else {
			// senao, caminha para direita na arvore
			if (H.codigoHuffman[i] == 1) {
				// se NULL, cria uma nova caixa na direita
				if ((*raiz)->dir == NULL)
					(*raiz)->dir = CriaNo(-1, 0);
					
				// chama recursivamente
				gerarArvore(&(*raiz)->dir, H, i+1);
				
				// se ja voltar ->dir->dir == NULL, quer dizer que chegou no lugar do item H
				if ((*raiz)->dir->dir == NULL)
					(*raiz)->dir->codigo = H.codigo;
			}
		}
	}
}

void lerArquivo (Lista **L, Tree **raiz) {
	FILE *ptr;
	Huffman H;
	int y=4;
	
	// exibe 'cabecalho' da tabela
	MolduraSuperior();
	ptr = fopen("tabela.dat", "rb");
	
	// leitura do arquivo
	fread(&H, sizeof(Huffman), 1, ptr);
	while (!feof(ptr)) {
		// exibe item Huffman na tabela
		MolduraItem(y, H);
		y++;
		
		// insere na Lista
		Enqueue(&*L, H);
		// insere ja na posicao de Huffman na arvore
		gerarArvore(&*raiz, H, 0);
		
		fread(&H, sizeof(Huffman), 1, ptr);
	}
	fclose(ptr);
	MolduraInferior(y);
}

void preencheVet (int fraseCod[], int *n) {
	FILE *ptr;
	int i;
	union byte Byte;
	
	ptr = fopen("frase.dat", "rb");
	i=0;
	fread(&*n, sizeof(int), 1, ptr);
	fread(&Byte.num, sizeof(char), 1, ptr);
	// ler do arquivo e gravar bit a bit no vetor fraseCod
	while(!feof(ptr) && i<*n) {
		fraseCod[i] = Byte.bi.b0;
		fraseCod[i+1] = Byte.bi.b1;
		fraseCod[i+2] = Byte.bi.b2;
		fraseCod[i+3] = Byte.bi.b3;
		fraseCod[i+4] = Byte.bi.b4;
		fraseCod[i+5] = Byte.bi.b5;
		fraseCod[i+6] = Byte.bi.b6;
		fraseCod[i+7] = Byte.bi.b7;
		i+=8;
		fread(&Byte.num, sizeof(char), 1, ptr);
	}
	
	while (i<*n) {
		fraseCod[i] = 0;
		i++;
	}
	fclose(ptr);
}

char *buscarPalavra (Lista *L, int cod) {
	while (L != NULL && L->registro.codigo != cod)
		L = L->prox;
	
	if (L != NULL)
		return L->registro.palavra;
}

void montarFrase (Tree *raiz, Lista *L, int fraseCod[], char frase[], int *i) {
	if (raiz != NULL) {
		if (raiz->esq == NULL && raiz->dir == NULL) {
			// no folha, encontrou o codigo pra palavra
			strcat(frase, buscarPalavra(L, raiz->codigo));
		}
		else {
			// recursivo pra esquerda
			if (fraseCod[*i] == 0) {
				*i = *i + 1;
				montarFrase(raiz->esq, L, fraseCod, frase, &*i);
			}
			// recursivo pra direita
			else {
				if (fraseCod[*i] == 1) {
					*i = *i + 1;
					montarFrase(raiz->dir, L, fraseCod, frase, &*i);
				}
			}
		}
	}
}

void executar () {
	Lista *L, lista;
	Tree *T;
	char frase[TF];
	int fraseCod[TF];
	int i, n;
	
	L=NULL;
	T= CriaNo(-1, 0);
	
	// onde sera lido o arquivo 'tabela.dat' para montar e exibir a Lista (tabela) e a arvore
	lerArquivo(&L, &T);
	ExibeH(T);
	
	// preenchimento de fraseCod a partir do arquivo 'frase.dat'
	preencheVet(fraseCod, &n);
	
	i=0;
	frase[0] = '\0';
	// exibir o resultado
	while (i<n)
		montarFrase(T, L, fraseCod, frase, &i);
	
	printf("\nFrase Codificada: ");
	for (i=0; i < TF && (fraseCod[i] == 0 || fraseCod[i] == 1); i++) {
		printf("%d", fraseCod[i]);
		if ((i+1)%8 == 0)
			printf(" ");
	}
	
	printf("\n\nFrase Decodificada: %s\n", frase);
}

int main () {
	executar();
	return 0;
}
