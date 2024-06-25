# IPK_Proj1

## FIT VUT - Computer communications and networks course. Repository for the first project: IPK Calculator Protocol

### Documentation
This program creates a UDP or TCP client that sends a message from STDIN to a server and receives a response. The program reads arguments from the command line to determine the hostname, port number, and mode (UDP or TCP).\
The program first creates a socket using the socket function with the appropriate address family (AF_INET), type (SOCK_DGRAM for UDP or SOCK_STREAM for TCP), and protocol (0).\
Then, it uses the gethostbyname function to get the address of the server specified by the hostname argument. If this function returns NULL, the program prints an error message and exits.\
Next, the program creates a sockaddr_in struct to hold the server address information. This struct is initialized with the server's address family, port number, and IP address.\
If the mode is UDP, the program enters a loop that reads lines of input from stdin, constructs a message using the line of input, sends the message to the server using the sendto function, and receives a response from the server using the recvfrom function. The program then prints the response to stdout. If an error occurs during the send or receive operations, the program prints an error message and exits.\
If the mode is TCP, the program checks for a "HELLO" message from client. Then it sends messages from stdin using the send function and receives a response from the server using the recv function. The program prints the response to stdout. If an error occurs during the send or receive operations, the program prints an error message and exits.\
If the user interrupts the program with a SIGINT signal (e.g., by pressing Ctrl-C), the program sends a "BYE" if communication was initialized and it is using TCP, and closes the socket before exiting.
The program defines several helper functions for handling errors and closing sockets.\

### Tests
Software was tested on the virtual machines located at FIT Nextcloud, ipkd.py pyhton test server and student's server 147.229.176.19:10002\

### Credit
Test server ipkd.py was made by GitHub -> https://github.com/lavirlifiliol, Discord -> lakmatiol#8020\