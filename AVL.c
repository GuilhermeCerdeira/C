#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct No{ //struct dos nodos
    int chave;
    struct No* pai;
    struct No* direita;
    struct No* esquerda;
}Nodo;

Nodo* criarno(int chave); //função para criar os nodos
Nodo* insercao(Nodo* raiz, int chave); //função de inserção dos nodos
bool busca(Nodo* raiz, int chave); //função de busca dos nodos
Nodo* sucessor(Nodo* nodo); //função para encontrar o sucessor usado na função de remoção
Nodo* remocao(Nodo* raiz, int chave); //função de remoção dos nodos
void imprime(Nodo* raiz, FILE* outFile); //função de imprimir nós

int main() {
    Nodo* raiz = NULL;
    FILE* inFile = fopen("in.txt", "r"); //abre o arquivo de entrada
    FILE* outFile = fopen("out.txt", "w"); //abre o arquivo de saida

    if (inFile == NULL || outFile == NULL) {
        perror("Erro ao abrir arquivos.");
        return 1;
    }

    char operacao;
    int valor;

    while (fscanf(inFile, " %c %d", &operacao, &valor) != EOF) {  //le as operações e as chaves
        if (operacao == 'i') {
            raiz = insercao(raiz, valor);
        } else if (operacao == 'r') {
            if (busca(raiz, valor)) {
                raiz = remocao(raiz, valor);
            }
        }
    }

    imprime(raiz, outFile); //imprime a arvore

    fclose(inFile); //fecha os arquivos
    fclose(outFile);

    return 0;
}


// Função para criar um novo nó
Nodo* criarno(int chave) { 
    Nodo* novono = (Nodo*)malloc(sizeof(Nodo));
    novono->chave = chave;
    novono->esquerda = NULL;
    novono->direita = NULL;
    novono->pai = NULL;
    return novono;
}

// Função para inserir um nó na árvore
Nodo* insercao(Nodo* raiz, int chave){  
  
    if(raiz == NULL){
        return criarno(chave); //Cria um novo nó se a árvore estiver vazia
    }

    if(chave <= raiz->chave){
        raiz->esquerda = insercao(raiz->esquerda, chave);  // Insere à esquerda se a chave for menor ou igual
        raiz->esquerda->pai = raiz;
    }
    else if(chave > raiz->chave){
        raiz->direita = insercao(raiz->direita, chave); // Insere à direita se a chave for maior
        raiz->direita->pai = raiz;
    }

    return raiz;
}

// Função para buscar um valor na árvore
bool busca(Nodo* raiz, int chave){  
    if(raiz == NULL){
        return false; // Retorna falso se a árvore estiver vazia ou o valor não for encontrado
    }
    if(raiz->chave == chave){
        return true; // Retorna verdadeiro se o valor for encontrado
    }else if(chave < raiz->chave){
        return busca(raiz->esquerda, chave); // Busca na subárvore esquerda se a chave for menor
    }else{
        return busca(raiz->direita, chave); // Busca na subárvore direita se a chave for maior
    }
}

// Função para encontrar o sucessor de um nó
Nodo* sucessor(Nodo* nodo){ 
    while(nodo->esquerda != NULL){
        nodo = nodo->esquerda; // Percorre a árvore para a esquerda até encontrar o nó mais à esquerda
    }
    return nodo; // Retorna o nó mais à esquerda, que é o sucessor
}

// Função para remover um nó da árvore
Nodo* remocao(Nodo* raiz, int chave){ 
    if(raiz == NULL){
        return NULL; // Retorna NULL se a árvore estiver vazia
    }

    if(chave < raiz->chave){
        raiz->esquerda = remocao(raiz->esquerda, chave); // Remove da subárvore esquerda se a chave for menor
    }else if(chave > raiz->chave){
        raiz->direita = remocao(raiz->direita, chave); // Remove da subárvore direita se a chave for maior
    }
    else{
        // Caso em que encontramos o nó a ser removido
        if(raiz->esquerda == NULL){
            Nodo* temp = raiz->direita;
            free(raiz);
            return temp; // Se o nó não tiver filho esquerdo, é substituido pelo direito
        }
        else if(raiz->direita == NULL){
            Nodo* temp = raiz->esquerda;
            free(raiz);
            return temp; // Se o nó não tiver filho direito, é substituido pelo esquerdo
        }
        Nodo* temp = sucessor(raiz->direita); // Encontra o sucessor na árvore direita
        raiz->chave = temp->chave; // Copia o valor do sucessor para o nó a ser removido
        raiz->direita = remocao(raiz->direita, temp->chave); //remove o sucessor
    }
    return raiz;
}

// Função para imprimir os nós da árvore
void imprime(Nodo* raiz, FILE* outFile) { 
    if (raiz != NULL) {
        imprime(raiz->esquerda, outFile); // Imprime os nós da subárvore esquerda
        fprintf(outFile, "%d\n", raiz->chave); // Escreve a chave do nó no arquivo de saída
        imprime(raiz->direita, outFile); // Imprime os nós da subárvore direita
    }
}