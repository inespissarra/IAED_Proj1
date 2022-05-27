/*
 * PROJETO IAED
 * INÊS PISSARRA Nº99236
 * DESCRIÇÃO: PROGRAMA DE GESTÃO DE TAREFAS
*/

/* Bibliotecas */
#include <stdio.h>
#include <string.h>


/*CONSTANTES*/

/*DIMENSÕES DE VETORES*/
#define MAX_DESCRICAO 51
#define MAX_TAREFAS 10000
#define MAX_NOME_ATIV 21
#define MAX_ATIVIDADES 10
#define MAX_NOME_UTIL 21
#define MAX_UTILIZADORES 50

/*ESTADOS*/
#define EXISTE 1
#define NAO_EXISTE 0
#define VALIDO 1
#define INVALIDO 0

/*TIPOS*/
#define DESCRICAO 1
#define UTILIZADOR 2
#define ATIVIDADE 3
#define COMECO 4

/*ATIVIDADES DE INICIO*/
#define TO_DO "TO DO"
#define IN_PROGRESS "IN PROGRESS"
#define DONE "DONE"

/*MENSAGENS DE ERRO*/
#define TASK "task"
#define TOO_MANY_TASKS "too many tasks"
#define INVALID_DURATION "invalid duration"
#define DUPLICATE_DESCRIPTION "duplicate description"
#define INVALID_TIME "invalid time"
#define TOO_MANY_USERS "too many users"
#define USER_ALREADY_EXISTS "user already exists"
#define DURATION "duration"
#define SLACK "slack"
#define NO_SUCH_ACTIVITY "no such activity"
#define NO_SUCH_USER "no such user"
#define TASK_ALREADY_STARTED "task already started"
#define NO_SUCH_TASK "no such task"
#define TOO_MANY_ACTIVITIES "too many activities"
#define DUPLICATE_ACTIVITY "duplicate activity"
#define INVALID_DESCRIPTION "invalid description"

    /*Estruturas*/
typedef struct{
    int identificador;
    char descricao[MAX_DESCRICAO];
    char utilizador[MAX_NOME_UTIL];
    char atividade[MAX_NOME_ATIV];
    int duracao;
    int comeco;
} Tarefa;

/*Variáveis globais*/
int tempo; /* Tempo atual do programa */
/* Variáveis que controlam a quantidade de elementos*/
int n_atividades = 3, n_tarefas, n_utilizadores;
/* Lista com os nomes das atividades */
char atividades[MAX_ATIVIDADES][MAX_NOME_ATIV] = {TO_DO, IN_PROGRESS, DONE};
/* Lista que contém todas as tarefas */
Tarefa tarefas[MAX_TAREFAS];
/* Lista com os nomes dos utilizadores */
char utilizadores[MAX_UTILIZADORES][MAX_NOME_UTIL];

/*Protótipos das funções para os Comandos*/
void nova_tarefa();
void listar_tarefas();
void avanca_tempo();
void add_utilizador();
void mover_tarefa();
void listar_tarefas_ativ();
void add_atividade();

/*Protótipos de Funções Auxiliares*/
int procura(char procurado[], int max, int tipo);
int le_input(char linha[], int max, int tipo);
char tira_espacos();
void ordena(Tarefa lista[], int esq, int dir, int tipo);
int menor(Tarefa t1, Tarefa t2, int tipo);

/*--------------------------------- FUNÇÕES ---------------------------------*/

/* 
 * FUNÇÃO PRINCIPAL
 * Input: <comando> - string
 * Lê o comando do terminal, de forma a invocar a função dedicada ao mesmo
 */
int main(){
    char comando;

    while((comando = getchar())!='q'){
        switch(comando){
            case 't':
                nova_tarefa();
                break;
            case 'l':
                listar_tarefas();
                break;
            case 'n':
                avanca_tempo();
                break;
            case 'u':
                add_utilizador();
                break;
            case 'm':
                mover_tarefa();
                break;
            case 'd':
                listar_tarefas_ativ();
                break;
            case 'a':
                add_atividade();
                break;
        }
    }
    return 0;
}


/*
 * comando t <duração> <descrição>
 * <duração> - int, <descrição> - string
 * cria uma nova tarefa na atividade TO DO
 * Output: Task <identificador>
*/
void nova_tarefa(){
    int duracao;
    char descricao[MAX_DESCRICAO];

    scanf("%d", &duracao);
    le_input(descricao, MAX_DESCRICAO, DESCRICAO);

    if (n_tarefas<MAX_TAREFAS){
        if(procura(descricao, n_tarefas, DESCRICAO) == NAO_EXISTE)
            if(duracao>0){
                /* se todas as condicoes se verificarem, criar nova tarefa */
                tarefas[n_tarefas].identificador = n_tarefas + 1;
                strcpy(tarefas[n_tarefas].descricao, descricao);
                strcpy(tarefas[n_tarefas].atividade, TO_DO);
                tarefas[n_tarefas].duracao = duracao;
                printf("%s %d\n", TASK, tarefas[n_tarefas].identificador);
                n_tarefas++;
            }
            else
                printf("%s\n", INVALID_DURATION);
        else
            printf("%s\n", DUPLICATE_DESCRIPTION);
    }
    else
        printf("%s\n", TOO_MANY_TASKS);
}


/*
 * comando l [<id> <id> ...]
 * <id> - int
 * Caso o input não tenha id's, todas as tarefas sao listadas
    por ordem alfabetica da descrição
 * Output: <id> <atividade> #<duração> <descrição>
*/
void listar_tarefas(){ 
    char c='\0';
    int id, estado = NAO_EXISTE;

    while(c!='\n' && c!=EOF){
        /* ir lendo todos os id's do terminal e verificando cada um deles */
        id = 0;
        c = tira_espacos();
        while('0'<=c && c<='9'){
            estado=EXISTE;
            id = id * 10 + (c -'0');
            c = getchar();
        }

        if(0<id && id<=n_tarefas){
            printf("%d %s ", id, tarefas[id-1].atividade);
            printf("#%d %s\n", tarefas[id-1].duracao, tarefas[id-1].descricao);
        }
        else if(estado==EXISTE)
            printf("%d: %s\n", id, NO_SUCH_TASK);
    }
    /* caso não seja introduzido nenhum id, listar todas as tarefas */
    if(estado==NAO_EXISTE){
        int i;
        Tarefa lista[MAX_TAREFAS];
        for(i=0; i < n_tarefas; i++)
            lista[i] = tarefas[i];
        ordena(lista, 0, n_tarefas-1, DESCRICAO);
        for(i=0; i < n_tarefas; i++){
            printf("%d %s ", lista[i].identificador, lista[i].atividade);
            printf("#%d %s\n", lista[i].duracao, lista[i].descricao);
        }
    }
}


/*
 * comando n <duração>
 * <duração> - int
 * Atualiza a variável global tempo
 * Output: novo valor do tempo atual
*/
void avanca_tempo(){
    int duracao;

    scanf("%d", &duracao);

    if(duracao>=0){
        tempo += duracao;
        printf("%d\n", tempo);
    }
    else
        printf("%s\n", INVALID_TIME);
}


/*
 * comando u [<utilizador>]
 * <utilizador> - string
 * Cria um novo utilizador caso o input tenha utilizador
 * Caso contrário lista todos utilizadores já existentes
*/
void add_utilizador(){
    char utilizador[MAX_NOME_UTIL];
    int i;

    le_input(utilizador, MAX_NOME_UTIL, UTILIZADOR);

    if(utilizador[0]!='\0'){
        if(procura(utilizador, n_utilizadores, UTILIZADOR)==NAO_EXISTE){
            if (n_utilizadores<MAX_UTILIZADORES){
                /*se todas as condições se verificarem, criar novo utilizador*/
                strcpy(utilizadores[n_utilizadores], utilizador);
                n_utilizadores++;
            }
            else
                printf("%s\n", TOO_MANY_USERS);
        }
        else
            printf("%s\n", USER_ALREADY_EXISTS);
    }
    else
    /*se não for introduzido nenhum utilizador, listar todos os utilizadores*/
        for(i=0; i<n_utilizadores; i++)
            printf("%s\n", utilizadores[i]);
}


/*
 * comando m <id> <utilizador> <atividade>
 * <id> - int, <utilizador> - string, <atividade> - string
 * Move a tarefa <id> para <atividade>
 * Output: duration=<tempo gasto> slack=<diferença entre o mesmo
    e o tempo estimado> 
    apenas se a atividade de destino for DONE
*/
void mover_tarefa(){
    int id;
    char utilizador[MAX_NOME_UTIL], atividade[MAX_NOME_ATIV];

    scanf("%d%s", &id, utilizador);
    le_input(atividade, MAX_NOME_ATIV, ATIVIDADE);

    if(id<=n_tarefas){
        if(strcmp(atividade, TO_DO)){
            if(procura(utilizador, n_utilizadores, UTILIZADOR)==EXISTE){
                if(procura(atividade, n_atividades, ATIVIDADE)==EXISTE){
                    /* mover apenas se todas as condições se verificarem */
                    /* se for iniciada uma tarefa, registar o tempo */
                    if(!strcmp(tarefas[id-1].atividade, TO_DO))
                        tarefas[id-1].comeco = tempo;
                    /* se a tarefa for movida para DONE existe output */
                    if(!strcmp(atividade, DONE) 
                            && strcmp(tarefas[id-1].atividade, DONE)){
                        int gasto = tempo-tarefas[id-1].comeco;
                        int slack = gasto-tarefas[id-1].duracao;
                        printf("%s=%d ", DURATION, gasto);
                        printf("%s=%d\n", SLACK, slack);
                    }
                    /* mover tarefa internamente */
                    strcpy(tarefas[id-1].atividade, atividade);
                    strcpy(tarefas[id-1].utilizador, utilizador);
                }
                else
                    printf("%s\n", NO_SUCH_ACTIVITY);
            }
            else
                printf("%s\n", NO_SUCH_USER);
        }
        /* apenas existe mensagem de erro se a tarefa já tiver sido iniciada*/
        else if(strcmp(tarefas[id-1].atividade, TO_DO))
            printf("%s\n", TASK_ALREADY_STARTED);
    }
    else
        printf("%s\n", NO_SUCH_TASK);
}


/*
 * comando d <atividade>
 * <atividade> - string
 * Output: <id> <início> <descrição> 
    das tarefas existentes em <atividade>
    por ordem crescente de instante de inicio
*/
void listar_tarefas_ativ(){
    char atividade[MAX_NOME_ATIV];

    le_input(atividade, MAX_NOME_ATIV, ATIVIDADE);
    if (procura(atividade, n_atividades, ATIVIDADE)==EXISTE){
        int i, a=0;
        Tarefa lista[MAX_TAREFAS];
        /* guardar num vetor todas as tarefas da atividade inserida */
        for(i=0; i<n_tarefas; i++){
            if (strcmp(tarefas[i].atividade, atividade)==0){
                lista[a] = tarefas[i];
                a++;
            }
        }
        ordena(lista, 0, a-1, COMECO);
        for(i=0; i<a; i++){
            printf("%d %d ", lista[i].identificador, lista[i].comeco);
            printf("%s\n", lista[i].descricao);
        }
    }
    else{
        printf("%s\n", NO_SUCH_ACTIVITY);
    }
}


/*
 * comando a [<atividade>]
 * <atividade> - string
 * Caso o input tenha atividade, é criada uma nova com esse nome
 * Caso contrario, são listadas todas as atividades já existentes
*/
void add_atividade(){
    char atividade[MAX_NOME_ATIV];

    if (le_input(atividade, MAX_NOME_ATIV, ATIVIDADE)==VALIDO){
        if (atividade[0]!='\0'){
            if(procura(atividade, n_atividades, ATIVIDADE) == NAO_EXISTE){
                if(n_atividades<MAX_ATIVIDADES){
                    /* caso todas as condições se verifique, criar atividade */
                    strcpy(atividades[n_atividades], atividade);
                    n_atividades++;
                }
                else
                    printf("%s\n", TOO_MANY_ACTIVITIES);
            }
            else
                printf("%s\n", DUPLICATE_ACTIVITY);
        }
        else{
            /* listar todas as atividades se não houver input */
            int i;
            for(i=0; i<n_atividades; i++)
                printf("%s\n", atividades[i]);
        }
    }
    else
        printf("%s\n", INVALID_DESCRIPTION);
}

/*--------------------------- FUNCOES AUXILIARES ---------------------------*/

/* 
 * Procura na lista de atividades se o seu argumento já lá existe
*/
int procura(char procurado[], int max, int tipo){
    int estado = NAO_EXISTE, i;

    for(i=0; i<max && estado==NAO_EXISTE; i++){
        if(tipo==UTILIZADOR){
            if(strcmp(utilizadores[i], procurado)==0)
                estado = EXISTE;
        }
        else if(tipo==ATIVIDADE){
            if(strcmp(atividades[i], procurado)==0)
                estado = EXISTE;
        }
        else{
            if(strcmp(tarefas[i].descricao, procurado)==0)
                estado = EXISTE;
        }
    }
    return estado;
}


/* 
 * Lê o input dos tipos descrição, utilizador e atividade
 * Avalia ainda a validade do tipo atividade (que não pode ter minusculas)
 * O utilizador não contém espaços
*/
int le_input(char linha[], int max, int tipo){
    int i=0, estado = VALIDO;
    char c;

    c = tira_espacos();

    for(i=0; i<max-1 && c!='\n' && c!=EOF; i++){
        if(tipo == ATIVIDADE && 'a'<=c && c<='z')
            estado = INVALIDO;
        else if(tipo==UTILIZADOR && (c==' ' || c=='\t'))
            estado = INVALIDO;

        if(estado==VALIDO)
            linha[i] = c;
        c=getchar();
    }
    linha[i]='\0';
    return estado;
}


/*
 * Lê os espaços do input que não serão necessários
 * Para quando lê um numero ou caracter diferente de ' ' e '\t'
 * Devolve este último pois será necessário para completar um dos nomes
*/
char tira_espacos(){
    char c;
    do{
        c = getchar(); 
    } while(c==' ' || c =='\t');
    return c;
}


/*
 * Ordena uma lista de Tarefas
 * Invoca a função menor de modo a que as tarefas 
    sejam ordenadas de acordo com o seu tipo
*/
void ordena(Tarefa lista[], int esq, int dir, int tipo){
    if(dir<=esq)
        return;
    else{
        int i = esq-1, j = dir;
        Tarefa pivot = lista[dir], aux;
        while(i<j){
            while(menor(lista[++i], pivot, tipo));
            while(menor(pivot, lista[--j], tipo))
                if(j==esq)
                    break;
            if(i<j){
                aux = lista[i];
                lista[i] = lista[j];
                lista[j] = aux;
            }

        }
        lista[dir] = lista[i];
        lista[i] = pivot;
        ordena(lista, esq, i-1, tipo);
        ordena(lista, i+1, dir, tipo);
    }
}


/*
 * Compara duas tarefas, devolvendo 1 se a primeira deve estar à esquerda
    da segunda na lista ordenada, e 0 caso contrario.
    (dependendo do tipo, que pode ser o tempo de começo ou a descrição)
*/
int menor(Tarefa t1, Tarefa t2, int tipo){
    if(tipo==COMECO){
        if(t1.comeco<t2.comeco)
            return 1;
        else if(t1.comeco>t2.comeco)
            return 0;
    }
    return strcmp(t1.descricao, t2.descricao)<0;
}
