////////* Bibliotecas *////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// typedef abstração
typedef struct Sala {
    char nome[100];
    char pista[200];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// typedef abstração
typedef struct PistaNode {
    char pista[200];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

//abstração tabela hash
typedef struct HashItem {
    char pista[200];
    char suspeito[100];
    struct HashItem* prox;
} HashItem;

#define TAM_HASH 20
HashItem* tabelaHash[TAM_HASH] = {0};

// hash (extremamente dificil)
unsigned int hash(const char* s) {
    unsigned int h = 0;
    while (*s) h = h * 31 + *s++;
    return h % TAM_HASH;
}

// pista-suspeito
void inserirNaHash(const char* pista, const char* suspeito) {
    unsigned int i = hash(pista);
    HashItem* novo = malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[i];
    tabelaHash[i] = novo;
}

// encontrar suspeito
char* encontrarSuspeito(const char* pista) {
    unsigned int i = hash(pista);
    HashItem* atual = tabelaHash[i];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->prox;
    }
    return NULL;
}
//criar sala 
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
//cria pistas
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
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

    return raiz;
}
//exibe pistas
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}
//wxplora sala
void explorarSalasComPistas(Sala* sala, PistaNode** pistasColetadas) {
    if (sala == NULL) {
        printf("\nVocê chegou ao fim de um corredor.\n");
        return;
    }
//Painel
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
        printf("Sala escura tocando música de terror e sem saída!\n");
        return;
    }
//Painel
    char escolha;
    int validoEsquerda = (sala->esquerda != NULL);
    int validoDireita = (sala->direita != NULL);

    printf("\nEscolha uma direção:\n");
    if (validoEsquerda) printf("  (e) Esquerdinha papai\n");
    if (validoDireita) printf("  (d) direitinha papai\n");
    printf("  (s) Sair da exploração\n");
    printf("\nDigite sua escolha: ");
    scanf(" %c", &escolha);
//Funcionalidade
    switch (escolha) {
        case 'e': case 'E':
            if (validoEsquerda) explorarSalasComPistas(sala->esquerda, pistasColetadas);
            break;
        case 'd': case 'D':
            if (validoDireita) explorarSalasComPistas(sala->direita, pistasColetadas);
            break;
        case 's': case 'S':
            printf("\nVocê Saiu, Parabéns!\n");
            return;
        default:
            printf("\nOpção inválida!\n");
            explorarSalasComPistas(sala, pistasColetadas);
    }
}

// libera Memória
void liberarPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}
//Libera memória
void liberarMansao(Sala* sala) {
    if (sala != NULL) {
        liberarMansao(sala->esquerda);
        liberarMansao(sala->direita);
        free(sala);
    }
}
//pista ligada ao suspeito
int contarPistasPorSuspeito(PistaNode* r, const char* suspeito) {
    if (!r) return 0;

    int c = 0;
    char* s = encontrarSuspeito(r->pista);
    if (s && strcmp(s, suspeito) == 0) c++;

    return c + contarPistasPorSuspeito(r->esquerda, suspeito)
             + contarPistasPorSuspeito(r->direita, suspeito);
}

// julgamento
void verificarSuspeitoFinal(PistaNode* pistas) {
    char nome[100];
    getchar(); 
    printf("\nDigite o suspeito que deseja acusar: ");
    fgets(nome, 100, stdin);
    nome[strcspn(nome, "\n")] = 0;

    int total = contarPistasPorSuspeito(pistas, nome);

    printf("\nPistas que apontam para %s: %d\n", nome, total);

    if (total >= 2)
        printf("ACUSAÇÃO CONFIRMADA! %s é o culpado.\n", nome);
    else
        printf("ACUSAÇÃO FRACA. %s provavelmente é inocente.\n", nome);
}
//suspeitos
void carregarSuspeitos() {
    inserirNaHash("Dinheiro.", "Alfred");
    inserirNaHash("Sangue.", "Era");
    inserirNaHash("Mesa suja.", "Grayson");
    inserirNaHash("Um diário com anotações misteriosas.", "Alfred");
    inserirNaHash("Documentos confidenciais escondidos.", "Bruce");
    inserirNaHash("Embalagens suspeitas🤨.", "Kara");
    inserirNaHash("Ferramentas manchadas de tinta🤨.", "Alfred");
}

/*
 * main() – Monta o mapa
 */
int main() {
  
    printf("╔═══════════════════════════════════════=╗\n");
    printf("║     Bem vindo a Mansão                 ║\n");
    printf("║                                        ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    printf("Instruções: Use 'e' para ir à esquerda, 'd' para ir à direita,\n");
    printf("ou 's' para sair a qualquer momento.\n");
    printf("════════════════════════════════════════\n");
   
    printf("Este texto volta ao padrão.\n");

    // Criar salas com pistas
    Sala* hallEntrada = criarSala("Hall de Entrada", "Dinheiro.");
    hallEntrada->esquerda = criarSala("Sala dde Estar", "Sangue.");
    hallEntrada->direita = criarSala("Cozinha", "Mesa suja.");

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
