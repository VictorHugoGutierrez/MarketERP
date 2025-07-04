#ifndef PRODUTO_H
#define PRODUTO_H

typedef struct {
    int id;
    int codigo;
    char nome[100];
    float preco;
} Produto;

void mainProduto();
void listarProdutos();
Produto buscarProdutoId(int id, int exibe);
Produto buscarProdutoCodigo(int id, int exibe);

#endif
