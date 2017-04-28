#define PCAP_MAGIC                  0xa1b2c3d4
#define PCAP_SWAPPED_MAGIC          0xd4c3b2a1
#define PCAP_MODIFIED_MAGIC         0xa1b2cd34
#define PCAP_SWAPPED_MODIFIED_MAGIC 0x34cdb2a1

struct timev {
    unsigned int tv_sec;
    unsigned int tv_usec;
};

struct my_pkthdr {
    struct timev ts;
    int caplen;
    int len;
};


static unsigned int b_sec;
static unsigned int c_sec;
static int b_usec;
static int c_usec;

void packet_reader(int fd, int BUFSZ);
void ip_packet_print(char *bufp);
void arp_packet_print(char *bufp);
