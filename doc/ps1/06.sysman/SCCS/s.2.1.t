h02876
s 00008/00007/00103
d D 6.2 86/05/12 01:01:18 karels 3 2
c update, correct, rephrase and re-propose
e
s 00000/00001/00110
d D 6.1 86/05/08 12:26:54 mckusick 2 1
c format updates for 4.3BSD
e
s 00111/00000/00000
d D 5.1 86/05/08 00:03:24 mckusick 1 0
c document distributed with 4.2BSD
e
u
U
t
T
I 1
.\" Copyright (c) 1983 Regents of the University of California.
.\" All rights reserved.  The Berkeley software License Agreement
.\" specifies the terms and conditions for redistribution.
.\"
.\"	%W% (Berkeley) %G%
.\"
D 2
.\" %M% %I% %E%
E 2
.sh "Generic operations
.PP
.PP
Many system abstractions support the
operations \fIread\fP, \fIwrite\fP and \fIioctl\fP.  We describe
the basics of these common primitives here.
Similarly, the mechanisms whereby normally synchronous operations
may occur in a non-blocking or asynchronous fashion are
common to all system-defined abstractions and are described here.
.NH 3
Read and write
.PP
The \fIread\fP and \fIwrite\fP system calls can be applied
to communications channels, files, terminals and devices.
They have the form:
.DS
cc = read(fd, buf, nbytes);
result int cc; int fd; result caddr_t buf; int nbytes;

cc = write(fd, buf, nbytes);
result int cc; int fd; caddr_t buf; int nbytes;
.DE
The \fIread\fP call transfers as much data as possible from the
object defined by \fIfd\fP to the buffer at address \fIbuf\fP of
size \fInbytes\fP.  The number of bytes transferred is
returned in \fIcc\fP, which is \-1 if a return occurred before
any data was transferred because of an error or use of non-blocking
operations.
.PP
The \fIwrite\fP call transfers data from the buffer to the
object defined by \fIfd\fP.  Depending on the type of \fIfd\fP,
it is possible that the \fIwrite\fP call will accept some portion
of the provided bytes; the user should resubmit the other bytes
in a later request in this case.
Error returns because of interrupted or otherwise incomplete operations
are possible.
.PP
Scattering of data on input or gathering of data for output
is also possible using an array of input/output vector descriptors.
D 3
The type for the descriptors is defined in <sys/uio.h> as:
E 3
I 3
The type for the descriptors is defined in \fI<sys/uio.h>\fP as:
E 3
.DS
._f
struct iovec {
	caddr_t	iov_msg;	/* base of a component */
	int	iov_len;	/* length of a component */
};
.DE
The calls using an array of descriptors are:
.DS
cc = readv(fd, iov, iovlen);
result int cc; int fd; struct iovec *iov; int iovlen;

cc = writev(fd, iov, iovlen);
result int cc; int fd; struct iovec *iov; int iovlen;
.DE
Here \fIiovlen\fP is the count of elements in the \fIiov\fP array.
.NH 3
Input/output control
.PP
Control operations on an object are performed by the \fIioctl\fP
operation:
.DS
ioctl(fd, request, buffer);
int fd, request; caddr_t buffer;
.DE
This operation causes the specified \fIrequest\fP to be performed
on the object \fIfd\fP.  The \fIrequest\fP parameter specifies
whether the argument buffer is to be read, written, read and written,
or is not needed, and also the size of the buffer, as well as the
request.
Different descriptor types and subtypes within descriptor types
may use distinct \fIioctl\fP requests.  For example,
operations on terminals control flushing of input and output
queues and setting of terminal parameters; operations on
disks cause formatting operations to occur; operations on tapes
control tape positioning.
.PP
D 3
The names for basic control operations are defined in <sys/ioctl.h>.
E 3
I 3
The names for basic control operations are defined in \fI<sys/ioctl.h>\fP.
E 3
.NH 3
Non-blocking and asynchronous operations
.PP
A process that wishes to do non-blocking operations on one of
its descriptors sets the descriptor in non-blocking mode as
described in section 1.5.4.  Thereafter the \fIread\fP call will
return a specific EWOULDBLOCK error indication if there is no data to be
\fIread\fP.  The process may
D 3
\fIdselect\fP the associated descriptor to determine when a read is
E 3
I 3
\fIselect\fP the associated descriptor to determine when a read is
E 3
possible.
.PP
Output attempted when a descriptor can accept less than is requested
will either accept some of the provided data, returning a shorter than normal
length, or return an error indicating that the operation would block.
More output can be performed as soon as a \fIselect\fP call indicates
the object is writeable.
.PP
Operations other than data input or output
may be performed on a descriptor in a non-blocking fashion.
These operations will return with a characteristic error indicating
that they are in progress
D 3
if they cannot return immediately.  The descriptor
E 3
I 3
if they cannot complete immediately.  The descriptor
E 3
may then be \fIselect\fPed for \fIwrite\fP to find out
D 3
when the operation can be retried.  When \fIselect\fP indicates
the descriptor is writeable, a respecification of the original
operation will return the result of the operation.
E 3
I 3
when the operation has been completed.  When \fIselect\fP indicates
the descriptor is writeable, the operation has completed.
Depending on the nature of the descriptor and the operation,
additional activity may be started or the new state may be tested.
E 3
E 1
