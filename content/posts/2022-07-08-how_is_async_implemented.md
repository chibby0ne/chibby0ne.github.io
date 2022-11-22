---
title: "How is async implemented"
date: 2022-07-08T22:26:47+01:00
draft: true
---

Bibliography:
References:



https://stackoverflow.com/questions/59458794/how-to-implement-async-i-o-efficiently-in-the-kernel
https://medium.com/nerd-for-tech/what-is-asynchronous-i-o-b37994359471

https://oxnz.github.io/2016/10/13/linux-aio/
https://developer.ibm.com/articles/l-async/

<aio.h>
man aio_read
man aio

Apparently the Linux implementation of the POSIX AIO is written in userspace
and part of the librt component of the larger glibc. Which means if you look
into the implementation is uses pthreads, and enqueing stuff.

The problems:

https://lwn.net/Articles/671649/

There's a ongoing rewrite in kernelspace for a state-machine based
implementation of the POSIX AIO using io_submit, io_setup, io_cancel,
io_destory, io_getevents.




Which can be foudn as part of the <libaio.h> for those of us who are kernel
hackers.

man io_submit


https://blog.cloudflare.com/io_submit-the-epoll-alternative-youve-never-heard-about/



https://elixir.bootlin.com/linux/latest/C/ident/aio_prep_rw

`___io_submit_one` https://elixir.bootlin.com/linux/latest/source/fs/aio.c#L1938

(kernel) aio_read https://elixir.bootlin.com/linux/latest/source/fs/aio.c#L1541

There's of course now io_uring




Mention that rust has the async/await syntax that in the end resolve into
functions that return a Futures.

https://os.phil-opp.com/async-await/#state-machine-transformation



https://en.wikipedia.org/wiki/Async/await
