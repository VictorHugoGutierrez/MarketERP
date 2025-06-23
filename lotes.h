#ifndef LOTES_H
#define LOTES_H

typedef enum {
    ENTRADA = 1,
    SAIDA = 2
} TipoMovimentacao;

typedef struct {
    int id;
    int produtoId;
    int quantidade;
    int diaValidade, mesValidade, anoValidade;
    int diaMovimentacao, mesMovimentacao, anoMovimentacao;
    TipoMovimentacao tipo;
    int loteOrigemId;
} LoteProduto;

void mainLote();
int obterEstoqueProduto(int produtoId);
int verificarValidade(int dia, int mes, int ano);
const char* tipoParaTexto(TipoMovimentacao tipo);
int obterQuantidadeDisponivelLote(int idLoteEntrada);

#endif
