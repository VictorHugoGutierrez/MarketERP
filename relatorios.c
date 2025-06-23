#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "lotes.h"
#include "produto.h"
#include "estoque.h"

typedef struct {
    int produtoId;
    int quantidadeSaida;
} SaidaProduto;

void relatorioEstoqueGeral() {
    FILE *f = fopen("produto.dat", "rb");
    if (!f) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    Produto produto;
    printf("\n=== ESTOQUE GERAL ===\n");
    while (fread(&produto, sizeof(Produto), 1, f)) {
        if (produto.id != 0) {
            int estoque = obterEstoqueProduto(produto.id);
            printf("Produto: %s\n", produto.nome);
            printf("Estoque disponível: %d\n", estoque);
            printf("-------------------------\n");
        }
    }
    fclose(f);
}

void relatorioEstoqueCritico(int limiteMinimo) {
    FILE *f = fopen("produto.dat", "rb");
    if (!f) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    Produto produto;
    printf("\n=== ESTOQUE CRÍTICO (abaixo de %d) ===\n", limiteMinimo);
    while (fread(&produto, sizeof(Produto), 1, f)) {
        if (produto.id != 0) {
            int estoque = obterEstoqueProduto(produto.id);
            if (estoque < limiteMinimo) {
                printf("Produto: %s\n", produto.nome);
                printf("Estoque: %d\n", estoque);
                printf("-------------------------\n");
            }
        }
    }
    fclose(f);
}

void relatorioLotesVencidos() {
    FILE *f = fopen("lotes.dat", "rb");
    if (!f) {
        printf("Nenhum lote cadastrado.\n");
        return;
    }

    LoteProduto lote;
    printf("\n=== LOTES VENCIDOS ===\n");
    while (fread(&lote, sizeof(LoteProduto), 1, f)) {
        if (lote.tipo == ENTRADA && !verificarValidade(lote.diaValidade, lote.mesValidade, lote.anoValidade)) {
            Produto p = buscarProdutoId(lote.produtoId, 0);
            printf("Produto: %s\n", p.nome);
            printf("Lote ID: %d\n", lote.id);
            printf("Validade: %02d/%02d/%04d\n", lote.diaValidade, lote.mesValidade, lote.anoValidade);
            printf("-------------------------\n");
        }
    }
    fclose(f);
}

void relatorioMovimentacoesProduto(int produtoId) {
    FILE *f = fopen("lotes.dat", "rb");
    if (!f) {
        printf("Nenhuma movimentação encontrada.\n");
        return;
    }

    Produto prod = buscarProdutoId(produtoId, 0);
    printf("\n=== MOVIMENTAÇÕES DO PRODUTO: %s ===\n", prod.nome);

    LoteProduto lote;
    while (fread(&lote, sizeof(LoteProduto), 1, f)) {
        if (lote.produtoId == produtoId) {
            printf("%s - %d unidades em %02d/%02d/%04d\n",
                tipoParaTexto(lote.tipo), lote.quantidade, lote.diaMovimentacao, lote.mesMovimentacao, lote.anoMovimentacao);
        }
    }

    fclose(f);
}

void relatorioDetalhadoLotesProduto(int produtoId) {
    FILE *f = fopen("lotes.dat", "rb");
    if (!f) {
        printf("Arquivo de lotes não encontrado.\n");
        return;
    }

    Produto p = buscarProdutoId(produtoId, 0);
    printf("\n=== LOTES DO PRODUTO: %s ===\n", p.nome);

    LoteProduto lote;
    while (fread(&lote, sizeof(LoteProduto), 1, f)) {
        if (lote.produtoId == produtoId && lote.tipo == ENTRADA) {
            int disponivel = obterQuantidadeDisponivelLote(lote.id);
            printf("Lote ID: %d\n", lote.id);
            printf("Validade: %02d/%02d/%04d\n", lote.diaValidade, lote.mesValidade, lote.anoValidade);
            printf("Total Entrada: %d | Disponível: %d\n", lote.quantidade, disponivel);
            printf("-------------------------\n");
        }
    }
    fclose(f);
}

int obterEstoqueProduto(int produtoId) {
    FILE *f = fopen("lotes.dat", "rb");
    if (!f) return 0;

    LoteProduto lote;
    int totalEntrada = 0;
    int totalSaida = 0;

    while (fread(&lote, sizeof(LoteProduto), 1, f)) {
        if (lote.produtoId == produtoId && verificarValidade(lote.diaValidade, lote.mesValidade, lote.anoValidade)) {
            if (lote.tipo == ENTRADA) {
                totalEntrada += lote.quantidade;
            } else if (lote.tipo == SAIDA) {
                totalSaida += lote.quantidade;
            }
        }
    }

    fclose(f);
    int estoque = totalEntrada - totalSaida;
    if (estoque < 0) estoque = 0;
    return estoque;
}

void obterDataAtual(int *dia, int *mes, int *ano) {
    time_t t = time(NULL);
    struct tm *dataAtual = localtime(&t);
    *dia = dataAtual->tm_mday;
    *mes = dataAtual->tm_mon + 1;
    *ano = dataAtual->tm_year + 1900;
}

int diasParaVencimento(int dia, int mes, int ano) {
    struct tm dataAtual = {0}, dataValidade = {0};
    time_t tAtual, tValidade;
    double diffSegundos;

    int diaAtual, mesAtual, anoAtual;
    obterDataAtual(&diaAtual, &mesAtual, &anoAtual);

    dataAtual.tm_mday = diaAtual;
    dataAtual.tm_mon = mesAtual - 1;
    dataAtual.tm_year = anoAtual - 1900;

    dataValidade.tm_mday = dia;
    dataValidade.tm_mon = mes - 1;
    dataValidade.tm_year = ano - 1900;

    tAtual = mktime(&dataAtual);
    tValidade = mktime(&dataValidade);

    diffSegundos = difftime(tValidade, tAtual);
    return (int)(diffSegundos / (60 * 60 * 24)); // converte segundos em dias
}

void relatorioLotesProximosVencer(int limiteDias) {
    FILE *f = fopen("lotes.dat", "rb");
    if (!f) {
        printf("Nenhum lote cadastrado.\n");
        return;
    }

    LoteProduto lote;
    printf("\n=== LOTES PRÓXIMOS AO VENCIMENTO (até %d dias) ===\n", limiteDias);

    while (fread(&lote, sizeof(LoteProduto), 1, f)) {
        if (lote.tipo == ENTRADA) {
            int diasRestantes = diasParaVencimento(lote.diaValidade, lote.mesValidade, lote.anoValidade);
            if (diasRestantes >= 0 && diasRestantes <= limiteDias) {
                Produto p = buscarProdutoId(lote.produtoId, 0);
                printf("Produto: %s\n", p.nome);
                printf("Lote ID: %d\n", lote.id);
                printf("Validade: %02d/%02d/%04d (em %d dias)\n", lote.diaValidade, lote.mesValidade, lote.anoValidade, diasRestantes);
                printf("-------------------------\n");
            }
        }
    }

    fclose(f);
}

void relatorioValorTotalEstoque() {
    FILE *f = fopen("produto.dat", "rb");
    if (!f) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    Produto produto;
    printf("\n=== VALOR TOTAL DO ESTOQUE POR PRODUTO ===\n");

    while (fread(&produto, sizeof(Produto), 1, f)) {
        if (produto.id != 0) {
            int estoque = obterEstoqueProduto(produto.id);
            float valorTotal = estoque * produto.preco;
            printf("Produto: %s\n", produto.nome);
            printf("Estoque disponível: %d\n", estoque);
            printf("Preço unitário: R$ %.2f\n", produto.preco);
            printf("Valor total em estoque: R$ %.2f\n", valorTotal);
            printf("-------------------------\n");
        }
    }

    fclose(f);
}

void mainRelatorios() {
    setlocale(LC_ALL, "Portuguese");
    system("chcp 1252 > null");

    int opcao, id, minimo, limiteDias;

    do {
        printf("\n=== Relatórios ===\n");
        printf("1. Estoque geral\n");
        printf("2. Estoque crítico\n");
        printf("3. Lotes vencidos\n");
        printf("4. Movimentações de um produto\n");
        printf("5. Lotes detalhados de um produto\n");
        printf("6. Lotes próximos ao vencimento\n");
        printf("7. Valor total do estoque por produto\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                system("cls");
                relatorioEstoqueGeral();
                break;
            case 2:
                system("cls");
                printf("Digite o limite mínimo: ");
                scanf("%d", &minimo);
                relatorioEstoqueCritico(minimo);
                break;
            case 3:
                system("cls");
                relatorioLotesVencidos();
                break;
            case 4:
                system("cls");
                listarProdutos();
                printf("Digite o ID do produto: ");
                scanf("%d", &id);
                relatorioMovimentacoesProduto(id);
                break;
            case 5:
                system("cls");
                listarProdutos();
                printf("Digite o ID do produto: ");
                scanf("%d", &id);
                relatorioDetalhadoLotesProduto(id);
                break;
            case 6:
                system("cls");
                printf("Digite o limite de dias para aviso: ");
                scanf("%d", &limiteDias);
                relatorioLotesProximosVencer(limiteDias);
                break;
            case 7:
                system("cls");
                relatorioValorTotalEstoque();
                break;
            case 0:
                system("cls");
                printf("\nSaindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

        system("pause");
        system("cls");
    } while (opcao != 0);
}
