#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

uint32_t ip_to_uint(const char *ip_str)
{
    uint32_t ip = 0;
    uint8_t octet;
    const char *ptr = ip_str;

    for (int i = 0; i < 4; i++)
    {
        octet = (uint8_t)atoi(ptr);
        ip = (ip << 8) | octet;
        ptr = strchr(ptr, '.');
        if (ptr != NULL)
        {
            ptr++;
        }
    }
    return ip;
}

uint32_t generate_random_ip()
{
    return (rand() % 256) << 24 | (rand() % 256) << 16 | (rand() % 256) << 8 | (rand() % 256);
    // return ( 192 << 24 | 168 << 16 | (rand() % 256) << 8 | (rand() % 256) );
}

int is_in_subnet(uint32_t ip, uint32_t subnet, uint32_t mask)
{
    return (ip & mask) == (subnet & mask);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <gateway_ip> <subnet_mask> <packet_count>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *gateway_ip_str = argv[1];
    const char *subnet_mask_str = argv[2];
    int packet_count = atoi(argv[3]);

    if (packet_count <= 0)
    {
        fprintf(stderr, "Invalid packet count: %d\n", packet_count);
        return EXIT_FAILURE;
    }

    uint32_t gateway_ip = ip_to_uint(gateway_ip_str);
    uint32_t subnet_mask = ip_to_uint(subnet_mask_str);

    srand(time(NULL));

    int same_subnet_count = 0;
    int other_subnet_count = 0;

    for (int i = 0; i < packet_count; i++)
    {
        uint32_t random_ip = generate_random_ip();
        if (is_in_subnet(random_ip, gateway_ip, subnet_mask))
        {
            printf("%d.%d.%d.%d\n", random_ip >> 24, (random_ip << 8) >> 24, (random_ip << 16) >> 24, (random_ip << 24) >> 24);
            same_subnet_count++;
        }
        else
        {
            other_subnet_count++;
        }
    }

    printf("Total packets: %d\n", packet_count);
    printf("Packets to the same subnet: %d (%.2f%%)\n", same_subnet_count, (same_subnet_count * 100.0) / packet_count);
    printf("Packets to other subnets: %d (%.2f%%)\n", other_subnet_count, (other_subnet_count * 100.0) / packet_count);

    return EXIT_SUCCESS;
}