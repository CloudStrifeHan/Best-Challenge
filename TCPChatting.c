/*

I retype a small program to send single packet between client and server chat. This is full context, however, you can make this code smaller.
It based on some docs, which I found on Internet.

Reference: + http://www.geeksforgeeks.org/socket-programming-cc/
           + http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
Special thanks with:  + TCP/IP Sockets in C - Practical Guide for Programmers 
           
 

*/

/* ================ This code for Server ====================*/
#include <stdio.h>      /* for printf(), perror() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close()  */


void DieWithError(char *errorMessage);  /* Error handling function */
void HandleTCPClient(int clntSocket);   /* TCP client handling function */


int main(int argc, char const *argv[]) {
/* Declare variable */
  int servSock;           		/* Socket descriptor for server */
  int clntSock;         		/* Socket descriptor for client */
  struct sockaddr_in echoServAddr;    	/* Local address */
  struct sockaddr_in echoClntAddr;    	/* Client address */
  unsigned short echoServPort;        	/* Server port */
  unsigned int clntLen;               	/* Length of client address data structure */
	
  /* Test for correct number of argument */
  if (argc != 2) {
    fprintf(stderr, "Usage: %s  <Server Port>\n", argv[0]);
    exit(1);
  }

  /* Get argv */
  echoServPort = atoi(argv[1]);

  /* Create socket for incoming connection */
  if ((servSock = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP)) < 0){
    DieWithError("socket() failed!");
  }

  /* Construct local address structure */
  memset(&echoServAddr, 0, sizeof(echoServAddr)); 	/*set echoServerAddr zero out */
  echoServAddr.sin_family = AF_INET;    		/* Internet family */
  echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);	/* any incoming interface */
  echoServAddr.sin_port = htons(echoServPort);    	/* Local port */

  /* Bind to local address */
  if(bind(servSock , (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
    DieWithError("bind() failed");
  }

  /* Mark the socket so it will listen for incoming connections */
  if(listen(servSock, 3) < 0) {
    DieWithError("listen() failed");
  }

  while(1)  {
    /* Set the size of the in-out parameter */
    clntLen = sizeof(echoClntAddr);

    /* Wait for a client to connect */
    if((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)  {
      DieWithError("accept() failed");
    }

    /* clntSock is coonected to a client! */
    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

    HandleTCPClient(clntSock);
  }

  return 0;
}

/* Declare function */
void DieWithError(char *errorMessage) {
  fprintf(stderr, "%s\n", errorMessage);
  exit(1);
}

void HandleTCPClient(int clntSock)  {
  char echoBuffer[32];    /*Buffer for echo string */

  char *msg = "Hello! I am Server! Your messge below: \n";		/* Message */

  int recvMsgSize;        /* Size of received message */

  /* Receive message from client */
  if ((recvMsgSize = recv(clntSock, echoBuffer, 32, 0)) < 0 ) {
    DieWithError("recv() failed");
  }
 
  while (recvMsgSize > 0) {
    /*Echo message back to client */
    
    send(clntSock, msg, strlen(msg), 0);

    if(send(clntSock, echoBuffer, recvMsgSize, 0) != recvMsgSize) {
      DieWithError("send() failed");
    }

    /* See if there is more date to receive */
    if ((recvMsgSize = recv(clntSock, echoBuffer, 32, 0)) < 0)  {
      DieWithError("recv() failed");
    }
    close(clntSock); /* Close client socket() */
  }
}


/* ============== This code for Client ================== */

#include <stdio.h>  /* for printf() and fprintf(), perror()*/
#include <sys/socket.h>   /* for socket(), connect(), send(), recv(), accept(),.. */
#include <arpa/inet.h>    /* for sockaddr_in and inet_addr() */
#include <string.h>       /* for strlen(), memset() */
#include <unistd.h>       /* for close() */
#include <stdlib.h>       /* for exit() */

/* Declare DiewithError() function */
void DieWithError(char *errorMessage) {
  perror(errorMessage);
  exit(1);
}


int main(int argc, char const *argv[]) {
  /* Declare variable */
  int sock;                 /* Socket descriptor */
  struct sockaddr_in echoServAddr;    /* Echo server address */
  unsigned short echoServPort;        /* Echo server port */
  char *servIP;            /* Server IP address */
  char *echoString;       /* String to send to echo server */
  char echoBuffer[1024];  /* Buffer for echo string */
  unsigned int echoStringLen;   /* Length of string to echo */
  int bytesRcvd, totalBytesRcvd;  /*Bytes read in single recv(), and total bytes read */


  /*  Body */

  /* Check for correct input number of argument */
  if(argc < 3 || argc > 4)  {
    fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
    exit(1);
  }

  /* Get parameter from argv */
  servIP = argv[1];       /* First arg: server IP address (dotted quad) */
  echoString = argv[2];    /* Second arg: string to echo */

  /* Set fourth arv as echoServer Port if having 4 arg was passing */
  if(argc == 4) {
    echoServPort = htons(argv[3]);     /* Use given port, if any */
  }
  else {
    echoServPort = htons(1997);     /* Use 1997 as port for program */
  }


  /* Create a reliable, stream socket using TCP */
  if((sock = socket(AF_INET /* PF_INET */, SOCK_STREAM, IPPROTO_TCP /* 0 */)) < 0)  {
      DieWithError("socket() failed!");
  }

  /* Construct the server address structure */
  memset(&echoServAddr, 0,sizeof(echoServAddr));        /* Zero out structure */
  echoServAddr.sin_family       = AF_INET;              /* Internet address family */
  echoServAddr.sin_addr.s_addr  = inet_addr(servIP);    /* Server IP address */
  echoServAddr.sin_port         = htons(echoServPort);   /* Server Port. Note: echoServPort was parse type binary before in line 39-44 */

  /* Establish the connection to the echo server */
  if(connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)  {
    DieWithError("Connect() failed!");
  }

  /* Determine input length */
  echoStringLen = strlen(echoString);

  /* Send the string to the server */
  if (send(sock, echoString, echoStringLen, 0) != echoStringLen)  {
    DieWithError("send() sent a different number of bytes than expected!");
  }

  /* Receive the same string back from the server */
  totalBytesRcvd = 0;
  printf("Received: ");
  while(totalBytesRcvd < echoStringLen) {
    /* Receive up to the buffer size (minus 1 to leave space for a null terminator) */
    if((bytesRcvd = recv(sock, echoBuffer, 1024-1, 0)) <= 0)  {
      DieWithError("recv() failed or connection closed prematurely");
    }
    totalBytesRcvd += bytesRcvd;
    echoBuffer[bytesRcvd] = '\0'; /* Null byte for terminal message */
    printf("%s", echoBuffer);
  }

  printf("\n");

  /* Close socket  and finish program */
  close(sock);
  return 0;
}


