h53075
s 00001/00002/00017
d D 1.2 81/06/10 00:37:49 mckusic 2 1
c move the text of error messages from ERROR to the functions
c which generate them.
e
s 00019/00000/00000
d D 1.1 80/10/30 00:33:14 mckusick 1 0
c date and time created 80/10/30 00:33:14 by mckusick
e
u
U
t
T
I 1
/* Copyright (c) 1979 Regents of the University of California */

static char sccsid[] = "%Z%%M% %I% %G%";

#include "h00vars.h"
D 2
#include "h01errs.h"
E 2

FLUSH(curfile)

	register struct iorec	*curfile;
{
	if (curfile->fblk >= MAXFILES || _actfile[curfile->fblk] != curfile) {
D 2
		ERROR(ENOFILE, 0);
E 2
I 2
		ERROR("Reference to an inactive file\n", 0);
E 2
		return;
	}
	if (curfile->funit & FWRITE) {
		fflush(curfile->fbuf);
	}
}
E 1
