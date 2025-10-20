#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Estrutura que representa um cômodo da mansão
struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
};

/// Estrutura que representa um nó da árvore de pistas (BST)
struct Pista {
    char conteudo[100];
    struct Pista* esquerda;
    struct Pista* direita;
};

/// ----------------------------------------------------------------------
/// Função: criarSala()
/// Cria dinamicamente uma nova sala com um nome e, opcionalmente, uma pista.
/// ----------------------------------------------------------------------
struct Sala* criarSala(const char* nome, const char* pista) {
    struct Sala* novaSala = (struct Sala*) malloc(sizeof(struct Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    if (pista != NULL)
        strcpy(novaSala->pista, pista);
    else
        strcpy(novaSala->pista, "");
    
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/// ----------------------------------------------------------------------
/// Função: inserirPista()
/// Insere uma nova pista em ordem alfabética na árvore de pistas (BST).
/// ----------------------------------------------------------------------
struct Pista* inserirPista(struct Pista* raiz, const char* conteudo) {
    if (raiz == NULL) {
        struct Pista* nova = (struct Pista*) malloc(sizeof(struct Pista));
        if (nova == NULL) {
            printf("Erro ao alocar memória para a pista.\n");
            exit(1);
        }
        strcpy(nova->conteudo, conteudo);
        nova->esquerda = NULL;
        nova->direita = NULL;
        return nova;
    }

    if (strcmp(conteudo, raiz->conteudo) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    else if (strcmp(conteudo, raiz->conteudo) > 0)
        raiz->direita = inserirPista(raiz->direita, conteudo);

    return raiz;
}

/// ----------------------------------------------------------------------
/// Função: exibirPistas()
/// Exibe as pistas em ordem alfabética (travessia em ordem da BST).
/// ----------------------------------------------------------------------
void exibirPistas(struct Pista* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf(" - %s\n", raiz->conteudo);
        exibirPistas(raiz->direita);
    }
}

/// ----------------------------------------------------------------------
/// Função: explorarSalasComPistas()
/// Permite ao jogador navegar pela mansão, coletando pistas automaticamente.
/// ----------------------------------------------------------------------
void explorarSalasComPistas(struct Sala* atual, struct Pista** arvorePistas) {
    char escolha;

    printf("\nBem-vindo à exploração da mansão misteriosa!\n");

    while (atual != NULL) {
        printf("\nVocê está na: %s\n", atual->nome);

        // Se a sala tiver uma pista, coleta automaticamente
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        // Mostra opções de caminho
        printf("\nEscolha o caminho:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Ir para a esquerda → %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Ir para a direita  → %s\n", atual->direita->nome);
        printf("  (s) Sair da mansão\n");
        printf("Digite sua escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda != NULL)
                atual = atual->esquerda;
            else
                printf("Não há sala à esquerda!\n");
        } 
        else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita != NULL)
                atual = atual->direita;
            else
                printf("Não há sala à direita!\n");
        } 
        else if (escolha == 's' || escolha == 'S') {
            printf("\nVocê decidiu encerrar a exploração.\n");
            break;
        } 
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

/// ----------------------------------------------------------------------
/// Função principal: main()
/// Monta o mapa da mansão, inicia a exploração e exibe as pistas coletadas.
/// ----------------------------------------------------------------------
int main() {
    // Criação da árvore da mansão (mapa fixo)
    struct Sala* hallEntrada = criarSala("Hall de Entrada", "O relógio marca meia-noite...");
    struct Sala* salaEstar   = criarSala("Sala de Estar", "Um retrato antigo parece te observar.");
    struct Sala* cozinha     = criarSala("Cozinha", "Uma faca afiada foi deixada sobre a mesa.");
    struct Sala* jardim      = criarSala("Jardim", "Pegadas levam até o portão trancado.");
    struct Sala* biblioteca  = criarSala("Biblioteca", "Um livro aberto fala sobre segredos da mansão.");
    struct Sala* escritorio  = criarSala("Escritório", "Um cofre está escondido atrás do quadro.");

    // Estrutura da árvore da mansão
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita  = cozinha;
    salaEstar->esquerda   = jardim;
    salaEstar->direita    = biblioteca;
    cozinha->direita      = escritorio;

    // Criação da árvore de pistas (inicialmente vazia)
    struct Pista* arvorePistas = NULL;

    // Inicia a exploração
    explorarSalasComPistas(hallEntrada, &arvorePistas);

    // Exibe todas as pistas coletadas
    printf("\n===== PISTAS COLETADAS (em ordem alfabética) =====\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    // Libera memória (simples — sem destruição recursiva para simplificar)
    free(hallEntrada);
    free(salaEstar);
    free(cozinha);
    free(jardim);
    free(biblioteca);
    free(escritorio);

    printf("\nFim do jogo. Até a próxima investigação!\n");
    return 0;
}
