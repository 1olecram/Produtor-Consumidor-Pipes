#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <random>
#include <string>

int numGeneration(){
    // 1. Seed with a real hardware entropy source
    std::random_device rd; 
    
    // 2. Initialize a generator (Mersenne Twister is standard)
    std::mt19937 gen(rd()); 

    // 3. Define a range (e.g., integers between 1 and 100)
    std::uniform_int_distribution<> distrib(1, 100); 

    return distrib(gen);
}

int main() {
    // fd (file descriptor) é um array de 2 inteiros usado para armazenar os
    // identificadores do pipe.
    // fd[0] é usado para leitura (read end).
    // fd[1] é usado para escrita (write end).
    int fd[2];
    
    // Cria o pipe. Se retornar -1, houve um erro.
    if (pipe(fd) == -1) return 1;

    // A função fork() cria um novo processo (processo filho).
    // 'pid' armazenará o valor de retorno do fork(), que nos dirá
    // se estamos executando o código como o processo pai ou filho.
    pid_t pid = fork();

    // Se pid for menor que 0, houve um erro ao tentar criar o processo filho.
    if (pid < 0) return 1;

    if (pid == 0) {
        // Código do Processo Filho
        // O filho só vai ler do pipe, então fechamos a extremidade de escrita (fd[1]).
        close(fd[1]); 
        
        // 'buffer' é um array de caracteres onde armazenaremos a mensagem lida do pipe.
        char buffer[20];
        
        // Lê até 20 bytes do pipe (fd[0]) e os coloca no 'buffer'.
        read(fd[0], buffer, 20);
        
        std::cout << "Child received: " << buffer << std::endl;
        
        // Fecha a extremidade de leitura após o uso.
        close(fd[0]);
    } else {
        // Código do Processo Pai
        // O pai só vai escrever no pipe, então fechamos a extremidade de leitura (fd[0]).
        close(fd[0]); 
        
        // Converte o número para string e ajusta o tamanho para 20 bytes
        std::string s_msg = std::to_string(numGeneration());
        s_msg.resize(20, '\0'); // Preenche o resto com null bytes
        const char* msg = s_msg.c_str();
        
        // Escreve os 20 bytes da mensagem 'msg' no pipe usando a extremidade de escrita (fd[1]).
        write(fd[1], msg, 20);
        
        // Fecha a extremidade de escrita após enviar a mensagem.
        close(fd[1]);
        
        // O pai espera (wait) até que o processo filho termine sua execução
        // para evitar que ele se torne um processo zumbi.
        wait(NULL); 
    }
    return 0;
}
