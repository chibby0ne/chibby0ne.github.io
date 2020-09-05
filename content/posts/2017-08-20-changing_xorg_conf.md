---
layout: post
date: 2017-08-20
title: Removing of an Nvidia GPU from arch Linux with i3
---

This post is to help me and other people that might have had this issue before
(and mostly future me).

When you have installed an Nvidia GPU in Arch Linux with i3 as Window Manager
(WM), you probably needed to install and `nvidia`  or another similar package,
which contains drivers and generally includes a tool that automatically
creates/modifies the Xorg configuration file (Xorg.conf), for the correct
operation of the GPU in Arch and i3.

Now the problem is that if you for some reason need to remove the GPU, then
the Xorg.conf will still expect to use the GPU as device for rendering the
Display.

First a bit of background,

When you start the X Windows System, an X session will be created. The
standard ways of starting an X session are to run `startx` from console login
or to login using a Display Manager (DM), such as lightdm (the DM I was
using).

The solution is simple:
Just remove any reference to nvidia on the Section corresponding to the Device.

One one you can test the Xorg.conf is in a sane state is by using

Your /etc/X11/Xorg.conf will look something like this: 

```
Section "Monitor"
    Identifier      "Monitor0"
    VendorName      "Unknown"
    ModelName       "Unknown"
    HorizSync       28.0 - 33.0
    VertRefresh     43.0 - 72.0
    Option          "DPMS"
EndSection

Section "Device"
    Identifier      "Device0"
    Driver          "nvidia"
    VendorName      "NVIDIA Corporation"
EndSection

Section "Screen"
    Identifer       "Screen0"
    Device          "Device0"
    Monitor         "Monitor0"
    DefaultDepth    24
    SubSection      "Display"
        Depth       24
    EndSubSection
EndSection

```

You should modify it so that it looks like this:

```
Section "Monitor"
    Identifier      "Monitor0"
    VendorName      "Unknown"
    ModelName       "Unknown"
    HorizSync       28.0 - 33.0
    VertRefresh     43.0 - 72.0
    Option          "DPMS"
EndSection

Section "Screen"
    Identifer       "Screen0"
    Device          "Device0"
    Monitor         "Monitor0"
    DefaultDepth    24
    SubSection      "Display"
        Depth       24
    EndSubSection
EndSection
```
