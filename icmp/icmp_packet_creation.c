int packet_size = 0;
void icmp_packet_creation()
{
    printf("Creating ICMP packet...\n");
    printf("Enter the number of packets you want to send.\n");

    int number_of_packets;
    printf("The number wanted is: ");
    scanf("%d", &number_of_packets);


    printf("Creating ICMP header structure...\n");
    typedef struct icmp_header
    {
        uint8_t type;
        uint8_t code;
        uint16_t checksum;
        uint16_t id;
        uint16_t sequence;
    } icmp;

    printf("Filling ICMP header structure...\n");
    packet_size =  28;
    char icmp_packet[packet_size]; //BUFFER
    icmp *icmp_hdr = (icmp*)icmp_packet;
    {
        icmp_hdr->type = ICMP_ECHO;
        icmp_hdr->code = 0;
        icmp_hdr->checksum = 0;
        icmp_hdr->id = getpid();
        icmp_hdr->sequence = 1;
    }

    char payload[packet_size - 8];
    int payload_size = 0;
    for (int i = 0; i <= number_of_packets; i++)
    {
        printf("Creating payload for the ICMP packet...\n");
        //char payload[packet_size - 8];
        memset(&payload, 'A', sizeof(payload));
        payload_size = sizeof(payload);

        printf("Adding payload to the ICMP packet...\n");
        memcpy(icmp_packet + sizeof(struct icmp_header), payload, strlen(payload));
        //printf("The packet header and payload is: %c", )

        packet_size * 2;
    }

    printf("Copying the ICMP packet for the checksum calculation..\n");
    char icmp_packet_cpy[sizeof(struct icmp_header) + payload_size];
    memcpy(icmp_packet_cpy, icmp_packet, sizeof(struct icmp_header));
    memset(&((struct icmp_header *)icmp_packet_cpy)->checksum, 0, sizeof(((struct icmp_header *)icmp_packet_cpy)->checksum));

    uint16_t sum = 0;
    check_sum(icmp_packet_cpy, sum);
    ((struct icmp_header *)icmp_packet)->checksum = sum;
}