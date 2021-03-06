h56472
s 00005/00005/00107
d D 4.2 84/05/05 21:51:06 mckusick 3 2
c fixes from Lou Salkind at NYU
e
s 00000/00000/00112
d D 4.1 82/05/12 11:14:27 rrh 2 1
c sync to release 4.1
e
s 00112/00000/00000
d D 1.1 82/05/12 11:03:41 rrh 1 0
c date and time created 82/05/12 11:03:41 by rrh
e
u
U
t
T
I 1
D 3
#
E 3
I 3
#ifndef lint
E 3
static char sccsid[] = "	%M%	%I%	%E%	";
I 3
#endif

/* From Lou Salkind: compat/RCS/unixstart.c,v 1.2 84/01/31 13:34:27 */

E 3
/*	Start up a version 6 or version 7 pdp-11 UNIX compatability mode
 *	program. Must set up the memory layout with args etc.
 *	Art Wetzel	August 1979
 */
#include <stdio.h>
#include "defs.h"
#define	MAXARGS	100
start(argv, envp) unsigned char **argv, **envp; {
	register unsigned char *sp, *ap;
	register unsigned short *ssp;
	register int i, argc, envc;
	unsigned char *envps[MAXARGS], *argps[MAXARGS], **av, *p1, *p2;
	extern unsigned char *progname, *nameend;
	/* set up initial memory layout for unix */
	/* set stack pointer to top of memory */
	sp = memsiz;
#ifdef	V7UNIX
	/* zero top 2 bytes */
	*(--sp) = 0;
	*(--sp) = 0;
	/* point to environment pointer list */
	av = envp;
	envc = 0;
	/* count up number of env elements */
	while(*(av++)) envc++;
	/* last UNIX V7 env ptr is 0 */
	envps[envc] = (unsigned char *)0;
	/* copy actual environment (assume byte text) - last first */
	for(i=envc-1; i>=0; i--) {
		ap = envp[i];
		while(*(ap++)) ;
		while(ap != envp[i]) *(--sp) = *(--ap);
		/* force stack word alignment - required per arg in v7 */
		if((int)sp & 1) {
			ap = sp--;
			while((*(ap-1) = *ap)) ap++;
		}
		envps[i] = sp;
	}
#endif
	/* point to argument pointer list */
	av = argv;
	argc = 0;
	/* count up number of args */
	while(*(av++)) argc++;
#ifdef V7UNIX
	/* last UNIX V7 arg ptr is 0 */
	argps[argc] = (unsigned char *)0;
#endif
#ifdef V6UNIX
	/* last UNIX V6 arg ptr is -1 */
	argps[argc] = (unsigned char *)-1;
#endif
	/* copy actual arguments (assume byte text) - last first */
	for(i=argc-1; i>=0; i--) {
		ap = argv[i];
		while(*(ap++)) ;
		while(ap != argv[i]) *(--sp) = *(--ap);
		/* force stack word alignment - required per arg in v7 */
		if((int)sp & 1) {
			ap = sp--;
			while((*(ap-1) = *ap)) ap++;
		}
		argps[i] = sp;
	}
	ssp = (unsigned short *)sp;
#ifdef	V7UNIX
D 3
	/* clear a word */
	*(--ssp) = 0;
E 3
	/* set up environment pointers */
	for(i=envc; i>=0; i--) {
		*(--ssp) = (short)(long)envps[i];
	}
D 3
	/* clear another word */
	*(--ssp) = 0;
E 3
#endif
	/* set up argument pointers */
	for(i=argc; i>=0; i--) {
		*(--ssp) = (short)(long)argps[i];
	}
	/* then argument count */
	*(--ssp) = argc;
	/* set up stack pointer */
	regs[6] = (int)ssp;
	/* set up a psl with cleared condition codes */
	psl = 0x83c00000;
	/* copy out part of the program name and args where ps can get them */
	/* flag it with a * so it shows up as a compatability mode process */
	/* check for case with no env and reset nameend */
	if(nameend < progname) nameend = (unsigned char *)2147483647;
	for(p1=progname, *p1++ = '*', i=1, p2=argv[0]; p1<nameend; p1++) {
		if((*p1 = *p2))
			p2++;
		else if(argv[i])
			p2 = argv[i++];
		else break;
	}
	while(p1 < nameend) *p1++ = ' ';
	*p1 = 0;
	/* clear out registers other than sp */
	regs[0] = 0;
	regs[1] = 0;
	regs[2] = 0;
	regs[3] = 0;
	regs[4] = 0;
	regs[5] = 0;
	/* finally get around to actually starting up in compatability mode */
	incompat++;
	compat();
}
E 1
