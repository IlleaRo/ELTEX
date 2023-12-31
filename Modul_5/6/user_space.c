#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#define NETLINK_USER 31

#define MAX_PAYLOAD 1024 /* maximum payload size*/



int main()
{
    struct nlmsghdr *nlh = NULL;
    struct iovec iov;
    struct msghdr msg;
    int sock_fd;

    struct sockaddr_nl src_addr, dest_addr;
    
    if ((sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER)) == -1)
    {
        perror("SOCKET: ");
        exit(EXIT_FAILURE);
    }
    memset(&src_addr, 0, sizeof(src_addr));
    
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid(); /* self pid */

    bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

    memset(&dest_addr, 0, sizeof(dest_addr));

    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; /* For Linux Kernel */
    dest_addr.nl_groups = 0; /* unicast */

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    strcpy(NLMSG_DATA(nlh), "Hello from user space!");

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    printf("Sending message to kernel\n");
    if (sendmsg(sock_fd, &msg, 0) == -1)
    {
        perror("SENDMSG: ");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for message from kernel\n");

    /* Read message from kernel */
    if (recvmsg(sock_fd, &msg, 0) == -1)
    {
        perror("RECMSG: ");
        exit(EXIT_FAILURE);
    }
    printf("Received message payload: %s\n", NLMSG_DATA(nlh));
    close(sock_fd);
    exit(EXIT_SUCCESS);
}