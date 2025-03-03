/*
 *   $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/radvd-1.5/radvdump.c#1 $
 *
 *   Authors:
 *    Lars Fenneberg		<lf@elemental.net>
 *    Marko Myllynen		<myllynen@lut.fi>
 *
 *   This software is Copyright 1996-2000 by the above mentioned author(s),
 *   All Rights Reserved.
 *
 *   The license which is distributed with this software in the file COPYRIGHT
 *   applies to this software. If your distribution is missing this file, you
 *   may request it from <pekkas@netcore.fi>.
 *
 */

#include <config.h>
#include <includes.h>
#include <radvd.h>

char usage_str[] = "[-vhfe] [-d level]";

#ifdef HAVE_GETOPT_LONG
struct option prog_opt[] = {
	{"debug", 1, 0, 'd'},
	{"file-format", 0, 0, 'f'},
	{"exclude-defaults", 0, 0, 'e'},
	{"version", 0, 0, 'v'},
	{"help", 0, 0, 'h'},
	{NULL, 0, 0, 0}
};
#endif

char *pname;
int sock = -1;

void version(void);
void usage(void);
void print_ff(unsigned char *, int, struct sockaddr_in6 *, int, unsigned int, int);
void print_preferences(int);

int
main(int argc, char *argv[])
{
	unsigned char msg[MSG_SIZE];
	int c, len, hoplimit;
	int edefs = 0;
	struct sockaddr_in6 rcv_addr;
        struct in6_pktinfo *pkt_info = NULL;
#ifdef HAVE_GETOPT_LONG
	int opt_idx;
#endif

	pname = ((pname=strrchr(argv[0],'/')) != NULL)?pname+1:argv[0];

	/* parse args */
#ifdef HAVE_GETOPT_LONG
	while ((c = getopt_long(argc, argv, "d:fehv", prog_opt, &opt_idx)) > 0)
#else
	while ((c = getopt(argc, argv, "d:fehv")) > 0)
#endif
	{
		switch (c) {
		case 'd':
			set_debuglevel(atoi(optarg));
			break;
		case 'f':
			break;
		case 'e':
			edefs = 1;
			break;
		case 'v':
			version();
			break;
		case 'h':
			usage();
#ifdef HAVE_GETOPT_LONG
		case ':':
			fprintf(stderr, "%s: option %s: parameter expected\n", pname,
				prog_opt[opt_idx].name);
			exit(1);
#endif
		case '?':
			exit(1);
		}
	}
	
	if (log_open(L_STDERR, pname, NULL, 0) < 0)
		exit(1);

	/* get a raw socket for sending and receiving ICMPv6 messages */
	sock = open_icmpv6_socket();
	if (sock < 0)
		exit(1);
		
	for(;;)
	{
	        len = recv_rs_ra(sock, msg, &rcv_addr, &pkt_info, &hoplimit);
   	     	if (len > 0)
       	 	{
			struct icmp6_hdr *icmph;
	
			/*
			 * can this happen?
			 */

			if (len < sizeof(struct icmp6_hdr))
			{
				flog(LOG_WARNING, "received icmpv6 packet with invalid length: %d",
					len);
				exit(1);
			}

			icmph = (struct icmp6_hdr *) msg;

			if (icmph->icmp6_type != ND_ROUTER_SOLICIT &&
			    icmph->icmp6_type != ND_ROUTER_ADVERT)
			{
				/*
				 *	We just want to listen to RSs and RAs
				 */
			
				flog(LOG_ERR, "icmpv6 filter failed");
				exit(1);
			}

			dlog(LOG_DEBUG, 4, "receiver if_index: %u", pkt_info->ipi6_ifindex);

			if (icmph->icmp6_type == ND_ROUTER_SOLICIT)
			{
				/* not yet */	
			}
			else if (icmph->icmp6_type == ND_ROUTER_ADVERT)
				print_ff(msg, len, &rcv_addr, hoplimit, (unsigned int)pkt_info->ipi6_ifindex, edefs);
        	}
		else if (len == 0)
       	 	{
       	 		flog(LOG_ERR, "received zero lenght packet");
       	 		exit(1);
        	}
        	else
        	{
			flog(LOG_ERR, "recv_rs_ra: %s", strerror(errno));
			exit(1);
        	}
        }                                                                                            

	exit(0);
}

void
print_ff(unsigned char *msg, int len, struct sockaddr_in6 *addr, int hoplimit, unsigned int if_index, int edefs)
{
	/* XXX: hoplimit not being used for anything here.. */
	struct nd_router_advert *radvert;
	char addr_str[INET6_ADDRSTRLEN];
	uint8_t *opt_str;
	int orig_len = len;
	char if_name[IFNAMSIZ] = "";

	print_addr(&addr->sin6_addr, addr_str);
	printf("#\n# radvd configuration generated by radvdump %s\n", VERSION);
	printf("# based on Router Advertisement from %s\n", addr_str);
	if_indextoname(if_index, if_name);
	printf("# received by interface %s\n", if_name);
	printf("#\n\ninterface %s\n{\n\tAdvSendAdvert on;\n", if_name);

	printf("\t# Note: {Min,Max}RtrAdvInterval cannot be obtained with radvdump\n");

	radvert = (struct nd_router_advert *) msg;

	if (!edefs || DFLT_AdvManagedFlag != (ND_RA_FLAG_MANAGED == (radvert->nd_ra_flags_reserved & ND_RA_FLAG_MANAGED)))
	printf("\tAdvManagedFlag %s;\n", 
		(radvert->nd_ra_flags_reserved & ND_RA_FLAG_MANAGED)?"on":"off");

	if (!edefs || DFLT_AdvOtherConfigFlag != (ND_RA_FLAG_OTHER == (radvert->nd_ra_flags_reserved & ND_RA_FLAG_OTHER)))
	printf("\tAdvOtherConfigFlag %s;\n", 
		(radvert->nd_ra_flags_reserved & ND_RA_FLAG_OTHER)?"on":"off");

	if (!edefs || DFLT_AdvReachableTime != ntohl(radvert->nd_ra_reachable))
	printf("\tAdvReachableTime %u;\n", ntohl(radvert->nd_ra_reachable));

	if (!edefs || DFLT_AdvRetransTimer != ntohl(radvert->nd_ra_retransmit))
	printf("\tAdvRetransTimer %u;\n", ntohl(radvert->nd_ra_retransmit));

	if (!edefs || DFLT_AdvCurHopLimit != radvert->nd_ra_curhoplimit)
	printf("\tAdvCurHopLimit %u;\n", radvert->nd_ra_curhoplimit);

	if (!edefs || (3*DFLT_MaxRtrAdvInterval) != ntohs(radvert->nd_ra_router_lifetime))
	printf("\tAdvDefaultLifetime %hu;\n", ntohs(radvert->nd_ra_router_lifetime));

	/* Mobile IPv6 ext */
	if (!edefs || DFLT_AdvHomeAgentFlag != (ND_RA_FLAG_HOME_AGENT == (radvert->nd_ra_flags_reserved & ND_RA_FLAG_HOME_AGENT)))
	printf("\tAdvHomeAgentFlag %s;\n", 
		(radvert->nd_ra_flags_reserved & ND_RA_FLAG_HOME_AGENT)?"on":"off");

        /* Route Preferences and more specific routes */
        /* XXX two middlemost bits from 8 bit field */
	if (!edefs || (((radvert->nd_ra_flags_reserved & 0x18) >> 3) & 0xff) != DFLT_AdvDefaultPreference) {
	        printf("\tAdvDefaultPreference ");
	        print_preferences(((radvert->nd_ra_flags_reserved & 0x18) >> 3) & 0xff);
	        printf(";\n");
	}

	len -= sizeof(struct nd_router_advert);

	if (len == 0)
		return;
		
	opt_str = (uint8_t *)(msg + sizeof(struct nd_router_advert));
		
	while (len > 0)
	{
		int optlen;
		struct nd_opt_mtu *mtu;
		struct HomeAgentInfo *ha_info;

		if (len < 2)
		{
			flog(LOG_ERR, "trailing garbage in RA from %s", 
				addr_str);
			break;
		}
		
		optlen = (opt_str[1] << 3);

		if (optlen == 0) 
		{
			flog(LOG_ERR, "zero length option in RA");
			break;
		} 
		else if (optlen > len)
		{
			flog(LOG_ERR, "option length greater than total"
				" length in RA (type %d, optlen %d, len %d)", 
				(int)*opt_str, optlen, len);
			break;
		} 		

		switch (*opt_str)
		{
		case ND_OPT_MTU:
			mtu = (struct nd_opt_mtu *)opt_str;

			if (!edefs || DFLT_AdvLinkMTU != ntohl(mtu->nd_opt_mtu_mtu))
			printf("\tAdvLinkMTU %u;\n", ntohl(mtu->nd_opt_mtu_mtu));
			break;
		case ND_OPT_SOURCE_LINKADDR:
			/* XXX: !DFLT depends on current DFLT_ value */
			if (!edefs || !DFLT_AdvSourceLLAddress)
			printf("\tAdvSourceLLAddress on;\n");
			break;
		/* Mobile IPv6 ext */
		case ND_OPT_RTR_ADV_INTERVAL:
			/* XXX: !DFLT depends on current DFLT_ value */
			if (!edefs || !DFLT_AdvIntervalOpt)
			printf("\tAdvIntervalOpt on;\n");
			break;
		/* Mobile IPv6 ext */
		case ND_OPT_HOME_AGENT_INFO:
			ha_info = (struct HomeAgentInfo *)opt_str;

			/* XXX: we check DFLT_HomeAgentInfo by interface, and it's outside
			   of context here, so we always need to print it out.. */
			printf("\tAdvHomeAgentInfo on;\n");

			/* NEMO ext */
			if (!edefs || DFLT_AdvMobRtrSupportFlag != (ha_info->flags_reserved & ND_OPT_HAI_FLAG_SUPPORT_MR))
				printf("\tAdvMobRtrSupportFlag %s;\n", (ha_info->flags_reserved & ND_OPT_HAI_FLAG_SUPPORT_MR)?"on":"off");

			if (!edefs || DFLT_HomeAgentPreference != ntohs(ha_info->preference))
			printf("\tHomeAgentPreference %hu;\n", ntohs(ha_info->preference));
			/* Hum.. */
			if (!edefs || (3*DFLT_MaxRtrAdvInterval) != ntohs(ha_info->lifetime))
			printf("\tHomeAgentLifetime %hu;\n", ntohs(ha_info->lifetime));
			break;
		case ND_OPT_TARGET_LINKADDR:
		case ND_OPT_REDIRECTED_HEADER:
			flog(LOG_ERR, "invalid option %d in RA", (int)*opt_str);
			break;
		case ND_OPT_PREFIX_INFORMATION:
			break;
		case ND_OPT_ROUTE_INFORMATION:
			break;
		case ND_OPT_RDNSS_INFORMATION:
			break;
		default:
			dlog(LOG_DEBUG, 1, "unknown option %d in RA",
				(int)*opt_str);
			break;
		}
		
		len -= optlen;
		opt_str += optlen;
	}

	orig_len -= sizeof(struct nd_router_advert);

	if (orig_len == 0)
		return;

	opt_str = (uint8_t *)(msg + sizeof(struct nd_router_advert));
		
	while (orig_len > 0)
	{
		int optlen;
		struct nd_opt_prefix_info *pinfo;
		struct nd_opt_route_info_local *rinfo;
		struct nd_opt_rdnss_info_local *rdnss_info;
		char prefix_str[INET6_ADDRSTRLEN];

		if (orig_len < 2)
		{
			flog(LOG_ERR, "trailing garbage in RA from %s", 
				addr_str);
			break;
		}
		
		optlen = (opt_str[1] << 3);

		if (optlen == 0) 
		{
			flog(LOG_ERR, "zero length option in RA");
			break;
		} 
		else if (optlen > orig_len)
		{
			flog(LOG_ERR, "option length greater than total"
				" length in RA (type %d, optlen %d, len %d)", 
				(int)*opt_str, optlen, orig_len);
			break;
		} 		

		switch (*opt_str)
		{
		case ND_OPT_PREFIX_INFORMATION:
			pinfo = (struct nd_opt_prefix_info *) opt_str;
			
			print_addr(&pinfo->nd_opt_pi_prefix, prefix_str);	
				
			printf("\n\tprefix %s/%d\n\t{\n", prefix_str, pinfo->nd_opt_pi_prefix_len);

			if (ntohl(pinfo->nd_opt_pi_valid_time) == 0xffffffff)
			{		
				if (!edefs || DFLT_AdvValidLifetime != 0xffffffff)
				printf("\t\tAdvValidLifetime infinity; # (0xffffffff)\n");
			}
			else
			{
				if (!edefs || DFLT_AdvValidLifetime != ntohl(pinfo->nd_opt_pi_valid_time))
				printf("\t\tAdvValidLifetime %u;\n", ntohl(pinfo->nd_opt_pi_valid_time));
			}
			if (ntohl(pinfo->nd_opt_pi_preferred_time) == 0xffffffff)
			{
				if (!edefs || DFLT_AdvPreferredLifetime != 0xffffffff)
				printf("\t\tAdvPreferredLifetime infinity; # (0xffffffff)\n");
			}
			else
			{
				if (!edefs || DFLT_AdvPreferredLifetime != ntohl(pinfo->nd_opt_pi_preferred_time))
				printf("\t\tAdvPreferredLifetime %u;\n", ntohl(pinfo->nd_opt_pi_preferred_time));
			}

			if (!edefs || DFLT_AdvOnLinkFlag != (ND_OPT_PI_FLAG_ONLINK == (pinfo->nd_opt_pi_flags_reserved & ND_OPT_PI_FLAG_ONLINK)))
			printf("\t\tAdvOnLink %s;\n", 
				(pinfo->nd_opt_pi_flags_reserved & ND_OPT_PI_FLAG_ONLINK)?"on":"off");

			if (!edefs || DFLT_AdvAutonomousFlag != (ND_OPT_PI_FLAG_AUTO == (pinfo->nd_opt_pi_flags_reserved & ND_OPT_PI_FLAG_AUTO)))
			printf("\t\tAdvAutonomous %s;\n", 
				(pinfo->nd_opt_pi_flags_reserved & ND_OPT_PI_FLAG_AUTO)?"on":"off");

			/* Mobile IPv6 ext */
			if (!edefs || DFLT_AdvRouterAddr != (ND_OPT_PI_FLAG_RADDR == (pinfo->nd_opt_pi_flags_reserved & ND_OPT_PI_FLAG_RADDR)))
			printf("\t\tAdvRouterAddr %s;\n", 
				(pinfo->nd_opt_pi_flags_reserved & ND_OPT_PI_FLAG_RADDR)?"on":"off");

			printf("\t}; # End of prefix definition\n\n");
			break;
		case ND_OPT_ROUTE_INFORMATION:
			rinfo = (struct nd_opt_route_info_local *) opt_str;
			
			print_addr(&rinfo->nd_opt_ri_prefix, prefix_str);	
				
			printf("\n\troute %s/%d\n\t{\n", prefix_str, rinfo->nd_opt_ri_prefix_len);

			if (!edefs || (((radvert->nd_ra_flags_reserved & 0x18) >> 3) & 0xff) != DFLT_AdvRoutePreference) {
				printf("\t\tAdvRoutePreference ");
				print_preferences(((rinfo->nd_opt_ri_flags_reserved & 0x18) >> 3) & 0xff);
				printf(";\n");
			}
			
			/* XXX: we check DFLT_AdvRouteLifetime by interface, and it's outside of context here */
			if (ntohl(rinfo->nd_opt_ri_lifetime) == 0xffffffff)
				printf("\t\tAdvRouteLifetime infinity; # (0xffffffff)\n");
			else
				printf("\t\tAdvRouteLifetime %u;\n", ntohl(rinfo->nd_opt_ri_lifetime));

			printf("\t}; # End of route definition\n\n");
			break;
		case ND_OPT_RDNSS_INFORMATION:
			rdnss_info = (struct nd_opt_rdnss_info_local *) opt_str;

			printf("\n\tRDNSS");

			print_addr(&rdnss_info->nd_opt_rdnssi_addr1, prefix_str);
			printf(" %s", prefix_str);

			if (rdnss_info->nd_opt_rdnssi_len >= 5) {
				print_addr(&rdnss_info->nd_opt_rdnssi_addr2, prefix_str);
				printf(" %s", prefix_str);
			}
			if (rdnss_info->nd_opt_rdnssi_len >= 7) {
				print_addr(&rdnss_info->nd_opt_rdnssi_addr3, prefix_str);
				printf(" %s", prefix_str);
			}
			
			printf("\n\t{\n");
			if (!edefs 
			    || ((rdnss_info->nd_opt_rdnssi_pref_flag_reserved & ND_OPT_RDNSSI_PREF_MASK) >> ND_OPT_RDNSSI_PREF_SHIFT) != DFLT_AdvRDNSSPreference)
				printf("\t\tAdvRDNSSPreference %d;\n", 
				  (rdnss_info->nd_opt_rdnssi_pref_flag_reserved & ND_OPT_RDNSSI_PREF_MASK) >> ND_OPT_RDNSSI_PREF_SHIFT);

			if (!edefs 
			    || ((rdnss_info->nd_opt_rdnssi_pref_flag_reserved & ND_OPT_RDNSSI_FLAG_S) == 0 ) == DFLT_AdvRDNSSOpenFlag)
				printf("\t\tAdvRDNSSOpen %s;\n", rdnss_info->nd_opt_rdnssi_pref_flag_reserved & ND_OPT_RDNSSI_FLAG_S ? "on" : "off");

			/* as AdvRDNSSLifetime may depend on MaxRtrAdvInterval, it could change */
			if (ntohl(rdnss_info->nd_opt_rdnssi_lifetime) == 0xffffffff)
				printf("\t\tAdvRDNSSLifetime infinity; # (0xffffffff)\n");
			else
				printf("\t\tAdvRDNSSLifetime %u;\n", ntohl(rdnss_info->nd_opt_rdnssi_lifetime));
			
			printf("\t}; # End of RDNSS definition\n\n");
			break;
		default:
			break;
		}
		orig_len -= optlen;
		opt_str += optlen;
	}

	printf("}; # End of interface definition\n");

	fflush(stdout);
}

void
print_preferences(int p)
{
	switch (p) {
		case 0:
			printf("medium");
			break;
		case 1:
			printf("high");
			break;
		case 2:
			/* reserved, ignore */
			break;
		case 3:
			printf("low");
			break;
	}		
}

void
version(void)
{
	fprintf(stderr,"Version: %s\n\n", VERSION);
	fprintf(stderr,"Please send bug reports and suggestions to %s\n",
		CONTACT_EMAIL);
	exit(1);	
}

void
usage(void)
{
	fprintf(stderr,"usage: %s %s\n", pname, usage_str);
	exit(1);	
}
