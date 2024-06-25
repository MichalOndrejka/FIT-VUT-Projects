// Author: Michal Ondrejka
// Login: xondre15

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <time.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/igmp.h>
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <getopt.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <net/ethernet.h>

#define OPTIONAL_ARGUMENT_IS_PRESENT \
    ((optarg == NULL && optind < argc && argv[optind][0] != '-') \
     ? (bool) (optarg = argv[optind++]) \
     : (optarg != NULL))

pcap_t *handle;
struct bpf_program fp;

// Struct for saving command line arguments
typedef struct {
    char interface[256];
    char port[16];
    bool handleTCP;
    bool handleUDP;
    bool handleARP;
    bool handleNDP;
    bool handleICMP4;
    bool handleICMP6;
    bool handleIGMP;
    bool handleMLD;
    int numberOfPackets;
} arg_options;

// Print Usage when --help arg
void printUsage() {
    printf("./ipk-sniffer [-i interface | --interface interface] {-p port [--tcp|-t] [--udp|-u]} [--arp] [--icmp4] [--icmp6] [--igmp] [--mld] {-n num}");
    printf("\n");
    printf("where:");
    printf("\n");
    printf("\t-h | --help                 help message\n");
    printf("\t-i                          specify interface\n");
    printf("\t-p                          set packet port to filter\n");
    printf("\t-t | --tcp                  filter TCP packets\n");
    printf("\t-u | --udp                  filter UDP packets\n");
    printf("\t--arp                       filter ARP frames\n");
    printf("\t--icmp                      filter ICMPv4 nad ICMPv6 packets\n");
    printf("\t-n                          set packet limit (unlimited if not set)\n");
    exit(EXIT_SUCCESS);
}

// Stop packet capture
void stopCapture(){
    pcap_freecode(&fp);
    pcap_close(handle);
    exit(EXIT_SUCCESS);
}

// Handle Ctr + C interrupt
void cCinterrupt() {
    stopCapture();
}

void printActiveInterfaces() {
    pcap_if_t *interfaces;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Find all active interfaces
    if (pcap_findalldevs(&interfaces, errbuf) == -1) {
        fprintf(stderr, "Error finding devices: %s\n", errbuf);
        exit(EXIT_FAILURE);
    }

    // Print out interface information
    printf("\n");
    for (pcap_if_t *iface = interfaces; iface != NULL; iface = iface->next) {
        printf("%s\n", iface->name);
    }

    // Free the list of interfaces
    pcap_freealldevs(interfaces);
}

pcap_t *packetCaptureHandle(char *interface, char *filter_expression) {
    //pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    //struct bpf_program fp;
    uint32_t net;
    uint32_t mask;

    handle = pcap_open_live(interface, 65535, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device: %s\n", errbuf);
        return NULL;
    }

    if (pcap_lookupnet(interface, &net, &mask, errbuf) == PCAP_ERROR) {
        fprintf(stderr, "Can't get netmask for device: %s\n", errbuf);
        return NULL;
    }

    if (pcap_compile(handle, &fp, filter_expression, 0, net) == PCAP_ERROR) {
        fprintf(stderr, "Couldn't parse filter: %s\n", pcap_geterr(handle));
        return NULL;
    }

    if (pcap_setfilter(handle, &fp) == PCAP_ERROR) {
        fprintf(stderr, "Couldn't install filter: %s\n", pcap_geterr(handle));
        return NULL;
    }

    return handle;
}

// Check if header of frame is supported
void checkHeader() {
    if (pcap_datalink(handle) != DLT_EN10MB) {
	fprintf(stderr, "Device doesn't provide Ethernet headers - not supported\n");
	exit(EXIT_FAILURE);
    }
}

// print time stamp in RFC 3339 format
void printTimeStamp(const struct pcap_pkthdr *header) {
    char time[100];
    time_t seconds = header->ts.tv_sec;
    struct timeval tv = header->ts;
    struct tm *local = localtime(&seconds);

    strftime(time, sizeof(time), "%Y-%m-%dT%H:%M:%S", local);

    char tz_offset[8];
    int hours = (int)(local->tm_gmtoff / 3600);
    int minutes = (int)((local->tm_gmtoff / 60) % 60);
    snprintf(tz_offset, sizeof(tz_offset), "%+03d:%02d", hours, minutes);

    snprintf(time + strlen(time), sizeof(time) - strlen(time), ".%03d%s", (int)tv.tv_usec / 1000, tz_offset);

    printf("timestamp: %s\n", time);
}

void printByteOffset(const struct pcap_pkthdr *header, const u_char *packet) {
    printf("\n");

    for (int i = 0; i < header->caplen; i += 16) {
        printf("0x%04x: ", i);
        for (int j = 0; j < 16; j++) {
            if (i + j < header->caplen) {
                printf("%02x ", packet[i + j]);
            } else {
                printf("   ");
            }
        }
        printf(" ");
        for (int j = 0; j < 16; j++) {
            if (i + j < header->caplen) {
                if (j== 8) {
                    printf(" ");
                }
                if (isprint(packet[i + j])) {
                    printf("%c", packet[i + j]);
                } else {
                    printf(".");
                }
            }
        }
        printf("\n");
    }

    printf("\n");
}

// callback function used for printing available data based on present protocols
void callback(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    struct ether_header *eth_header;
    const u_char *source, *dest;
    char mac_src[256], mac_dst[256];
    char ip_src[256], ip_dst[256];
    int port_src, port_dst;

    eth_header = (struct ether_header *) packet;
    source = eth_header->ether_shost;
    dest = eth_header->ether_dhost;

    // Format mac addresses and save them to variables
    snprintf(mac_src, sizeof(mac_src), "%02x:%02x:%02x:%02x:%02x:%02x", source[0], source[1], source[2], source[3], source[4], source[5]);
    snprintf(mac_dst, sizeof(mac_dst), "%02x:%02x:%02x:%02x:%02x:%02x", dest[0], dest[1], dest[2], dest[3], dest[4], dest[5]);

    printTimeStamp(header);
    printf("src MAC: %s\n", mac_src);
    printf("dst MAC: %s\n", mac_dst);
    printf("frame lentgh: %d bytes\n", header->len);

    // Get ip addresses based on IPV4, IPV6 or ARP protocol
    
    switch(ntohs(eth_header->ether_type)) {

        case ETHERTYPE_IP:
            struct iphdr *ip_header = (struct iphdr *) (packet + ETH_HLEN);

            // Format ip addresses and save them to variables
            inet_ntop(AF_INET, &(ip_header->saddr), ip_src, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &(ip_header->daddr), ip_dst, INET_ADDRSTRLEN);

            printf("src IP: %s\n", ip_src);
            printf("dst IP: %s\n", ip_dst);

            if (ip_header->protocol == IPPROTO_TCP) { //TCP
                struct tcphdr *tcp_header = (struct tcphdr *) (packet + ETH_HLEN + ip_header->ihl * 4);

                port_src = ntohs(tcp_header->source);
                port_dst = ntohs(tcp_header->dest);

                printf("src port: %d\n", port_src);
                printf("dst port: %d\n", port_dst);
            }

            else if (ip_header->protocol == IPPROTO_UDP) { //UDP
                struct udphdr *udp_header = (struct udphdr *) (packet + ETH_HLEN + ip_header->ihl * 4);

                port_src = ntohs(udp_header->source);
                port_dst = ntohs(udp_header->dest);

                printf("src port: %d\n", port_src);
                printf("dst port: %d\n", port_dst);
            }

            else if (ip_header->protocol == IPPROTO_ICMP) { //ICMPv4
            }
            
            else if (ip_header->protocol == IPPROTO_IGMP) { //IGMP
            }
            
            else {
                //fprintf(stderr, "Unknown packet received\n");
                return;
            }
            break;

        case ETHERTYPE_IPV6:
            struct ip6_hdr *ip6_header = (struct ip6_hdr *) (packet + ETH_HLEN);

            // Format ip addresses and save them to variables
            inet_ntop(AF_INET, &(ip6_header->ip6_src), ip_src, INET6_ADDRSTRLEN);
            inet_ntop(AF_INET, &(ip6_header->ip6_dst), ip_dst, INET6_ADDRSTRLEN);

            printf("src IP: %s\n", ip_src);
            printf("dst IP: %s\n", ip_dst);

            if (ip6_header->ip6_nxt == IPPROTO_TCP) { //TCP
                struct tcphdr *tcp_header = (struct tcphdr *) (packet + ETH_HLEN + sizeof(struct ip6_hdr));

                port_src = ntohs(tcp_header->source);
                port_dst = ntohs(tcp_header->dest);

                printf("src port: %d\n", port_src);
                printf("dst port: %d\n", port_dst);
            }

            else if (ip6_header->ip6_nxt == IPPROTO_UDP) { //UDP
                struct udphdr *udp_header = (struct udphdr *) (packet + ETH_HLEN + sizeof(struct ip6_hdr));

                port_src = ntohs(udp_header->source);
                port_dst = ntohs(udp_header->dest);

                printf("src port: %d\n", port_src);
                printf("dst port: %d\n", port_dst);
            }
            
            else if (ip6_header->ip6_nxt == IPPROTO_ICMPV6) { //ICMPv6
            }
            
            else {
                //fprintf(stderr, "Unknown packet received\n");
                return;
            }
            break;

        case ETHERTYPE_ARP: //ARP
            struct ether_arp *arp_header = (struct ether_arp *) (packet + ETH_HLEN);

            // Format ip addresses and save them to variabl
            inet_ntop(AF_INET, arp_header->arp_spa, ip_src, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, arp_header->arp_tpa, ip_dst, INET_ADDRSTRLEN);

            printf("src IP: %s\n", ip_src);
            printf("dst IP: %s\n", ip_dst);
            break;
        
        default:
            //fprintf(stderr, "Unknown packet received\n");
            return;
            break;
    }

    printByteOffset(header, packet);
    
}

int main(int argc, char **argv) {

    // variable for storing options
    arg_options options;
    strcpy(options.interface, "");
    strcpy(options.port, " port ");
    options.numberOfPackets = -1;
    options.handleTCP = false;
    options.handleUDP = false;
    options.handleARP = false;
    options.handleNDP = false;
    options.handleICMP4 = false;
    options.handleICMP6 = false;
    options.handleIGMP = false;
    options.handleMLD = false;

    // Get values of arguments
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"interface", optional_argument, 0, 'i'},
        {"tcp", no_argument, 0, 't'},
        {"udp", no_argument, 0, 'u'},
        {"arp", no_argument, 0, 'a'},
        {"ndp", no_argument, 0, 'd'},
        {"icmp4", no_argument, 0, '4'},
        {"icmp6", no_argument, 0, '6'},
        {"igmp", no_argument, 0, 'g'},
        {"mld", no_argument, 0, 'm'},
        {0, 0, 0, 0}
    };

    int opt;
    while((opt = getopt_long(argc, argv, "hi::p:tun:", long_options, NULL)) != -1) {
        switch(opt) 
        {
            case 'h':
                printUsage();
                break;
            case 'i':
                if (OPTIONAL_ARGUMENT_IS_PRESENT) {
                    strcat(options.interface, optarg);
                } else {
                    strcat(options.interface, "");
                }
                break;
            case 'p':
                strcat(options.port, optarg);
                break;
            case 't':
                options.handleTCP = true;
                break;
            case 'u':
                options.handleUDP = true;
                break;
            case 'a':
                options.handleARP = true;
                break;
            case 'd':
                options.handleNDP = true;
                break;
            case '4':
                options.handleICMP4 = true;
                break;
            case '6':
                options.handleICMP6 = true;
                break;
            case 'g':
                options.handleIGMP = true;
                break;
            case 'm':
                options.handleMLD = true;
                break;
            case 'n':
                options.numberOfPackets = atoi(optarg);
                break;
            case '?':
                exit(EXIT_FAILURE);
                break;
        }
    }

    // Check provided arguments
    if (strcmp(options.interface, "") == 0) {
        printActiveInterfaces();
        exit(EXIT_SUCCESS);
    }

    // Create filter
    char filter_expression[256] = "";

    if (!options.handleTCP && !options.handleUDP && !options.handleARP && !options.handleNDP && !options.handleICMP4 && !options.handleICMP6 && !options.handleIGMP && !options.handleMLD) {
        options.handleTCP = true;
        options.handleUDP = true;
        options.handleARP = true;
        options.handleICMP4 = true;
        options.handleICMP6 = true;
        options.handleIGMP = true;
    }
    
    if (options.handleTCP == true) {
        strcat(filter_expression, "(tcp");
        if (strcmp(options.port, " port ") != 0) {
            strcat(filter_expression, options.port);
        }
        strcat(filter_expression, ") or ");
    }

    if (options.handleUDP == true) {
        strcat(filter_expression, "(udp");
        if (strcmp(options.port, " port ") != 0) {
            strcat(filter_expression, options.port);
        }
        strcat(filter_expression, ") or ");
    }
        
    if (options.handleARP == true) {
        strcat(filter_expression, "arp or ");
    }

    if (options.handleNDP == true && options.handleICMP6 == false) {
        strcat(filter_expression, "(icmp6 and ip6[40] == 135) or ");
    }

    if (options.handleICMP4 == true) {
        strcat(filter_expression, "icmp or ");
    }

    if (options.handleICMP6 == true) {
        strcat(filter_expression, "icmp6 or ");
    }

    if (options.handleIGMP == true) {
        strcat(filter_expression, "igmp or ");
    }

    if (options.handleMLD == true && options.handleICMP6 == false) {
        strcat(filter_expression, "(icmp6 and ip6[40] == 143) or (icmp6 and ip[20] == 130) or ");
    }

    int len = strlen(filter_expression);
    if (len >= 4) {
        filter_expression[len - 4] = '\0';
    }

    // Ctrl + C interrupt handler
    signal(SIGINT, cCinterrupt);

    // Handle for packets
    handle = packetCaptureHandle(options.interface, filter_expression);
    if (handle == NULL) {
        exit(EXIT_FAILURE);
    }

    // Get type of link layer
    checkHeader();

    // Loop through the incoming packets, each iteration calls callback function
    if (pcap_loop(handle, options.numberOfPackets, callback, NULL) == PCAP_ERROR) {
        fprintf(stderr, "Pcap loop failed: %s\n", pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    stopCapture();
}