# 🏦 Simulador de Caixa Eletrônico (ATM) - Linguagem C

Este projeto é um simulador de operações bancárias desenvolvido em **Linguagem C**, com foco em lógica de algoritmos financeiros e manipulação de estruturas de dados. O sistema gerencia saldos, realiza saques inteligentes considerando as cédulas disponíveis e mantém um histórico de operações (extrato).

## 🚀 Funcionalidades

- **Gestão de Saldo e Cota Mínima**: O sistema monitora o saldo total e impede saques que violem a reserva mínima de segurança do caixa.
- **Saque Inteligente**: Algoritmo que calcula a distribuição de notas (R$ 100, 50, 20, 10, 5 e 2), evitando restos impossíveis de processar (como R$ 1 e R$ 3).
- **Depósitos Dinâmicos**: Permite a entrada de valores, atualizando o saldo disponível e o histórico.
- **Extrato de Sessão**: Registro em tempo real de todas as entradas e saídas realizadas durante a execução.

## 🛠️ Tecnologias e Ferramentas

<table>
  <tr>
    <th bgcolor="#222" align="center"><font color="#58a6ff">Linguagem</font></th>
    <th bgcolor="#222" align="center"><font color="#58a6ff">Ambiente & Ferramentas</font></th>
  </tr>
  <tr>
    <td align="center" valign="top">
      <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/c/c-original.svg" width="50" height="50" alt="C" />
      <br>C Puro
    </td>
    <td align="center" valign="top">
      <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/vscode/vscode-original.svg" width="50" height="50" alt="VS Code" />
      <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/gcc/gcc-original.svg" width="50" height="50" alt="GCC" />
      <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/git/git-original.svg" width="50" height="50" alt="Git" />
      <br>VS Code | GCC | Git
    </td>
  </tr>
</table>

## 🧠 Conceitos de ADS Aplicados

Como estudante de **Análise e Desenvolvimento de Sistemas**, este projeto foi construído utilizando conceitos fundamentais:

1. **Structs Complexas**: Uso de uma estrutura centralizada (`CaixaEletronico`) para encapsular o estado do sistema (saldo, cota, histórico).
2. **Passagem por Referência (Ponteiros)**: Implementação de funções que alteram o estado original dos dados na memória RAM via endereçamento.
3. **Regras de Negócio**: Validações de segurança para impedir saques inválidos e garantir a integridade financeira do caixa.
4. **Persistência em Memória**: Uso de arrays para armazenamento de logs de transações (extrato).



## 🔧 Como Executar

1. Clone o repositório:
   ```bash
   git clone [https://github.com/SEU_USUARIO/NOME_DO_REPOSITORIO.git](https://github.com/SEU_USUARIO/NOME_DO_REPOSITORIO.git)
