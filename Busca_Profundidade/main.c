#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0
#define BRANCO 0
#define AMARELO 1
#define VERMELHO 2

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
/// Busca em Profundidade ==============================================================================================
void visitaP(GRAFO *g, int u, int *cor){
    cor[u] = AMARELO;
    ADJACENCIA *v = g->adj[u].cab;
    printf("V%d->", u);
    while(v){
        if(cor[v->vertice] == BRANCO){

            visitaP(g,v->vertice,cor);
        }
        v = v->prox;
    }
    cor[u] = VERMELHO;
}
//---------------------------------------------------------
void profundidade(GRAFO *g, int raiz){
    printf("================= Busca em Profundidade =================\n");
    int cor[g->vertices];
    int u;
    for(u=0; u<g->vertices; u++){
        cor[u] = BRANCO;
    }
    u=raiz;
    printf(">> Raiz: V%d\n", u);
        if (cor[u] == BRANCO){
            visitaP(g,u,cor);
            printf("fim.\n");
        }
    printf("=========================================================\n\n");
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
    profundidade(gr,raiz);
    raiz= informarRaiz(gr);
    profundidade(gr,raiz);
}
