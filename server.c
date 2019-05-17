#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8081


int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Resposta ao GET /
    char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nFuncionou !!";
    
    // Criação do descritor do Socket (IPv4, via TCP, )
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    //Espaço de endereçamento IPv4
    address.sin_family = AF_INET;
    //Endereço 0.0.0.0 (todas as interfaces)
    address.sin_addr.s_addr = INADDR_ANY;
    //Definindo a porta do servidor baseado na constante PORT
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    //Fazendo Binding do socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        //Caso Bind falhe
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    //Ouve na porta definida em PORT
    if (listen(server_fd, 10) < 0)
    {
        //Caso falhe (como em caso de algo já estar na porta definida, ou de ter mais de 10 conexões aguardando)
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    //Funcionamento -de facto- do servidor
    while(1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        //Lê a requisição
        valread = read( new_socket , buffer, 30000);
        //Printa requisição
        printf("%s\n",buffer );
        //Devolve uma resposta
        write(new_socket , response , strlen(response));
        printf("------------------------------\n\n");
        close(new_socket);
    }
    return 0;
}