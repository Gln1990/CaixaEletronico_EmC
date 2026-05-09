🏦 ATM System - Banco ADS (C + MySQL)

Este projeto é um sistema de terminal bancário desenvolvido em Linguagem C integrado ao banco de dados MySQL/MariaDB. O software gerencia autenticação de usuários, persistência de saldos em tempo real e regras complexas de saque e depósito.

🚀 Funcionalidades
Autenticação de Usuário: Sistema de login via RA (Registro Acadêmico) e Senha consultados diretamente no banco de dados.

Persistência de Dados: Integração com MySQL/MariaDB para garantir que o saldo e as informações não sejam perdidos ao fechar o programa.

Saque Inteligente: Algoritmo que calcula a distribuição de notas (R$ 100, 50, 20, 10, 5 e 2), com validação de cota mínima de reserva do caixa.

Sincronização em Tempo Real: Cada operação financeira realiza um UPDATE imediato no banco de dados, garantindo a integridade dos dados.

Extrato de Sessão: Registro detalhado de todas as movimentações realizadas durante o acesso.

🛠️ Tecnologias e Ferramentas
🧠 Conceitos de ADS Aplicados
Como estudante de Análise e Desenvolvimento de Sistemas, apliquei neste projeto:

Integração C/MySQL: Uso da biblioteca mysql.h para realizar operações de CRUD (Create, Read, Update).

Manipulação de Memória: Uso de Ponteiros para passagem de dados entre funções e structs para encapsulamento.

Internacionalização (i18n): Tratamento de setlocale para compatibilidade de caracteres especiais e separadores decimais entre C e SQL.

Segurança de Fluxo: Lógica de login com autenticação cruzada e tratamento de erros de conexão com o banco.

🗄️ Estrutura do Banco de Dados
Para rodar o projeto, é necessário criar a seguinte estrutura no MySQL:

SQL

CREATE DATABASE teste_conexao;

USE teste_conexao;

CREATE TABLE contas (
    id INT AUTO_INCREMENT PRIMARY KEY,
    ra INT UNIQUE NOT NULL,
    nome VARCHAR(100) NOT NULL,
    senha VARCHAR(20) NOT NULL,
    saldo DECIMAL(10, 2) DEFAULT 0.00
);


🔧 Como Executar
Requisito: Ter o MySQL ou MariaDB instalado e rodando.

Configuração: Adicione o MySQL Connector/C ao seu compilador (Search Directories & Linker Settings).

DLL: Coloque a libmysql.dll na pasta raiz do executável.

Clone o repositório e compile o projeto:

Bash
git clone https://github.com/Gln1990/CaixaEletronico_EmC.git
