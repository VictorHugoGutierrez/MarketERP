#ifndef LOTES_H
#define LOTES_H

typedef struct {
    int id;
    int codigo;
    char nome[100];
    float preco;
} Produto;

void mainProduto();
void listarProdutos();
Produto buscarProdutoId(int id);

#endif
