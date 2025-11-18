////////* Bibliotecas *//////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Permite Abstração*/
typedef struct Sala {
    char nome[100];
    char pista[200];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// nó de pistas
typedef struct PistaNode {
    char pista[200];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

/*criarSala() – cria dinamicamente um cômodo com ou sem pista.
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));

    if (novaSala == NULL) {
        fprintf(stderr, "Falha ao alocar memória para nova sala.\n");
        exit(EXIT_FAILURE);
    }

    strncpy(novaSala->nome, nome, sizeof(novaSala->nome) - 1);
    novaSala->nome[sizeof(novaSala->nome) - 1] = '\0';

    strncpy(novaSala->pista, pista, sizeof(novaSala->pista) - 1);
    novaSala->pista[sizeof(novaSala->pista) - 1] = '\0';

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/*
 * inserirPista() – insere uma nova pista na árvore de pistas .
 */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        if (novo == NULL) {
            fprintf(stderr, "Falha ao alocar memória para nova pista.\n");
            exit(EXIT_FAILURE);
        }
        strncpy(novo->pista, pista, sizeof(novo->pista) - 1);
        novo->pista[sizeof(novo->pista) - 1] = '\0';
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se igual não duplica

    return raiz;
}

/*
 * exibirPistas() Percorre a árvore em ordem (inorder) para exibir as pistas alfabeticamente.
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/*
 * liberarPistas() Realiza limpeza recursiva da memória.
 */
void liberarPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
} 

/*explorarSalasComPistas() – controla a navegação entre salas e coleta de pistas.
 */
void explorarSalasComPistas(Sala* sala, PistaNode** pistasColetadas) {
    if (sala == NULL) {
        printf("\nVocê chegou ao fim de um corredor.\n");
        return;
    }

    printf("\n========================================\n");
    printf("Você está em: %s\n", sala->nome);
    if (strlen(sala->pista) > 0) {
        printf("Pista encontrada: %s\n", sala->pista);
        *pistasColetadas = inserirPista(*pistasColetadas, sala->pista);
    } else {
        printf("Nenhuma pista nesta sala.\n");
    }
    printf("========================================\n");

    if (sala->esquerda == NULL && sala->direita == NULL) {
        printf("Sala escura tocando musíca de terror e sem saída!\n");
        return;
    }

    char escolha;
    int validoEsquerda = (sala->esquerda != NULL);
    int validoDireita = (sala->direita != NULL);

    printf("\nEscolha uma direção:\n");
    if (validoEsquerda) {
        printf("  (e) Ir para a esquerda\n");
    }
    if (validoDireita) {
        printf("  (d) Ir para a direita\n");
    }
    printf("  (s) Sair da exploração\n");
    printf("\nDigite sua escolha: ");

    scanf(" %c", &escolha);

    switch (escolha) {
        case 'e':
        case 'E':
            if (validoEsquerda) {
                explorarSalasComPistas(sala->esquerda, pistasColetadas);
            } else {
                printf("\nTenta a direita vai que dá sorte\n");
                explorarSalasComPistas(sala, pistasColetadas);
            }
            break;

        case 'd':
        case 'D':
            if (validoDireita) {
                explorarSalasComPistas(sala->direita, pistasColetadas);
            } else {
                printf("\nTenta a esquerda vai que dá sorte\n");
                explorarSalasComPistas(sala, pistasColetadas);
            }
            break;

        case 's':
        case 'S':
            printf("\nVocê Saiu, Parabens!\n");
            break;

        default:
            printf("\nOpção inválida! Digite 'e', 'd' ou 's'.\n");
            explorarSalasComPistas(sala, pistasColetadas);
            break;
    }
}

/*
 * liberarMansao() Realiza limpeza recursiva da memória.
 */
void liberarMansao(Sala* sala) {
    if (sala != NULL) {
        liberarMansao(sala->esquerda);
        liberarMansao(sala->direita);
        free(sala);
    }
}

/*
 * main() – Monta o mapa
 */
int main() {
  
    printf("Este texto é vermelho sobre fundo cinza.\n");
    printf("╔═══════════════════════════════════════=╗\n");
    printf("║     Bem vindo a Mansão                 ║\n");
    printf("║                                        ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    printf("Instruções: Use 'e' para ir à esquerda, 'd' para ir à direita,\n");
    printf("ou 's' para sair a qualquer momento.\n");
    printf("════════════════════════════════════════\n");
   
    printf("Este texto volta ao padrão.\n");

    // Criar salas com pistas
    Sala* hallEntrada = criarSala("Hall de Entrada", "Uma chave antiga repousa no chão.");
    hallEntrada->esquerda = criarSala("Sala dde Estar", "tlvz tenha algum enigma aqui.");
    hallEntrada->direita = criarSala("Cozinha", "Um bilhete rasgado na mesa.");

    hallEntrada->esquerda->esquerda = criarSala("Biblioteca", "Um diário com anotações misteriosas.");
    hallEntrada->esquerda->direita = criarSala("Arquivo Secreto", "Documentos confidenciais escondidos.");

    hallEntrada->direita->esquerda = criarSala("Armário", "Embalagens suspeitas🤨.");
    hallEntrada->direita->direita = criarSala("àrea", "Ferramentas manchadas de tinta🤨.");

    // Inicializar pistas coletadas
    PistaNode* pistasColetadas = NULL;

    // Explorar e coletar pistas
    explorarSalasComPistas(hallEntrada, &pistasColetadas);

    // Exibir pistas 
    printf("\n========================================\n");
    printf("Pistas coletadas (ordem alfabética)abcdefg:\n");
    printf("========================================\n");
    exibirPistas(pistasColetadas);

    // Liberar memória
    liberarMansao(hallEntrada);
    liberarPistas(pistasColetadas);

    printf("\nPrograma finalizado. Memória liberada com sucesso.\n");
    return 0;
}
