#include <stdio.h>
#include <string.h>
#include <winsock.h>
#include <bits/stdc++.h>
#include <sys/types.h>
using namespace std;
#define PORT 9090
struct sockaddr_in srv, clnt;
double eval(string expr)
{
  string xxx; // Get Rid of Spaces
  for (int i = 0; i < expr.length(); i++)
  {
    if (expr[i] != ' ')
    {
      xxx += expr[i];
    }
  }

  string tok = ""; // Do parantheses first
  for (int i = 0; i < xxx.length(); i++)
  {
    if (xxx[i] == '(')
    {
      int iter = 1;
      string token;
      i++;
      while (true)
      {
        if (xxx[i] == '(')
        {
          iter++;
        }
        else if (xxx[i] == ')')
        {
          iter--;
          if (iter == 0)
          {
            i++;
            break;
          }
        }
        token += xxx[i];
        i++;
      }
      tok += to_string(eval(token)); // calling recursive function
    }
    tok += xxx[i];
  }

  for (int i = 0; i < tok.length(); i++)
  {
    if (tok[i] == '+')
    {
      return eval(tok.substr(0, i)) + eval(tok.substr(i + 1, tok.length() - i - 1)); // calling recursive function
    }
    else if (tok[i] == '-')
    {
      if (tok.substr(0, i).length() != 0 && tok[i - 1] != '*' && tok[i - 1] != '/')
        return eval(tok.substr(0, i)) + eval("-" + tok.substr(i + 1, tok.length() - i - 1)); // calling recursive function
    }
  }

  for (int i = 0; i < tok.length(); i++)
  {
    if (tok[i] == '*')
    {
      return eval(tok.substr(0, i)) * eval(tok.substr(i + 1, tok.length() - i - 1)); // calling recursive function
    }
    else if (tok[i] == '/')
    {
      return eval(tok.substr(0, i)) / eval(tok.substr(i + 1, tok.length() - i - 1)); // calling recursive function
    }
  }

  return stod(tok.c_str()); // Return the value...
}
int main()
{
  int sockfd, newsockfd;
  char buffer[255];
  WSADATA ws;
  int nStatus = WSAStartup(MAKEWORD(2, 2), &ws);
  if (nStatus < 0)
  {
    printf("WSAStartup failed\n");
    return EXIT_FAILURE;
  }
  else
  {
    printf("Server started\n");
  }
  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sockfd < 0)
  {
    printf("Socket API failed to open listener\n");
    return EXIT_FAILURE;
  }
  else
  {
    printf("Server started\n");
  }

  srv.sin_family = AF_INET;
  srv.sin_port = htons(PORT);
  srv.sin_addr.s_addr = INADDR_ANY;

  nStatus = bind(sockfd, (struct sockaddr *)&srv, sizeof(srv));
  if (nStatus < 0)
  {
    printf("Bind failed\n");
    return EXIT_FAILURE;
  }
  nStatus = listen(sockfd, 5); // listening

  if (nStatus < 0)
  {
    printf("Listen failed\n");
    return EXIT_FAILURE;
  }

  int cliLen = sizeof(clnt);
  newsockfd = accept(sockfd, (struct sockaddr *)&clnt, &cliLen); // connect
  if (newsockfd < 0)
  {
    printf("Client socket not connected\n");
    return EXIT_FAILURE;
  }

  while (1)
  {
    memset(buffer, '\0', 255);
    int n = recv(newsockfd, buffer, 255, 0);
    if (n < 0)
    {
      printf("Read failed\n");
      return EXIT_FAILURE;
    }
    if (!strncmp(buffer, "NO", 2)) // comparing string to terminate
    {
      cout << "Terminated" << endl;
      break;
    }
    system("color 0c");
    n = eval(buffer);
    cout << "The Answer is : " << n << endl; // output printing
    string s = to_string(n);
    memset(buffer, '\0', 255);
    strcpy(buffer, s.c_str());
    n = send(newsockfd, buffer, strlen(buffer), 0);
    memset(buffer, '\0', 255);
  }
  closesocket(sockfd);    // closing socket
  closesocket(newsockfd); // closing socket
  return 0;
}