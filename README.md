# Comunicação entre Processos com Pipes

Este projeto é uma demonstração de Comunicação entre Processos (IPC - Inter-Process Communication) em C++ utilizando **Anonymous Pipes** (Pipes anônimos) e a chamada de sistema `fork()`.

## Descrição do Projeto

O programa consiste em dois processos que se comunicam através de um pipe unidirecional:

1. **Processo Produtor (Pai):**
   - Gera uma sequência de números inteiros aleatórios e estritamente crescentes utilizando a fórmula:  
     $N_i = N_{i-1} + \Delta$, onde $N_0 = 1$ e $\Delta \in [1, 100]$.
   - Escreve os números gerados na ponta de escrita do pipe.

2. **Processo Consumidor (Filho):**
   - Lê os números continuamente da ponta de leitura do pipe.
   - Para cada número recebido, verifica se ele é um número primo utilizando um algoritmo otimizado de complexidade $O(\sqrt{n})$.

## Como Compilar

Para compilar o código fonte, certifique-se de ter um compilador C++ (como o `g++`) instalado e execute o seguinte comando no terminal:

```bash
g++ -o pipes pipes.cpp
```

## Como Executar

Após a compilação, execute o programa passando a quantidade desejada de números a serem gerados como argumento:

```bash
./pipes <quantidade_de_numeros>
```

### Exemplo de uso:

Para gerar e avaliar 1000 números:

```bash
./pipes 1000
```
