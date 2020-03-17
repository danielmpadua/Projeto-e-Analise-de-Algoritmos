#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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
/// Dijkstra ===========================================================================
void inicializaD(GRAFO *g, int *distancia, int *predecessor, int inicio){
    int v;
    for(v=0; v<g->vertices; v++){
        distancia[v] = INT_MAX/2;
        predecessor[v] = -1;
    }
    distancia[inicio] = 0;
}
//---------------------------------------------------------
void relaxamento(GRAFO *g, int *distancia, int *predecessor, int u, int v){
    ADJACENCIA *ad = g->adj[u].cab;
    while(ad && (ad->vertice != v)){
      //  printf("||D(V%d-> V%d) = %d\n" , inicio,ad->vertice,distancia[ad->vertice]);
        ad = ad->prox;
    }
    if(ad){
        if(distancia[v] > (distancia[u] + ad->peso)){
            distancia[v] = distancia[u] + ad->peso;
            predecessor[v] = u;
        }
    }
}
//---------------------------------------------------------
void dijkstra(GRAFO *g, int inicio){
    printf("======================= DIJKSTRA ========================\n");
    printf("||Distancia a partir de V%d = menor peso possivel [caminho percorrido (do caminho final ao inicial)]\n", inicio);

    int *distancia = (int *)malloc(g->vertices*sizeof(int));
    int predecessor[g->vertices];
    bool aberto[g->vertices];
    inicializaD(g,distancia,predecessor,inicio);

    for(int i=0; i<g->vertices; i++){
        aberto[i] = true;
    }
    while (existeAberto(g,aberto)){
        int u = menorDist(g,aberto,distancia);
        aberto[u] = false;
        ADJACENCIA *ad = g->adj[u].cab;
        while(ad){
            relaxamento(g,distancia,predecessor,u,ad->vertice);
            ad = ad->prox;
        }
        printf("||D(V%d-> V%d) = %d [" , inicio,u,distancia[u]);
        int k=u;
        while(k != -1){
            printf("V%d->", k);
            k = predecessor[k];
        }
        printf("fim]\n");

    }
    printf("=========================================================\n\n");
}
//---------------------------------------------------------
bool existeAberto(GRAFO *g, int *aberto){
    for(int i=0;i<g->vertices; i++){
        if(aberto[i]){
            return (true);
        }
    }
    return(false);
}
//---------------------------------------------------------
int menorDist(GRAFO *g, int *aberto, int *distancia){
    int i;
    for(i=0;i<g->vertices; i++){
        if(aberto[i]){
            break;
        }
    }
    if(i==g->vertices){
        return (-1);
    }
    int menor = i;
    for(i=menor+1; i<g->vertices; i++){
        if(aberto[i] && (distancia[menor]>distancia[i])){
            menor = i;
        }
    }
    return (menor);
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
    dijkstra(gr,raiz);
    raiz= informarRaiz(gr);
    dijkstra(gr,raiz);
}
