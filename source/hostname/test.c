
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
void  show_ip()
{
	int flags=NI_NUMERICHOST;
//	struct addrinfo *res;
//	struct addrinfo hints;
	struct ifaddrs *ifa, *ifap;
	int family ;
	int addrlen ;
	int ret ;
	char buf[255];

	if (getifaddrs(&ifa) != 0)
		errx(1, "%s", strerror(errno));
	for (ifap = ifa; ifap != NULL; ifap = ifap->ifa_next) {
		family = ifap->ifa_addr->sa_family;
		addrlen = (family == AF_INET) ? sizeof(struct sockaddr_in)  :sizeof(struct sockaddr_in6);
		ret = getnameinfo(ifap->ifa_addr, addrlen,buf, sizeof(buf), NULL, 0, flags);
		printf("==>%s \n", buf);
	}
	freeifaddrs(ifa);
}
void show_gateway()
{

}
void show_mask()
{

}
int main(){
	show_ip();
//	struct addrinfo *res;
//	struct addrinfo hints;
//	struct ifaddrs *ifa, *ifap;
//	char *p;
//	int ret;
//
//char buf[255];
//			int flags, family, addrlen;
//
//			/* What kind of information do we want from getnameinfo()? */
//			flags = NI_NUMERICHOST ;
//
//			if (getifaddrs(&ifa) != 0)
//				errx(1, "%s", strerror(errno));
//			for (ifap = ifa; ifap != NULL; ifap = ifap->ifa_next) {
//				/* Skip interfaces that have no configured addresses */
//				if (ifap->ifa_addr == NULL)
//					continue;
//				/* Skip the loopback interface */
//				if (ifap->ifa_flags & IFF_LOOPBACK)
//					continue;
//				/* Skip interfaces that are not UP */
//				if (!(ifap->ifa_flags & IFF_UP))
//					continue;
//
//				/* Only handle IPv4 and IPv6 addresses */
//				family = ifap->ifa_addr->sa_family;
//				if (family != AF_INET && family != AF_INET6)
//					continue;
//
//				addrlen = (family == AF_INET) ? sizeof(struct sockaddr_in) :
//								sizeof(struct sockaddr_in6);
//
//				/* Skip IPv6 link-local addresses */
//				if (family == AF_INET6) {
//					struct sockaddr_in6 *sin6;
//
//					sin6 = (struct sockaddr_in6 *)ifap->ifa_addr;
//					if (IN6_IS_ADDR_LINKLOCAL(&sin6->sin6_addr) ||
//							IN6_IS_ADDR_MC_LINKLOCAL(&sin6->sin6_addr))
//						continue;
//				}
//
//				ret = getnameinfo(ifap->ifa_addr, addrlen,
//						  buf, sizeof(buf), NULL, 0, flags);
//
//				/* Just skip addresses that cannot be translated */
//				if (ret != 0) {
//				  //  if (type != ALL_FQDNS && ret != EAI_NONAME)
//					errx(1, "%s", gai_strerror(ret));
//				} else
//					printf("==>%s ", buf);
//			}
//			printf("\n");
//			freeifaddrs(ifa);
}
