---
layout: post
date: 2018-03-06
title: libtinfo in Arch Linux
---

***pacaur*** is "a AUR Helper which minimizes user interaction"
meaning it helps us install AUR packages without too much hassle.

With it I install and update packages like spotify and google-chrome which are
not in the official repositories i.e: core, extra, community or multilib
repositories.

What does this have to do with libtinfo?

Well libtinfo (more specifically libtinfo5) was one of those packages managed
by pacaur. Problem is every time I did an update it warned with:

```
libtinfo5 not present in AUR with pacaur --skipping
```

Meaning that it wasn't being maintained anymore since it is not in the AUR.

So I decide to remove it. I couldn't remember what I was using it for, but I
was sure it wasn't very important, and in case something failed I'd just
google it. I also removed several other packages because I disliked seeing
that message.

Uninstalled. Rebooted. Opened a Terminal.... Couldn't open a Terminal. Looks
like one of the packages was actually important.

I switch to console mode, since I need a shell if I want to reinstall any of
the packages I uninstalled again.

I try logging in with my user... A quick refresh of the terminal and nothing happens.

Mmm... let's try loggin in as root. It worked!

Now just for science (queue meme), I tried try to su as my user, I get the
clue:

```
zsh: error while loading shared libraries: libtinfo.so.6: cannot open shared object file: No such file or directory
```

Turns out `libtinfo` is a library that *normally* (read: in other distros)
gets installed when installing `libncurses`, just ***not*** in Arch.

`libtinfo` is in turn is required by zsh (I discovered from the message),
which is the shell I'm using, and since this failed it prevented me from
opening a terminal emulator.

A quick google revealed the best working solution:

```
ln -s libncursesw.so.6 libtinfo.so.6
```

This solution doesn't require you to install the old unmaintained package again,
and can be done for any version of `libncurses`.

In the future, the package maintainers of `libncurses` should probably add
this symbolic link as part of the package installation.

Credits to [this Arch Linux Forum
thread](https://bbs.archlinux.org/viewtopic.php?id=142384) for the idea.

I later also noticed that someone commented on the [AUR package
website](https://aur.archlinux.org/packages/libtinfo5/) itself with the same
approach.
