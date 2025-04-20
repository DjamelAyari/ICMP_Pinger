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