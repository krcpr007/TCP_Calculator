#include <stdio.h>
#include <string.h>
#include <winsock.h>
#include <sys/types.h>
#include <bits/stdc++.h>
using namespace std;
#define PORT 9090
struct sockaddr_in srv; // inbuild
int main()
{
	int sockfd, n;
	char buffer[255];
	WSADATA ws; // for running in windows we use WSDATA
	int nStatus = WSAStartup(MAKEWORD(2, 2), &ws);
	if (nStatus < 0)
	{
		printf("Failed to call WSAStartup\n");
		return EXIT_FAILURE;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // family , stream , protocol
	if (sockfd < 0)
	{
		printf("Not able to call socket successfully\n");
		return EXIT_FAILURE;
	}
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = inet_addr("10.1.3.235");
	memset(&srv.sin_zero, 0, sizeof(srv));

	nStatus = connect(sockfd, (struct sockaddr *)&srv, sizeof(srv)); // connecting...
	if (nStatus < 0)
	{
		printf("Connect failed\n");
		return EXIT_FAILURE;
	}

	while (1)
	{
		system("color 0a");
		cout << "Enter Expression And for termination type No : " << endl;
		fgets(buffer, 255, stdin);
		if (!strncmp(buffer, "NO", 2)) // comparing string to terminate
		{
			cout << "Terminated" << endl;
			send(sockfd, buffer, strlen(buffer), 0);
			break;
		}
		n = send(sockfd, buffer, strlen(buffer), 0); // sending to server
		memset(buffer, '\0', 255);
		if (n < 0)
		{
			printf("Error on writing\n");
			return EXIT_FAILURE;
		}

		n = recv(sockfd, buffer, 255, 0);
		cout << "Answer : " << buffer << endl;
		memset(buffer, '\0', 255);
	}

	closesocket(sockfd);

	return 0;
}
