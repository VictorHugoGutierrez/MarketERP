#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "produto.h"

int main(){
    setlocale(LC_ALL, "Portuguese");

    int opcao;

    do{
        printf("\n=== MARKET ERP ===\n");
        printf("1. Produtos \n");
        printf("2. Controle de Estoque \n");
        printf("3. Controle de Validade \n");
        printf("4. Relatórios \n");
        printf("0. Sair \n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao){
            case 1:
                system("cls");
                mainProduto();
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
                system("pause");
                system("cls");
        }

    }while (opcao != 0);

    return 0;
}
