#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

typedef int bool;

/// Estruturas e Funções basicas para o funcionamento do grafo ================================================================
typedef struct adjacencia{
 int vertice;
 int peso;
 struct adjacencia *prox;
}ADJACENCIA;
//---------------------------------------------------------
typedef struct vertice{
    ADJACENCIA *cab;
}VERTICE;
//---------------------------------------------------------
typedef struct grafo{
    int vertices;
    int arestas;
    VERTICE *adj;
}GRAFO;
//---------------------------------------------------------
typedef struct{
    int chave;
}REGISTRO;
//---------------------------------------------------------
typedef struct aux{
    REGISTRO reg;
    struct aux* prox;
}ELEMENTO, *PONT;
//---------------------------------------------------------
typedef struct{
    PONT inicio;
    PONT fim;
}FILA;
//---------------------------------------------------------
GRAFO *criaGrafo(int nVertices){
    GRAFO *g = (GRAFO *) malloc(sizeof(GRAFO));
    g->vertices = nVertices;
    g->arestas = 0;
    g->adj = (VERTICE *) malloc(nVertices*sizeof(VERTICE));
    int i;
    for(i=0; i<nVertices; i++){
        g->adj[i].cab = NULL;
    }
    return(g);
}
//---------------------------------------------------------
ADJACENCIA *criaAdj(int v, int peso){
    ADJACENCIA *temp = (ADJACENCIA *) malloc(sizeof(ADJACENCIA));
    temp->vertice = v;
    temp->peso = peso;
    temp->prox = NULL;
    return(temp);
}
//---------------------------------------------------------
bool criaAresta(GRAFO *gr, int vi, int vf, int peso){
    if(!gr){
        printf("ERRO: Grafo inexistente!\n\n");
        return(false);
    }
    if((vf<0)||(vf>=gr->vertices)){
        printf("ERRO: Vertice destino está fora dos limites!\n\n");
        return(false);
    }
    if((vi<0)||(vi>=gr->vertices)){
        printf("ERRO: Vertice origem está fora dos limites!\n\n");
        return(false);
    }
    ADJACENCIA *novo = criaAdj(vf, peso);
    novo->prox = gr->adj[vi].cab;
    gr->adj[vi].cab = novo;
    gr->arestas++;
    return(true);
}
//---------------------------------------------------------
void imprime(GRAFO *gr){
    printf("============== GRAFO ===============\n");
    printf("||Vertices: %d. Arestas: %d (peso).\n", gr->vertices,gr->arestas);
    int i;
    for(i=0; i<gr->vertices; i++){
        if(i<10){
            printf("|| V%d: ",i);
        }else{
            printf("||V%d: ",i);
        }
        ADJACENCIA *ad = gr->adj[i].cab;
        while(ad){
            printf("V%d(%d) ",ad->vertice,ad->peso);
            ad = ad->prox;
        }
        printf("\n");
    }
    printf("=====================================\n\n");
}
//---------------------------------------------------------
GRAFO *gerarGrafo(){
    int x=1, nVertices,peso;
    GRAFO *g;
    do{
        char nomeArquivo[100];
        printf("Digite o nome do arquivo: ");
        scanf("%s",nomeArquivo);
        printf("\n");
        FILE* arquivo = fopen(nomeArquivo,"r");
        if(arquivo != NULL){
            fscanf(arquivo, "%d" , &nVertices);
            g = criaGrafo(nVertices);

            for(int u=0;u<nVertices;u++){
                for(int v=0; v<nVertices; v++){
                    fscanf(arquivo, "%d" , &peso);
                    if(peso>0){
                        criaAresta(g,u,v,peso);
                    }
                }
            }
            fclose(arquivo);
            x=0;
            return g;
        }else{
            printf("Arquivo inválido! Tente novamente.\n\n");
        }
    }while(x==1);
    return g;
}
//---------------------------------------------------------
/// Busca em Largura ==============================================================================================
 void largura(GRAFO *g, int raiz){
    printf("=================== Busca em Largura ===================\n");
    bool explorado[g->vertices];
    int u;
    for(u=0; u<g->vertices;u++){
        explorado[u] = false;
    }
    u=raiz;
    printf(">> Raiz: V%d\n", u);
        if(!explorado[u]){
            visitaL(g,u,explorado);
            printf("fim.\n");
        }
    printf("=========================================================\n\n");
 }
//---------------------------------------------------------
void visitaL(GRAFO *g, int s, bool *explorado){
    FILA f;
    inicializarFila(&f);
    explorado[s] = true;
    REGISTRO *u = (REGISTRO *) malloc(sizeof(REGISTRO));
    u->chave =s;
    inserirNaFila(&f,*u);
    printf("V%d->", s);
    while(f.inicio){
        excluirDaFila(&f,u);
        ADJACENCIA *v = g->adj[u->chave].cab;
        while(v){
            if(!explorado[v->vertice]){
                printf("V%d->", v->vertice);
                explorado[v->vertice] = true;
                u->chave = v->vertice;
                inserirNaFila(&f,*u);
            }
            v = v->prox;
        }
    }
    free(u);
}
//---------------------------------------------------------
/// Fila ==============================================================================================
void inicializarFila(FILA* f){
    f->inicio = NULL;
    f->fim = NULL;
}
//---------------------------------------------------------
bool inserirNaFila(FILA* f,REGISTRO reg){
    PONT novo = (PONT) malloc(sizeof(ELEMENTO));
    novo->reg = reg;
    novo->prox = NULL;
    if(f->inicio==NULL){
        f->inicio = novo;
    }else{
        f->fim->prox= novo;
    }
    f->fim = novo;
    return true;
}
//---------------------------------------------------------
bool excluirDaFila(FILA* f,REGISTRO* reg){
    if(f->inicio==NULL){
        return false;
    }
    *reg = f->inicio->reg;
    PONT apagar = f->inicio;
    f->inicio = f->inicio->prox;
    free(apagar);
    if(f->inicio ==NULL){
        f->fim = NULL;
    }
    return true;
}
//---------------------------------------------------------
int informarRaiz(GRAFO *gr){
    int x=1,raiz=-1;
    do{
        printf("Infome o vertice raiz (de 0 a %d): ", gr->vertices-1);
        scanf("%d",&raiz);
        printf("\n");
        if(raiz>=0 && raiz<gr->vertices){
            x=0;
        }else{
            printf("Raiz invalida! Tente novamente.\n\n");
        }
    }while(x==1);
    return raiz;
}
//---------------------------------------------------------
/// Main ===============================================================================================================
int main(void){
    GRAFO *gr = gerarGrafo();
    imprime(gr);
    int raiz = informarRaiz(gr);
    largura(gr,raiz);
    raiz= informarRaiz(gr);
    largura(gr,raiz);
}
