---
layout: post
title: Automount SMB directory with systemd in Arch
---

# Problem statement

I wanted to mount a directory from a local desktop in my laptop regardless of
the OS it might be running at the moment. Also, not just mount it. *Automount*
it either whenever I boot or on demand.

At the moment in my particular case, both laptop and desktop are running Arch
Linux.

First let's do a bit of a primer:

## SMB Primer

*SMB* is a network protocol (that also acts as a network/distributed file
system) used for sharing file and print services between Windows clients. The
original version was also known as *CIFS*, and since then both names are used
interchangeably.

Samba is a FOSS implementation of the SMB protocol that runs on \*nix systems.

The other competitor to SMB is NFS, but the former is more Windows oriented as
it was originally created for the Microsoft ecosystem, while the latter was
created towards a more \*nix friendly environment.

Using SMB is the traditionally way of sharing directories between Linux/Windows.

TL;DR: SMB for Windows/Linux. NFS for Linux/Linux.

# What to install

You need the install `cifs-utils` which provides user-space tools for mounting
SMB/CIFS file systems (or shares as they are also called). 

Basically, `cifs-utils` add the possibility of selecting cifs as filesystem to
`mount`.

## Credentials

We create a file containing the credentials used for that.
Unfortunately there's no other way that I'm aware of, of storing the
credentials other than *plaintext*.

The file should contain two entries as such:

```
username=USERNAME
password=PASSWORD
```

You should store it somewhere in /etc/samba/ and set its permissions to 600 so
that only root has read/write access.

## Mount file

The mount file contains the information on how to mount the filesystem.

One important note is that **the filename should encode the mount point,
otherwise systemd will fail at mounting it**.

The filename should be of the pattern:
`directory1-directory2-directory3.mount`

Where the actual path where it is mounted is:

`/directory1/directory2/directory3`

That is, the internal forward slashes of the path should be the minuses ("-")
in the filename.

Otherwise you will get an error like this

```
$ systemctl start mntdir.mount
$ systemctl status mntdir.mount
...
...
mntdir.mount's Where= setting doesn't match unit name. Refusing
```

This file should contain something like this:

```
[Unit]
Description=DESCRIPTION
Requires=systemd-networkd.service
After=network-online.target
Wants=network-online.target

[Mount]
What=PATH_IN_SMB_SERVER
Where=PATH_TO_MOUNT_POINT
Options=vers=2.1,credentials=CREDENTIALS_PATH,iocharset=utf8,rw,x-systemd.automount
Type=cifs
TimeoutSec=30

[Install]
WantedBy=multi-user.target
```

Replacing the upper case values with the relevant parameters in your case.


## Automount file

The automount file contains the information as to where and how to automount.
This automount works on-demand, i.e: it mounts the filesystem when you
navigate to that directory.

Just like the mount file: the name of the file should encode the mount
location. i.e: it should have the same name as the `.mount` file but with the
file extension: `.automount`

Its contents should be something like this:

```
[Unit]
Description=DESCRIPTION
Requires=network-online.target

[Automount]
Where=PATH_TO_MOUNT_POINT
TimeoutIdleSec=0

[Install]
WantedBy=multi-user.target
```

Replacing the upper case values with the relevant parameters in your case, but
keeping in mind that the `PATH_TO_MOUNT_POINT` should be the same as the one
in the `.mount` file.


This file should be enabled so that it runs at boot.


```
systemctl enable mnt-dir.automount
```

Asumming the mount point is /mnt/dir (and therefore the filename is
mnt-dir.automout).


## Tips

In my particulaar case no matter what `Requires`, `After` and/or `Wants`
service you select, enabling the `.mount` file so that it runs automatically
at boot, the system will fail to mount it.

Even more, I found that most guides and tutorials used the automount as the
actual enabled unit, which mounts on demand and not on boot.
