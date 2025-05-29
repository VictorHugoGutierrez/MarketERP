#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define ARQUIVO "estoque.dat"

typedef enum{
    ENTRADA,
    SAIDA
} TipoMovimentacao;

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int codigo;
    int codigoProduto;
    int quantidade;
    TipoMovimentacao tipoMovimentacao;
    Data dataMovimentacao;
} Estoque;

int obterProximoCodigo(){
    FILE *f = fopen(ARQUIVO, "rb");
    Estoque estoque;
    int id = 0;

    if(f == NULL) return 1;

    while(fread(&estoque, sizeof(Estoque), 1, f)){
        if(estoque.codigo > id){
            id = estoque.codigo;
        }
    }

    fclose(f);
    return id + 1;
}

void registraEntradaSaida(Estoque estoque){
    FILE *f = fopen(ARQUIVO, "ab");
    fwrite(&estoque, sizeof(Estoque), 1, f);
    fclose(f);
}


void mainEstoque(){
    setlocale(LC_ALL, "Portuguese");

    int opcao, codigo;
    Estoque estoque;

    do{
        printf("\n=== MARKET ERP ===\n");
        printf("1. Registrar Entrada \n");
        printf("2. Registrar Saída  \n");
        printf("0. Sair \n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao){
            case 1:
                system("cls");
                estoque.codigo = obterProximoCodigo();
                printf("\nCódigo: %d\n", estoque.codigo);
                estoque.tipoMovimentacao = 0;
                printf("\nTipo Movimentação: %s\n", estoque.tipoMovimentacao);
                printf("\nCódigo do Produto: ");
                scanf("%d", &estoque.codigoProduto);
                printf("\nQuantidade: ");
                scanf("%d", &estoque.quantidade);
                printf("Data de Entrada (dd/mm/aaaa): ");
                scanf("%02d/%02d/%d", &estoque.dataMovimentacao.dia, &estoque.dataMovimentacao.mes, &estoque.dataMovimentacao.ano);
                registraEntradaSaida(estoque);
                system("pause");
                system("cls");
                break;
            case 2:
                system("cls");
                estoque.codigo = obterProximoCodigo();
                printf("\nCódigo: %d\n", estoque.codigo);
                estoque.tipoMovimentacao = 1;
                printf("\nTipo Movimentação: %s\n", estoque.tipoMovimentacao);
                printf("\nCódigo do Produto: ");
                scanf("%d", &estoque.codigoProduto);
                printf("\nQuantidade: ");
                scanf("%d", &estoque.quantidade);
                printf("Data de Saída (dd/mm/aaaa): ");
                scanf("%02d/%02d/%d", &estoque.dataMovimentacao.dia, &estoque.dataMovimentacao.mes, &estoque.dataMovimentacao.ano);
                registraEntradaSaida(estoque);
                system("pause");
                system("cls");
                break;
            case 0:
                printf("Saindo...\n");
                system("pause");
                system("cls");
                break;
            default:
                printf("Opção inválida!\n");
                system("pause");
                system("cls");
        }

    } while(opcao != 0);

    return;
}
