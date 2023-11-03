#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

typedef struct{
    char nome[50], unidade[2];
    int quantEstoque, codItem;
    float precoUnit;
} Tcadastro;

FILE *pCadastro;
Tcadastro itens, itensNull;

void abrirArquivo(){
    pCadastro = fopen("cadastros.txt", "r+b");
    if (pCadastro == NULL) pCadastro = fopen("cadastros.txt", "w+b");
}

int fazerNovamente(char nomeOp[10]){
    int opc;
    printf("\n\nDeseja %s outro item (1- sim / 0- nao)? ", nomeOp);
    scanf("%d", &opc);

    return opc;
}

void mostrarItem(int pos){
    fseek(pCadastro, pos*sizeof(Tcadastro), SEEK_SET);
    fread(&itens, sizeof(Tcadastro), 1, pCadastro);

    printf("\n========================================");
    printf("\nCodigo do Item.......: %d", itens.codItem);
    printf("\nNome do Item.........: %s", itens.nome);
    printf("\nPreco do Item........: R$%.2f", itens.precoUnit);
    printf("\nQuantidade em estoque: %d", itens.quantEstoque);
    printf("\nUnidade..............: %s", itens.unidade);
    printf("\n========================================");
}

int descobrirPosicao(int codigoItem){
    rewind(pCadastro);
    fread(&itens, sizeof(Tcadastro), 1, pCadastro);
    
    for(int pos = 0; feof(pCadastro) == 0; pos++){
        if (itens.codItem == codigoItem) return pos;

        fread(&itens, sizeof(Tcadastro), 1, pCadastro);
    }
    return -1;
}

void removerItem(){

}

void consultarItens(){
    int opc, pos, cod;
    do{
        printf("\nInsira o codigo do item: ");
        scanf("%d", &cod);

        pos = descobrirPosicao(cod);

        if(pos == -1) printf("Item nao encontrado.\n");
        else mostrarItem(pos);

        opc = fazerNovamente("consultar");
    } while(opc == 1);
}

void alterarItem(){
    int pos, opc, itemCorr = 0, cod;
    do{
        printf("\n==========ALTERAR ITENS==========\n");

        printf("\nInsira o codigo do item: ");
        scanf("%d", &cod);
        pos = descobrirPosicao(cod);

        if (pos == -1)
            printf("Codigo de item invalido ou nao existe.\n");
        else{
            mostrarItem(pos);
            printf("\nEste e o item correto (1- sim / 0- nao)? ");
            scanf("%d", &itemCorr);
        }
        if (itemCorr == 1){
            printf("\nAlterando item...\n\n");

            printf("NOME..........: ");
            fflush(stdin);
            gets(itens.nome);
            printf("PRECO.........: ");
            scanf("%f", &itens.precoUnit);
            printf("UNIDADE.......: ");
            fflush(stdin);
            gets(itens.unidade);
            printf("QUANTIDADE....: ");
            scanf("%d", &itens.quantEstoque);

            printf("\nItem alterado com sucesso!");
            
            fseek(pCadastro, pos*sizeof(Tcadastro), SEEK_SET);
            fwrite(&itens, sizeof(Tcadastro), 1, pCadastro);

            mostrarItem(pos);
        }
        opc = fazerNovamente("alterar");
    } while(opc == 1);  
}

void incluirItens(){
    int opc;
    do{
        printf("\n==========INCLUSAO DE ITENS==========\n\n");

        printf("CODIGO DO ITEM: ");                
        scanf("%d", &itens.codItem);
        printf("NOME..........: ");
        fflush(stdin);
        gets(itens.nome);
        printf("PRECO.........: ");
        scanf("%f", &itens.precoUnit);
        printf("UNIDADE.......: ");
        fflush(stdin);
        gets(itens.unidade);
        printf("QUANTIDADE....: ");
        scanf("%d", &itens.quantEstoque);

        fseek(pCadastro, 0, SEEK_END);
        fwrite(&itens, sizeof(Tcadastro), 1, pCadastro);

        printf("\nItem cadastrado com sucesso!\n");
        opc = fazerNovamente("cadastrar");
    } while(opc == 1);
}
void cadastrarItem(){
    int opc;
    
    do{
        printf("\n==========CADASTRO DE ITENS==========\n");
        printf("\n1- INCLUIR \n2- ALTERAR \n3- CONSULTAR \n4- EXCLUIR \n0- RETORNAR\n");
        printf("\nOpcao: ");
        scanf("%d", &opc);

        switch(opc){
            case 1: incluirItens();   break; 
            case 2: alterarItem();    break;
            case 3: consultarItens(); break;
            case 4: removerItem();    break;
            case 0: break;

            default: printf("Opcao invalida!"); break;
        }
    } while (opc != 0);
}

int main(){
    int pag;
    abrirArquivo();
    
    do{
        printf("============= COMERCIO LTDA ============\n");
        printf("==== SISTEMA DE CONTROLE DE ESTOQUE ====\n");
        printf("\n1- CADASTRO DE ITENS \n2- MOVIMENTACAO \n3- REAJUSTE DE VALOR \n4- RELATORIO \n0- FINALIZAR\n");
        printf("\nOpcao: ");
        scanf("%d", &pag);

        switch(pag){
        case 1: cadastrarItem();   break;
        case 2:                    break;
        case 3:                    break;
        case 4:                    break;
        case 0: fclose(pCadastro); break;

        default: printf("Opcao invalida\n\n"); break;
        }
    }while (pag != 0);
}