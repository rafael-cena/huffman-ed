#include <stdio.h>
#include <conio2.h>
#include <string.h>
#include <stdlib.h>

int n = 0;
#include "tad.h"

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
	Lista *L = lista;
	
	Ptr = fopen("tabela.dat", "wb");
	while (L != NULL) {
		item = L->registro;
		fwrite(&item, sizeof(Huffman), 1, Ptr);
		printf("%d\t|%s \t|%d\n", item.codigo, item.palavra, item.codigoHuffman);
		L = L->prox;
	}
	fclose(Ptr);
}

/*
void exibeh (Tree A) {
	static int x = -1;
	int i;
	if (A != NULL) {
		x++;
		exibeh (A->dir);
		for (i=0; i<5*x; i++) printf(" ");
		printf("(%d, %d) \n",A->cod,A->freq);
		exibeh (A->esq);
		n--;
	}
}
*/

int tamFloresta (Floresta *F) {
	int i;
	Floresta *aux = F;
	i=0;
	while (aux != NULL) {
		i++; aux=aux->prox;
	}
	return i;
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
		printf("floresta: %d\n", tamFloresta(*F));
	}
	printf("\nArvore pronta\n");
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
	printf("floresta: %d\n", tamFloresta(floresta));
	montarArvore(&floresta);
	gerarBin(lista);
	exibeL(lista);
}

int main (void) {
	Executar();
	return 0;
}
