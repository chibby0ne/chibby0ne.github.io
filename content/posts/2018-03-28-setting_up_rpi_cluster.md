---
layout: post
title: 'Setting up a Raspberry Pi Cluster (Part 1)'
---

I recently decided to set up a raspberry pi cluster, with whom I could
experiment, learn and use cluster technologies like Kubernetes, Apache Hadoop,
Spark, and even small programs using RPC and OpenMP.

In setting up the raspberry pi (a process which I think I'm fairly acquainted
with) I ran through a few things which caught me by surprise, or were
completely new to me, so after being half way into the process I wanted to
write it down so that people that might want to undertake it, can it setup up
much more smoothly.

## Hardware

I chose to be as minimal and cheap as possible.

### Devices and MicroSD
The cluster consists of 3 Raspberry Pi's: 2 of which are RPI 3 B+, and other
one is a RPI 3 B, with 2 32 GB and 1 64 GB microSDs respectively.

### Network
They will be connected to the network using the integrated wifi module. No
external dongles, or ethernet cables are required. Saving money!

### Case

I bought a dog bone cluster case in Amazon, that fits 4 Pis in case future me
decides to expand the cluster.

### Power supply

30W 3-Port USB Wall Charger. It can output up to 2 A per port.

## Flashing the cards

I flashed the minimal Raspbian Stretch image, since I don't need any GUI or
X session running for applications I have mind (so far at least).

This process was simple just repeat it 3 times with different micro sd cards.

I knew that once flashed, I could mount the sd in my desktop, and edit the
`boot/` and the `rootfs/` partition to configure them whichever way I want, but
there were still a few things I didn't have completely clear.

## Configuring each device

At first I decided to configure one of them manually so that I can take note
on what group of file configuration constitute the minimum to be able to ssh
to them remotely.

Immediately I realized I had flashed the smallest image without GUI, therefore
I couldn't attach a Monitor. Ethernet for ssh'ing wasn't an option since I
didn't have any spare cables, that could reach the router.

Fortunately, I have a USB-to-Serial 3.3V cable that can be used to open a
Linux console. In fact, one could argue it is even more useful than having an
Ethernet connection, since it captures even the boot sequence, something you
can't do with ssh.

So I attached usb cable from my desktop to the pi and powered it on, and I
waited for a bit. Then a longer bit. There was nothing. No output.

An auspicious start :p

I quickly remembered something about the RPI 3 changing the way serial
connection is used. So I consult [elinux page for RPi Serial Connection][1]
and effectively, ever since RPI 3, UART is disabled by default.

I went back, took the microsd out of the pi, and edited the `/boot/config.txt`
as mentioned in the wiki, adding a line with `enable_uart=1`, popped the
microSD back and now I could see the booting sequence.

We're in business!

So I start editing:

1. `/etc/hostname` and `/etc/hosts` with the hostnames I wanted to use. In my
   case, I'm using `rpiX` where `X` is an increasing number for each device
   in the cluster, starting from 0.

   ***Note:*** *Initially I only edited `/etc/hostname/`, in addition to the
   other files,  and rebooted, but during the boot sequence I noticed the pi
   still used the default `raspberry` hostname. After reading the [Debian
   Wiki][2], I learned that changing /etc/hosts is also required so that local
   addresses can resolve with the new hostname*

2. `/etc/wpa_supplicant/wpa_supplicant.conf`, adding the network entry
   with the wifi's SSID and Credentials. This is one of those files that I
   could copy to the rest in the cluster.

3. `/etc/dhcpcd.conf`, setting the static ip address, router, and gateway for
   the `wlan0` interface (the name used for the interfaces by the raspbian
   images). In my case, I'm using from `192.168.178.X` where `X` is an
   increasing number for each device in the cluster starting with 5.

Those are the three files I could think of.

Then I ran the `raspi-config` tool to enable wifi, shrink the GPU, set the
locale, and keyboard layout.

I rebooted. Everything seem OK. Verified that wifi was working.

Now all I had to do was copy all the files I edited to the other two microSD
cards, with the consideration of increasing the hostname number (as explained
in #1, and ip address (as explained in #3).

After doing so, and inserting the remaining microSDs, I was expecting to see
the other two pi's in nmap as I could see the first one, but I couldn't.

At this point I remembered that even with the configuration of the interface
(dhcpcd more precisely) and wpa_supplicant, I still had to run `raspi-config`
to enable the wifi and bluetooth. I thought this might be a rfkill switch on
disabling the radio signals.

Using the usb serial cable I checked the status of dhcpcd.service:

***Note*** *The artifacts shown in the output are part of the minicom output*

```
pi@rpi1:/dev$ sudo systemctl status dhcpcd.service
â● dhcpcd.service - dhcpcd on all interfaces
   Loaded: loaded (/lib/systemd/system/dhcpcd.service; enabled; vendor preset: e
  Drop-In: /etc/systemd/system/dhcpcd.service.d
           â└â─wait.conf
   Active: active (running) since Tue 2018-03-13 22:53:56 UTC; 2h 1min ago
  Process: 331 ExecStart=/usr/lib/dhcpcd5/dhcpcd -q -w (code=exited, status=0/SU
 Main PID: 457 (dhcpcd)
   CGroup: /system.slice/dhcpcd.service
           â├â417 wpa_supplicant -B -c/etc/wpa_supplicant/wpa_supplicant.conf -iw
           â└â─457 /sbin/dhcpcd -q -w

Mar 13 22:53:26 rpi1 dhcpcd[331]: wlan0: if_up: Operation not possible due to RF
Mar 13 22:53:26 rpi1 dhcpcd[331]: wlan0: waiting for carrier
```

The line that confirms it is:
```
Mar 13 22:53:26 rpi1 dhcpcd[331]: wlan0: if_up: Operation not possible due to RF
```

I knew about Linux's rfkill because that's what laptops enable/disable when
you press the function (Fn) key that enables/disable wifi in Linux. There are
two types of block: soft and hard block. Normally laptops' Fn key shortcut
enable/disable a soft block.

To find out what case is there is I ran:

```
pi@rpi1:/dev$ rfkill list all
0: phy0: Wireless LAN
        Soft blocked: yes
        Hard blocked: no
1: hci0: Bluetooth
        Soft blocked: no
        Hard blocked: no
```

I ran `rfkill unblock all` and verified that the Wireless was indeed
unblocked.

I didn't know of another way of unblocking the radios, other than the command
line utility, therefore no copying of file there. Or is there?

I could add this line to the `/etc/rc.local` script, that is run by init after
the system finished changing runlevels as part of the boot process.

Although there was only one more pi left to be configured I wanted to see if I
could set it up the wifi connection from the microSD without booting the pi.

So I did this for the 3rd pi's microSD card, and verified with nmap from my
desktop.

```
Nmap scan report for 192.168.178.7
Host is up (0.073s latency).
```

Finally, I was able to set up a pi's wifi by directly editing the microsd on
my desktop, without needing to boot into the device. All in all, it was a
very enlightening experience.

Now that I can ssh to it, I don't expect to disassemble the cluster again
again to attach the usb-serial cable (which I did two times =( ), to set up
the kubernetes/hadoop tech stack.


[1]: https://elinux.org/RPi_Serial_Connection
[2]: https://wiki.debian.org/HowTo/ChangeHostname
