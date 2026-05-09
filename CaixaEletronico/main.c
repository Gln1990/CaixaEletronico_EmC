#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>


#define MAX_EXTRATO 100

// Estrutura para controlar o estado do Caixa
typedef struct{
    float saldoTotal;
    float cotaMinima;
    float extrato[MAX_EXTRATO];
    int qtdOperacoes;
} CaixaEletronico;

//Prototipos de funcao
void realizarSaque(CaixaEletronico *ce);
void realizarDeposito(CaixaEletronico *ce);
void exibirExtrato(CaixaEletronico ce);

int main()
{
    setlocale(LC_ALL, "Portuguese");

    //Inicialização do Caixa
    CaixaEletronico meuCaixa;
    meuCaixa.saldoTotal = 1000.00; // começa com R$1000
    meuCaixa.cotaMinima = 100.00; //Não pode sacar se sobrar menos de R$100
    meuCaixa.qtdOperacoes = 0;

    int opcao;

    do{
        printf("\n===========================");
        printf("\n ATM SYSTEM - BANCO ADS");
        printf("\n Saldo Atual: r$ %.2F", meuCaixa.saldoTotal);
        printf("\n===========================");
        printf("\n1. Saque");
        printf("\n2. Depósito");
        printf("\n3. Extrato");
        printf("\n0. Sair");
        printf("\nEscolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: realizarSaque(&meuCaixa); break;
            case 2: realizarDeposito(&meuCaixa); break;
            case 3: exibirExtrato(meuCaixa); break;
        }

    }while(opcao != 0);

    return 0;
}

void realizarSaque(CaixaEletronico *ce){
    int valor;
    printf("\nValor do saque: ");
    scanf("%d", &valor);

    // Validação de regras de negócio
    if(valor <= 0 || valor == 1 || valor == 3){
        printf("[ERRO] Valor inválido ou notas indisponíveis.\n");
        return;
    }

    if (ce->saldoTotal - valor < ce->cotaMinima){
        printf("[BLOQUEADO] O saque excede a cota mínima de reserva do caixa.\n");
        return;
    }

    // Lógica das cédulas (Simplificada para o exemplo)
    int valorRestante = valor;
    int cedulas[] = {100, 50, 20, 10, 5, 2};

    printf("Notas entregues:\n");
    for(int i = 0; i < 6; i++){
        int qtd = valorRestante / cedulas[i];

        // Regra do resto 1 ou 3
        if (cedulas[i] > 2 && (valorRestante - (qtd * cedulas[i]) == 1 || valorRestante - (qtd * cedulas[i]) == 3) && qtd > 0){
            qtd--;
        }
        if (qtd > 0){
            printf("- %d nota(s) de R$ %d\n", qtd, cedulas[i]);
            valorRestante -= (qtd * cedulas[i]);
        }
    }
    if (valorRestante == 0){
        ce->saldoTotal -= valor;
        ce->extrato[ce->qtdOperacoes] = -valor;
        ce->qtdOperacoes++;
        printf("[SUCESSO] Retire seu dinheiro.\n");
    }else{
        printf("[ERRO] Não foi possível compor o valor exato.\n");
    }

}

void realizarDeposito(CaixaEletronico *ce){
    float valor;
    printf("\nValor do depósito: R$ ");
    scanf("%f", &valor);

    if(valor > 0){
        ce->saldoTotal += valor;
        ce->extrato[ce->qtdOperacoes] = valor;
        ce->qtdOperacoes++;
        printf("[SUCESSO] Depósito realizado!\n");
    }
}

void exibirExtrato(CaixaEletronico ce){
    printf("\n--- EXTRATO DA SESSÃO ---");
    for(int i = 0; i < ce.qtdOperacoes; i++){
        if(ce.extrato[i] > 0)
            printf("\n[%d] DEPÓSITO: + R$ %.2f", i+1, ce.extrato[i]);
        else
            printf("\n[%d] SAQUE:    - R$ %.2f", i+1, -ce.extrato[i]);
    }
    printf("\nTOTAL EM CAIXA: R$ %.2f", ce.saldoTotal);
    printf("\n-------------------------\n");
}
