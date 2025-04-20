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
    scanf("%s", client_input);

    //Creating a struct to tell the getaddrinfo() how to interpret the user input.
    printf("Interpreting the user input...\n");
    struct addrinfo client_input_struct, *client_input_struct_result;
    memset(&client_input_struct, 0, sizeof(client_input_struct));
    client_input_struct.ai_family = AF_INET;
    client_input_struct.ai_socktype = SOCK_RAW;
    client_input_struct.ai_protocol = IPPROTO_ICMP;
    printf("ai_family: %d\n", client_input_struct_result->ai_family);
    printf("ai_socktype: %d\n", client_input_struct_result->ai_socktype);
    printf("ai_protocol: %d\n", client_input_struct_result->ai_protocol);


    //Filling with client_input_struct the getaddrinfo().
    //Converting the user input into an IP address.
    int error_code;
    if ((error_code = getaddrinfo(client_input, NULL, &client_input_struct, &client_input_struct_result)) != 0)
    {
        //fprintf(stderr, "getaddrinfo() failed: %d\n", gai_strerror(error_code));
        // Error handling based on platform
        // Print the numerical error code to understand what went wrong
        fprintf(stderr, "getaddrinfo() failed: %d\n", error_code);
        // Use gai_strerror to print the associated error message for POSIX systems like WSL
        fprintf(stderr, "Error message: %s\n", gai_strerror(error_code));
        return(1);
    }
    printf("ai_family: %d\n", client_input_struct_result->ai_family);
    printf("ai_socktype: %d\n", client_input_struct_result->ai_socktype);
    printf("ai_protocol: %d\n", client_input_struct_result->ai_protocol);

    
    char ip_address[INET6_ADDRSTRLEN];  // Declare a buffer large enough for the IP address string
    if (inet_ntop(client_input_struct_result->ai_family, &((struct sockaddr_in *)client_input_struct_result->ai_addr)->sin_addr, ip_address, sizeof(ip_address)) == NULL)
    {
        perror("inet_ntop failed");
        return 1;
    }
    printf("The IP address is: %s\n", ip_address);

    printf("Creating ICMP client socket...\n");
    SOCKET icmp_client_socket = socket(client_input_struct_result->ai_family, client_input_struct_result->ai_socktype, client_input_struct_result->ai_protocol);
    if (icmp_client_socket < 0)
    {
        perror("socket() failed !\n");
        return(1);
    }
    printf("ICMP client socket creation ok !\n");

    freeaddrinfo(client_input_struct_result);

    printf("Entering ICMP packet creation function...\n");
    icmp_packet_creation();
    
    
    
        
    struct timeval current_time, end_time;
    START_TIMER(&current_time);
    printf("Sending packet...\n");
        /*I have checked my MTU and the IP Segment size in order to avoid packet fragmentation.
        My MTU is 1500 and the maximum total size of an IPv4 packet (header + payload) is 65,535 bytes.
        Since the biggest packet can be 1344 + 8 for the ICMP header and 20 for the IP segment, I will avoid fragmentation.*/

        //sendto(icmp_client_socket, icmp_packet, sizeof(icmp_packet), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));

        //NEED TO USE MALLOC/REALLOC !!!


    
    //USE A LOOP AND TIMEOUT (FOR PACKETS LOSES).
    //printf("Receiving packet...\n");
    //printf("The recv_packet_buffer size will be: %d\n", packet_size); 
    //char recv_packet_buffer[packet_size];
    //recvfrom(icmp_client_socket, icmp_packet, sizeof(icmp_packet), 0, (struct sockaddr*) &from, &fromlen);
    //struct timeval current_time, end_time;
    START_TIMER(&end_time);

    printf("Closing ICMP client socket...\n");
    close(icmp_client_socket);
    printf("ICMP client socket closed ok !\n");
}