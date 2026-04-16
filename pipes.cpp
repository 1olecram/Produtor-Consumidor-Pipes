#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <random>
#include <vector>

bool isPrime(int num)
{
    int cnt = 0;
    if (num <= 1)
    {
        return false;
    }
    else
    {
        // Conta os divisores n considerando o quadrados desses numeros -> O(sqrt(n))
        for (int i = 2; i * i <= num; i++)
        {
            if (num % i == 0)
                cnt++;
        }
        // Se n e divisivel por mais de 2
        if (cnt > 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

std::vector<int> numGeneration(int genNums)
{
    std::vector<int> numsToRead;
    // Seed com fonte de entropia via hardware
    std::random_device rd;
    // Inicializacao do gerador Mersenne Twister (standard)
    std::mt19937 gen(rd());
    //∆ ∈ [1, 100]
    std::uniform_int_distribution<> distrib(1, 100);

    //N0 = 1
    int current = 1;
    numsToRead.push_back(current);

    for (int i = 1; i < genNums; i++)
    {
        // Ni = Ni-1 + ∆
        current += distrib(gen);
        numsToRead.push_back(current);
    }

    numsToRead.push_back(0); // Último elemento
    return numsToRead;
}

int main()
{
    const int BUFFER_SIZE = 1000;
    int fd[2];

    // Cria o pipe. Se retornar -1, houve um erro.
    if (pipe(fd) == -1)
        return 1;

    pid_t pid = fork();

    if (pid < 0)
        return 1;

    if (pid == 0)
    {
        // Código do Processo Filho
        close(fd[1]);

        // Agora 'buffer' é um array de inteiros
        int buffer[BUFFER_SIZE];

        // Lê os bytes do pipe diretamente para o array de inteiros
        ssize_t read_bytes = read(fd[0], buffer, sizeof(buffer));

        // Descobre quantos números (inteiros) foram lidos
        int num_elements = read_bytes / sizeof(int);

        std::cout << "Child received: " << std::endl;
        for (int i = 0; i < num_elements && buffer[i] != 0; i++)
        {

            std::cout << buffer[i] << (isPrime(buffer[i]) ? " e primo" : " nao e primo") << std::endl;
        }

        close(fd[0]);
    }
    else
    {
        // Código do Processo Pai
        close(fd[0]);

        // Gera os números para caber no buffer (considerando o 0 no final)
        std::vector<int> nums = numGeneration(BUFFER_SIZE - 1);

        // Escreve os inteiros diretamente no pipe
        write(fd[1], nums.data(), nums.size() * sizeof(int));

        close(fd[1]);
        wait(NULL);
    }
    return 0;
}