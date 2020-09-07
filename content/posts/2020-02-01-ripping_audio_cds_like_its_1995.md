---
layout: post
date: 2020-02-01
title: Mounting/Playing/Ripping audio CDs like it's 1995 (in Arch Linux)
---

First post of the year! Yey!

Like many other posts, the motivation of this one is to facilitate other
people running into the same issues as me, to quickly find
their solution.

So here's the problem statement:

You want to play an audio CD in an Arch Laptop or simply get the audio files. What do you do?

Well first you buy an external CD/DVD ROM player, which you can connect (and
power) through USB.

Then you should mount it, to later get the files or simply rip them, right?...

If you assume that mounting a CD ROM is exactly the same as mounting a
USB, then you would just connect the CD ROM player to the laptop, insert the
CD and
mount it with a command like:

```bash
sudo mount /dev/sr0 /mnt/cdrom
```

But you would get an error:

```bash
mount: /mnt/cdrom: can't read superblock on /dev/sr0.
```

So you search online the error and realize that it's due to the fact that CD
doesn't use the same filesystem as the laptop, which by default for most
distros is ext4. CDs/DVDs use a different filesystem. That filesystem is
ISO9660, which is the regular filesystem for optical media.

So now you search online: for something like *"mount+CD+ROM+linux"*, which tells you the right command is:

```bash
mount -t iso9660 -o ro /dev/sr0 /mnt/cdrom
```

But when you try it, you get this error:

```bash
mount: /mnt/cdrom: wrong fs type, bad option, bad superblock on /dev/sr0, missing codepage or helper program, or other
error.
```

Then you wonder if it's a hardware issue and you look for the message buffer
of the kernel, using `dmesg` and find some errors like:

```bash
[ 4325.239696] sr 1:0:0:0: [sr0] tag#0 FAILED Result: hostbyte=DID_OK driverbyte=DRIVER_SENSE
[ 4325.239708] sr 1:0:0:0: [sr0] tag#0 Sense Key : Illegal Request [current]
[ 4325.239718] sr 1:0:0:0: [sr0] tag#0 Add. Sense: Illegal mode for this track
[ 4325.239729] sr 1:0:0:0: [sr0] tag#0 CDB: Read(10) 28 00 00 04 0c 60 00 00 02 00 00 00
[ 4325.239741] blk_update_request: I/O error, dev sr0, sector 1061248 op 0x0:(READ) flags 0x80700 phys_seg 1 prio class
 0
[ 4325.241965] attempt to access beyond end of device
[ 4325.241974] sr0: rw=0, want=1061256, limit=1061248
[ 4325.241982] Buffer I/O error on dev sr0, logical block 132656, async page read
[ 4325.242014] blk_update_request: I/O error, dev loop0, sector 1061248 op 0x0:(READ) flags 0x80700 phys_seg 1 prio cla
ss 0
[ 4325.243811] attempt to access beyond end of device
[ 4325.243820] sr0: rw=0, want=1061256, limit=1061248
[ 4325.243828] Buffer I/O error on dev sr0, logical block 132656, async page read
[ 4325.243855] blk_update_request: I/O error, dev loop0, sector 1061248 op 0x0:(READ) flags 0x0 phys_seg 1 prio class 0
[ 4325.243862] Buffer I/O error on dev loop0, logical block 530624, async page read
[ 4325.243880] attempt to access beyond end of device
[ 4325.243885] sr0: rw=0, want=1061256, limit=1061248
[ 4325.243890] Buffer I/O error on dev sr0, logical block 132656, async page read
[ 4325.243904] blk_update_request: I/O error, dev loop0, sector 1061250 op 0x0:(READ) flags 0x0 phys_seg 1 prio class 0
[ 4325.243909] Buffer I/O error on dev loop0, logical block 530625, async page read
[ 4325.243919] attempt to access beyond end of device
[ 4325.243924] sr0: rw=0, want=1061256, limit=1061248
```

Which suggest we are still trying to mount it wrongly.

At which point you then start searching for: *"mount+Audio+CD+linux"*, which
leads you to this [answer in unix
stackexchange](https://unix.stackexchange.com/a/204032), which explains that
you **can't** mount Audio CDs, you simply *play them directly*.

You can try the command in the answer to verify that it in fact works:

```
mplayer -cdrom-device /dev/cdrom -cache 5000 cdda://1
```

This should get the ROM spinning and you would start hearing one of the audio tracks.

Ok so **Audio CDs can't be mounted**, so you skip to the second part, which is ripping the tracks.

As usual, the ArchWiki has the answer. 99% of the times you have an issue in an
Arch installation it is already addressed in one article or another, but it
never ceases to surprise me how wide their breadth of topics is!


The article: [Rip Audio
CDs](https://wiki.archlinux.org/index.php/Rip_Audio_CDs#Ripping), has the
command that would get you all tracks, which requires installing `cdrtools`:

```bash
cdda2wav -vall cddb=0 speed=4 -paranoia paraopts=proof -B -D /dev/sr0
```

That will get your CD ROM spinning and after some minutes all the tracks in
your current directory as wav files.

There's more information in the article in case you would like to encode them
into other formats, like mp3 or flac.

Happy listening!
