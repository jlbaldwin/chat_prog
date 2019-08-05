/******************************************************************************
 * NAME:       Jonathan Baldwin (baldwijo)
 * COURSE:     CS372 Intro to Computer Networks
 * LAST MOD:   Feb 9, 2019
 * PROG NAME:  chatclient
 * PROG DESC:  Client for chat program. Client connects to server by host IP
 *             and port provided from command line. Once connected, client 
 *             sends first message then waits for response from server. The
 *             server or client can quit the session with the command "\quit".
 *             Quit kills the client program but the server will remain active
 *             listening for another session.
 *             Messages are limited to 500 characters and will be truncated 
 *             if longer.      
 *             To compile "gcc -o chatclient chatclient.c"
 *             To run: ./chatclient <server hostname> <server port>    
 * CITATION:   This code has been influenced by the following:
 *             Beej's Guide to Network Programming
 *             www.linuxhowtos.og/C_C++/socket.htm
 *             OSU CS344 Operating Systems, C client code, B. Brewster 
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAXDATASIZE 500 //max bytes allowed
#define MAXMSGSIZE 511  //message plus handle plus '>'

//function prototype to get client handle
char * getHandle(void);


int main(int argc, char *argv[]){

   //validate number of command line arguments 
   if (argc != 3){
      printf("Invalid number of command line arguments, exiting.\n");
      exit(0);
   }

   char * handle = getHandle();
   int servPort = atoi(argv[2]);                      //server port from command line
   int msg;
   struct sockaddr_in serv_addr;                      //for server address
   char buffMsgIn[MAXMSGSIZE];                        //read and hold message here  
   char buffTempMsg[MAXDATASIZE];                     //assist to build send message
   char buffMsgOut[MAXMSGSIZE];                       //message to send out with handle
   int sockfd = socket(AF_INET, SOCK_STREAM, 0);      //setup TCP connection socket
   //connection error message 
   if (sockfd < 0){
      error("Error opening socket.");
   }

   //holds host info including address
   struct hostent * servHost = gethostbyname(argv[1]);
   //message and exit if server does not exist
   if (servHost == NULL){
      fprintf(stderr, "Error, host does not exist.");
      exit(0);
   }

   //address struct info
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *) servHost->h_addr,
         (char *) &serv_addr.sin_addr.s_addr,
         servHost->h_length);
   //convert server port to network order
   serv_addr.sin_port = htons(servPort);   

   //connect to the server
   connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
   
   //chat loop, stay connected until client or server quits, then term program
   while(1){

      //display handle then get message to send 
      printf(handle);
      bzero(buffMsgOut, MAXMSGSIZE);
      bzero(buffTempMsg, MAXDATASIZE);
      fgets(buffTempMsg, MAXDATASIZE, stdin);
      buffTempMsg[strcspn(buffTempMsg, "\n")] = '\0';

      //if client quits, display message then quit program
      if(strcmp(buffTempMsg, "\\quit") == 0){
         msg = write(sockfd, buffTempMsg, strlen(buffTempMsg)); 
         printf("Quit triggered by client - closing. You can reconnect.\n");
         close(sockfd);
         exit(0);
      }

      //Client has not quit so build string with handle, to send
      strcat(buffMsgOut, handle);
      strcat(buffMsgOut, buffTempMsg);

      //send message to server
      msg = write(sockfd, buffMsgOut, strlen(buffMsgOut)); 
      
      //receive message, clear out buffer to avoid any issues
      bzero(buffMsgIn, MAXMSGSIZE);
      msg = recv(sockfd, buffMsgIn, MAXMSGSIZE, 0);

      //if server wants to quit, display message and terminate program
      if((strcmp(buffMsgIn, "\\quit") == 0) || (msg == 0)){
         printf("Quit triggered by server - closing. You can reconnect.\n");
         close(sockfd);
         exit(0);
      }
      
      //server has not quit so display message   
      printf(buffMsgIn);
      printf("\n");
   }
   

   close(sockfd);
   return 0;
}


/****************************************************************************
 * getHandle()
 * Desc: get client handle, validating a length of 1 to 10 characters
 *       not including the >
 * Pre-cond: none
 * Post-cond: returns array of char (string) with '>' appended to end      
 * *************************************************************************/

char * getHandle(void){
   static char name[1000];
      
   //prompt user for handle 
   printf("Enter handle name (between 1 and 10 chars): ");
   fgets(name, sizeof name, stdin);
   name[strcspn(name, "\n")] = 0;

   //if handle is invalid length, reprompt for valid handle
   while(strlen(name) > 10 || strlen(name) < 1){
      printf("Handle must be between 1 and 10 chars - enter again: ");
      fgets(name, sizeof name, stdin);
      name[strcspn(name, "\n")] = 0;
   }

   //add > character
   strcat(name, ">");

   return name;
}
