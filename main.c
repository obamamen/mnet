#include <stdio.h>
#include "mnet.h"

void print_addr(const mnet_sockaddr_t* addr)
{
    mnet_address_family_t af;
    if (mnet_get_family(addr, &af)
        == mnet_error) return;

    char addr_str[MNET_ADDR_STRLEN];
    if (mnet_addr_ip_to_string(
        addr, addr_str, sizeof(addr_str))
        == mnet_error) return;

    printf("%s:%u\n", addr_str, mnet_addr_get_port(addr));
}

void server()
{
    printf("server()\n");

    mnet_socket_t server = mnet_socket(mnet_af_inet, mnet_sock_dgram, mnet_ipproto_udp);
    if (!mnet_socket_is_valid(server))
    {
        printf("Failed to create socket\n");
        return;
    }

    mnet_sockaddr_in_t addr;
    if (mnet_addr_any_ipv4(&addr, 9090) != mnet_ok)
    {
        printf("Failed to create address\n");
        mnet_close(server);
        return;
    }

    if (mnet_bind(server, MNET_SOCKADDR(addr), sizeof(addr)) != mnet_ok)
    {
        printf("Failed to bind: %s\n", mnet_error_string(mnet_get_platform_error()));
        mnet_close(server);
        return;
    }

    printf("UDP server listening on port 9090...\n");

    while (1)
    {
        char buffer[1024];
        mnet_sockaddr_in_t client_addr;
        socklen_t client_len = sizeof(client_addr);

        int bytes = mnet_recvfrom(server, buffer, sizeof(buffer) - 1,
                                  mnet_msg_default,
                                  (mnet_sockaddr_t*)&client_addr,
                                  &client_len);

        if (bytes > 0)
        {
            buffer[bytes] = '\0';  // Null terminate

            char client_ip[MNET_IP_STRLEN];
            const uint16_t client_port = mnet_addr_get_port((mnet_sockaddr_t*)&client_addr);
            if (mnet_addr_ip_to_string((mnet_sockaddr_t*)&client_addr,
                                   client_ip, sizeof(client_ip)) == mnet_ok)
            {
                printf("Received from %s:%u: %s\n", client_ip, client_port, buffer);
            }

            mnet_sendto(server, buffer, (size_t)bytes, mnet_msg_default,
                       (mnet_sockaddr_t*)&client_addr, client_len);
        }
        else if (bytes < 0)
        {
            printf("Error receiving: %s\n", mnet_error_string(mnet_get_platform_error()));
            break;
        }
    }

    mnet_close(server);
}

void client()
{
    if (mnet_initialize() != 0)
    {
        printf("Failed to initialize mnet\n");
        return;
    }

    mnet_socket_t client = mnet_socket(mnet_af_inet, mnet_sock_dgram, mnet_ipproto_udp);
    if (!mnet_socket_is_valid(client))
    {
        printf("Failed to create socket\n");
        mnet_cleanup();
        return;
    }

    mnet_sockaddr_in_t server_addr;
    if (mnet_addr_ipv4(&server_addr, "127.0.0.1", 9090) != mnet_ok)
    {
        printf("Failed to create address\n");
        mnet_close(client);
        return;
    }

    const char* message = "Hello, UDP server!";
    int sent = mnet_sendto(client, message, strlen(message), mnet_msg_default,
                          MNET_SOCKADDR(server_addr), sizeof(server_addr));

    if (sent > 0)
    {
        printf("Sent %d bytes to server\n", sent);

        char buffer[1024];
        mnet_sockaddr_in_t from_addr;
        socklen_t from_len = sizeof(from_addr);

        int bytes = mnet_recvfrom(client, buffer, sizeof(buffer) - 1,
                                  mnet_msg_default,
                                  (mnet_sockaddr_t*)&from_addr,
                                  &from_len);

        if (bytes > 0)
        {
            buffer[bytes] = '\0';
            printf("Received echo: %s\n", buffer);
        }
    }
    else
    {
        printf("Failed to send: %s\n", mnet_error_string(mnet_get_platform_error()));
    }

    mnet_close(client);
}

void dev_main()
{

}

int main(void)
{
    if (mnet_initialize() != 0)
    {
        fprintf(stderr,"mnet_initialize() failed\n");
    }

#ifdef DEV_CLIENT
    client();
#elif defined(DEV_SERVER)
    server();
#else
    dev_main();
#endif

    mnet_cleanup();
}