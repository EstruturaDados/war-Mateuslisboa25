#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ===============================================================
// Estrutura que representa um território no jogo
// ===============================================================
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// ===============================================================
// Função: cadastrarTerritorios
// Cadastra todos os territórios do mapa
// ===============================================================
void cadastrarTerritorios(Territorio* mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);
        printf("Cor do exército: ");
        scanf(" %[^\n]", mapa[i].cor);
        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// ===============================================================
// Função: exibirMapa
// Exibe todos os territórios cadastrados com seus dados
// ===============================================================
void exibirMapa(Territorio* mapa, int qtd) {
    printf("\n=== MAPA DE TERRITÓRIOS ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("\n[%d] %s\n", i, mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}

// ===============================================================
// Função: atacar
// Simula uma batalha entre dois territórios com dados aleatórios
// ===============================================================
void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n❌ Você não pode atacar um território da mesma cor!\n");
        return;
    }

    if (atacante->tropas < 2) {
        printf("\n⚠️ O território atacante precisa de pelo menos 2 tropas!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n🎲 Dados lançados!\n");
    printf("%s (Atacante) tirou: %d\n", atacante->nome, dadoAtacante);
    printf("%s (Defensor) tirou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\n🏆 Ataque bem-sucedido!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;
    } else {
        printf("\n💥 O defensor resistiu! O atacante perde uma tropa.\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

// ===============================================================
// Função: atribuirMissao
// Sorteia uma missão aleatória e a atribui ao jogador
// ===============================================================
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// ===============================================================
// Função: exibirMissao
// Mostra a missão atual do jogador
// ===============================================================
void exibirMissao(char* missao) {
    printf("\n🎯 Sua missão é: %s\n", missao);
}

// ===============================================================
// Função: verificarMissao
// Verifica se a missão do jogador foi cumprida
// ===============================================================
// Obs: A lógica é simplificada — ajustável conforme o jogo evolui
int verificarMissao(char* missao, Territorio* mapa, int qtd, char* corJogador) {
    int cont = 0;

    // Exemplo de missões simples
    if (strcmp(missao, "Conquistar 3 territorios seguidos") == 0) {
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0)
                cont++;
        }
        if (cont >= 3) return 1;
    }

    if (strcmp(missao, "Eliminar todas as tropas da cor vermelha") == 0) {
        int existeVermelho = 0;
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0) {
                existeVermelho = 1;
                break;
            }
        }
        if (!existeVermelho) return 1;
    }

    if (strcmp(missao, "Controlar metade do mapa") == 0) {
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0)
                cont++;
        }
        if (cont >= qtd / 2) return 1;
    }

    return 0;
}

// ===============================================================
// Função: liberarMemoria
// Libera memória de missões e territórios
// ===============================================================
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int numJogadores) {
    free(mapa);
    for (int i = 0; i < numJogadores; i++) {
        free(missoesJogadores[i]);
    }
    free(missoesJogadores);
    printf("\n🧹 Memória liberada com sucesso!\n");
}

// ===============================================================
// Função principal (main)
// ===============================================================
int main() {
    srand(time(NULL));

    int qtdTerritorios, numJogadores;

    printf("Digite o número de territórios do mapa: ");
    scanf("%d", &qtdTerritorios);

    // Alocação dinâmica dos territórios
    Territorio* mapa = (Territorio*) calloc(qtdTerritorios, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, qtdTerritorios);
    exibirMapa(mapa, qtdTerritorios);

    // Cadastro dos jogadores
    printf("\nDigite o número de jogadores: ");
    scanf("%d", &numJogadores);

    // Vetor de missões pré-definidas
    char* missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar metade do mapa",
        "Dominar todos os territórios azuis",
        "Ter pelo menos 10 tropas em um território"
    };
    int totalMissoes = 5;

    // Alocação dinâmica das missões dos jogadores
    char** missoesJogadores = (char**) malloc(numJogadores * sizeof(char*));
    for (int i = 0; i < numJogadores; i++) {
        missoesJogadores[i] = (char*) malloc(100 * sizeof(char));
        atribuirMissao(missoesJogadores[i], missoes, totalMissoes);
        printf("\n=== Jogador %d ===", i + 1);
        exibirMissao(missoesJogadores[i]);
    }

    // Laço principal do jogo
    char continuar;
    int atacante, defensor, jogadorAtual = 0;

    do {
        printf("\n--- Turno do Jogador %d ---\n", jogadorAtual + 1);
        exibirMapa(mapa, qtdTerritorios);

        printf("\nEscolha o território atacante: ");
        scanf("%d", &atacante);
        printf("Escolha o território defensor: ");
        scanf("%d", &defensor);

        atacar(&mapa[atacante], &mapa[defensor]);

        // Verifica se o jogador atual venceu
        if (verificarMissao(missoesJogadores[jogadorAtual], mapa, qtdTerritorios, mapa[atacante].cor)) {
            printf("\n🎉 PARABÉNS! O jogador %d cumpriu sua missão: %s\n", 
                   jogadorAtual + 1, missoesJogadores[jogadorAtual]);
            break;
        }

        printf("\nDeseja continuar o jogo? (s/n): ");
        scanf(" %c", &continuar);

        jogadorAtual = (jogadorAtual + 1) % numJogadores; // alterna jogador

    } while (continuar == 's' || continuar == 'S');

    liberarMemoria(mapa, missoesJogadores, numJogadores);

    printf("\n🏁 Fim do jogo!\n");

    return 0;
}
