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

void escreve_arquivo(char *buffer, int sockfd);

int main(int argc, char *argv[]) {
	struct addrinfo hints, *res; /*hints que especifica os requisitos para filtrar as estruturas de socket recuperadas, *res é onde a função irá alocar dinamicamente uma lista vinculada de estruturas addrinfo*/
	int sockfd, i, j, cont;
	char header[4096], *host, buffer[SIZE], url_aux[2083], *path;
	host = argv[1]; 

	i = 7;
	j = 0;
	cont = strlen(host);

	while(i<cont){
		url_aux[j] = host[i];
		i++;
		j++;
	}

	host = strtok(url_aux,"/");

	path = strtok(NULL, "\n");

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	getaddrinfo(host, "80", &hints, &res);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	connect(sockfd, res->ai_addr, res->ai_addrlen);

	sprintf(header, "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n", path, host);
	
	send(sockfd, header, strlen(header), 0);
	
	memset(buffer, 0, SIZE);
	recv(sockfd, buffer, SIZE, 0);

	if((strncmp("HTTP/1.0 200 OK", buffer, 15) != 0) && (strncmp("HTTP/1.1 200 OK", buffer, 15) != 0)){
		printf("%s\n", buffer);
	}else{
		escreve_arquivo(buffer, sockfd);
	}
	
	return 0;
}

void escreve_arquivo(char *buffer, int sockfd){
	FILE *arq;
	
	arq = fopen("http.html", "w");
	if (arq == NULL){
		fprintf(stderr, "Nao foi possivel abrir o arquivo\n");
	}else{
		while(recv(sockfd, buffer, SIZE, 0) > 0){
			fprintf(arq, "%s", buffer);
			memset(buffer, 0, SIZE);
		}
	}
	
	fclose(arq);
}

