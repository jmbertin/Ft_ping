#!/bin/bash

pause_and_clear() {
    echo
    read -p "Press any key to continue..." -n1 -s
    echo
    clear
}

echo_test_name() {
    echo
    echo -e "${BLUE}$1${NC}"
}

echo_command() {
    echo
    echo -e "${LIGHT_BLUE}$1${NC}"
    echo
}

# Color codes
BLUE="\033[0;34m"
LIGHT_BLUE="\033[1;34m"
NC="\033[0m" # No Color

# Mandatory part
clear
echo_test_name "------Mandatory part-------"

# Verify version
echo_test_name "Verifying ping version (binutils 2.0 mandatory)"
echo_command "ping -V"
ping -V
pause_and_clear

# Display help
echo_test_name "Testing help display"
echo_command "./ft_ping -h"
./ft_ping -h
pause_and_clear

# Test good IP
echo_test_name "Testing with good IP - Press CTRL+C to stop"
echo_command "./ft_ping 8.8.8.8"
./ft_ping 8.8.8.8
echo_command "ping 8.8.8.8"
ping 8.8.8.8
pause_and_clear

# Test bad IP
echo_test_name "Testing with bad IP (192.168.1.1) - Press CTRL+C to stop"
echo_command "./ft_ping 192.168.1.1"
./ft_ping 192.168.1.1
echo_command "ping 192.168.1.1"
ping 192.168.1.1
pause_and_clear

# Test good IP with verbose mode
echo_test_name "Testing with good IP and verbose mode - Press CTRL+C to stop"
echo_command "./ft_ping 8.8.8.8 -v"
./ft_ping 8.8.8.8 -v
echo_command "ping 8.8.8.8 -v"
ping 8.8.8.8 -v
pause_and_clear

# Test bad IP with verbose mode
echo_test_name "Testing with bad IP and verbose mode - Press CTRL+C to stop"
echo_command "./ft_ping 192.168.1.1 -v"
./ft_ping 192.168.1.1 -v
echo_command "ping 192.168.1.1 -v"
ping 192.168.1.1 -v
pause_and_clear

# Test good hostname
echo_test_name "Testing with good hostname (www.google.com) - Press CTRL+C to stop"
echo_command "./ft_ping www.google.com"
./ft_ping www.google.com
echo_command "ping www.google.com"
ping www.google.com
pause_and_clear

# Test bad hostname
echo_test_name "Testing with bad hostname (www.google.com12)"
echo_command "./ft_ping www.google.com12"
./ft_ping www.google.com12
echo_command "ping www.google.com12"
ping www.google.com12
pause_and_clear

# Test bad hostname with verbose mode
echo_test_name "Testing with bad hostname (www.google.com) and verbose mode"
echo_command "./ft_ping www.google.com12 -v"
./ft_ping www.google.com12 -v
echo_command "ping www.google.com12 -v"
ping www.google.com12 -v
pause_and_clear

# Testing parsing (error cases)
echo_test_name "-----------------------"
echo_test_name "Testing parsing (error cases)"

echo_test_name "Missing option for argument"
echo_command "./ft_ping www.google.com -c"
./ft_ping www.google.com -c
pause_and_clear

echo_test_name "Multiple hosts"
echo_command "./ft_ping www.google.com www.google.com"
./ft_ping www.google.com www.google.com
pause_and_clear

echo_test_name "No host"
echo_command "./ft_ping -v"
./ft_ping -v
echo_command "./ft_ping"
./ft_ping
pause_and_clear

echo_test_name "Unknown argument"
echo_command "./ft_ping www.google.com -cm"
./ft_ping www.google.com -cm
echo_command "./ft_ping www.google.com -z"
./ft_ping www.google.com -z
pause_and_clear

echo_test_name "Forbidden argument combination"
echo_command "./ft_ping www.google.com -f -i 5"
./ft_ping www.google.com -f -i 5
pause_and_clear

# Bonus commands
echo_test_name "-------------------------"
echo_test_name "Bonus commands"

echo_test_name "Command limited in number of requests (5 requests)"
echo_command "./ft_ping -c 5 www.google.com"
./ft_ping -c 5 www.google.com
echo_command "ping -c 5 www.google.com"
ping -c 5 www.google.com
pause_and_clear

echo_test_name "Command limited in time (5 seconds)"
echo_command "./ft_ping -w 5 www.google.com"
./ft_ping -w 5 www.google.com
echo_command "ping -w 5 www.google.com"
ping -w 5 www.google.com
pause_and_clear

echo_test_name "Command with waiting between packets (3 seconds) - Press CTRL+C to stop"
echo_command "./ft_ping -i 3 www.google.com"
./ft_ping -i 3 www.google.com
echo_command "ping -i 3 www.google.com"
ping -i 3 www.google.com
pause_and_clear

echo_test_name "Flood command - Press CTRL+C to stop"
echo_command "./ft_ping -f www.google.com"
./ft_ping -f www.google.com
echo_command "ping -f www.google.com"
ping -f www.google.com
