# ANMP
Across Network Messaging Protocol

## Table of contents
- [Getting Started](#getting-started)
  - [Dependencies](#dependencies)
  - [Installation](#installation)
  - [Usage](#usage)
- [License](LICENSE)

## Getting Started

  ### Dependencies
  #### Run 
  ```bash
  sudo chmod +x setup.sh
  ./setup.sh
  ```
  - if you dont have the dependencies already installed

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
  ### Usage
  
- To run the server you run
```bash
./server PORT
```
- To run the client you run
```bash
./client IP PORT
```
Define the arguments to your needs

- Enter your username when prompted then you'll be sent to the main room
- Use ```!help``` to view the list of commands

