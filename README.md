# ft_irc

A simple IRC server written in C++98, developed as part of the 42 School curriculum.  
The project implements a minimal Internet Relay Chat server following the IRC protocol (RFC 1459 inspired), supporting multiple clients, channels, authentication, and basic IRC commands.

## Features

- C++98 compliant
- Non-blocking I/O using poll()
- Multiple clients support
- Channel management
- Operator privileges
- Password authentication
- Message broadcasting
- Private messages
- Signal handling (Ctrl+C safe exit)

## Supported Commands

- PASS
- NICK
- USER
- JOIN
- PART
- PRIVMSG
- NOTICE
- KICK
- INVITE
- TOPIC
- MODE
- QUIT

## Project Structure
src/
server/
client/
channel/
commands/
utils/

include/

Makefile

## Build

```bash
make
```
Clean:
```
make fclean
```
## RUN
```
./ircserv <port> <password>
```
Example:
```
./ircserv 6667 secretpass
```
Connect using netcat
```
nc localhost 6667
```
## Learning Goals

This project focuses on:
Network programming
Socket API
poll / select
Protocol implementation
Multi-client server architecture
Object-oriented design in C++98
Memory management
Error handling
