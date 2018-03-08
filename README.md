







----------------------------
Design Issues and Questions:

Q:How do you ensure that input response time(keystrokeread, transmission of command, receiving at the server) is minimized?
A: send and receive information as fewer as possible to reduce the transmission time
and minimize time between receive and send at server side.

Q:How do you minimize the amount of traffic flowing back and forth between clients and server?
A: server only sends all players positions to the client and client only sends its own move which is only a single char to the server.

Q:How do you handle multiple commands being sent from a client between two successive game board updates? (see also notes)
A: server only gets last input from the client in one update period.

Q:How do you ensure that the same(consistent) state is broadcast to all clients?
A: server put all players positions in an array and send this array to all clients.

Q:How do you ensure the periodic broadcast updates happen as quickly as possible?
A: minimize the respond time.

Q:How do you deal with clients that fail without harming the gamestate?
A:

Q:How do you handlethe graceful termination of theserver when it receives a signal? (Provideand implementa reasonable
form of graceful termination.)
A:

Q:How do you deal with clients that unilaterally break theconnection to theserver? (Assumetheclient can exit,e.g.crash,
withoutadvance warning.)
A:

Q:How can you improvethe user interface without hurting performance?
A:
