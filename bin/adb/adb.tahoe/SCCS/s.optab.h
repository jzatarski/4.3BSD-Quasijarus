h51350
s 00032/00000/00000
d D 1.1 86/02/25 16:04:41 sam 1 0
c date and time created 86/02/25 16:04:41 by sam
e
u
U
t
T
I 1
/*	%M%	%I%	%E%	*/

/*
 * Argument access types
 */
#define ACCA	(8<<3)	/* address only */
#define ACCR	(1<<3)	/* read */
#define ACCW	(2<<3)	/* write */
#define ACCM	(3<<3)	/* modify */
#define ACCB	(4<<3)	/* branch displacement */
#define ACCI	(5<<3)	/* XFC code */

/*
 * Argument data types
 */
#define TYPB	0	/* byte */
#define TYPW	1	/* word */
#define TYPL	2	/* long */
#define TYPQ	3	/* quad */
#define TYPF	4	/* float */
#define TYPD	5	/* double */

TYPE	struct optab	*OPTAB;
struct optab {
	char *iname;
	char val;
	char nargs;
	char argtype[6];
} optab[];
#define SYSSIZ 151
STRING	systab[];
STRING	regname[];
E 1
