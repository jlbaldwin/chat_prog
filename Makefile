#This makefile is for the chat client, server is in python 

chatclient: chatclient.c
	gcc -o chatclient chatclient.c 
#after compile, run with ./chatclient <hostname> <port>


clean:
	rm -f chatclient.o chatclient
