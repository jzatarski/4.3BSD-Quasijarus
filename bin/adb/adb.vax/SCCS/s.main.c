h47053
s 00000/00000/00143
d D 4.3 82/04/01 06:36:25 root 3 2
c bug fixes to -w, output overflow, kernel debugging
e
s 00005/00000/00138
d D 4.2 81/05/15 00:53:54 root 2 1
c new attempt
e
s 00138/00000/00000
d D 4.1 81/05/14 15:19:47 root 1 0
c date and time created 81/05/14 15:19:47 by root
e
u
U
t
T
I 1
static	char sccsid[] = "%Z%%M% %I% %G%";
/*
 * adb - main command loop and error/interrupt handling
 */
#include "defs.h"

MSG		NOEOR;

INT		mkfault;
INT		executing;
INT		infile;
CHAR		*lp;
L_INT		maxoff;
L_INT		maxpos;
ADDR		sigint;
ADDR		sigqit;
INT		wtflag;
L_INT		maxfile;
STRING		errflg;
L_INT		exitflg;

CHAR		lastc;
INT		eof;

INT		lastcom;

long	maxoff = MAXOFF;
long	maxpos = MAXPOS;
char	*Ipath = "/usr/lib/adb";

main(argc, argv)
	register char **argv;
	int argc;
{

	mkioptab();
another:
	if (argc>1) {
		if (eqstr("-w", argv[1])) {
			wtflag = 2;		/* suitable for open() */
			argc--, argv++;
			goto another;
		}
I 2
		if (eqstr("-k", argv[1])) {
			kernel = 1;
			argc--, argv++;
			goto another;
		}
E 2
		if (argv[1][0] == '-' && argv[1][1] == 'I') {
			Ipath = argv[1]+2;
			argc--, argv++;
		}
	}
	if (argc > 1)
		symfil = argv[1];
	if (argc > 2)
		corfil = argv[2];
	xargc = argc;
	setsym(); setcor(); setvar();

	if ((sigint=signal(SIGINT,SIG_IGN)) != SIG_IGN) {
		sigint = fault;
		signal(SIGINT, fault);
	}
	sigqit = signal(SIGQUIT, SIG_IGN);
	setexit();
	if (executing)
		delbp();
	executing = 0;
	for (;;) {
		flushbuf();
		if (errflg) {
			printf("%s\n", errflg);
			exitflg = errflg;
			errflg = 0;
		}
		if (mkfault) {
			mkfault=0;
			printc('\n');
			prints(DBNAME);
		}
		lp=0; rdc(); lp--;
		if (eof) {
			if (infile) {
				iclose(-1, 0); eof=0; reset();
			} else
				done();
		} else
			exitflg = 0;
		command(0, lastcom);
		if (lp && lastc!='\n')
			error(NOEOR);
	}
}

done()
{
	endpcs();
	exit(exitflg);
}

L_INT
round(a,b)
REG L_INT a, b;
{
	REG L_INT w;
	w = (a/b)*b;
	IF a!=w THEN w += b; FI
	return(w);
}

/*
 * If there has been an error or a fault, take the error.
 */
chkerr()
{
	if (errflg || mkfault)
		error(errflg);
}

/*
 * An error occurred; save the message for later printing,
 * close open files, and reset to main command loop.
 */
error(n)
	char *n;
{
	errflg = n;
	iclose(0, 1); oclose();
	reset();
}

/*
 * An interrupt occurred; reset the interrupt
 * catch, seek to the end of the current file
 * and remember that there was a fault.
 */
fault(a)
{
	signal(a, fault);
	lseek(infile, 0L, 2);
	mkfault++;
}
E 1
