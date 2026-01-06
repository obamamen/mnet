#include <stdio.h>
#include "mnet.h"

int main(void)
{
    if (mnet_initialize() != 0)
    {
        fprintf(stderr,"mnet_initialize() failed\n");
    }

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

    mnet_listen(host_socket, 1024);

    mnet_cleanup();
}