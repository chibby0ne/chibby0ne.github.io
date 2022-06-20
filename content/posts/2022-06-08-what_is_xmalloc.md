---
title: What is xmalloc?
date: 2022-06-20T00:28:23+02:00
description: And when should I use it?
featured_image: "/img/xmalloc.png"
tags: [c, malloc, gcc, libc]
category: memory allocation
---

## What is it? What does it do?

xmalloc is a non-standard function that wraps `malloc` and ***terminates the
program if malloc fails to the allocate*** the requested memory.

I was running some of the examples in [bork's linux tracing
post](https://jvns.ca/blog/2017/07/05/linux-tracing-systems/), and noticed
that when running the uprobe example:

```
sudo ./bin/uprobe 'r:bash:readline +0($retval):string'
```

it returned an error:

```
ERROR: missing symbol "malloc" in /usr/bin/bash
```

which was odd but perhaps that version of bash used by bork was indeed using
`malloc` and current versions of bash used a different **flavor** of it, like
`calloc`, `realloc`, etc...

So I queried the binary for the string `malloc` and grepped the output:

```
strings /bin/bash | grep malloc
```

And got:

```
xmalloc
malloc_trace_at_exit
xmalloc
```

## Down the rabbithole

Which led me to this article on the [xmalloc
convention](https://www.gnu.org/software/libc/manual/html_node/Malloc-Examples.html).

In addition there's a quite interesting discussion on the usage of `xmalloc` vs
`malloc` in [SO](https://stackoverflow.com/a/7590738)

## When to use it?

The gist of it is that if you're writing library code, you shouldn't use
`xmalloc` since applications depending on that library might lose work or
information since they cannot gracefully shutdown.

Though there are cases where it's not necessary (and "pollutes the codebase")
to check for the return code of malloc for example writing a user-end
application, and considering the kernel's memory overcommitment behavior being
as it, xmalloc could be an alright choice.

TIL!
