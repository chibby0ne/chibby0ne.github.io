---
title: "Defining installation location in build systems"
date: 2021-05-23T20:16:34+02:00
description: Prefix and Destdir meaning
featured_image: "/img/build_systems.png"
tags: [autotools, cmake, meson, make, installation]
category: build systems
---

There's always a way to change the default installation location in every
build system, and fortunately it's almost always the same naming convention
used to change them.

The way to change the installation directory is usually through these
variables: `DESTDIR` and `PREFIX`.

The exact purpose of each, is not exactly clear from the name, and since you'd
only change set if you're doing a custom installation building from source
(which is not very common) it's been a common source of confusion.

But as everything, once we read the manual and run an example, it's clear what
the distinction is between them and which one to use at any one time.

## Autotools (GNU Build System)

To that end let's illustrate with an example in Autotools. Autotools (or
formally *GNU Build System*) being the oldest and (in my opinion) the most
cumbersome to use (slow, complicated, hard to debug):

Let's take a simpler version of the example shown in the [documentation of
autotools](https://www.gnu.org/software/automake/manual/html_node/Creating-amhello.html):

In summary, we need to create 4 files (`src/main.c`, `src/Makefile.am`,
`Makefile.am`, `configure.ac`) which you can create by executing the shell
commands posted here, with the help of heredocs.

1. Create the directory structure:
```bash
$ mkdir ~/amhello/src
$ cd ~/amhello
```

2. Source file
```bash
$ cat <<EOF > src/main.c
#include <config.h>
#include <stdio.h>

int
main (void)
{
  puts ("Hello World!");
  puts ("This is " PACKAGE_STRING ".");
  return 0;
}
EOF
```

3. Automake instruction to generate the binary:
```bash
$ cat <<EOF > src/Makefile.am
bin_PROGRAMS = hello
hello_SOURCES = main.c
EOF
```

3. Automake instruction for this directory:
```bash
$ cat <<EOF > Makefile.am
SUBDIRS = src
EOF
```

4. Autoconf instructions to create the `configure` script:
```bash 
cat <<EOF > configure.ac
AC_INIT([amhello], [1.0], [bug-automake@gnu.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
EOF
```

Now let's generate the `GNU Build System` files:

```bash
$ autoreconf --install
configure.ac:3: installing './compile'
configure.ac:2: installing './install-sh'
configure.ac:2: installing './missing'
src/Makefile.am: installing './depcomp'
```

Your directory should now look like this:

```bash
.
├── aclocal.m4
├── autom4te.cache
│   ├── output.0
│   ├── output.1
│   ├── output.2
│   ├── requests
│   ├── traces.0
│   ├── traces.1
│   └── traces.2
├── compile
├── config.h.in
├── configure
├── configure.ac
├── depcomp
├── install-sh
├── Makefile.am
├── Makefile.in
├── missing
└── src
    ├── main.c
    ├── Makefile.am
    └── Makefile.in
```

The usual flow for Autotools based systems is: `configure`, `make`, `make
install`. 

So let's start with `configure` which checks the host systems
characteristics and creates the Makefiles needed to build the program.

```bash
$ ./configure 
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for a race-free mkdir -p... /usr/bin/mkdir -p
checking for gawk... gawk
checking whether make sets $(MAKE)... yes
checking whether make supports nested variables... yes
checking for gcc... gcc
checking whether the C compiler works... yes
checking for C compiler default output file name... a.out
checking for suffix of executables...
checking whether we are cross compiling... no
checking for suffix of object files... o
checking whether the compiler supports GNU C... yes
checking whether gcc accepts -g... yes
checking for gcc option to enable C11 features... none needed
checking whether gcc understands -c and -o together... yes
checking whether make supports the include directive... yes (GNU style)
checking dependency style of gcc... gcc3
checking that generated files are newer than configure... done
configure: creating ./config.status
config.status: creating Makefile
config.status: creating src/Makefile
config.status: creating config.h
config.status: executing depfiles commands
```

You can see from the output that it checks for compilers and features of the
current host.

If there was any issue with the configuration of the build system with your
host system, such as missing dependencies, `configure` would have failed
preventing you to continue to build the program. In our example this shouldn't
happen but when building from source this is the place where you would usually
find the error messages informing of a missing/not found dependency.

After `configure` succeeds we can build the program by executing `make`, which
builds the binary: 

```bash
$ make
make  all-recursive
make[1]: Entering directory '/home/chimp/experiments_in_building_stuff/amhello'
Making all in src
make[2]: Entering directory '/home/chimp/experiments_in_building_stuff/amhello/src'
gcc -DHAVE_CONFIG_H -I. -I..     -g -O2 -MT main.o -MD -MP -MF .deps/main.Tpo -c -o main.o main.c
mv -f .deps/main.Tpo .deps/main.Po
gcc  -g -O2   -o hello main.o
make[2]: Leaving directory '/home/chimp/experiments_in_building_stuff/amhello/src'
make[2]: Entering directory '/home/chimp/experiments_in_building_stuff/amhello'
make[2]: Leaving directory '/home/chimp/experiments_in_building_stuff/amhello'
make[1]: Leaving directory '/home/chimp/experiments_in_building_stuff/amhello'
```

At this point the binary `hello` is in the `src` directory, and all that is
missing is installing this somewhere in the system. That's what the `make
install` does, so let's run it:

```bash
$ make install
Making install in src
make[1]: Entering directory '/home/chimp/experiments_in_building_stuff/amhello/src'
make[2]: Entering directory '/home/chimp/experiments_in_building_stuff/amhello/src'
 /usr/bin/mkdir -p '/usr/local/bin'
  /usr/bin/install -c hello '/usr/local/bin'
/usr/bin/install: cannot create regular file '/usr/local/bin/hello': Permission denied
make[2]: *** [Makefile:283: install-binPROGRAMS] Error 1
make[2]: Leaving directory '/home/chimp/experiments_in_building_stuff/amhello/src'
make[1]: *** [Makefile:454: install-am] Error 2
make[1]: Leaving directory '/home/chimp/experiments_in_building_stuff/amhello/src'
make: *** [Makefile:350: install-recursive] Error 1
```

As we can see, by default `make install` tries to install the binary in
`/usr/local/bin`, but fails because I invoked the command as a regular user
and not with superuser rights, and therefore it fails to write to
`/usr/local/bin` due to "permission denied". More generally, `make install` by
default install to `/usr/local` and then to one of: `doc/`, `lib/` and/or
`bin` depending of whether it is installing documentation (e.g: man pages), a
library, or  a binary respectively.

Here's where the `DESTDIR` could be used, so let's try to run make install
with a directory in somewhere else:

```bash
$ make install DESTDIR=/tmp/installdir
Making install in src
make[1]: Entering directory '/home/chimp/experiments_in_building_stuff/amhello/src'
make[2]: Entering directory '/home/chimp/experiments_in_building_stuff/amhello/src'
 /usr/bin/mkdir -p '/tmp/installdir/usr/local/bin'
  /usr/bin/install -c hello '/tmp/installdir/usr/local/bin'
make[2]: Nothing to be done for 'install-data-am'.
make[2]: Leaving directory '/home/chimp/experiments_in_building_stuff/amhello/src'
make[1]: Leaving directory '/home/chimp/experiments_in_building_stuff/amhello/src'
make[1]: Entering directory '/home/chimp/experiments_in_building_stuff/amhello'
make[2]: Entering directory '/home/chimp/experiments_in_building_stuff/amhello'
make[2]: Nothing to be done for 'install-exec-am'.
make[2]: Nothing to be done for 'install-data-am'.
make[2]: Leaving directory '/home/chimp/experiments_in_building_stuff/amhello'
make[1]: Leaving directory '/home/chimp/experiments_in_building_stuff/amhello'
```

Ah! So it installed the binary to the `DESTDIR` given but not exactly there.
When we list the files there we find:

```bash
/tmp/installdir
└── usr
    └── local
        └── bin
            └── hello
```

So `DESTDIR` changed the destination directory but it still created the
`/usr/local/bin` in which it tried to install by default!

Basically ***DESTDIR gave us a way to check what and how something would be
installed without actually installing it***! In fact if we look at [the
documentation](https://www.gnu.org/software/automake/manual/html_node/DESTDIR.html)
the documentation that's pretty much what it says:

> The DESTDIR variable can be used to perform a staged installation. The
> package should be configured as if it was going to be installed in its final
> location (e.g., --prefix /usr), but when running make install, the DESTDIR
> should be set to the absolute name of a directory into which the
> installation will be diverted. From this directory it is easy to review
> which files are being installed where, and finally copy them to their final
> location by some means. 

This snippet also hints as to what prefix does! *Prefix describes the
beginning (or prefix) of the absolute path used as installation directory!
Another difference is **that prefix is set when running the `configure` script**.

To illustrate that let's set prefix to `boo` by rerunning `./configure`,
`make` and `make install`. We don't need to clean the directory before we do
so:

```bash
$ ./configure --prefix=boo
configure: error: expected an absolute directory name for --prefix: boo
```

> You can see there's a bit of inconsistency in between setting these
variables. That's because configure is a script and the script handles options
the regular Unix way with double dash. Whilst Makefiles use variables which
are usually uppercase and don't use the double dash.

As mentioned before, **prefix is an absolute path** therefore the first
character **must be a forward slash (/)**.

For the sake of brevity I will omit the output of the commands except when
needed:

```bash
$ ./configure --prefix=boo
$ make 
$ make install
Making install in src
make[1]: Entering directory '/home/chimp/experiments_in_building_stuff/amhello/src'
make[2]: Entering directory '/home/chimp/experiments_in_building_stuff/amhello/src'
 /usr/bin/mkdir -p '/boo/bin'
/usr/bin/mkdir: cannot create directory ‘/boo’: Permission denied
make[2]: *** [Makefile:283: install-binPROGRAMS] Error 1
make[2]: Leaving directory '/home/chimp/experiments_in_building_stuff/amhello/src'
make[1]: *** [Makefile:454: install-am] Error 2
make[1]: Leaving directory '/home/chimp/experiments_in_building_stuff/amhello/src'
make: *** [Makefile:350: install-recursive] Error 1
```

As can be seen, make tried to install the binary in `/boo/bin` so the prefix
is indeed the beginning of the install path.


## CMake

`DESTDIR` is a CMake environment variable, therefore its initial value comes
from the environment of the calling process. Documentation can be seen
[here](https://cmake.org/cmake/help/latest/envvar/DESTDIR.html)

It can be set by either exporting it or setting it in the same line cmake is
invoked:


// Some example here of using destdir and its output
```bash
```

On the other hand the prefix can be set by setting the variable
`CMAKE_INSTALL_PREFIX`.

// Some example here of using `CMAKE_INSTALL_PREFIX` and its output
```bash
```

NOTE: `CMAKE_INSTALL_PREFIX` is also added to `CMAKE_SYSTEM_PREFIX_PATH`
which is used in calls to `find_package`, `find_program`, `find_library` and
others. Check the
[documentation](https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html)
for more information.


## Meson

Meson uses `DESTDIR` and `MESON_INSTALL_PREFIX` as environment variables which
can be set in order to customize the installation directory.

An additional caveat, is that to be able to install something using meson, the
target first needs to be defined as installable, like so:

// Some example here of using `DESTDIR` and its output
```bash
```

```bash
executable('prog', 'prog.c', install : true)
```

// Some example here of using `MESON_INSTALL_PREFIX` and its output
```bash
```

In addition to that, meson allows a further fine grained customization of
the installation directory for a given target by setting the `install_dir`,
which would be equivalent to `DESTDIR`.

```bash
executable('prog', 'prog.c', install : true, install_dir : 'my/special/dir')
```

[Installing](https://mesonbuild.com/Installing.html#installing)
[DESTDIR](https://mesonbuild.com/Installing.html#destdir-support)


## Summary

For changing the installation directory directories in common for a given
software package's files (i.e: documentation, libraries, sources, binaries,
etc..), use `PREFIX`.

For changing where the installation directory is use `DESTDIR`.

Hope this helps!

