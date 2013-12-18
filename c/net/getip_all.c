#include <stdio.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

#define INT_TO_ADDR(_addr) \
    (_addr & 0xFF), \
    (_addr >> 8 & 0xFF), \
    (_addr >> 16 & 0xFF), \
    (_addr >> 24 & 0xFF)

void test()
{
    struct ifconf ifc;
    struct ifreq ifr[10];
    int sd, ifc_num, addr, bcast, mask, network, i;

    /* Create a socket so we can use ioctl on the file 
     * descriptor to retrieve the interface info. 
     */

    sd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sd > 0)
    {
        ifc.ifc_len = sizeof(ifr);
        ifc.ifc_ifcu.ifcu_buf = (caddr_t)ifr;

        if (ioctl(sd, SIOCGIFCONF, &ifc) == 0)
        {
            ifc_num = ifc.ifc_len / sizeof(struct ifreq);
            printf("%d interfaces found\n", ifc_num);

            for (i = 0; i < ifc_num; ++i)
            {
                if (ifr[i].ifr_addr.sa_family != AF_INET)
                {
                    continue;
                }

                /* display the interface name */
                printf("%d) interface: %s\n", i+1, ifr[i].ifr_name);

                /* Retrieve the IP address, broadcast address, and subnet mask. */
                if (ioctl(sd, SIOCGIFADDR, &ifr[i]) == 0)
                {
                    addr = ((struct sockaddr_in *)(&ifr[i].ifr_addr))->sin_addr.s_addr;
                    printf("%d) address: %d.%d.%d.%d\n", i+1, INT_TO_ADDR(addr));

                    struct in_addr inaddr = ((struct sockaddr_in *)(&ifr[i].ifr_addr))->sin_addr;
                    printf("\t%s\n", inet_ntoa(inaddr));
                }
                if (ioctl(sd, SIOCGIFHWADDR, &ifr[i]) == 0)
                {
                    char *mac = (unsigned char*)(ifr[i].ifr_hwaddr.sa_data);
                    printf("mac : %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                }


                if (ioctl(sd, SIOCGIFBRDADDR, &ifr[i]) == 0)
                {
                    bcast = ((struct sockaddr_in *)(&ifr[i].ifr_broadaddr))->sin_addr.s_addr;
                    printf("%d) broadcast: %d.%d.%d.%d\n", i+1, INT_TO_ADDR(bcast));
                }
                if (ioctl(sd, SIOCGIFNETMASK, &ifr[i]) == 0)
                {
                    mask = ((struct sockaddr_in *)(&ifr[i].ifr_netmask))->sin_addr.s_addr;
                    printf("%d) netmask: %d.%d.%d.%d\n", i+1, INT_TO_ADDR(mask));
                }                

                /* Compute the current network value from the address and netmask. */
                network = addr & mask;
                printf("%d) network: %d.%d.%d.%d\n", i+1, INT_TO_ADDR(network));
            }                      
        }

        close(sd);
    }

}

int main()
{
    test();
    return 0;
}
