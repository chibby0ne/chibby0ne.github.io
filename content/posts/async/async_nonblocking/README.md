
Interesting tidbit:

aiocb makes use of signals through the use of a `struct sigevent *` named
`aio_sigevent`. This struct is used to configure the notification method to
the caller when an asynchronous I/O operation has been completed.

This struct makes use of two important members: a member named `sigev_notify`
that if it's set to `SIGEV_SIGNAL` it will notify the process sending the
signal in the other member: `sigev_signo`.

This struct's `sigev_notify` in addition can be set up with `SIGEV_NONE` in
which case no notification is made when the I/O is completed (which is not
very useful), and with `SIGEV_THREAD` in which case it makes use of the
function defined in the `sigev_notify_function` as if it were the start
function on a new thread.



On a different tangent something I learned is that in the signal's world, the
action taken when a signal is received is called the **signal disposition**,
and by default there are 4 actions: 
* Terminate process
* Ignore signal
* Terminate process and dump core
* Stop process
* Continue process if it's currently stopped


The man page for `signal(7)` is very well written and is an easy read that
doesn't need too much context.
