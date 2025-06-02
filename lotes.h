#ifndef LOTES_H
#define LOTES_H

typedef struct {
    int id;
    int produtoId;
    int quantidade;
    int dia, mes, ano;
} LoteProduto;

void mainLote();
int obterEstoqueProduto(int produtoId);
int verificarValidade(int dia, int mes, int ano);

#endif
