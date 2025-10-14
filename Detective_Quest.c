#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Estrutura que representa uma sala da mansão
struct Sala {
    char nome[50];
    struct Sala* esquerda;  // Caminho à esquerda
    struct Sala* direita;   // Caminho à direita
};

/// Cria uma sala com o nome informado
struct Sala* criarSala(const char* nome) {
    struct Sala* novaSala = (struct Sala*) malloc(sizeof(struct Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/// Função que permite a exploração da mansão
void explorarSalas(struct Sala* atual) {
    char escolha;

    printf("\nBem-vindo à exploração da mansão!\n");

    // Enquanto a sala atual existir
    while (atual != NULL) {
        printf("\nVocê está na: %s\n", atual->nome);

        // Se for uma sala sem saídas (folha), encerra
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou ao fim da exploração! Fim de jogo.\n");
            break;
        }

        // Mostra opções
        printf("Escolha o caminho:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Ir para a esquerda → %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Ir para a direita  → %s\n", atual->direita->nome);
        printf("  (s) Sair da mansão\n");
        printf("Digite sua escolha: ");
        scanf(" %c", &escolha);

        // Navegação
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
            printf("Saindo da mansão...\n");
            break;
        } 
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

/// Função principal – monta a estrutura da mansão e inicia o jogo
int main() {
    // Criação das salas (árvore binária fixa)
    struct Sala* hallEntrada = criarSala("Hall de Entrada");
    struct Sala* salaEstar   = criarSala("Sala de Estar");
    struct Sala* cozinha     = criarSala("Cozinha");
    struct Sala* jardim      = criarSala("Jardim");
    struct Sala* biblioteca  = criarSala("Biblioteca");
    struct Sala* escritorio  = criarSala("Escritório");

    // Conexões (estrutura fixa da mansão)
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita  = cozinha;

    salaEstar->esquerda = jardim;
    salaEstar->direita  = biblioteca;

    cozinha->direita = escritorio;

    // Inicia exploração
    explorarSalas(hallEntrada);

    // Libera memória
    free(hallEntrada);
    free(salaEstar);
    free(cozinha);
    free(jardim);
    free(biblioteca);
    free(escritorio);

    return 0;
}
