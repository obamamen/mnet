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

    mnet_sockaddr_in_t host_addr;
    mnet_socket_t host_socket;

    host_socket = mnet_socket(
        mnet_af_inet,
        mnet_sock_stream,
        mnet_ipproto_default);

    mnet_addr_any_ipv4(
        &host_addr,
        2020);

    mnet_bind(host_socket, MNET_SOCKADDR(host_addr), sizeof(host_addr));

    if (mnet_socket_is_valid(host_socket))
    {
        printf("valid socket: %llu \n", host_socket);
    } else
    {
        printf("invalid socket: %llu \n", host_socket);
    }

    print_addr((mnet_sockaddr_t*)&host_addr);

    mnet_listen(host_socket, 1024);

    mnet_sockaddr_in_t client1_addr;
    socklen_t client1_len;
    mnet_accept(host_socket,
        (void*)&client1_addr, &client1_len);

    print_addr((void*)&client1_addr);

    mnet_close(host_socket);
}

void client()
{
    printf("client()\n");
    struct addrinfo* google_addr;
    mnet_getaddrinfo(
        "google.com",
        "http",
        NULL,
        &google_addr);

    print_addr(google_addr->ai_addr);

    mnet_freeaddrinfo(google_addr);

    mnet_socket_t client_socket;
    mnet_sockaddr_in_t host_addr;

    client_socket = mnet_socket(
        mnet_af_inet,
        mnet_sock_stream,
        mnet_ipproto_default);

    mnet_addr_ipv4(&host_addr,
        "127.0.0.1",
        2020);

    MNET_CONNECT(
        client_socket,
        host_addr);

    mnet_sockaddr_in_t local_addr;
    socklen_t local_len;
    mnet_getsockname(client_socket,
        (void*)&local_addr, &local_len);

    print_addr((void*)&local_addr);

    mnet_close(client_socket);
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