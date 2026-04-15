#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <random>
#include <vector>

std::vector<int> numGeneration(int genNums){
    std::vector<int> numsToRead;
    // 1. Seed with a real hardware entropy source
    std::random_device rd; 
    // 2. Initialize a generator (Mersenne Twister is standard)
    std::mt19937 gen(rd()); 
    // 3. Define a range (e.g., integers between 1 and 100)
    std::uniform_int_distribution<> distrib(1, 100); 

    int current = 1;
    numsToRead.push_back(current);
    std::cout << current << std::endl; // Pai exibe 1 (o primeiro número)

    for (int i = 1; i < genNums; i++)
    {
        // Ni = Ni-1 + ∆, N0 = 1, ∆ ∈ [1, 100].
        current += distrib(gen);
        numsToRead.push_back(current);
    }

    numsToRead.push_back(0); // Último elemento
    return numsToRead;
}

int main() {
    int fd[2];
    
    // Cria o pipe. Se retornar -1, houve um erro.
    if (pipe(fd) == -1) return 1;

    pid_t pid = fork();

    if (pid < 0) return 1;

    if (pid == 0) {
        // Código do Processo Filho
        close(fd[1]); 
        
        // Agora 'buffer' é um array de inteiros
        int buffer[10]; 
        
        // Lê os bytes do pipe diretamente para o array de inteiros
        ssize_t read_bytes = read(fd[0], buffer, sizeof(buffer));
        
        // Descobre quantos números (inteiros) foram lidos
        int num_elements = read_bytes / sizeof(int);
        
        std::cout << "Child received: " << std::endl;
        for (int i = 0; i < num_elements; i++)
        {

            std::cout << buffer[i] << std::endl;
        }
        
        close(fd[0]);
    } else {
        // Código do Processo Pai
        close(fd[0]); 
        
        // Gera os números
        std::vector<int> nums = numGeneration(4);
        
        // Escreve os inteiros diretamente no pipe
        write(fd[1], nums.data(), nums.size() * sizeof(int));
        
        close(fd[1]);
        wait(NULL); 
    }
    return 0;
}