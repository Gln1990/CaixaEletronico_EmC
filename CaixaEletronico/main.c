#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <mysql.h>
#include <conio.h>

void configurarJanela() {
    // Define o título da janela
    SetConsoleTitle("BANCO ADS - SISTEMA DE AUTOATENDIMENTO");

    // Ajusta o tamanho do buffer e da janela (Largura, Altura)
    // 120 colunas é um ótimo tamanho para a linha que você criou
    system("mode con: cols=125 lines=35");
}

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

// --- PROTÓTIPOS ---
void cabecalho(CaixaEletronico *ce);
void sincronizarBanco(int ra, float novoSaldo);
int realizarLogin(CaixaEletronico *ce, int ra, char *senhaDigitada);
void realizarSaque(CaixaEletronico *ce);
void realizarDeposito(CaixaEletronico *ce);
void exibirExtrato(CaixaEletronico ce);

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
                if (strcmp(senhaDigitada, row[1]) == 0) {
                    setlocale(LC_NUMERIC, "C");
                    ce->saldoTotal = atof(row[0]);
                    setlocale(LC_NUMERIC, "Portuguese");
                    ce->raLogado = ra;
                    strcpy(ce->senhaLogada, senhaDigitada);
                    sucesso = 1;
                } else {
                    printf("\n\033[1;31m[ERRO] Senha incorreta!\033[0m\n");
                }
            } else {
                printf("\n\033[1;31m[ERRO] RA nao encontrado!\033[0m\n");
            }
            mysql_free_result(res);
        }
        mysql_close(conn);
    } else {
        printf("\n\033[1;31m[ERRO] Falha ao conectar ao MySQL.\033[0m\n");
    }
    return sucesso;
}

void sincronizarBanco(int ra, float novoSaldo) {
    MYSQL *conn = mysql_init(NULL);
    if (mysql_real_connect(conn, "localhost", "root", "", "teste_conexao", 3306, NULL, 0)) {
        char query[200];
        setlocale(LC_NUMERIC, "C");
        sprintf(query, "UPDATE contas SET saldo = %.2f WHERE ra = %d", novoSaldo, ra);
        setlocale(LC_NUMERIC, "Portuguese");
        mysql_query(conn, query);
        mysql_close(conn);
    }
}

// --- FUNÇÕES DE INTERFACE E OPERAÇÕES ---

void cabecalho(CaixaEletronico *ce) {
    system("cls");
    printf("\033[1;34m");
    printf("\xDB========================================================================================================================\xDB\n");
    printf("                                            BANCO ADS - ATM DIGITAL               \n");
    printf("========================================================================================================================\n");
    if(ce->raLogado != 0) {
        printf("\n                                   RA: %-10d | SALDO: \033[1;32mR$ %.2f\033[0m\n", ce->raLogado, ce->saldoTotal);
        printf("\033[1;34m                              ----------------------------------------------------\033[0m\n");
    }
}

void realizarSaque(CaixaEletronico *ce) {
    int valor, aux;
    int n100=0, n50=0, n20=0, n10=0, n5=0, n2=0;

    printf("\n                                                             Digite o valor para saque: R$ ");
    scanf("%d", &valor);

    if (valor <= 0 || valor == 1 || valor == 3) {
        printf("\n\033[1;31m                                              [ERRO] Valor indisponivel para as notas atuais.\033[0m\n");
        return;
    }

    if (ce->saldoTotal - valor < ce->cotaMinima) {
        printf("\n\033[1;31m                                            [BLOQUEADO] Saldo insuficiente para manter cota minima.\033[0m\n");
        return;
    }

    aux = valor;
    if (aux % 2 != 0 && aux >= 5) { n5 = 1; aux -= 5; }

    n100 = aux / 100; aux %= 100;
    n50  = aux / 50;  aux %= 50;
    n20  = aux / 20;  aux %= 20;
    n10  = aux / 10;  aux %= 10;
    n2   = aux / 2;   aux %= 2;

    if (aux > 0) {
        printf("\n\033[1;31m[ERRO] Nao ha notas para este valor.\033[0m\n");
        return;
    }

    ce->saldoTotal -= valor;
    ce->extrato[ce->qtdOperacoes++] = (float)-valor;
    sincronizarBanco(ce->raLogado, ce->saldoTotal);

    printf("\n\033[1;33m--- NOTAS SACADAS ---\033[0m\n");
    if(n100) printf("                                      Notas de 100: %d\n", n100);
    if(n50)  printf("                                      Notas de  50: %d\n", n50);
    if(n20)  printf("                                      Notas de  20: %d\n", n20);
    if(n10)  printf("                                      Notas de  10: %d\n", n10);
    if(n5)   printf("                                      Notas de   5: %d\n", n5);
    if(n2)   printf("                                      Notas de   2: %d\n", n2);
    printf("\033[1;32m                                    SAQUE REALIZADO COM SUCESSO!\033[0m\n");
}

void realizarDeposito(CaixaEletronico *ce) {
    float valor;
    printf("\n\033[1;33m                             --- OPERACAO DE DEPOSITO ---\033[0m");
    printf("\n                                       Digite o valor do deposito: R$ ");
    scanf("%f", &valor);

    if (valor > 0) {
        ce->saldoTotal += valor;
        if (ce->qtdOperacoes < MAX_EXTRATO) {
            ce->extrato[ce->qtdOperacoes] = valor;
            ce->qtdOperacoes++;
        }
        sincronizarBanco(ce->raLogado, ce->saldoTotal);
        printf("\n\033[1;32m                                        [SUCESSO] Deposito de R$ %.2f realizado!\033[0m\n", valor);
    } else {
        printf("\n\033[1;31m                                         [ERRO] Valor de deposito invalido.\033[0m\n");
    }
}

void exibirExtrato(CaixaEletronico ce) {
    printf("\n\033[1;33m--- EXTRATO DA SESSAO ---\033[0m\n");
    if (ce.qtdOperacoes == 0) printf(" Nenhuma operacao realizada.\n");
    for(int i = 0; i < ce.qtdOperacoes; i++) {
        if(ce.extrato[i] > 0)
            printf("                                     [+] DEPOSITO: R$ %7.2f\n", ce.extrato[i]);
        else
            printf("                                     [-] SAQUE:    R$ %7.2f\n", -ce.extrato[i]);
    }
    printf("-------------------------\n");
}

int main() {
    configurarJanela();
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "Portuguese");

    // Inicialização correta da estrutura
    CaixaEletronico meuCaixa = {0.0, 100.0, {0}, 0, 0, ""};
    int ra, opcao;
    char senha[20];
    printf("\033[1;34m");
    cabecalho(&meuCaixa);
    printf("\n                                            LOGIN ACESSO\n");
    printf("\n                                            RA: "); scanf("%d", &ra);
    printf("\n                                            SENHA: "); scanf("%s", senha);

    if (realizarLogin(&meuCaixa, ra, senha)) {
        do {
            cabecalho(&meuCaixa);

            printf("                                \033[1;34m1.\033[1;34mSAQUE\n");
            printf("                                \033[1;34m2.\033[1;34mDEPOSITO\n");
            printf("                                \033[1;34m3.\033[1;34mEXTRATO\n");
            printf("                                \033[1;31m0.\033[1;34mSAIR\n");
            printf("\n                              Escolha: ");
            scanf("%d", &opcao);

            switch(opcao) {
                case 1: realizarSaque(&meuCaixa); break;
                case 2: realizarDeposito(&meuCaixa); break; // CORRECAO: Passar endereço (&)
                case 3: exibirExtrato(meuCaixa); break;
                case 0: printf("\n                                          Obrigado por usar o Banco ADS!\n"); break;
                default: printf("\n                                         Opcao invalida!\n");
            }
            if(opcao != 0) {
                printf("\n                                                  Pressione qualquer tecla para continuar...");
                getch();
            }
        } while(opcao != 0);
    }

    return 0;
}
