//Codificado no Windows
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

typedef struct stCarta{
    short int num;
    char naipe;
    struct stCarta *prox;
} tCarta;

tCarta *primMesa[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
tCarta *primNaipe[4] = {NULL, NULL, NULL, NULL};
tCarta *temp[4] = {NULL, NULL, NULL, NULL};
tCarta *primMonte = NULL;
int totalMonte = 0;


void red() {
  printf("\033[1;31m");
}

void reset(){
    printf("\x1b[0m");
}

void gerarBaralho(){
    int naipe, num, i, pos;

    //Naipes
    for(naipe = 3; naipe <= 6; naipe++){
        //Numeros das cartas
        for(num = 1; num <= 13; num++){
            //Geração da carta
            tCarta *novo = (tCarta *)malloc(sizeof(tCarta));
            novo->num = num;
            novo->naipe = naipe;
            
            //Posicionamento da carta no monte
            pos = rand() % (totalMonte+1);
            if(pos == 0){
                novo->prox = primMonte;
                primMonte = novo;
            }else{
                tCarta * atual = primMonte;
                for(i=0; i < pos-1; i++){
                    atual = atual->prox;
                }
                novo->prox=atual->prox;
                atual->prox=novo;
            }
            totalMonte++;
        }
    }
}//end gerarBaralho

void distribuirMesa(){
    int mesa, i;
    tCarta *atual, *ant, *novo;

    for(mesa = 0; mesa < 4; mesa++){
        for(i = 0; i < 7; i++){
            //Passando a carta do monte p/ novo
            novo = (tCarta *)malloc(sizeof(tCarta));
            novo->naipe = primMonte->naipe;
            novo->num = primMonte->num;
            novo->prox = NULL;
            primMonte = primMonte->prox;

            //Encontrando a posição de inserção
            if(primMesa[mesa] != NULL){
                ant = NULL;
                atual = primMesa[mesa];
                while(atual != NULL){
                    ant = atual;
                    atual = atual->prox;
                }
                ant->prox = novo; 
            }else{
                primMesa[mesa] = (tCarta *)malloc(sizeof(tCarta));
                primMesa[mesa] = novo;
            }               
        }
    }
    for(mesa = 4; mesa < 8; mesa++){
        for(i = 0; i < 6; i++){
            //Passando a carta do monte p/ novo
            novo = (tCarta *)malloc(sizeof(tCarta));
            novo->naipe = primMonte->naipe;
            novo->num = primMonte->num;
            novo->prox = NULL;
            primMonte = primMonte->prox;

            //Encontrando a posição de inserção
            if(primMesa[mesa] != NULL){
                ant = NULL;
                atual = primMesa[mesa];
                while(atual != NULL){
                    ant = atual;
                    atual = atual->prox;
                }
                ant->prox = novo; 
            }else{
                primMesa[mesa] = (tCarta *)malloc(sizeof(tCarta));
                primMesa[mesa] = novo;
            }               
        }
    }
}//end distribuirMesa

void imprime(){
    int i;
    tCarta *atual;

    system("cls");
    //TEMP
    printf("[TEMP] = ");
    for(i = 0; i < 4; i++){
        if(temp[i] != NULL){
            printf("%d-", i);
            if(temp[i]->naipe <= 4) red();
            printf("[%2d/%c] ", temp[i]->num, temp[i]->naipe);
            reset();
        }else{
            printf("%d-[    ] ", i);
        }
    }
    printf("\n\n");

    //NAIPES
    for(i = 0; i < 4; i++){
        atual = primNaipe[i];
        printf("[NAIPE %d] = ", i);

        while(atual != NULL){
            if(atual->naipe <= 4) red();
            printf("[%2d/%c]", atual->num, atual->naipe);
            reset();
            atual = atual->prox;
        }
        printf("\n");
    }
    printf("\n\n");

    //MESAS
    for(i = 0; i < 8; i++){
        atual = primMesa[i];
        printf("[MESA %d] = ", i);

        while(atual != NULL){
            if(atual->naipe <= 4) red();
            printf("[%2d/%c]", atual->num, atual->naipe);
            reset();
            atual = atual->prox;
        }
        printf("\n");
    }
    printf("\n\n");
}//end imprime

void moveMesaNape(){
    int posMesa, posNaipe;
    tCarta *atualMesa, *antMesa, *atualNaipe, *antNaipe;

    printf("\n-- MESA => TEMP --\n\n");

    //Leitura das posições de Mesa e Naipe do movimento
    do{
        printf("Mesa: ");
        scanf("%d", &posMesa);
    }while((posMesa < 0) || (posMesa > 7));
    do{
        printf("Naipe: ");
        scanf("%d", &posNaipe);
    }while((posNaipe < 0) || (posNaipe > 3));

    //Encontrando o ultimo elemento da mesa indicada
    antMesa = NULL;
    atualMesa = primMesa[posMesa];
    while(atualMesa->prox != NULL){
        antMesa = atualMesa;
        atualMesa = atualMesa->prox;
    }
    
    //Verificação de mesa vazia
    if(atualMesa != NULL){
        //Verificação de pilha de naipes vazia
        if(primNaipe[posNaipe] == NULL){
            //Se a pilha de naipes estiver vazia, apenas um Ás pode ser inserido
            if(atualMesa->num == 1){
                primNaipe[posNaipe] = (tCarta *)malloc(sizeof(tCarta));
                primNaipe[posNaipe] = atualMesa;
                antMesa->prox = NULL;
            }else{
                red();
                printf("A primeira carta deve ser um As (1)...");
                reset();
                getchar();
                getchar();
            }
        }else{
            //Encontrando o ultimo elemento da pilha de naipes indicada
            antNaipe = NULL;
            atualNaipe = primNaipe[posNaipe];
            while(atualNaipe->prox != NULL){
                antNaipe = atualMesa;
                atualNaipe = atualNaipe->prox;
            }

            //Validação da sequência de inserção na pilha
            if((atualMesa->num-1) == atualNaipe->num){
                //Validação do naipe da pilha
                if(atualMesa->naipe == atualNaipe->naipe){
                    atualNaipe->prox = atualMesa;
                    antMesa->prox = NULL;
                }else{
                    red();
                    printf("A carta da mesa deve possuir o mesmo naipe da pilha de naipes...");
                    reset();
                    getchar();
                    getchar();
                }
            }else{
                red();
                printf("A carta da mesa deve ser a subsequente posterior da pilha de naipes...");
                reset();
                getchar();
                getchar();
            }
        }
    }else{
        red();
        printf("Mesa vazia...");
        reset();
        getchar();
        getchar();
    }
}//end moveMesaNape

void moveMesaTemp(){
    int posTemp, posMesa;
    bool livre = false;
    tCarta *atual, *ant;

    printf("\n-- MESA => TEMP --\n\n");

    //Varredura TEMP
    for(posTemp = 0; posTemp < 4; posTemp++){
        if(temp[posTemp] == NULL){
            livre = true;
            break;
        }
    }

    if(livre){
        do{
            printf("Mesa: ");
            scanf("%d", &posMesa);
        }while((posMesa < 0) || (posMesa > 7));

        ant = NULL;
        atual = primMesa[posMesa];

        //Atual = Ultima carta da mesa indicada
        while(atual->prox != NULL){
            ant = atual;
            atual = atual->prox;
        }
        ant->prox = NULL;
        temp[posTemp] = atual;
    }else{
        red();
        printf("TEMP cheio...");
        reset();
        getchar();
    }
}//end moveMesaTemp

void moveTempMesa(){
    int posTemp, posMesa;
    tCarta *atual, *ant;

    printf("\n-- TEMP => MESA --\n\n");

    do{
        printf("Temp: ");
        scanf("%d", &posTemp);
    }while((posTemp < 0) || (posTemp > 3));

    if(temp[posTemp] != NULL){
        do{
            printf("Mesa: ");
            scanf("%d", &posMesa);
        }while((posMesa < 0) || (posMesa > 7));

        ant = NULL;
        atual = primMesa[posMesa];
        //Atual = Ultima carta da mesa indicada
        while(atual->prox != NULL){
            ant = atual;
            atual = atual->prox;
        }

        //Prossegue se a mesa esta vazia ou possui uma carta maior que a selecionada
        if((atual == NULL) || (atual->num > temp[posTemp]->num)){
            //Verificação das cores
            if(((atual->naipe <= 4) && (temp[posTemp]->naipe > 4)) || ((atual->naipe > 4) && (temp[posTemp]->naipe <= 4))){
                //Passando a carta do TEMP[i] p/ mesa indicada
                tCarta *novo = (tCarta *)malloc(sizeof(tCarta));
                novo->naipe = temp[posTemp]->naipe;
                novo->num = temp[posTemp]->num;
                novo->prox = NULL;
                atual->prox = novo;

                temp[posTemp] = NULL;
            }else{
                red();
                printf("Cartas de mesma cor...");
                reset();
                getchar();
                getchar();
            }
        }else{
            red();
            printf("Carta da mesa menor que a selecionada...");
            reset();
            getchar();
            getchar();
        }
    }else{
        red();
        printf("Indice TEMP vazio...");
        reset();
        getchar();
        getchar();
    }
}//end moveTempMesa


int main(int argc, char **argv){
    int op=0;
    srand(time(NULL));
    gerarBaralho();
    distribuirMesa();
    while(op!=4){
        imprime();
        printf("(1)Move Mesa-Nape\n");
        printf("(2)Move Mesa-Temp\n");
        printf("(3)Move Temp-Mesa\n");
        printf("(4)Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();
        switch(op){
            case 1:
                moveMesaNape();
                break;
            case 2:
                moveMesaTemp();
                break;
            case 3:
                moveTempMesa();
                break;
            default:
                if(op != 4) printf("Opcao Invalida.\n");
                break;
        }
    }
    return 0;
}//end main
