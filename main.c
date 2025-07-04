#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "produto.h"
#include "relatorios.h"
#include "lotes.h"

int main(){
    setlocale(LC_ALL, "Portuguese");
    system("chcp 1252 > null");

    int opcao;

    do{
        printf("\n=== MARKET ERP ===\n");
        printf("1. Produtos \n");
        printf("2. Controle de Lote \n");
        printf("3. Relat�rios / Estoque \n");
        printf("0. Sair \n");
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);

        switch (opcao){
            case 1:
                system("cls");
                mainProduto();
                break;
            case 2:
                system("cls");
                mainLote();
                break;
            case 3:
                system("cls");
                mainRelatorios();
                break;
            case 0:
                system("cls");
                printf("Saindo...\n");
                break;
            default:
                printf("Op��o inv�lida!\n");
                system("pause");
                system("cls");
        }

    }while (opcao != 0);

    return 0;
}
