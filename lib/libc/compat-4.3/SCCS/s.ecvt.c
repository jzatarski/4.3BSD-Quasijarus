h08050
s 00004/00001/00104
d D 5.1 86/03/15 16:40:35 mckusick 2 1
c added LIBC_SCCS condition for sccs ids
e
s 00105/00000/00000
d D 4.1 80/12/21 16:39:36 wnj 1 0
c date and time created 80/12/21 16:39:36 by wnj
e
u
U
t
T
I 1
D 2
/* %W% (Berkeley) %G% */
E 2
I 2
#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "%W% (Berkeley) %G%";
#endif LIBC_SCCS and not lint

E 2
/*
 *	ecvt converts to decimal
 *	the number of digits is specified by ndigit
 *	decpt is set to the position of the decimal point
 *	sign is set to 0 for positive, 1 for negative
 */

char	*cvt();

#define	NDIG	80
char*
ecvt(arg, ndigits, decpt, sign)
double arg;
int ndigits, *decpt, *sign;
{
	return(cvt(arg, ndigits, decpt, sign, 1));
}

char*
fcvt(arg, ndigits, decpt, sign)
double arg;
int ndigits, *decpt, *sign;
{
	return(cvt(arg, ndigits, decpt, sign, 0));
}

static char*
cvt(arg, ndigits, decpt, sign, eflag)
double arg;
int ndigits, *decpt, *sign;
{
	register int r2;
	double fi, fj;
	register char *p, *p1;
	static char buf[NDIG];
	double modf();

	if (ndigits<0)
		ndigits = 0;
	if (ndigits>=NDIG-1)
		ndigits = NDIG-2;
	r2 = 0;
	*sign = 0;
	p = &buf[0];
	if (arg<0) {
		*sign = 1;
		arg = -arg;
	}
	arg = modf(arg, &fi);
	p1 = &buf[NDIG];
	/*
	 * Do integer part
	 */
	if (fi != 0) {
		p1 = &buf[NDIG];
		while (fi != 0) {
			fj = modf(fi/10, &fi);
			*--p1 = (int)((fj+.03)*10) + '0';
			r2++;
		}
		while (p1 < &buf[NDIG])
			*p++ = *p1++;
	} else if (arg > 0) {
		while ((fj = arg*10) < 1) {
			arg = fj;
			r2--;
		}
	}
	p1 = &buf[ndigits];
	if (eflag==0)
		p1 += r2;
	*decpt = r2;
	if (p1 < &buf[0]) {
		buf[0] = '\0';
		return(buf);
	}
	while (p<=p1 && p<&buf[NDIG]) {
		arg *= 10;
		arg = modf(arg, &fj);
		*p++ = (int)fj + '0';
	}
	if (p1 >= &buf[NDIG]) {
		buf[NDIG-1] = '\0';
		return(buf);
	}
	p = p1;
	*p1 += 5;
	while (*p1 > '9') {
		*p1 = '0';
		if (p1>buf)
			++*--p1;
		else {
			*p1 = '1';
			(*decpt)++;
			if (eflag==0) {
				if (p>buf)
					*p = '0';
				p++;
			}
		}
	}
	*p = '\0';
	return(buf);
}
E 1
