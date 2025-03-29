#include "icmp_header.h"
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

    //Creating a struct to tell the getaddrinfo() how to interpret the user input.
    printf("Interpreting the user input...\n");
    struct addrinfo client_input_struct, *client_input_struct_result;
    memset(&client_input_struct, 0, sizeof(client_input_struct));
    client_input_struct.ai_family = AF_INET;
    client_input_struct.ai_socktype = SOCK_RAW;
    client_input_struct.ai_protocol = IPPROTO_ICMP;

    //Filling with client_input_struct the getaddrinfo().
    //Converting the user input into an IP address.
    if (getaddrinfo(client_input, NULL, client_input_struct, client_input_struct_result) != 0)
    {
        perror("getaddrinfo() failed !\n");
        exit(EXIT_FAILURE);
    }

    //Extracting the IP from client_input_struct_result and copy it in the final_address struct.
    /* final_address;
    memset(&final_address, 0, sizeof(final_address));
    final_address.sin_family = AF_INET;
    final_address.sin_addr = ((struct sockaddr_in*)client_input_struct_result->ai_addr)->sin_addr;
    */
    freeaddrinfo(client_input_struct_result);

    printf("Creating ICMP client socket...\n");
    SOCKET icmp_client_socket = socket(client_input_struct_result->ai_family, client_input_struct_result->ai_socktype, client_input_struct_result->ai_protocol);
    if (icmp_client_socket < 0)
    {
        perror("socket() failed !\n");
        exit(EXIT_FAILURE);
    }

    printf("Creating ICMP packet...\n");
    printf("Enter the number of packets you want to send.\n");
    int number_of_packets;
    scanf("%d", &number_of_packets);
    #define PACKET_SIZE 56;
    char icmp_packet[PACKET_SIZE]; //BUFFER
    for (i = 0; i < number_of_packets; i++)
    {
        struct icmp_header *icmp = (struct icmp_header)icmp_packet;
        icmp->type = ICMP_ECHO;
        icmp->code = 0;
        icmp->checksum = 0;
        icmp->id = getpid();
        icmp->sequence = 1;

        // Add a small payload (optional but recommended)
        const char *payload = "PINGTEST"; 
        memcpy(buffer + sizeof(struct icmp_header), payload, strlen(payload));

    }

    // Calculate the checksum before sending

    printf("Sending packet...\n");
    //sendto(icmp_client_socket, icmp_packet, sizeof(icmp_packet), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    
    printf("Receiving packet...\n");
    //recvfrom(icmp_client_socket, icmp_packet, sizeof(icmp_packet), 0, (struct sockaddr*) &from, &fromlen);

    printf("Closing ICMP client socket...\n");
    close(icmp_client_socket);
    printf("ICMP client socket closed ok !\n");
}