#include <stdio.h>
#include <conio2.h>
#include <string.h>
#include <stdlib.h>

#define TF 100

#include "tad.h"
#include "util.h"

char* getPalavra (char frase[310], int j) {
	char *palavra;
	int i;
	palavra = (char*)malloc(50*sizeof(char));
	i=0;
	while (frase != NULL && frase[j] != ' ' && frase[j] != '\0') {
		palavra[i] = frase[j];
		j++; i++;
	}
	palavra[i]='\0';
	return palavra;
}

// procura se a palavra ja esta na lista, para nao contar repetido
char buscaPalavra (char *palavra, Lista *lista) {
	Lista *aux;
	aux=lista;
	while (aux!=NULL) {
		if (iguais(aux->registro.palavra, palavra))
			return 1;
		aux=aux->prox;
	}
	return 0;
}

void contagem (Lista **L, Floresta **F, char *frase) {
	int i, j, cod;
	char *palavra, *aux;
	Huffman H;
	Tree *T;
	cod = 0;
	
	palavra=aux="";
	
	// fazer a contagem para espaco " "
	H = NewHuffman(cod, " ");
	T = CriaNo(cod, 0);
	cod++;
	for (i=0; frase[i] != '\0'; i++)
		if (frase[i] == ' ')
			T->freq += 1;
	
	Enqueue(&*L, H);
	InserirArvore(&*F, T);
	
	i=0;
	palavra="";
	palavra=getPalavra(frase, i);
	while (strlen(palavra) > 0) {
		i += strlen(palavra)+1; // add o tamanho da palavra +1 (espaco)
		
		// busca a palavra na Lista, se nao achar faz a contagem e insere na Lista
		if (!buscaPalavra(palavra, *L)) {
			// Cria o item do tipo Huffman
			H = NewHuffman(cod, palavra);
			// Cria o no de arvore ja com frequencia 1 (foi lido 1 vez em palavra)
			T = CriaNo(cod, 1);
			cod++;
			
			// inicia a busca no restante da frase por palavras iguais a da variavel palavra, a partir da proxima palavra
			j = i;
			aux = getPalavra(frase, j);
			while (strlen(aux) > 0) {
				j += strlen(aux) + 1; // add o tamanho da palavra em aux +1 (espaco)
				
				// verifica se sao iguais, pela funcao comparando caractere por caractere
				if (iguais(palavra, aux))
					T->freq += 1;
				
				aux = getPalavra(frase, j);
			}
			
			Enqueue(&*L, H);
			InserirArvore(&*F, T);
		}
		// reinicia var palavra, busca a proxima palavra da frase
		palavra="";
		palavra=getPalavra(frase, i);
	}
}

void inserirHuffman (int cod, Lista *L, int codigo[]) {
	int i;
	// busca o registro na Lista
	while (L != NULL  && L->registro.codigo != cod)
		L = L->prox;
	
	if (L != NULL) {
		// insere o codigo Huffman no registro
		for (i=0; codigo[i] != -1; i++)
			L->registro.codigoHuffman[i] = codigo[i];
		
		// marcar o fim do codigo
		L->registro.codigoHuffman[i] = -1;
	}
}

void gerarHuffman (Tree *T, Lista *L, int codigo[], int pos) {
	if (T != NULL) {
		codigo[pos] = 0; // 0 - andou pra esquerda
		pos++;
		// andar para esquerda
		gerarHuffman(T->esq, L, codigo, pos);
		pos--;
		codigo[pos] = -1;
		// insere o codigo gerado na Lista
		inserirHuffman(T->codigo, L, codigo);
		
		codigo[pos] = 1; // 1 - andou pra direita
		pos++;
		// andar para direita
		gerarHuffman(T->dir, L, codigo, pos);
		pos--;
		codigo[pos] = -1;
		// insere o codigo gerado na Lista
		inserirHuffman(T->codigo, L, codigo);
	}
}

void gerarBin (Lista *L) {
	Huffman H;
	FILE *ptr;
	int i;
	MolduraSuperior();
	i=4;
	ptr = fopen("tabela.dat", "wb");
	while (L != NULL) {
		H = L->registro;
		fwrite(&H, sizeof(Huffman), 1, ptr);
		MolduraItem(i, H);
		i++;
		L = L->prox;
	}
	fclose(ptr);
	MolduraInferior(i);
}

void zerarVetor(int frase[], int i) {
	int j;
	for (j=i; j<TF; j++)
		frase[j] = 0;
}

void fraseBin (Lista *L) {
	char *frase, *palavra;
	Huffman H;
	int i, j, k, l, codigoFrase[TF];
	union byte Byte, bAux;
	FILE *ptr;
	
	// frase = "tentar errar cair seguir aprender e levantar e um caminho para vencer\0";
	frase = "cair levantar e seguir tentar errar e aprender levaremos tempo para vencer\0";
	k=0; j=0; i=0;
	
	// pega a palavra da frase
	palavra = "";
	palavra = getPalavra(frase, j);
	while (strlen(palavra) > 0) {
		// define j para o inicio da proxima palavra da frase
		j += strlen(palavra)+1;
		
		// define para H o item Huffman que esta na lista
		H = getItem(L, palavra);
		
		// insere em codigoFrase o codigo de Huffman da palavra
		for(k=0; H.codigoHuffman[k] != -1 && i<TF; k++, i++)
			codigoFrase[i] = H.codigoHuffman[k];
		
		// insere um espaco
		codigoFrase[i] = 0;
		i++;
		// pega a proxima palavra da frase
		palavra = getPalavra(frase, j);
	}
	zerarVetor(codigoFrase, i);
	
	// salvar a frase no arquivo 'frase.dat'
	ptr = fopen("frase.dat", "wb+");
	// grava a quantidade de Bytes armazenados da frase
	fwrite(&i, sizeof(int), 1, ptr);
	// gravacao da frase
	for (j=0; j < TF; j+=8) {
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
	
	// reinicia o ponteiro para ler
	rewind(ptr);
	i=8;
	// le a quantidade de Bytes existentes
	fread(&j, sizeof(int), 1, ptr);
	fread(&Byte.num, sizeof(char), 1, ptr);
	
	// exibe a frase normal, definida anteriormente
	printf("\n\nFrase original: '%s'\n", frase);
	printf("Frase codificada: ");
	
	// exibe a frase codificada
	while(!feof(ptr) && i<j) {
		printf("%d", Byte.bi.b0);
		printf("%d", Byte.bi.b1);
		printf("%d", Byte.bi.b2);
		printf("%d", Byte.bi.b3);
		printf("%d", Byte.bi.b4);
		printf("%d", Byte.bi.b5);
		printf("%d", Byte.bi.b6);
		printf("%d ", Byte.bi.b7);
		i+=8;
		fread(&Byte.num, sizeof(char), 1, ptr);
	}
	fclose(ptr);
	printf("\n");
}

void executar () {
	char *frase;
	Lista *L;
	Floresta *F;
	int codigo[40];
	
	L = NULL;
	F = NULL;
	frase="buscar e tentar tentar e aprender aprender e errar errar e acertar acertar e buscar cada um tem um caminho cada um faz o caminho cada um escolhe o caminho levaremos um tempo para cair levaremos um tempo para levantar levaremos um tempo para seguir levaremos um tempo para mudar levaremos um tempo para vencer";
	
	// monta a lista sem codigo Huffman, e faz a base da floresta, com todas raiz folha
	contagem(&L, &F, frase);
	// monta a arvore a partir da floresta base
	MontarArvore(&F);
	// add os codigos Huffman para cada registro da lista, percorrendo a arvore
	gerarHuffman(F->raiz, L, codigo, 0);
	// cria o arquivo binario da lista (codigo, codigo Huffman, palavra)
	gerarBin(L);
	// exibe a arvore na horizontal
	ExibeH(F->raiz);
	// cria o arquivo com a frase codificada com os codigos Huffman
	fraseBin(L);
}

int main () {
	executar();
	return 0;
}
