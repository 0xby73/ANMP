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
  - Makefile
  ```bash
  make // To compile both the server and the client
  ```
  ```bash
  make server // To compile the server
  ```
  ```bash
  make client // To compile the client
  ```
  ```bash
  make clean // To clean / Remove the compiled binaries
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

