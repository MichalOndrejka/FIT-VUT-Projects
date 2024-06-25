# IPK_Proj2
Design and implement a network analyzer that will be able to capture and filter packets on a specific network interface
## Author
Michal Ondrejka (xondre15)

### Theory summary:
To understand encapsulation is it important to have basic understanding of the OSI model. There are 7 layers. In second layer in the frame header 13th and 14th bytes indicates the type of protocol that follows the Ethernet header. Based on this we are able to identify eiher IPv4, IPv6 or ARP. From this 3rd layer we are able to obtain source and destination MAC addresses. IPv4 has inconsistent length, so if we want to get to the next header we will need to access it's bytes and calculate it. Then in the 4th layer we are able to get source and destination IP addresses. Again after optaining value of next header protocol we are able to identify ,based on which IP protocol we are in, which protocol is next. From TCP and UDP we can get source and destination ports. Only TCP and UDP can get these.

### Testing:
Testing was done on NixOS in C Developer enviroment provide by the course. I tested based functionality as well as edge cases, multiple parameters and different ports.
First I started the program with specific command line arguments, then I pinged different packets using sudo nping \[protocol\] \[port\] \[address\] 
[A test image](test.png)

### Resources:
- https://www.youtube.com/watch?v=--tR2XqAZm0&ab_channel=Tedro
- https://www.youtube.com/watch?v=03vhsOO5kcs&ab_channel=alinyaung
- https://www.tcpdump.org/pcap.html
- https://en.wikipedia.org/