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
This is a socket server - mult client program using pthread
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
command: 
where path is the absolute or relative path to the directory to monitor,and period is the reporting period in seconds.
Open another terminal to send the process a SIGUSR1 Unix Signal.
command: ps -aux | grep diffdir379 
to get the process id of the program and
command: kill -10 process id
to send SIGUSR1 Unix Signal.

-------------------
Division of labour:
-------------------
Yongjia Huang contributes to dealing with files and printing the output.
Yuhan Ye contributes to dealing with time,signal handling,Makefile and README.
We work together to test the program and debug.
The code was tested on VM(XubuntuCS.08.2015-06.2014 CMPUT174)

----------------------------
Design Issues and Questions:

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
