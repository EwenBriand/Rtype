# RFC REngine server

The typical interaction between a server and a client should unroll as described below.

## Units

The protocol is a binary protocol. Each directive is composed of words:
    b: bytes -> 1 byte
    w: word -> 4 bytes
    dw: double word -> 8 bytes
    qw: quad word -> 16 bytes

## Instructions:

An instruction refers to an operation that must be done by the other side.
All instructions are encoded on a word by a unique code.

Error codes are considered instructions.
All instructions with a numeric value below 1000 (32 bit integer) are considered errors by the server and the clients.

All message shall end with ten times the byte 0x42.

## Message:

The structure of a message is the following:

[instruction: w][expect answer: b][params: ...]

If [expect answer] is equal to 0, the sender does not expect the other to send a message upon completion of the action. Else, the opcode of the answer should be equal to the expect answer code.
Else, no further checks will be performed.

## Instructions:

### Errors:

The parameters to all errors are strings with a message giving more details.

0 - No error, No instruction
    Ok. Operation was performed successfully

1 - Still alive, the clients are required to send this to the server every second to indicate that they are still connected.

2 - 255 - reserved for expect answer codes

300 - Access refused, Server full

400 - Denied because of anti cheat


### Orders:

    - 1001 : the client wishes to join the game.
        C -> 1001 23
            S ->  0 23            if request has been accepted
            S -> 300 23           else

    - 1002 : load scene with id (string).
        S -> 1002 40 6 level1       server asks clients to load the level1
            C -> 0 40             client responds when the action is completed

    - 1003 : start game loop (scene must be loaded first)
        S -> 1003 0             server authorizes all players to start playing
