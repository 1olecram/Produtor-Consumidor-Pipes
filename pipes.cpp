#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

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
        char buffer[100];
        
        // Lê até 100 bytes do pipe (fd[0]) e os coloca no 'buffer'.
        read(fd[0], buffer, 100);
        
        std::cout << "Child received: " << buffer << std::endl;
        
        // Fecha a extremidade de leitura após o uso.
        close(fd[0]);
    } else {
        // Código do Processo Pai
        // O pai só vai escrever no pipe, então fechamos a extremidade de leitura (fd[0]).
        close(fd[0]); 
        
        // 'msg' é um ponteiro para a string (mensagem) que será enviada pelo pipe.
        const char* msg = "Hello from Parent!";
        
        // Escreve os 19 bytes da mensagem 'msg' no pipe usando a extremidade de escrita (fd[1]).
        write(fd[1], msg, 19);
        
        // Fecha a extremidade de escrita após enviar a mensagem.
        close(fd[1]);
        
        // O pai espera (wait) até que o processo filho termine sua execução
        // para evitar que ele se torne um processo zumbi.
        wait(NULL); 
    }
    return 0;
}
