/* IP-related user commands */
#include <stdio.h>
#include "options.h"	/* added for SERIALTEST */
#include "global.h"
#include "mbuf.h"
#include "internet.h"
#include "timer.h"
#include "netuser.h"
#include "iface.h"
#include "ip.h"
#include "cmdparse.h"

int doipaddr(),doipstat(),dottl(),atoi();
extern char badhost[];
struct cmds ipcmds[] = {
	"address",	doipaddr,	0,	NULLCHAR,	NULLCHAR,
	"status",	doipstat,	0,	NULLCHAR,	NULLCHAR,
	"ttl",		dottl,		0,	NULLCHAR,	NULLCHAR,
	NULLCHAR,	NULLFP,		0,
		"ip subcommands: address status ttl",	NULLCHAR,
};
doip(argc,argv)
int argc;
char *argv[];
{
	return subcmd(ipcmds,argc,argv);
}
int
doipaddr(argc,argv)
int argc;
char *argv[];
{
	char *inet_ntoa();
	int32 n;

	if(argc < 2) {
		printf("%s\n",inet_ntoa(ip_addr));
	} else if((n = resolve(argv[1])) == 0){
		printf(badhost,argv[1]);
		return 1;
	} else
		ip_addr = n;
	return 0;
}
int
dottl(argc,argv)     /* note that ip_ttl is declared as char but it probably */
char *argv[];			/* should have been unsigned char - K5JB */
{
	if(argc < 2)
		printf("%u\n",uchar(ip_ttl));
	else
		ip_ttl = (char)atoi(argv[1]);
	return 0;
}

/* "route" subcommands */
int doadd(),dodrop();
static struct cmds rtcmds[] = {
	"add", doadd, 3,
	"route add <dest addr>[/<bits>] <if name> [gateway] [metric]",
	"Add failed",

	"drop", dodrop, 2,
	"route drop <dest addr>[/<bits>]",
	"Not in table",

	NULLCHAR, NULLFP, 0,
	"route subcommands: add, drop",
	NULLCHAR, 
};

/* Display and/or manipulate routing table */
int
doroute(argc,argv)
int argc;
char *argv[];
{
	if(argc < 2){
		dumproute();
		return 0;
	}
	return subcmd(rtcmds,argc,argv);
}
/* Add an entry to the routing table
 * E.g., "add 1.2.3.4 ax0 5.6.7.8 3"
 */
int
doadd(argc,argv)
int argc;
char *argv[];
{
	struct interface *ifp;
	int32 dest,gateway;
	unsigned bits;
	char *bitp;
	int metric;

	if(strcmp(argv[1],"default") == 0){
		dest = 0;
		bits = 0;
	} else {
		if((dest = resolve(argv[1])) == 0){
			printf(badhost,argv[1]);
			return 1;
		}

		/* If IP address is followed by an optional slash and
		 * a length field, (e.g., 128.96/16) get it;
		 * otherwise assume a full 32-bit address
		 */
		if((bitp = index(argv[1],'/')) != NULLCHAR){
			bitp++;
			bits = (unsigned)atoi(bitp);
		} else
			bits = 32;
	}
	for(ifp=ifaces;ifp != NULLIF;ifp = ifp->next){
		if(strcmp(argv[2],ifp->name) == 0)
			break;
	}
	if(ifp == NULLIF){
		printf("Interface \"%s\" unknown\n",argv[2]);
		return 1;
	}
	if(argc > 3){
		if((gateway = resolve(argv[3])) == 0){
			printf(badhost,argv[3]);
			return 1;
		}
	} else {
		gateway = 0;
	}
	if(argc > 4)
		metric = atoi(argv[4]);
	else
		metric = 0;

	rt_add(dest,bits,gateway,metric,ifp);
	return 0;
}
/* Drop an entry from the routing table
 * E.g., "drop 128.96/16
 */
int
dodrop(argc,argv)
int argc;
char *argv[];
{
	char *bitp;
	unsigned bits;
	int32 n;

	if(strcmp(argv[1],"default") == 0){
		n = 0;
		bits = 0;
	} else {
		/* If IP address is followed by an optional slash and length field,
		 * (e.g., 128.96/16) get it; otherwise assume a full 32-bit address
		 */
		if((bitp = index(argv[1],'/')) != NULLCHAR){
			bitp++;
			bits = (unsigned)atoi(bitp);
		} else
			bits = 32;

		if((n = resolve(argv[1])) == 0){
			printf(badhost,argv[1]);
			return 1;
		}
	}
	return rt_drop(n,bits);
}

/* Dump IP routing table
 * Dest              Length    Interface    Gateway          Metric
 * 192.001.002.003   32        sl0          192.002.003.004       4
 */
int
dumproute()
{
	register unsigned int i,bits;
	register struct route *rp;

	printf("Dest              Length    Interface    Gateway          Metric\n");
	if(r_default.interface != NULLIF){
		printf("default           0         %-13s",
		 r_default.interface->name);
		if(r_default.gateway != 0)
			printf("%-17s",inet_ntoa(r_default.gateway));
		else
			printf("%-17s","");
		printf("%6u\n",r_default.metric);
	}
	for(bits=1;bits<=32;bits++){
		for(i=0;i<NROUTE;i++){
			for(rp = routes[bits-1][i];rp != NULLROUTE;rp = rp->next){
				printf("%-18s",inet_ntoa(rp->target));
				printf("%-10u",bits);
				printf("%-13s",rp->interface->name);
				if(rp->gateway != 0)
					printf("%-17s",inet_ntoa(rp->gateway));
				else
					printf("%-17s","");
				printf("%6u\n",rp->metric);
			}
		}
	}
	return 0;
}

int16 freeps;	/* can be removed after test, see various ax_recv() calls */

extern int16 sliphiwater;	/* maybe remove after test */
#if defined(MSDOS) && defined(SERIALTEST)
int16 serial_err;
int32 tot_rframes;
#endif


int
doipstat(argc,argv)
int argc;
char *argv[];
{
	extern struct ip_stats ip_stats;
	extern struct reasm *reasmq;
	register struct reasm *rp;
	register struct frag *fp;
	char *inet_ntoa();
	extern int16 maxslipq,slipbalks;	/* in slip.c */

	if(argc > 1 && strcmp(argv[1],"clear") == 0){
		slipbalks = 0;
		sliphiwater = 0;
		freeps = 0;
#if defined(SERIALTEST) && defined(MSDOS)
		serial_err = 0;
		tot_rframes = 0;
#endif
		return 0;
	}
#if defined(SERIALTEST) && defined(MSDOS)
	printf(
	"SLIP: TX balks %u, (hi water %u/%u); Bad KISS hdrs: %u, RX ORuns: %u/%lu\n",
		slipbalks,sliphiwater,maxslipq,freeps,serial_err,tot_rframes);
#else
	printf("SLIP: TX slip balks %u, (hi water %u/%u); Bad KISS hdrs: %u\n",
		slipbalks,sliphiwater,maxslipq,freeps);
#endif
	printf("IP: total %ld, runt %u, len err %u, vers err %u,",
		ip_stats.total,ip_stats.runt,ip_stats.length,ip_stats.version);
	printf(" chksum err %u, badproto %u\n",
		ip_stats.checksum,ip_stats.badproto);

	if(reasmq != NULLREASM)
		printf("Reassembly fragments:\n");
	for(rp = reasmq;rp != NULLREASM;rp = rp->next){
		printf("src %s,",inet_ntoa(rp->source));
		printf(" dest %s,",inet_ntoa(rp->dest));
		printf(" id %u, pctl %u, time %lu, len %u\n",
			rp->id,uchar(rp->protocol),rp->timer.count,rp->length);
		for(fp = rp->fraglist;fp != NULLFRAG;fp = fp->next){
			printf(" offset %u, last %u\n",fp->offset,fp->last);
		}
	}
	doicmpstat();
	return 0;
}

