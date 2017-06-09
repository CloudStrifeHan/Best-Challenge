/*
I write a small program to send single packet between client and server chat.
It based on some docs, which I found on Internet.

Reference: + http://www.geeksforgeeks.org/socket-programming-cc/
           + http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
           
*/

//======================*** SERVER *** ========================
//============================== ** Server ** ===========================
#include <stdio.h>
#include <sys/socket.h>

#define PORT 2003

int main()  {
  int socket_create,setsockopt, connection;
  char buffer[1024] = {0};
  char *msg;
  struct sockaddr_in address;

  // Socket Creation
  if(socket_create = socket(AF_INET, SOCK_STREAM, 0) == 0){ // Create socket decriptor
    perror("Create socket descriptor unsuccesful!\n");
    exit(1);
  }

  //Attaching socket to the port 2003
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );

  //Blind to server
  if(blind(socket_create, (const struct *) &address, sizeof(address) < 0))  {
    perror("Blind failed!\n");
    exit(1);
  }
  puts(">> ");
  gets(msg);
  while(1)  {
    // Listen
    listen(socket_create, 3);

    // Accept any connection
    connection = accept(socket_create, address, (socklen_t*)&addrlen);

    // Get packet
    valread = read(socket_create, buffer, 1028);
    // Print received packet
    printf("%s\n", buffer);
    // Send packet
    send(socket_create, msg, strlen(msg), 0);
    puts(">> ");
    gets(msg);
  }

  return 0;
}


//=================================** Client ** ===============================

#include <stdio.h>
#include <sys/socket.h>

#define PORT 2003

int main()  {
  struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char *msg, *host;
  char buffer[1024] = {0};

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)  {
    printf("Socket creation unsuccesful!\n");
    return -1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons( PORT );

  // COnvert to text IPv4 to binary form
  if(inet_pton(AF_INT, host, &serv_addr.sin_addr) <= 0) {
    printf("Invalid address/ Address not supported! Converting unsuccesful!\n");
    return -1;
  }

  //Check connection
  if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("Connection faild!\n");
    return -1;
  }

  // Send message!
  send(sock, msg, strlen(msg), 0);
  printf("Message was send!\n");


  // Listen and receive packet
  while(1)  {
    valread = read(sock, buffer, 1024);
  }
  while(varread != 0) {
    printf("%s\n", buffer);
  }

  return 0;
}
