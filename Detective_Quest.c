#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// ============================================================
// ESTRUTURAS DE DADOS
// ============================================================

/// Estrutura que representa um cômodo (nó da árvore binária da mansão)
struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
};

/// Estrutura da árvore de pistas (BST)
struct Pista {
    char conteudo[100];
    struct Pista* esquerda;
    struct Pista* direita;
};

/// Estrutura de um item da tabela hash (associação pista → suspeito)
struct ItemHash {
    char pista[100];
    char suspeito[50];
    struct ItemHash* prox;
};

// ============================================================
// FUNÇÕES AUXILIARES
// ============================================================

/// Função hash simples: soma dos caracteres mod tamanho da tabela
int hash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

// ============================================================
// FUNÇÕES DE ESTRUTURAS
// ============================================================

/// ------------------------------------------------------------
/// criarSala() – cria dinamicamente um cômodo com nome e pista.
/// ------------------------------------------------------------
struct Sala* criarSala(const char* nome, const char* pista) {
    struct Sala* nova = (struct Sala*) malloc(sizeof(struct Sala));
    if (!nova) {
        printf("Erro ao alocar memória para sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/// ------------------------------------------------------------
/// inserirPista() – insere uma pista na BST de forma ordenada.
/// ------------------------------------------------------------
struct Pista* inserirPista(struct Pista* raiz, const char* conteudo) {
    if (raiz == NULL) {
        struct Pista* nova = (struct Pista*) malloc(sizeof(struct Pista));
        strcpy(nova->conteudo, conteudo);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }
    if (strcmp(conteudo, raiz->conteudo) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    else if (strcmp(conteudo, raiz->conteudo) > 0)
        raiz->direita = inserirPista(raiz->direita, conteudo);
    return raiz;
}

/// ------------------------------------------------------------
/// exibirPistas() – exibe todas as pistas em ordem alfabética.
/// ------------------------------------------------------------
void exibirPistas(struct Pista* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf(" - %s\n", raiz->conteudo);
        exibirPistas(raiz->direita);
    }
}

/// ------------------------------------------------------------
/// inserirNaHash() – insere a associação pista/suspeito na tabela.
/// ------------------------------------------------------------
void inserirNaHash(struct ItemHash* tabela[], const char* pista, const char* suspeito) {
    int indice = hash(pista);
    struct ItemHash* novo = (struct ItemHash*) malloc(sizeof(struct ItemHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[indice];
    tabela[indice] = novo;
}

/// ------------------------------------------------------------
/// encontrarSuspeito() – retorna o suspeito associado à pista.
/// ------------------------------------------------------------
const char* encontrarSuspeito(struct ItemHash* tabela[], const char* pista) {
    int indice = hash(pista);
    struct ItemHash* atual = tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

/// ------------------------------------------------------------
/// verificarSuspeitoFinal() – conta pistas e define desfecho.
/// ------------------------------------------------------------
void verificarSuspeitoFinal(struct Pista* raiz, struct ItemHash* tabela[], const char* acusado, int* contagem) {
    if (raiz == NULL) return;
    verificarSuspeitoFinal(raiz->esquerda, tabela, acusado, contagem);
    
    const char* suspeito = encontrarSuspeito(tabela, raiz->conteudo);
    if (suspeito != NULL && strcmp(suspeito, acusado) == 0)
        (*contagem)++;

    verificarSuspeitoFinal(raiz->direita, tabela, acusado, contagem);
}

// ============================================================
// FUNÇÃO PRINCIPAL DE EXPLORAÇÃO
// ============================================================

/// ------------------------------------------------------------
/// explorarSalas() – navega pela árvore da mansão e coleta pistas.
/// ------------------------------------------------------------
void explorarSalas(struct Sala* atual, struct Pista** arvorePistas) {
    char escolha;
    printf("\n=== Início da exploração ===\n");

    while (atual != NULL) {
        printf("\nVocê está na: %s\n", atual->nome);
        
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        printf("\nEscolha o caminho:\n");
        if (atual->esquerda) printf(" (e) Esquerda → %s\n", atual->esquerda->nome);
        if (atual->direita)  printf(" (d) Direita  → %s\n", atual->direita->nome);
        printf(" (s) Sair da mansão\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda) atual = atual->esquerda;
            else printf("Não há sala à esquerda!\n");
        }
        else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita) atual = atual->direita;
            else printf("Não há sala à direita!\n");
        }
        else if (escolha == 's' || escolha == 'S') {
            printf("\nVocê decidiu encerrar a exploração.\n");
            break;
        }
        else {
            printf("Opção inválida!\n");
        }
    }
}

// ============================================================
// FUNÇÃO PRINCIPAL (MAIN)
// ============================================================

int main() {
    // ---------------------------
    // Mapa fixo da mansão
    // ---------------------------
    struct Sala* hall = criarSala("Hall de Entrada", "Pegadas de lama levam à cozinha.");
    struct Sala* salaEstar = criarSala("Sala de Estar", "Uma luva feminina deixada no sofá.");
    struct Sala* cozinha = criarSala("Cozinha", "Um copo de vinho quebrado no chão.");
    struct Sala* jardim = criarSala("Jardim", "Uma pegada grande próxima à fonte.");
    struct Sala* biblioteca = criarSala("Biblioteca", "Um livro com páginas rasgadas sobre venenos.");
    struct Sala* escritorio = criarSala("Escritório", "Documentos de seguro de vida rasgados.");

    // Conexões da árvore binária
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = jardim;
    salaEstar->direita = biblioteca;
    cozinha->direita = escritorio;

    // ---------------------------
    // Tabela hash de pistas/suspeitos
    // ---------------------------
    struct ItemHash* tabela[TAM_HASH] = { NULL };

    inserirNaHash(tabela, "Pegadas de lama levam à cozinha.", "Sr. Carlos");
    inserirNaHash(tabela, "Uma luva feminina deixada no sofá.", "Sra. Helena");
    inserirNaHash(tabela, "Um copo de vinho quebrado no chão.", "Sr. Carlos");
    inserirNaHash(tabela, "Uma pegada grande próxima à fonte.", "Jardineiro");
    inserirNaHash(tabela, "Um livro com páginas rasgadas sobre venenos.", "Sra. Helena");
    inserirNaHash(tabela, "Documentos de seguro de vida rasgados.", "Sr. Carlos");

    // ---------------------------
    // Exploração e coleta de pistas
    // ---------------------------
    struct Pista* arvorePistas = NULL;
    explorarSalas(hall, &arvorePistas);

    // ---------------------------
    // Exibir pistas coletadas
    // ---------------------------
    printf("\n===== PISTAS COLETADAS =====\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    // ---------------------------
    // Julgamento final
    // ---------------------------
    char acusado[50];
    printf("\nDigite o nome do suspeito que você acusa: ");
    scanf(" %[^\n]", acusado);

    int contagem = 0;
    verificarSuspeitoFinal(arvorePistas, tabela, acusado, &contagem);

    printf("\n===== JULGAMENTO FINAL =====\n");
    if (contagem >= 2)
        printf("As evidências confirmam! %s é o culpado.\n", acusado);
    else
        printf("As pistas são insuficientes. %s é inocente (ou esperto demais).\n", acusado);

    printf("\nFim da investigação.\n");

    // Liberação simplificada (sem recursão)
    free(hall); free(salaEstar); free(cozinha);
    free(jardim); free(biblioteca); free(escritorio);
    return 0;
}
