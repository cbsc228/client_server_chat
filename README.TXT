Creator: Craig Scarboro
Class: CS371
Assignment: Client/Server Socket Programming Project

----DESCRIPTION----
This pair of programs allows 2 users to talk to one another over a TCP connnection via socket programming.
The server allows for at most 1 connection at a time to be attempted. When the server begins listeningn a
client may attempt to connect. After a successful connect each program takes turns sending messages to one
another staring with the server.

----INSTRUCTIONS----
First start the server program. The server will then provide the user with the ip address for the client to
connect to. Next the server user needs to input a port number for the connection to use. After this the server
begins listening for a client request to connect. On the client side, after the program is started the user needs
to input the port number that the server side as selected. Once this port number is input and server accepts connection,
the talking portion begins. The server gets the first message while the client waits for the server to send its message.
After the client receives the message, it takes it turn to send a message while the server waits for the client. This loop
continues until the client enters the command "!quit" which ends the session on the client side and lets the server
know that the client is shut down. The server then goes back to listening for another client to connect to it and repeat
this process while the server remains online.
