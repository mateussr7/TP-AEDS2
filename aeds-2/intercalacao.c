#include "intercalacao.h"
#include "arvore_binaria.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HIGH_VALUE INT_MAX
#define POSICAO -1


void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes) {

    int fim = 0; //variavel que controla fim do procedimento
    FILE *out; //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL) {
        printf("Erro ao abrir arquivo de sa?da\n");
    } else {
        //cria vetor de particoes
        TVet v[num_p];

        //abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
        //e primeiro funcionario do arquivo no campo func do vetor
        for (int i=0; i < num_p; i++) {
            v[i].f = fopen(nome_particoes->nome, "rb");
            v[i].aux_p = 0;
            if (v[i].f != NULL) {
                fseek(v[i].f, v[i].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[i].f);
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posi??o do vetor
                    v[i].func = funcionario(INT_MAX, "","","",0);
                }
                else {
                    //conseguiu ler funcionario, coloca na posi??o atual do vetor
                    v[i].func = f;
                }
            }
            else {
                fim = 1;
            }
            nome_particoes = nome_particoes->prox;
        }

        int aux = 0;
        while (!(fim)) { //conseguiu abrir todos os arquivos
            int menor = INT_MAX;
            int pos_menor;
            //encontra o funcionario com menor chave no vetor
            for(int i = 0; i < num_p; i++){
                if(v[i].func->cod < menor){
                    menor = v[i].func->cod;
                    pos_menor = i;
                }
            }
            if (menor == INT_MAX) {
                fim = 1; //terminou processamento
            }
            else {
                //salva funcionario no arquivo de saída
                fseek(out, aux * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[pos_menor].func, out);
                //atualiza posição pos_menor do vetor com pr?ximo funcionario do arquivo
                v[pos_menor].aux_p++;
                fseek(v[pos_menor].f, v[pos_menor].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[pos_menor].f);
                aux++;
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posiçao do vetor
                    v[pos_menor].func = funcionario(INT_MAX, "", "", "",0.0);
                }
                else {
                    v[pos_menor].func = f;
                }

            }
        }

        //fecha arquivos das partiÇões de entrada
        for(int i = 0; i < num_p; i++){
            fclose(v[i].f);
        //    free(v[i].func);
        }
        //fecha arquivo de saída
        fclose(out);
    }
}

void intercalacao_arvore_de_vencedores(TPilha **pilha, int *vetTop, char *nome_arquivo_saida, int num_p){
    //implementar segunda parte do trabalho

    TFunc *funHV = funcionario(INT_MAX,"MAXIMO","","",0);
    TPilha *pilhaHV = (TPilha*)malloc(sizeof(TPilha));
    int valor[1];
    int *vetTopInicial = (int*) malloc(sizeof(int)*num_p);
    //memcpy(vetTopInicial,vetTop,sizeof(vetTop)*num_p);
        for (int i = 0; i<num_p; i++){
            vetTopInicial[i] = vetTop[i];
        }

    //TPilha **pilha = pilha1;

    pilhaHV->info = funHV;
    pilhaHV->p = valor;


    //TNoV folhas[num_p];
    int fim = 0; //variavel que controla fim do procedimento
    FILE *out; //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL) {
        printf("Erro ao abrir arquivo de sa?da\n");
    } else {
        int cont;
        //TNoV *aux = (TNoV*)malloc(sizeof(TNoV));
        int quantiaFile = 0;
        TNoV **folhas = (TNoV**)malloc(sizeof(TNoV*)*num_p);
            TNoV **folhas1 = (TNoV**)malloc(sizeof(TNoV*)*num_p);
        while (1){

            cont = 0;
            for(int i=0; i<num_p; i++){
                if(vetTop[i] > -1){
                    folhas[i] = criaNo_arvore_binaria_vencedores(pilha[i], &vetTop[i]);
                    folhas[i]->vencedor = pilha[i][vetTop[i]].info;
                    folhas1[i] = criaNo_arvore_binaria_vencedores(pilha[i], &vetTop[i]);
                    folhas1[i]->vencedor = pilha[i][vetTop[i]].info;
                    cont = 1;
                }else{
                    folhas[i] = criaNo_arvore_binaria_vencedores(pilhaHV,valor);
                    folhas[i]->vencedor = funHV;
                    folhas1[i] = criaNo_arvore_binaria_vencedores(pilhaHV,valor);
                    folhas1[i]->vencedor = funHV;
                }

            }
            if (cont == 0){
                break;
            }
            int j=0;
            int verifica = 1;
            while((folhas[j] != NULL) && verifica == 1){
                //cria no pai
                TNoV *aux = (TNoV*)malloc(sizeof(TNoV));

                if(folhas[j+1] != NULL){
                    if(folhas[j]->vencedor->cod < folhas[j+1]->vencedor->cod){
                        aux = folhas[j];
                    }
                    else{
                        aux = folhas[j+1];
                    }

                    //adiciona propiedade do pai da folha[j] e da folha[j+1]

                    aux->dir = folhas1[j+1];
                    aux->esq = folhas1[j];
                    aux->endVencedor = aux;
                    folhas1[j+1]->pai= aux;
                    folhas1[j]->pai= aux;

                    //remove as duas folhas da lista e coloca o no pai nela

                    folhas[j+1] = NULL;
                    folhas[j] = NULL;
                    folhas[j/2] = aux;
                }else{
                    aux = folhas[j];
                    folhas[j] = NULL;
                    folhas[j/2] = aux;
                    if (j==0){
                        verifica = 0;
                    }
                }

                j = j+2;
                if(folhas[j] == NULL){
                    j=0;
                }

            }

            fseek(out, quantiaFile * tamanho_registro(), SEEK_SET);
            salva_funcionario(folhas[0]->vencedor,out);
            quantiaFile++;
            (*folhas[0]->tamanhoPilha)--;
            for(int i=0;i<num_p;i++){
                folhas1[i]=NULL;
                folhas[i]=NULL;
            }

        }
        for(int i=0;i<num_p;i++){
                free(folhas1[i]);
                free(folhas[i]);
            }

        for (int i = 0; i<num_p; i++){
            vetTop[i] = vetTopInicial[i];
        }
        free(vetTopInicial);

    }

    printf("\nEBA");
}
