---
layout: post
title: Fixing Firefox screen tearing while scrolling / video playback
---

For a while now I've noticed screen tearing while scrolling on Wikipedia or on
YouTube videos whenever there's sudden movement.

Today I decided to do something about it, and considering I hadn't touch the
Xorg conf files "generated" by `nvidia-xconfig` in more than 4 years, I was
optimistic that it would be some configuration issue that would have been
automatically resolved by now with newer drivers and OpenGL versions.
Furthermore as the quotes on generated suggest, they weren't pristine
generated Xorg conf files since I had tinkered with them
quite a bit in the past when I was running two Nvidia cards on Arch which I
mentioned in passing in a [previous
post](https://www.antoniojgutierrez.com/2017/08/20/changing_xorg_conf.html)

So the first thing I did was generate a new Xorg conf, using the command:

```
$ nvidia-xconfig
```

Which graciously backups the previous config and generates a new one in its stead.

Having done that I rebooted the computer, opened Firefox and scrolled some
long articles in Wikipedia (like the one for the current [Covid-19
Pandemic](https://en.wikipedia.org/wiki/COVID-19_pandemic)) but the tearing
was still there.

So I searched the web and found [this post on
reddit](https://www.reddit.com/r/linuxmint/comments/9pb5ur/screen_tearing_while_scrolling/)
which basically describes exactly what was happening to me.

In it there are basically two solutions provided by the community:

1. Force GPU Acceleration on Firefox

2. Force Full Composition Pipeline of the Nvidia card.

The first one was self explanatory and the simplest, besides if you have the
hardware you would want to use it whenever and wherever you can, specially in
a desktop environment, where you're not restricted by power consumption from a
battery.

As for the second option, from the comments and after looking into the good
ole trusty [Arch Wiki for screen tearing on
nvidia](https://wiki.archlinux.org/index.php/NVIDIA/Troubleshooting#Avoid_screen_tearing),
it seems that while this _does_ avoid screen tearing not just in Firefox
but in every application, it comes with a performance penalty in OpenGL
applications as well as increasing the time needed for the GPU to "clock down"
i.e: bring its frequency back down due to changing power states after some
rendering task is finished.

Since in my case I've only noticed the screen tearing in Firefox, it seemed
like a no-brainer opt for this approach.

To change I simply needed to go to the advanced preferences page of Firefox
reached by navigating to `about:config` in the browser, search for
`layers.acceleration.force-enabled`, set it to `true` and restart Firefox.

Voilà.

Side note: The reason why GPU Acceleration is disabled by default in Firefox
on most, if not all Linux distributions is because WebGL is considered by many
as a security risk. For more a detailed explanation [check this
security.stackexchange
answer](https://security.stackexchange.com/questions/13799/is-webgl-a-security-concern)
