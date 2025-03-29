QUIC Pinger

Front End
    -Une page HTML, CSS (intégrer du responsive si possible).
    -Du JS pour éviter le rafraichissement des pages et éviter d'envoyer des requêtes pour certaines commandes.
    Le JS va aussi pouvoir envoyer les requêtes HTTP au server.
    -Permettre la possibilité pour le client de choisir le serveur qu'il veut contacter via un choix ainsi que le nombre
    et la taille des paquets à utiliser.

Server TCP
    -Le serveur TCP va permettre d'utiliser HTTP.
    -Pas besoin d'intégrer du TSL, car c'est juste un pinger.
        MAIS ATTENTION VERIFIER SI MÊME UNE REQUETE PING NE COMPORTE PAS DE RISQUE.

Process des requêtes
    -Une page en C pour recevoir, parser et envoyer la requête une fois traité à la page de traitement (le client ICMP).

Action sur les requêtes
    -Mise en place du client ICMP.
    -C'est dans ce fichier que le chronomètre, les calcul et les fonctions du pinger vont être misent en place et utilisés.
    -Et c'est aussi dans ce fichier qu'en fonction du choix du client le choix du server ainsi que le nombre et la taille 
    des paquets que le bonne requête va être envoyé au server cible.
    Renvoie des informations récupérées au client HTTP.

    Sending packet in a loop (Ask how mush loop and how much quantity in each packet)
    1. RTT
    2. Packet loss
    3. Jitter calculation
    4. Checksum (help to detect any corruption in all part of the packet).
    5. Bandwith estimation (Throughput-Based Method).
    6. TTL handling / Traceroute (Uses TTL to map the network path to a destination.)
    7. Packet sequence numbering (Assigns a unique ID to each ping request to check for out-of-order packets.)
    8. GeoIP & Reverse DNS Lookup
    
    ---
    1. RTT = end time (receiption of the response) - start time (sending of the packet).
    
    2. Packet Loss = nbr of packets sent - nbr of response (for the packets).
    
    3. Jitter calculation = measures how much the Round Trip Time (RTT) fluctuates between packets.
        Consecutive Packet Jitter Formula = |Current packet's Round Trip Time (RTT n) - Previous packet's Round Trip Time (RTT n-1)|.
        Min, Max, Means, Median.
        Need to be calculated at the end when each RTT for each packets has been calulated.
    
    4. Check sum = 
                    1. Break the packet into k number of blocks with n bits in each blocks.
                    2. sum all the k data blocks.
                    3. Add the carry (if any) to the sum.
                    4. Do the 1st complement to the sum (it means change 1 to 0 and 0 to 1).

                    Exemple:
                    The data unit is: 10011001111000100010010010000100
                    1. 10011001 11100010 00100100 10000100

                    2. 3.   10000100
                            00100100
                            11100010
                            10011001
                            --------
                            00100011
                            --------
                                10
                            --------
                            00100101
                    
                    4. 00100101 -> 11011010

                    The check sum is 11011010
    
    5. Bandwith estimation
        1. Send large ICMP Echo Requests (e.g., 56 bytes, 512 bytes, 1024 bytes, etc.).
        2. Measure Round-Trip Time (RTT).
        3. If RTT increases significantly with larger packets, bandwidth is likely limited.
        
        Estimated bandwidth (bits per second) = packet size (bits) / measured round-trip time (seconds).

    6. TTL handling / Traceroute (You don’t add TTL inside the ICMP packet. Instead, you set TTL at the IP level using setsockopt() before sending your ICMP request.)
        We have to increase each time TTL from 1.
        Set TTL = 1 → Send ICMP Echo Request.
        The first router decreases TTL to 0 and responds with "Time Exceeded" (ICMP Type 11, Code 0).
        Now, you know the first hop (router).

        Increase TTL = 2 → Send ICMP Echo Request again.
        The second router drops TTL to 0 and replies with "Time Exceeded".
        Now, you know the second hop.

        Repeat until you reach the destination.

        The destination does not reply with "Time Exceeded" but responds normally with an ICMP Echo Reply (Type 0).

        That means you've reached the final destination!

        🚀 From TTL Handling → Full Traceroute
        If you gradually increase TTL, your pinger becomes a traceroute tool.
        So, one feature (TTL handling) gives you both ping and traceroute!

        🔹 Steps to Retrieve the IP Address at Each Hop
        Set TTL for the ICMP request (setsockopt with IP_TTL).
        Send ICMP Echo Request.
        Receive ICMP Time Exceeded Response from the router.
        Extract Source IP from the response (router's IP).
        Repeat until reaching the destination.
    
    7. Packet sequence numbering
        Packet sequence numbering helps track each ICMP request and response to:
        
        Identify lost packets (if a request is sent but no response is received).
        Detect out-of-order packets (if replies arrive in a different order).
        Ensure each response corresponds to the correct request.
        
        📜 How ICMP Uses Sequence Numbers
        When sending an ICMP Echo Request (Type 8, Code 0), the ICMP header contains:
        Identifier (16 bits) → Identifies the session (e.g., process ID).
        Sequence Number (16 bits) → Increments for each sent packet.
        Each ICMP Echo Reply (Type 0, Code 0) must return the same identifier and sequence number.

        🔢 How to Implement Packet Sequence Numbering
        Initialize a sequence number (e.g., seq_num = 0).
        When sending a new ping request:
        Set the sequence number in the ICMP header.
        Increment seq_num++ for the next packet.
        
        When receiving a ping reply:
        Check if the received sequence number matches the sent one.
        If missing numbers (e.g., sent #1, #2, #3 but received only #1, #3), → packet loss.
        If the order is different (e.g., sent #1, #2, #3 but received #2, #1, #3) → out-of-order delivery.

    8.  Extract the IP address of each router.
        Use GeoIP API to get location details.
        Perform Reverse DNS Lookup to get the hostname



