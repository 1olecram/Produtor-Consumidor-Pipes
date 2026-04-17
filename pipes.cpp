#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <random>
#include <vector>
#include <string> 

bool isPrime(int num)
{
    if (num <= 1)
    {
        return false;
    }
    
    // Conta os divisores considerando a raiz quadrada -> O(sqrt(n))
    for (int i = 2; i * i <= num; i++)
    {
        // Se encontrar qualquer divisor, já sabemos que não é primo e retornamos imediatamente
        if (num % i == 0)
            return false;
    }
    return true; // Se passar por todo o laco, é primo
}

std::vector<int> numGeneration(int genNums)
{
    std::vector<int> numsToRead;
    
    // Seed com fonte de entropia via hardware
    std::random_device rd;
    // Inicializacao do gerador Mersenne Twister (standard)
    std::mt19937 gen(rd());
    // ∆ ∈ [1, 100]
    std::uniform_int_distribution<> distrib(1, 100);

    // Só geramos os números se a quantidade pedida for maior que 0
    if (genNums > 0) {
        // N0 = 1
        int current = 1;
        numsToRead.push_back(current);

        for (int i = 1; i < genNums; i++)
        {
            // Ni = Ni-1 + ∆
            current += distrib(gen);
            numsToRead.push_back(current);
        }
    }

    // Último elemento (condição de parada)
    numsToRead.push_back(0); 
    return numsToRead;
}

int main(int argc, char *argv[])
{
    // 1. Verificação de segurança na passagem de argumentos
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <quantidade_de_numeros>" << std::endl;
        return 1;
    }

    int amount_numbers = std::stoi(argv[1]);
   
    // Processos do pipe de escrita e leitura
    int fd[2];

    // Cria o pipe. Se retornar -1, houve um erro.
    if (pipe(fd) == -1) {
        std::cerr << "Falha ao criar o pipe" << std::endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Falha no fork" << std::endl;
        return 1;
    }

    if (pid == 0)
    {
        // Código do Processo Consumidor
        close(fd[1]); // Fecha a ponta de escrita

        // Lê iterativamente um inteiro por vez diretamente do pipe
        int recebido;
        while (read(fd[0], &recebido, sizeof(int)) > 0)
        {
            if (recebido == 0) {
                break; // Condicao de parada ao receber 0
            }
            std::cout << recebido << (isPrime(recebido) ? " e primo" : " nao e primo") << std::endl;
        }

        close(fd[0]);
    }
    else
    {
        // Código do Processo Produtor (Pai)
        close(fd[0]); // Fecha a ponta de leitura

        std::vector<int> nums = numGeneration(amount_numbers);

        // Escreve todos os inteiros diretamente no pipe
        write(fd[1], nums.data(), nums.size() * sizeof(int));

        close(fd[1]);
        wait(NULL); // Espera o processo filho finalizar
    }
    return 0;
}