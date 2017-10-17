
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#define __USE_GNU 1
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <ctype.h>
#include <err.h>
#include <rpcsvc/ypclnt.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>



int show_mac(char *buf,char *mac_dst)
{

	int argc=2;
	struct ifreq ifreq;
	int sock = 0;
	char mac[32] = "";

	if(argc < 2){
		printf("Usage: ./main eth0");
		return 1;
	}

	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("error sock");
		return 2;
	}

	strcpy(ifreq.ifr_name,buf);
	if(ioctl(sock,SIOCGIFHWADDR,&ifreq) < 0)
	{
		perror("error ioctl");
		return 3;
	}

	int i = 0;
	for(i = 0; i < 6; i++){
		sprintf(mac+3*i, "%02X:", (unsigned char)ifreq.ifr_hwaddr.sa_data[i]);
	}
	mac[strlen(mac) - 1] = 0;
	//printf("%s\n", mac);
	strcpy(mac_dst,mac);
	return 0;

}

int get_gateway(unsigned long *p)
{
	FILE *fp;
	char buf[256]; // 128 is enough for linux
	char iface[16];
	unsigned long dest_addr, gate_addr;
	*p = INADDR_NONE;
	fp = fopen("/proc/net/route", "r");
	if (fp == NULL)
		return -1;
	/* Skip title line */
	fgets(buf, sizeof(buf), fp);
	while (fgets(buf, sizeof(buf), fp)) {
		if (sscanf(buf, "%s\t%lX\t%lX", iface,       &dest_addr, &gate_addr) != 3 ||
				dest_addr != 0)
			continue;
		*p = gate_addr;
		break;
	}

	fclose(fp);
	return 0;
}

void show_gateway(char *gateway)
{
	unsigned long gw;
	get_gateway(&gw);
	struct in_addr a;
	a.s_addr=gw;
	char *b=inet_ntoa(a);
	strcpy(gateway,b);
//	printf("gate %s \n",b);
}
void get_ip(char *mac,char *ip,char *netmask,char *gateway )
{
	int flags=NI_NUMERICHOST;// get host ip rather than host name
	struct ifaddrs *ifa, *ifap;
	int family ;
	int addrlen ;
	int ret ;
	char buf[255];

	if (getifaddrs(&ifa) != 0)//get all address of interfaces
		errx(1, "%s", strerror(errno));
	for (ifap = ifa; ifap != NULL; ifap = ifap->ifa_next) {//do the loop on the chain table
		if (ifap->ifa_addr == NULL)// ip exist
			continue;

		if (ifap->ifa_flags & IFF_LOOPBACK)//skip loopback ip
			continue;


		if (!(ifap->ifa_flags & IFF_UP))// skip interface  which is not up
			continue;

		/* Only handle IPv4 and IPv6 addresses */
		family = ifap->ifa_addr->sa_family;
		if (family != AF_INET && family != AF_INET6)
			continue;
		addrlen = (family == AF_INET) ? sizeof(struct sockaddr_in) :
				sizeof(struct sockaddr_in6);

		/* Skip IPv6 link-local addresses */
		if (family == AF_INET6) {
			struct sockaddr_in6 *sin6;

			sin6 = (struct sockaddr_in6 *)ifap->ifa_addr;
			if (IN6_IS_ADDR_LINKLOCAL(&sin6->sin6_addr) ||
					IN6_IS_ADDR_MC_LINKLOCAL(&sin6->sin6_addr))
				continue;
		}
		addrlen = (family == AF_INET) ? sizeof(struct sockaddr_in)  :sizeof(struct sockaddr_in6);
		ret = getnameinfo(ifap->ifa_addr, addrlen,buf, sizeof(buf), NULL, 0, flags);
		strcpy(ip,buf);

		ret = getnameinfo(ifap->ifa_netmask, addrlen,buf, sizeof(buf), NULL, 0, flags);
		strcpy(netmask,buf);
		ret = getnameinfo(ifap->ifa_name, addrlen,buf, sizeof(buf), NULL, 0, flags);

		show_mac(ifap->ifa_name,mac);
		show_gateway(gateway);
	}
	freeifaddrs(ifa);
}
char local_mac[20];
char local_ip[20] = {'\0'};
char local_netmask[20] = {'\0'};
char local_gateway[20] = {'\0'};
int main()
{
	get_ip(local_mac,local_ip,local_netmask,local_gateway );
	printf("%s \n%s \n%s \n%s \n",local_mac,local_ip,local_netmask,local_gateway );
	return 1;
}
