# ANMP
Across Network Messaging Protocol

## Table of contents
- [Getting Started](#getting-started)
  - [Dependencies](#dependencies)
  - [Installation](#installation)
  - [Usage](#usage)

## Getting Started

  ### Dependencies
  #### Run ```setup.sh```
  This project requires the following dependencies:
  - make
  - g++
  - ncurses

  ### Installation
 To compile both the server and the client
  ```bash
  make
  ```
  To compile the server
  ```bash
  make server
  ```
  To compile the client
  ```bash
  make client
  ```
  To clean | To remove the compiled binaries
  ```bash
  make clean
  ```
Then to run the server you run
```bash
./server PORT
```
Define the port to your needs

For the client you run
```bash
./client IP PORT
```
Define the arguments to your needs

