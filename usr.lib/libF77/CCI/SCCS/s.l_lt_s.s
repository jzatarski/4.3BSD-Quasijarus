h57931
s 00042/00000/00000
d D 5.1 86/11/03 20:00:46 mckusick 1 0
c from CCI
e
u
U
t
T
I 1
#	"%W% (Berkeley) %G%"

.data
.text
LL0:.align	1
.globl	_l_lt
.set	MASK__,0x4
.data
.text
_l_lt:.word	MASK__
	movl	4(fp),r0		/* a */
	movl	8(fp),r1		/* b */
	cmpl	12(fp),16(fp)		/* if (la <= lb) */
	jgtr	LB
	movl	12(fp), r2		/* compare according to la */
	cmps3
	jlss	out1			/* if less return(1) */
	jgtr	out0			/* if greater return(0) */

	cmpl	12(fp),16(fp)		/* if (la == lb) */
	jeql	out0			/* then equal */

	addl3	8(fp), 16(fp), r2	/* bend */
LOOP1:
	cmpb	(r1), $32		/* if *b != space */
	jneq	out1			/* then astring < bstring */
	incl	r1			/* else continue */
	cmpl	r1, r2			/* till bend */
	jlssu	LOOP1
	jbr	out0

LB:					/* else */
	movl	16(fp), r2		/* compare according to lb */
	cmps3
	jlss	out1			/* if less return(0) */

out0:
	clrl	r0
	ret
out1:
	movl	$1, r0
	ret
E 1
