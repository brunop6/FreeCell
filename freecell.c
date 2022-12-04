//Codificado no Windows
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<ctype.h>

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

    printf("\n-- MESA => NAIPE --\n\n");

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
    atualMesa = NULL;
    if(primMesa[posMesa] != NULL){
        atualMesa = primMesa[posMesa];
        while(atualMesa->prox != NULL){
            antMesa = atualMesa;
            atualMesa = atualMesa->prox;
        }
    }
    
    //Verificação de mesa vazia
    if(atualMesa != NULL){
        //Verificação de pilha de naipes vazia
        if(primNaipe[posNaipe] == NULL){
            //Se a pilha de naipes estiver vazia, apenas um Ás pode ser inserido
            if(atualMesa->num == 1){
                primNaipe[posNaipe] = (tCarta *)malloc(sizeof(tCarta));
                primNaipe[posNaipe] = atualMesa;
                if(antMesa == NULL){
                    primMesa[posMesa] = NULL;
                }else{
                    antMesa->prox = NULL;
                }
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
                    if(antMesa == NULL){
                        primMesa[posMesa] = NULL;
                    }else{
                        antMesa->prox = NULL;
                    }
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
        atual = NULL;
        if(primMesa[posMesa] != NULL){
            atual = primMesa[posMesa];

            //Atual = Ultima carta da mesa indicada
            while(atual->prox != NULL){
                ant = atual;
                atual = atual->prox;
            }

            temp[posTemp] = atual;
            
            if(ant != NULL){
                ant->prox = NULL;
            }else{
                primMesa[posMesa] = NULL;
            }
        }else{
            red();
            printf("MESA vazia...");
            reset();
            getchar();
            getchar();
        }
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
        atual = NULL;
        if(primMesa[posMesa] != NULL){
            atual = primMesa[posMesa];
            //Atual = Ultima carta da mesa indicada
            while(atual->prox != NULL){
                ant = atual;
                atual = atual->prox;
            }
        }
        
        //Prossegue se a mesa esta vazia ou possui uma carta subsequente maior que a selecionada
        if((atual == NULL) || (atual->num == (temp[posTemp]->num + 1))){
            //Passando a carta do TEMP[i] p/ mesa indicada
            tCarta *novo = (tCarta *)malloc(sizeof(tCarta));
            novo->naipe = temp[posTemp]->naipe;
            novo->num = temp[posTemp]->num;
            novo->prox = NULL;

            if(atual != NULL){
                //Verificação das cores
                if(((atual->naipe <= 4) && (temp[posTemp]->naipe > 4)) || ((atual->naipe > 4) && (temp[posTemp]->naipe <= 4))){
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
                primMesa[posMesa] = (tCarta *)malloc(sizeof(tCarta));
                primMesa[posMesa] = novo;
                temp[posTemp] = NULL;
            }
            
        }else{
            red();
            printf("A carta da mesa deve ser a proxima subsequente...");
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

void moveNapeTemp(){
    int posTemp, posNaipe;
    bool livre = false;
    tCarta *atual, *ant;

    printf("\n-- NAIPE => TEMP --\n\n");

    //Varredura TEMP
    for(posTemp = 0; posTemp < 4; posTemp++){
        if(temp[posTemp] == NULL){
            livre = true;
            break;
        }
    }

    if(livre){
        do{
            printf("Naipe: ");
            scanf("%d", &posNaipe);
        }while((posNaipe < 0) || (posNaipe > 3));

        ant = NULL;
        atual = NULL;
        if(primNaipe[posNaipe] != NULL){
            atual = primNaipe[posNaipe];

            //Atual = Ultima carta do naipe indicado
            while(atual->prox != NULL){
                ant = atual;
                atual = atual->prox;
            }

            temp[posTemp] = atual;
            
            if(ant != NULL){
                ant->prox = NULL;
            }else{
                primNaipe[posNaipe] = NULL;
            }
        }else{
            red();
            printf("NAIPE vazio...");
            reset();
            getchar();
            getchar();
        }
    }else{
        red();
        printf("TEMP cheio...");
        reset();
        getchar();
    }
}

void moveTempNape(){
    int posTemp, posNaipe;
    tCarta *atualNaipe, *antNaipe;

    printf("\n-- TEMP => NAIPE --\n\n");

    //Leitura das posições de Mesa e Naipe do movimento
    do{
        printf("Temp: ");
        scanf("%d", &posTemp);
    }while((posTemp < 0) || (posTemp > 3));
    do{
        printf("Naipe: ");
        scanf("%d", &posNaipe);
    }while((posNaipe < 0) || (posNaipe > 3));    
    
    //Verificação de temp vazio
    if(temp[posTemp] != NULL){
        //Verificação de pilha de naipes vazia
        if(primNaipe[posNaipe] == NULL){
            //Se a pilha de naipes estiver vazia, apenas um Ás pode ser inserido
            if(temp[posTemp]->num == 1){
                primNaipe[posNaipe] = (tCarta *)malloc(sizeof(tCarta));
                primNaipe[posNaipe] = temp[posTemp];
                temp[posTemp] = NULL;
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
                atualNaipe = atualNaipe->prox;
            }

            //Validação da sequência de inserção na pilha
            if((temp[posTemp]->num-1) == atualNaipe->num){
                //Validação do naipe da pilha
                if(temp[posTemp]->naipe == atualNaipe->naipe){
                    atualNaipe->prox = temp[posTemp];
                    temp[posTemp] = NULL;
                }else{
                    red();
                    printf("A carta do temporario deve possuir o mesmo naipe da pilha de naipes...");
                    reset();
                    getchar();
                    getchar();
                }
            }else{
                red();
                printf("A carta do temporario deve ser a subsequente posterior da pilha de naipes...");
                reset();
                getchar();
                getchar();
            }
        }
    }else{
        red();
        printf("Temp vazio...");
        reset();
        getchar();
        getchar();
    }
}

void moveNapeMesa(){
    int posMesa, posNaipe;
    tCarta *atualMesa, *antMesa, *atualNaipe, *antNaipe;

    printf("\n-- NAIPE => MESA --\n\n");

    do{
        printf("Naipe: ");
        scanf("%d", &posNaipe);
    }while((posNaipe < 0) || (posNaipe > 3));

    if(primNaipe[posNaipe] != NULL){
        //Encontrando o ultimo elemento da pilha de naipes indicada
        antNaipe = NULL;
        atualNaipe = primNaipe[posNaipe];
        while(atualNaipe->prox != NULL){
            antNaipe = atualNaipe;
            atualNaipe = atualNaipe->prox;
        }
        
        do{
            printf("Mesa: ");
            scanf("%d", &posMesa);
        }while((posMesa < 0) || (posMesa > 7));

        //Encontrando o ultimo elemento da mesa indicada
        antMesa = NULL;
        atualMesa = NULL;
        if(primMesa[posMesa] != NULL){
            atualMesa = primMesa[posMesa];
            //Atual = Ultima carta da mesa indicada
            while(atualMesa->prox != NULL){
                antMesa = atualMesa;
                atualMesa = atualMesa->prox;
            }
        }
        
        //Prossegue se a mesa esta vazia ou possui uma carta subsequente maior que a selecionada
        if((atualMesa == NULL) || (atualMesa->num == (atualNaipe->num + 1))){
            //Passando a carta do naipe p/ mesa indicada
            tCarta *novo = (tCarta *)malloc(sizeof(tCarta));
            novo->naipe = atualNaipe->naipe;
            novo->num = atualNaipe->num;
            novo->prox = NULL;

            if(atualMesa != NULL){
                //Verificação das cores
                if(((atualMesa->naipe <= 4) && (atualNaipe->naipe > 4)) || ((atualMesa->naipe > 4) && (atualNaipe->naipe <= 4))){
                    atualMesa->prox = novo;
                    if(antNaipe != NULL){
                        antNaipe->prox= NULL;
                    }else{
                        primNaipe[posNaipe] = NULL;
                    }
                }else{
                    red();
                    printf("Cartas de mesma cor...");
                    reset();
                    getchar();
                    getchar();
                }
            }else{
                primMesa[posMesa] = (tCarta *)malloc(sizeof(tCarta));
                primMesa[posMesa] = novo;
                if(antNaipe != NULL){
                    antNaipe->prox= NULL;
                }else{
                    primNaipe[posNaipe] = NULL;
                }
            }
            
        }else{
            red();
            printf("A carta da mesa deve ser a proxima subsequente...");
            reset();
            getchar();
            getchar();
        }
    }else{
        red();
        printf("Indice NAIPE vazio...");
        reset();
        getchar();
        getchar();
    }
}

void moveMesaMesa(){
    int posMesa1, posMesa2;
    char enviarGp;
    tCarta *atualMesa1, *antMesa1, *atualMesa2, *antMesa2, *aux;
    bool seqValida = true;

    printf("\n-- MESA => MESA --\n\n");

    do{
        printf("Mesa origem: ");
        scanf("%d", &posMesa1);
    }while((posMesa1 < 0) || (posMesa1 > 7));

    if(primMesa[posMesa1] != NULL){
        do{
            printf("Mesa destino: ");
            scanf("%d", &posMesa2);
        }while((posMesa2 < 0) || (posMesa2 > 7) || (posMesa2 == posMesa1));
        do{
            printf("Enviar um grupo de cartas? (s/n)");
            getchar();
            enviarGp = getchar();
            getchar();
            enviarGp = tolower(enviarGp);
        }while((enviarGp != 's') && (enviarGp != 'n'));

        antMesa1 = NULL;
        atualMesa1 = primMesa[posMesa1];

        //Identificando o grupo de cartas da mesa 1 p/ o movimento
        if(enviarGp == 's'){
            //Início da busca de uma sequência válida
            do{
                if(!seqValida){
                    antMesa1 = atualMesa1;
                    atualMesa1 = atualMesa1->prox;
                }

                //Validação da sequência
                seqValida = true;
                aux = atualMesa1;
                while(aux->prox != NULL){
                    //Verifica se há um numero não subsequente ou uma sequência inválida de cores
                    if((aux->prox->num != aux->num-1) || (((aux->prox->naipe <= 4) && (aux->naipe <= 4)) || ((aux->prox->naipe > 4) && (aux->naipe > 4)))){ 
                        seqValida = false;
                        break;
                    }
                    aux = aux->prox;
                }
            }while(!(seqValida) && (atualMesa1->prox != NULL));

            if(!seqValida){
                red();
                printf("Sequencia invalida...\n");
                reset();
                system("pause");
                return;
            }
        }else{
            while(atualMesa1->prox != NULL){
                antMesa1 = atualMesa1;
                atualMesa1 = atualMesa1->prox;
            }
        }

        //Encontrando o ultimo elemento da mesa de destino indicada
        antMesa2 = NULL;
        atualMesa2 = NULL;
        if(primMesa[posMesa2] != NULL){
            atualMesa2 = primMesa[posMesa2];
            //Atual = Ultima carta da mesa indicada
            while(atualMesa2->prox != NULL){
                antMesa2 = atualMesa2;
                atualMesa2 = atualMesa2->prox;
            }
        }
        
        //Prossegue se a mesa esta vazia ou possui uma carta subsequente maior que a selecionada
        if((atualMesa2 == NULL) || (atualMesa2->num == (atualMesa1->num + 1))){
            //Passando a carta do naipe p/ mesa indicada
            tCarta *novo = (tCarta *)malloc(sizeof(tCarta));
            novo = atualMesa1;

            if(atualMesa2 != NULL){
                //Verificação das cores
                if(((atualMesa2->naipe <= 4) && (atualMesa1->naipe > 4)) || ((atualMesa2->naipe > 4) && (atualMesa1->naipe <= 4))){
                    atualMesa2->prox = novo;
                    if(antMesa1 != NULL){
                        antMesa1->prox= NULL;
                    }else{
                        primMesa[posMesa1] = NULL;
                    }
                }else{
                    red();
                    printf("Cartas de mesma cor...");
                    reset();
                    getchar();
                }
            }else{
                primMesa[posMesa2] = (tCarta *)malloc(sizeof(tCarta));
                primMesa[posMesa2] = novo;
                if(antMesa1 != NULL){
                    antMesa1->prox= NULL;
                }else{
                    primMesa[posMesa1] = NULL;
                }
            }
            
        }else{
            red();
            printf("A carta da mesa deve ser a proxima subsequente...");
            reset();
            getchar();
        }
    }else{
        red();
        printf("Mesa de origem vazia...");
        reset();
        getchar();
    }
}

bool validaVitoria(){
    int posNaipe;
    tCarta *atualNaipe;

    for(posNaipe = 0; posNaipe < 4; posNaipe++){
        //Encontrando o ultimo elemento da pilha de naipes indicada
        atualNaipe = primNaipe[posNaipe];

        if(atualNaipe != NULL){
            while(atualNaipe->prox != NULL){
                atualNaipe = atualNaipe->prox;
            }
        }else{
            return false;
        }
        if(atualNaipe->num != 13){
            return false; 
        }
    }

    return true;
}

int main(int argc, char **argv){
    int op=-1;
    srand(time(NULL));
    gerarBaralho();
    distribuirMesa();
    while(op!=0){
        imprime();
        printf("(1)Move Mesa-Mesa\n");
        printf("(2)Move Mesa-Nape\n");
        printf("(3)Move Mesa-Temp\n");
        printf("(4)Move Temp-Mesa\n");
        printf("(5)Move Temp-Nape\n");
        printf("(6)Move Nape-Mesa\n");
        printf("(7)Move Nape-Temp\n");
        printf("(0)Sair\n");

        if(!validaVitoria()){
            printf("Opcao: ");
            scanf("%d", &op);
            getchar();
            switch(op){
                case 1:
                    moveMesaMesa();
                    break;
                case 2:
                    moveMesaNape();
                    break;
                case 3:
                    moveMesaTemp();
                    break;
                case 4:
                    moveTempMesa();
                    break;
                case 5:
                    moveTempNape();
                    break;
                case 6:
                    moveNapeMesa();
                    break;
                case 7:
                    moveNapeTemp();
                    break;
                default:
                    if(op != 0) printf("Opcao Invalida.\n");
                    break;
            }
        }else {
            red();
            printf("Fim do Jogo!\n Parabens!!");
            reset();
            getchar();
            op = 0;
        }
    }
    return 0;
}//end main
