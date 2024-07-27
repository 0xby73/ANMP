#!/bin/bash

detect() {
    if command -v apt &> /dev/null; then
        echo "apt"
    elif command -v pacman &> /dev/null; then
        echo "pacman"
    else
        echo "setup.sh cant work on your system, please install the packages manually"
    fi
}

install() {
    case $1 in
        apt)
            sudo apt update && sudo apt install -y g++ make libncurses5-dev libncursesw5-dev
            ;;
        pacman)
            sudo pacman -Syu --noconfirm && sudo pacman -S --noconfirm gcc make ncurses
            ;;
        *)
            echo "APT || Pacman not installed, Install the packages manually: $1"
            exit 1
            ;;
    esac
}

PM=$(detect)
echo "Detected package manager: $PM"

read -p "Use detected package manager? (y/n): " use_detected

if [[ $use_detected != [Yy]* ]]; then
    read -p "Enter package manager (apt/pacman): " PM
fi

install $PM
echo "Finished installation."
