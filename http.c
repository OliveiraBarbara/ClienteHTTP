/****************************************************************
Nome: Bárbara Beatriz Bueno de Oliveira		RGA: 2019.0743.014-8
Nome: Beatriz Macedo Costa				RGA: 2015.0743.009-4
Trabalho 1 - CLiente HTTP
Disciplina: Redes de Computadores 2
Professor: Rodrigo Kishi
****************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIZE 1024

void escreve_arquivo(char *buffer, int sock);

int main(int argc, char *argv[]) {
	struct addrinfo hints, *res; /*hints que especifica os requisitos para filtrar as estruturas de socket recuperadas, *res é onde a função irá alocar dinamicamente uma lista vinculada de estruturas addrinfo*/
	int sock, i, j, cont, resp;
	char request[SIZE], *url, buffer[SIZE], url_aux[2083], *path;
	
	if(argc < 2){
        fprintf(stderr, "Modo de Uso: <./executavel + URL completa (com http://)>\n");
        return 0;
    }
	
	url = argv[1]; 

	i = 7;
	j = 0;
	cont = strlen(url);

	while(i<cont){
		url_aux[j] = url[i];
		i++;
		j++;
	}

	url = strtok(url_aux,"/");
	path = strtok(NULL, "\n");
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	resp = getaddrinfo(url, "80", &hints, &res);
	if(resp != 0){
		fprintf(stderr, "Host não encontrado!\n");
		exit(1);
	}
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(sock, res->ai_addr, res->ai_addrlen);

	sprintf(request, "GET /%s HTTP/1.0\r\nurl: %s\r\n\r\n", path, url);
	
	send(sock, request, strlen(request), 0);
	
	memset(buffer, 0, SIZE);
	recv(sock, buffer, SIZE, 0);

	if((strncmp("HTTP/1.0 200 OK", buffer, 15) != 0) && (strncmp("HTTP/1.1 200 OK", buffer, 15) != 0)){
		printf("%s\n", buffer);
	}else{
		escreve_arquivo(buffer, sock);
	}
	
	return 0;
}

void escreve_arquivo(char *buffer, int sock){
	FILE *arq;
	
	arq = fopen("http.html", "w");
	if (arq == NULL){
		fprintf(stderr, "Nao foi possivel abrir o arquivo\n");
	}else{
		while(recv(sock, buffer, SIZE, 0) > 0){
			fprintf(arq, "%s", buffer);
			memset(buffer, 0, SIZE);
		}
	}
	
	fclose(arq);
}

