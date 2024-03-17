/* System-dependent definitions of various files, spool directories, etc */

#include "options.h"
#include "config.h"	/* K5JB */
#include "global.h"

#if	(defined(__TURBOC__) || defined(LATTICE) || defined(MAC) || defined(ATARI_ST))
/* These compilers require special open modes when reading binary files
 *
 * "The single most brilliant design decision in all of UNIX was the
 * choice of a SINGLE character as the end-of-line indicator" -- M. O'Dell
 *
 * "Whoever picked the end-of-line conventions for MS-DOS and the Macintosh
 * should be shot!" -- P. Karn's corollary to O'Dells' declaration
 *
 * Index definitions for this array are in global.h
 */
char *binmode[] = {
	"rb",	/* Read binary */
	"wb",	/* Write binary */
	"ab"    /* Append binary */
};
#else
/* fopen modes for binary files under Aztec -- same as UNIX */
char *binmode[] = {
	"r",	/* Read */
	"w",	/* Write */
	"a"     /* Append */
};
#endif

#ifdef	ATARI_ST
char *startup	 = "\\net.rc";			/* Initialization file */
char *userfile	 = "\\ftpusers";		/* Authorized FTP users */
char *hosts	 = "\\hosts.net";		/* Unix style host table */
char *mailspool = "\\spool\\mail";		/* Incoming mail */
char *mailqdir	 = "\\spool\\mqueue";		/* Outgoing mail spool */
char *mailqueue = "\\spool\\mqueue\\*.wrk";	/* Outgoing mail work files */
char *routeqdir = "\\spool\\rqueue";		/* Queue for router */
char *alias	 = "\\alias";			/* The alias file */
#ifdef	_FINGER
char *fingersuf = ".txt";		/* Text info for finger command */
char *fingerpath = "\\finger\\";	/* Path to finger info files */
#endif
#endif

#if	defined(MSDOS) || defined(__TURBOC__) /* K5JB */
char *homedir = "";
char *startup = "autoexec.net";	/* Initialization file */
char *userfile = "ftpusers";		/* Authorized FTP users and passwords */
char *hosts = "hosts.net";		/* Network host table */
char *spool = "/spool";
char *mailspool = "mail";	/* Incoming mail */
char *mailqdir = "mqueue";	/* Outgoing mail spool */
char *mailqueue = "mqueue/*.wrk";/* Outgoing mail work files */
char *routeqdir = "rqueue";	/* queue for router */
char *alias = "alias";			/* the alias file */
char *helpbox = "helpbox.hlp";	/* help file for mbox */
char *public = "/public";		/* where downloadable files are */
#ifdef _FINGER
char *fingersuf = ".txt";		/* Text info for finger command */
char *fingerpath = "finger/";		/* Path to finger info files */
#endif
#endif

#ifdef MULPORT
char *mdigilist = "digilist"; /* GRAPES mulport code */
char *mexlist = "exlist";
#endif

#ifdef	UNIX
char *homedir = ".";
char *netexe = "/usr/net/net";	/* where the binary lives for reset */
char *startup = "startup.net";	/* Initialization file */
/*char *config = "config.net";*/	/* Device configuration list - not used */
char *userfile = "ftpusers";
char *hosts = "hosts.net";
char *spool = "/usr/spool";		/* path to the mail files */
char *mailspool = "mail";
char *mailqdir = "mqueue";
char *mailqueue = "mqueue/*.wrk";
char *routeqdir = "rqueue";		/* queue for router */
char *alias = "aliases";			/* the alias file */
char *helpbox = "helpbox.hlp";		/* mbox help file */
char *public = "/usr/public";		/* downloadable mbox files here */
#ifdef _FINGER
char *fingersuf = ".txt";		/* Text info for finger command */
char *fingerpath = "finger/";		/* Path to finger info files */
#endif
#endif

#ifdef	AMIGA
char startup[] = "TCPIP:net.start";
char config[] = "TCPIP:config.net";	/* Device configuration list */
char userfile[] = "TCPIP:ftpusers";
char hosts[] = "TCPIP:hosts.net";
char mailspool[] = "TCPIP:spool/mail";
char mailqdir[] = "TCPIP:spool/mqueue";
char mailqueue[] = "TCPIP:spool/mqueue/*.wrk";
char routeqdir[] = "TCPIP:spool/rqueue";		/* queue for router */
char alias[] = "TCPIP:alias";	/* the alias file */
#ifdef _FINGER
char fingersuf[] = ".txt";		/* Text info for finger command */
char fingerpath[] = "TCPIP:finger/";	/* Path to finger info files */
#endif
#endif

#ifdef	MAC
/* can you believe this?  I ought to clean all this crap out -- REAL ham
would use a MAC! -- Joe, K5JB */
char startup[] ="Mikes Hard Disk:net.start";
char config[] = "Mikes Hard Disk:config.net";	/* Device configuration list */
char userfile[] = "Mikes Hard Disk:ftpusers";
char hosts[] = "Mikes Hard Disk:hosts.net";
char mailspool[] = "Mikes Hard Disk:spool:mail:";
char mailqdir[] = "Mikes Hard Disk:spool:mqueue:";
char mailqueue[] = "Mikes Hard Disk:spool:mqueue:*.wrk";
char routeqdir[] = "Mikes Hard Disk:spool/rqueue:";	/* queue for router */
char alias[] = "Mikes Hard Disk:alias";	/* the alias file */
#ifdef _FINGER
/* is this for real? */
char fingersuf[] = ".txt";		/* Text info for finger command */
char fingerpath[] = "Mikes Hard Disk:finger/";	/* Path to finger info files */
#endif
#endif
