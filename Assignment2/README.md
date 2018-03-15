CMPUT 379 assignment2 08/03/2018
By Yongjia Huang and Yuhan Ye
-----------------
Files included:
-----------------
server.c
client.c	
Makefile
README.md
---------------------
General usage notes:
---------------------
This is a socket server - multi client program using pthread
Game interface:
+-------------------+
|                   |
|                   |
|        >          |
|           o       |
|           o       |
|           ^       |
|                   |
|                   |
|                   |
|                   |
|                   |
|                   |
|                   |
|                   |
|                   |
+-------------------+

use  j to move left, k to move down, l to move right, i to move up, space to fire
Good Luck!

--------------
How to run?
--------------
command: make

server side:
command: ./gameserver379 16 0.2 8989 1547543314
where the first parameter is the dimension of the square grid (16 means a 16x16 grid), followed by the update period (in seconds) the server uses to periodically send the game state to all clients (0.2 meaning a fifth of a second in this example),
followed by the port on which theserver should belistening (8989 in this example) and the last number is the "seed" for the random number generator (1547543314 in this example)

client side:
command: ./gameclient379 127.0.0.1 8989
where the first parameter is the hostname OR the IP address of the host on which the server is running and the second is the port number where the server listens. 

command: ps -aux | grep gameserver379 
to get the process id of the server and
command: kill -15 process id
to send SIGTERM Unix Signal to terminate the server

-------------------
Division of labour:
-------------------
Yongjia Huang contributes to most server side and part of client side
Yuhan Ye contributes to most client side and part server side and Makefile.
We work together to test the program,debug comment and write the README file.
The code was tested on VM(XubuntuCS.08.2015-06.2014 CMPUT174)

----------------------------
Design Issues and Questions:
----------------------------
Q:How do you ensure that input response time(keystrokeread, transmission of command, receiving at the server) is minimized?
A:Send and receive information as fewer as possible to reduce the transmission time
and minimize time between receive and send at server side.

Q:How do you minimize the amount of traffic flowing back and forth between clients and server?
A:Server only sends all players positions to the client and client only sends its own move which is only a single char to the server.

Q:How do you handle multiple commands being sent from a client between two successive game board updates? (see also notes)
A:Server only gets last input from the client in one update period.

Q:How do you ensure that the same(consistent) state is broadcast to all clients?
A:Server puts all players positions in an array and sends this array to all clients.

Q:How do you ensure the periodic broadcast updates happen as quickly as possible?
A:Minimize the respond time. server receive the information at once clients send
its information to the server.

Q:How do you deal with clients that fail without harming the game state?
A:Whenever a client is hit(server will check this), server will send a fail message to whoever fails. After client recives this fail message, it close the socket and print fail message on screen, exit peacefully. And server will no longer recv message from client,instead it will recive 0 from the recv function, and server will delete data about this client and exit this thread without harming the game state.

Q:How do you handle the graceful termination of the server when it receives a signal? (Provide and implement a reasonable form of graceful termination.)
A:We provide a signalHandler function to deal with signals received. If the server receives a SIGTERM signal, server will print the terminate message and then exit.

Q:How do you deal with clients that unilaterally break the connection to the server? (Assume the client can exit,e.g.crash,without advance warning.)
A:whenever a client disconnected to the server, server will no longer receive message from this client and it will recive -1 or 0 from the recv message. So we checked this condition and print the status of this client on the server side.

Q:How can you improve the user interface without hurting performance?
A:The interface will be much better if we choose to use an advance interface tool and this tool must support C99. 
-------------------------------------------------------------------------------------------
Copyright 2018 Yongjia Huang,Yuhan Ye CMPUT379, University of Alberta, All Rights Reserved.
You may use distribut, or modify this code under terms and conditions of the ode of Student Behavior at University of Alberta
You may find a copy of the license in this project. Otherwise please contact jajayongjia@gmail.com
