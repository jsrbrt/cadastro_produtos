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
int posicao, opcao, codigo, itemCorreto;

void abrirArquivo(){
    pCadastro = fopen("cadastros.txt", "r+b");
    if (pCadastro == NULL) pCadastro = fopen("cadastros.txt", "w+b");
}

int fazerNovamente(char nomeOp[10]){
    printf("\n\nDeseja %s outro item (1- sim / 0- nao)? ", nomeOp);
    scanf("%d", &opcao);

    return opcao;
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

int descobrirPosicao(int cod){
    rewind(pCadastro);
    fread(&itens, sizeof(Tcadastro), 1, pCadastro);
    
    for(int p = 0; feof(pCadastro) == 0; p++){
        if (itens.codItem == cod) return p;

        fread(&itens, sizeof(Tcadastro), 1, pCadastro);
    }
    return -1;
}

void removerItem(){
    do{
        printf("\n========== REMOVER ITEM ==========n\n");

        printf("Insira o codigo do item: ");
        scanf("%d", &codigo);

        posicao = descobrirPosicao(codigo);
        itemCorreto = checarCerteza(posicao);

        if(itemCorreto == 1){
            itensNull.codItem = 0;
            itensNull.precoUnit = 0;
            itensNull.quantEstoque = 0;

            fseek(pCadastro, posicao *sizeof(Tcadastro), SEEK_SET);
            fwrite(&itensNull, sizeof(Tcadastro), 1, pCadastro);

            printf("Item removido com sucesso.");
        }
        opcao = fazerNovamente("remover");
    } while (opcao == 1);
}

void consultarItens(){
    do{
        printf("\nInsira o codigo do item: ");
        scanf("%d", &codigo);

        posicao = descobrirPosicao(codigo);

        if(posicao == -1) printf("Item nao encontrado.\n");
        else mostrarItem(posicao);

        opcao = fazerNovamente("consultar");
    } while(opcao == 1);
}

int checarCerteza(int pos){
    itemCorreto = 0;

    if (pos == -1){
        printf("\nCodigo de item invalido ou nao existe.\n");
    }
    else{
        mostrarItem(pos);
        printf("\nEste e o item correto (1- sim / 0- nao)? ");
        scanf("%d", &itemCorreto);
    }
    return itemCorreto;
}

void alterarItem(){
    do{
        printf("\n==========ALTERAR ITENS==========\n");

        printf("Insira o codigo do item: ");
        scanf("%d", &codigo);

        posicao = descobrirPosicao(codigo);
        itemCorreto = checarCerteza(posicao);

        if (itemCorreto == 1){
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
            
            fseek(pCadastro, posicao *sizeof(Tcadastro), SEEK_SET);
            fwrite(&itens, sizeof(Tcadastro), 1, pCadastro);

            mostrarItem(posicao);
        }
        opcao = fazerNovamente("alterar");
    } while(opcao == 1);  
}

void incluirItens(){
    int tempCod, aux;  
    do{
        do{
            printf("\n==========INCLUSAO DE ITENS==========\n\n");
            printf("CODIGO DO ITEM: ");
            scanf("%d", &tempCod);
            aux = impedirIgual(tempCod);

            if (aux == 0) itens.codItem = tempCod;
        } while(aux == 1);
        
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
        opcao = fazerNovamente("cadastrar");
    } while(opcao == 1);
}

void cadastrarItem(){
    int pagina;    
    do{
        printf("\n==========CADASTRO DE ITENS==========\n");
        printf("\n1- INCLUIR \n2- ALTERAR \n3- CONSULTAR \n4- EXCLUIR \n0- RETORNAR\n");
        printf("\nOpcao: ");
        scanf("%d", &pagina);

        switch(pagina){
            case 1: incluirItens();   break; 
            case 2: alterarItem();    break;
            case 3: consultarItens(); break;
            case 4: removerItem();    break;
            case 0: break;

            default: printf("Opcao invalida!"); break;
        }
    } while (pagina != 0);
}

int impedirIgual(int cod){
    rewind(pCadastro);
    fread(&itens, sizeof(Tcadastro), 1, pCadastro);

    while(feof(pCadastro) == 0){
        if (itens.codItem == cod){
            printf("\nEsta codigo já existe!");
            mostrarItem(descobrirPosicao(cod));

            return 1;
        }
        fread(&itens, sizeof(Tcadastro), 1, pCadastro);
    }
    return 0;
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
    } while (pag != 0);
}