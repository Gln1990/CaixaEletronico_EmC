#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <mysql.h>

#define MAX_EXTRATO 100

// Estrutura para controlar o estado do Caixa
typedef struct {
    float saldoTotal;
    float cotaMinima;
    float extrato[MAX_EXTRATO];
    int qtdOperacoes;
    int raLogado;
    char senhaLogada[20];
} CaixaEletronico;

// --- FUNÇÕES DE BANCO DE DADOS ---

int realizarLogin(CaixaEletronico *ce, int ra, char *senhaDigitada) {
    MYSQL *conn = mysql_init(NULL);
    int sucesso = 0;

    if (mysql_real_connect(conn, "localhost", "root", "", "teste_conexao", 3306, NULL, 0)) {
        char query[300];
        sprintf(query, "SELECT saldo, senha FROM contas WHERE ra = %d", ra);

        if (!mysql_query(conn, query)) {
            MYSQL_RES *res = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(res);

            if (row) {
                char *senhaNoBanco = row[1];
                if (strcmp(senhaDigitada, senhaNoBanco) == 0) {
                    // Garante que o atof use o ponto decimal do banco
                    setlocale(LC_NUMERIC, "C");
                    ce->saldoTotal = atof(row[0]);
                    setlocale(LC_NUMERIC, "Portuguese"); // Volta para o padrão visual

                    ce->raLogado = ra;
                    strcpy(ce->senhaLogada, senhaDigitada);
                    sucesso = 1;
                } else {
                    printf("[ERRO] Senha incorreta!\n");
                }
            } else {
                printf("[ERRO] RA nao encontrado!\n");
            }
            mysql_free_result(res);
        }
        mysql_close(conn);
    } else {
        printf("[ERRO] Falha ao conectar ao servidor MySQL.\n");
    }
    return sucesso;
}

void sincronizarBanco(int ra, float novoSaldo) {
    MYSQL *conn = mysql_init(NULL);
    if (mysql_real_connect(conn, "localhost", "root", "", "teste_conexao", 3306, NULL, 0)) {
        char query[200];

        // CRITICAL: Força o ponto decimal para o SQL não quebrar com vírgula
        setlocale(LC_NUMERIC, "C");
        sprintf(query, "UPDATE contas SET saldo = %.2f WHERE ra = %d", novoSaldo, ra);
        setlocale(LC_NUMERIC, "Portuguese");

        if (mysql_query(conn, query) != 0) {
            printf("[ERRO] Falha no SQL: %s\n", mysql_error(conn));
        }
        mysql_close(conn);
    } else {
        printf("[ERRO] Falha de conexao ao sincronizar.\n");
    }
}

// --- FUNÇÕES DO CAIXA ---

void realizarSaque(CaixaEletronico *ce) {
    int valor;
    printf("\nValor do saque: ");
    scanf("%d", &valor);

    if(valor <= 0 || valor == 1 || valor == 3) {
        printf("[ERRO] Valor invalido para as notas disponiveis.\n");
        return;
    }

    if (ce->saldoTotal - valor < ce->cotaMinima) {
        printf("[BLOQUEADO] Saldo insuficiente (Cota Minima de R$ %.2f).\n", ce->cotaMinima);
        return;
    }

    // Atualiza localmente
    ce->saldoTotal -= valor;
    ce->extrato[ce->qtdOperacoes] = (float)-valor;
    ce->qtdOperacoes++;

    // Salva no Banco
    sincronizarBanco(ce->raLogado, ce->saldoTotal);
    printf("[SUCESSO] Saque realizado e salvo no banco!\n");
}

void realizarDeposito(CaixaEletronico *ce) {
    float valor;
    printf("\nValor do deposito: R$ ");
    scanf("%f", &valor);

    if(valor > 0) {
        ce->saldoTotal += valor;
        ce->extrato[ce->qtdOperacoes] = valor;
        ce->qtdOperacoes++;

        sincronizarBanco(ce->raLogado, ce->saldoTotal);
        printf("[SUCESSO] Deposito salvo no banco!\n");
    }
}

void exibirExtrato(CaixaEletronico ce) {
    printf("\n--- EXTRATO DA SESSAO (RA: %d) ---", ce.raLogado);
    if(ce.qtdOperacoes == 0) printf("\nNenhuma operacao realizada.");

    for(int i = 0; i < ce.qtdOperacoes; i++) {
        if(ce.extrato[i] > 0)
            printf("\n[%d] DEPOSITO: + R$ %.2f", i+1, ce.extrato[i]);
        else
            printf("\n[%d] SAQUE:    - R$ %.2f", i+1, -ce.extrato[i]);
    }
    printf("\nSALDO ATUAL: R$ %.2f", ce.saldoTotal);
    printf("\n----------------------------------\n");
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    system("chcp 1252 > nul"); // Ajusta acentuação no terminal Windows

    CaixaEletronico meuCaixa;
    meuCaixa.cotaMinima = 100.00;
    meuCaixa.qtdOperacoes = 0;

    int ra;
    char senha[20];
    int logado = 0;

    printf("--- BANCO ADS - ACESSO AO CAIXA ---\n");
    printf("RA: ");
    scanf("%d", &ra);
    printf("Senha: ");
    scanf("%s", senha);

    // Tenta realizar o login
    if (realizarLogin(&meuCaixa, ra, senha)) {
        printf("\nLogin realizado com sucesso! Bem-vindo.\n");
        logado = 1;
    } else {
        printf("\nFalha no acesso. Verifique suas credenciais.\n");
        system("pause");
        return 0;
    }

    int opcao;
    if (logado) {
        do {
            printf("\n===========================");
            printf("\n ATM - LOGADO: %d", meuCaixa.raLogado);
            printf("\n Saldo Atual: R$ %.2f", meuCaixa.saldoTotal);
            printf("\n===========================");
            printf("\n1. Saque");
            printf("\n2. Deposito");
            printf("\n3. Extrato");
            printf("\n0. Sair");
            printf("\nEscolha: ");
            scanf("%d", &opcao);

            switch(opcao) {
                case 1: realizarSaque(&meuCaixa); break;
                case 2: realizarDeposito(&meuCaixa); break;
                case 3: exibirExtrato(meuCaixa); break;
                case 0: printf("\nSaindo... Volte sempre!\n"); break;
                default: printf("\nOpcao invalida!\n");
            }
        } while(opcao != 0);
    }

    return 0;
}
