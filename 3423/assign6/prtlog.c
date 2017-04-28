#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "pcap.h"
#include "dnet.h"
#include "prtlog.h"




void sys_err(char *s){
    perror(s);
    exit(-1);
}

void main(int argc, char *argv[]){

    if (argc < 2){
        fprintf(stderr, "Usage: logprt <tcdump_file>\n");
        exit(-1);
    }
    
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) sys_err("Open err");
    struct pcap_file_header file_header;
    if(read(fd, &file_header, sizeof(struct pcap_file_header)) != sizeof(struct pcap_file_header)){
        sys_err("Read error");
    }
    
    printf("\nPCAP_MAGIC\n");
    printf("Version major number = %d\n", file_header.version_major);
    printf("Version minor number = %d\n", file_header.version_minor);
    printf("GMT to local correction = %d\n", file_header.thiszone);
    printf("Timestamp accuracy = %d\n", file_header.sigfigs);
    printf("Snaplen = %d\n", file_header.snaplen);
    printf("Linktype = %d\n", file_header.linktype);

    packet_reader(fd, file_header.snaplen);

    exit(0);
}

void packet_reader(int fd, int BUFSZ){
    int bytes_read, i=0, firsttime = 1, bswitch;
    char buf[BUFSZ];
    char *bufp;
    struct eth_hdr *ethhdr;
    struct my_pkthdr phdr;
    //struct pcap_pkthdr phdr;
    while((bytes_read = read(fd, &phdr, sizeof(struct my_pkthdr))) == sizeof(struct my_pkthdr)){

        if((bytes_read = read(fd, &buf[0], phdr.caplen)) != phdr.caplen){ 
            perror("read error");
            fprintf(stderr,"bytes expected: %d\n", phdr.caplen);
            fprintf(stderr,"bytes read: %d\n", bytes_read);
            exit(-1); 
        }
        
        if(firsttime){
            firsttime = 0;
            b_sec = phdr.ts.tv_sec;
            b_usec = phdr.ts.tv_usec;
        }

        c_sec = (unsigned)phdr.ts.tv_sec - b_sec;
        c_usec = (unsigned)phdr.ts.tv_usec - b_usec;
        while (c_usec < 0) {
            c_usec += 1000000;
            c_sec--;
        }
        printf("\nPacket %d\n", i);
        printf("%05u.%06u\n", (unsigned)c_sec, (unsigned)c_usec);
        printf("Captured Packet Length = %d\nActual Packet Length = %d\n", phdr.caplen, phdr.len);

        bufp = buf;
        ethhdr = (struct eth_hdr *) bufp;
        bufp += sizeof(struct eth_hdr);
        bswitch = 1;
        printf("Ethernet Header\n");
        if (ntohs(ethhdr->eth_type) == ETH_TYPE_IP){
            printf("\tIP\n");
            ip_packet_print(bufp);
        } else if(ntohs(ethhdr->eth_type) == ETH_TYPE_ARP){
            printf("\tARP\n");
            arp_packet_print(bufp);
        }
        else{
            printf("\tUNRECOGNIZED\n");
        }
        i++;
    }

    if(bytes_read == -1){
        sys_err("read error");
    }
}

void ip_packet_print(char *bufp){
    struct ip_hdr *iphdr;
    iphdr = (struct ip_hdr *) bufp;
    bufp += sizeof(struct ip_hdr);
    switch(iphdr->ip_p){
        case IP_PROTO_ICMP:
            printf("\t\tICMP\n");
            break;
        case IP_PROTO_IGMP:
            printf("\t\tIGMP\n");
            break;
        case IP_PROTO_TCP:
            printf("\t\tTCP\n");
            break;
        case IP_PROTO_UDP:
            printf("\t\tUDP\n");
            break;
        default:
            printf("\tUNRECOGNIZED\n");
    }
    
}

void arp_packet_print(char *bufp){
    struct arp_hdr *arphdr;
    arphdr = (struct arp_hdr *) bufp;
    bufp += sizeof(struct arp_hdr);
    printf("\t\tArp Operation = ");
    switch(ntohs(arphdr->ar_op)){
        case ARP_OP_REQUEST:
            printf("Arp Request\n");
            break;
        case ARP_OP_REPLY:
            printf("Arp Reply\n");
            break;
        case ARP_OP_REVREQUEST:
            printf("Arp Request\n");
            break;
        case ARP_OP_REVREPLY:
            printf("Arp Reply\n");
            break;
        default:
            printf("UNRECOGNIZED\n");
   }
}
