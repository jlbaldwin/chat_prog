#client/server chat

Description:
	This project consists of two programs: chatserve and chatclient. chatserve is written in python and chatclient is written in C.
	chatclient.c needs to be compiled before it is run. 

To compile chatclient.c 
	From command line: make, or gcc -o chatclient chatclient.c
	This will create the executable "chatclient"

To run program
	To start server from command line: python3 chatserve <portnum>

	To start client from command line after compiling: ./chatclient localhost <portnum>
	The port number must be the same as the server port.

	When both programs are running the client must send the first message, then messages alternate. When the client or server
	terminate the session, the client will close, but the server will continue to run and can accept new sessions. The messages
	are limited to 500 characters per the assignment and any message longer than that will be truncated.

	Client or server can quit the current session by sending "\quit". Server only terminates if SIGINT (ctrl-c) is received, which 
	will also terminate the client if it is currently open.