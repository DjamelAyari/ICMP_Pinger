#include "icmp_header.h"
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h> 


//https://medium.com/@future_fanatic/how-to-ping-a-host-in-c-in-5-steps-88d22415109c

int main() //The main() function will be replace by a custom one.
{
printf("Initializing WSA...\n");
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "Failed to initialize WSA.\n");
        return(1);
    }
    printf("WSA initializing ok !\n");
#endif

    printf("What server do you want to reach ?\n");
    char client_input[56];
    scanf("The server is: %s\n", client_input);

    //Creating a struct to tell the getaddrinfo() how to interpret the user input.
    printf("Interpreting the user input...\n");
    struct addrinfo client_input_struct, *client_input_struct_result;
    memset(&client_input_struct, 0, sizeof(client_input_struct));
    client_input_struct.ai_family = AF_INET;
    client_input_struct.ai_socktype = SOCK_RAW;
    client_input_struct.ai_protocol = IPPROTO_ICMP;

    //Filling with client_input_struct the getaddrinfo().
    //Converting the user input into an IP address.
    int error_code = 0;
    if (error_code = getaddrinfo(client_input, NULL, &client_input_struct, &client_input_struct_result) != 0)
    {
        fprintf(stderr, "getaddrinfo() failed: %s\n", gai_strerror(error_code));
        return(1);
    }
    
    char ip_address[INET_ADDRSTRLEN];  // Declare a buffer large enough for the IP address string
    if (inet_ntop(client_input_struct_result->ai_family, &((struct sockaddr_in *)client_input_struct_result->ai_addr)->sin_addr, ip_address, sizeof(ip_address)) == NULL)
    {
        perror("inet_ntop failed");
        return 1;
    }
    printf("The IP address is: %s\n", ip_address);

    freeaddrinfo(client_input_struct_result);

    printf("Creating ICMP client socket...\n");
    SOCKET icmp_client_socket = socket(client_input_struct_result->ai_family, client_input_struct_result->ai_socktype, client_input_struct_result->ai_protocol);
    if (icmp_client_socket < 0)
    {
        perror("socket() failed !\n");
        return(1);
    }
    printf("ICMP client socket creation ok !\n");

    printf("Entering ICMP packet creation function...\n");
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
    
    uint16_t check_sum(char *icmp_packet_cpy, uint16_t sum)
    {
        printf("Entering check_sum function...\n");
        int lenght = sizeof(icmp_packet_cpy);
        for (int j = 0; j < lenght; j += 2)
        {
            uint16_t sequence = (icmp_packet_cpy[j] << 8) | (icmp_packet_cpy[j + 1]);

            if ((sum + sequence) <= 65535)
            {
                sum += sequence;
            }
            else if ((sum + sequence) > 65535)
            {
                uint16_t carry = (sum + sequence) - 65535;
                sum += carry;
            }
        }

        printf("Sum is: %d", sum);
        sum = ~sum;
        printf("Sum is: %d", sum);

        return(sum);
    }
        
    struct timeval current_time, end_time;
    START_TIMER(&current_time);
    printf("Sending packet...\n");
        /*I have checked my MTU and the IP Segment size in order to avoid packet fragmentation.
        My MTU is 1500 and the maximum total size of an IPv4 packet (header + payload) is 65,535 bytes.
        Since the biggest packet can be 1344 + 8 for the ICMP header and 20 for the IP segment, I will avoid fragmentation.*/

        //sendto(icmp_client_socket, icmp_packet, sizeof(icmp_packet), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));

        //NEED TO USE MALLOC/REALLOC !!!


    
    //USE A LOOP AND TIMEOUT (FOR PACKETS LOSES).
    printf("Receiving packet...\n");
    printf("The recv_packet_buffer size will be: %d\n", packet_size);
    char recv_packet_buffer[packet_size];
    //recvfrom(icmp_client_socket, icmp_packet, sizeof(icmp_packet), 0, (struct sockaddr*) &from, &fromlen);
    //struct timeval current_time, end_time;
    START_TIMER(&end_time);

    printf("Closing ICMP client socket...\n");
    close(icmp_client_socket);
    printf("ICMP client socket closed ok !\n");
}