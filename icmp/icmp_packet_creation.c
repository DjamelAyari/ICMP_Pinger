#include "icmp_header.h"
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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
    memset(icmp_packet, 0, packet_size);
    icmp *icmp_hdr = (icmp*)icmp_packet;
    {
        icmp_hdr->type = ICMP_ECHO;
        icmp_hdr->code = 0;
        icmp_hdr->checksum = 0;
        icmp_hdr->id = getpid();
        icmp_hdr->sequence = 1;
    }
    printf("Type: %d\n", icmp_hdr->type);
    printf("Code: %d\n", icmp_hdr->code);
    printf("Checksum: %d\n", icmp_hdr->checksum);
    printf("Id: %d\n", icmp_hdr->id);
    printf("Sequence: %d\n", icmp_hdr->sequence);

    // Allocate an array of char pointers (each will point to one buffer).
    char **payload_pointers_array = malloc(number_of_packets * sizeof(char *));
    if (payload_pointers_array == NULL)
    {
        perror("malloc() for creating the array of pointers failed\n");
        exit(1);
    }
    printf("malloc() for creating the array of pointers ok !\n");

    // For each packet, allocate a buffer and fill it with dummy data.
    int payload_size = 20;
    for (int i = 0; i <= number_of_packets; i++)
    {
        payload_pointers_array[i] = malloc(payload_size);
        if (payload_pointers_array[i] == NULL)
        {
            printf("malloc() for buffer %d failed\n", i);
            exit(1);
        }

        // Fill buffer with A's
        memset(payload_pointers_array[i], 'A', payload_size);
        printf("Payload %d content: %.*s\n", i + 1, payload_size, payload_pointers_array[i]);
        printf("Payload %d is of size: %d\n", i + 1, payload_size);

        payload_size = payload_size * 2;
    }

    for (int i = 0; i < number_of_packets; i++)
    {
        free(payload_pointers_array[i]);
    }
    free(payload_pointers_array);

    /*printf("Adding payload to the ICMP packet...\n");
    memcpy(icmp_packet + sizeof(struct icmp), payload, payload_size);
    //printf("The packet header and payload is: %c", )
    
    printf("Payload: %s\n", payload);
    printf("Packet: %s\n", icmp_packet);

    printf("Copying the ICMP packet for the checksum calculation..\n");
    char icmp_packet_cpy[sizeof(struct icmp) + payload_size];
    memcpy(icmp_packet_cpy, icmp_packet, sizeof(struct icmp_header));
    memset(&((struct icmp_header *)icmp_packet_cpy)->checksum, 0, sizeof(((struct icmp_header *)icmp_packet_cpy)->checksum));

    uint16_t sum = 0;
    check_sum(icmp_packet_cpy, sum);
    ((struct icmp_header *)icmp_packet)->checksum = sum;*/
}