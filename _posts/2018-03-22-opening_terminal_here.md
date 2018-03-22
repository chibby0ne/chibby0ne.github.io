---
layout: post
title: '"Open Terminal Here" using thunar in i3'
---

I have i3 and *some* xfce4 packages installed in an Arch Linux installation.
I mostly use i3 because I like how it, as a tiling manager, efficiently uses
all of the monitor's real state.

Something that I might forget in the future and was mildly annoying when I
made the shift to i3, is making sure thunar (xfce4 file manager) had the
option of *Open Terminal Here* **and** that it opened the right terminal.
After a few minutes of googling, I figured out how to add the option.

Problem was that it was always opening `xfce4-terminal`, and not `urvxt` terminal
which is the terminal emulator I use for i3.

After digging in a bit, I discovered that the *Open Terminal Here* option is
actually a custom action of thunar itself, that can be edited in `Edit >
Configure custom actions...`, there you find the action *Open Terminal
Here* After clicking edit I could see the command that is being executed. The
funny thing is that it didn't execute xfce4-terminal. Instead it runs
`exo-open`.

That's how I learned that `exo-open` is a shell command that is comes with the
`exo` package, and allows you open urls and launch preferred applications in xfce4.

All I needed to do then is to modify the preferred application for terminal
emulator in `xfce4-settings-manager`, set `urxvt` and voila!
