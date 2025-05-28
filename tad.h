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
	int freq, codigo;
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
