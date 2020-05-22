---
layout: post
title: Backing up Google Authenticators's Keys from an Android Phone
---

I wanted to backup my 2FA codes stored in Google's Authenticator app. I don't
want lose access to my accounts, in case I lose that phone.

Sure there are also the backup codes, which are given/shown whenever 2FA is
set up, and are intended to be used on a rainy day such as when your
phone dies, gets stolen, or simply wiped accidentally or even on purpose but
they just forgot about the 2FA codes living there (not speaking from personal
experience at all of course :cry: ). Unfortunately, many people don't pay too
much attention to these, myself included.

I **strongly encourage you to store those backup codes** because you
never know when you're going to need them.  They have saved me in the past
from being able to access an account when my phone battery died, I had no
charger and needed to access it urgently.

But in those cases where everything goes wrong, well you just have to reset
all accounts where you have 2FA, i.e: reset passwords and somehow prove that
you are who you say you are and not a malicious attacker that obtained the
passwords for those accounts (which is kind of a tricky situation for many
services since they don't want to be accidentally victims of a social
engineering attack trying to collaborate with the so-claimed user).

Hopefully this serves as enough warning and encouragement to backup your 2FA
or simply write down the backup codes. If you get anything out of this post,
let it be that.

Now back to backing up the 2FA keys in Google Authenticator (GA) app: the main
advantage of this approach being that you don't have to change/edit or add any
new device into any of your accounts, but instead you simply add the keys
manually to another 2FA app whether it lives in app in your phone, a desktop
app or somewhere else.

You also don't need to use any of your *limited* backup codes in case you need
access and don't have that one phone with all the 2FA keys at hand in that
moment.


# Prerequisites

This guide assumes you have rooted your Android phone.

If you don't want to do this due to security concerns, then your only option
is to use the backup codes and that's the end of this road for you.

If you don't mind rooting your phone, and you haven't there are many ways of
doing so, a simple duckduckgo search will lead you down the rabbithole.

In addition we will need
[adb](https://developer.android.com/studio/command-line/adb) adb i.e: android
debug bridge. This is part of
the `android-tools` package in Arch Linux. You can also download and install
it as part of the platform-tools from the [Android
website](https://developer.android.com/studio/releases/platform-tools)

This guide also assumes your phone is connected to the computer and you have
USB debugging enabled (For more detailed steps please check this [official
android guide](https://developer.android.com/studio/debug/dev-options).

# Run adb as root

First of we need to run adb as root. This is easier said that done.

For this we can run `adb root` in a terminal, but it might be the case that after we do that
and try to fetch GA's database we get the following error:
```
$ adb pull /data/data/com.google.android.apps.authenticator2/databases/databases
adb: error: failed to stat remote object '/data/data/com.google.android.apps.authenticator2/databases/databases': Permission denied
```

```
$ sudo adb shell
AndroidPhone:/ $ ls -lh
ls: ./init.rc: Permission denied
ls: ./init: Permission denied
ls: ./ueventd.rc: Permission denied
```

You can also tell that the session is **not** a root session because the
prompt ends with `$` instead of `#`.


```
$ adb root
adbd cannot run as root in production builds
```

```
$ adb shell "su"
Permission denied
```

In my particular case, this is an issue caused of having Magisck Hide enabled,
mentioned in this [Github
issue](https://github.com/topjohnwu/Magisk/issues/425#issuecomment-409101558).

In case you're wondering Magisck Hide is part of
[Magisck](https://www.xda-developers.com/how-to-use-magisk/) and is a feature
that "*allows you put a cloaking device on root permissions for certain
apps.*". Very useful for banking apps, for instance, which don't allow you to
login on a rooted device.

So I decided to disable Magisck Hide for now and reboot.

Afterwards I was able to start an adb root session:

```
$ adb root
restarting adbd as root
timeout expired while waiting for device
```

And then checking the prompt for the `#` at the end.

```
$ adb shell
OnePlus3:/ # ls -lh
total 2.7M
dr-xr-xr-x  90 root   root      0 1972-06-20 21:28 acct
lrwxrwxrwx   1 root   root     11 1970-01-01 01:00 bin -> /system/bin
lrwxrwxrwx   1 root   root     19 1970-01-01 01:00 bt_firmware -> /vendor/bt_firmware
lrwxrwxrwx   1 root   root     50 1970-01-01 01:00 bugreports -> /data/user_de/0/com.android.shell/files/bugreports
```

# Fetching the database

Afterwards I was able to pull the database, and I noticed that it hadn't been
modified since many months ago, even though I had recently added new 2FA entries,
after trying to open it I only found a handful of 2FA keys, but I also noticed
something else.

There were two additional files next to the `database` file: a `-wal` and a `-shm` file.

After some more duckduckgo searching I found a well written in a well written and
useful [Medium
post](https://medium.com/@Zredna/browsing-your-android-apps-database-3c67aa3f4a3c)
by [@Zredna](https://medium.com/@Zredna), in which he explore sqlite
databases in android apps.

In it he explains that the `-wal` file, is used for the [write-ahead
logging](https://en.wikipedia.org/wiki/Write-ahead_logging), which is a
technique for providing atomicity and durability in databases, part of the
[ACID properties](https://en.wikipedia.org/wiki/ACID), and Wikipedia explains
that the technique relies on first writing the changes in the log **before**
writing them in the database. Kind of the equivalent of journaling in filesystems.
In short the directory looked something like this:

```
OnePlus3:/data/data/com.google.android.apps.authenticator2/databases# ls -lh
total 2.7M
lrwxrwxrwx   1 root   root     6K  1970-01-01 01:00 databases
lrwxrwxrwx   1 root   root     80  1970-01-01 01:00 databases-shm
lrwxrwxrwx   1 root   root     10K 1970-01-01 01:00 databases-wal
```


As to the meaning  of the `-shm` file, that is a **shared-memory file** and is
used by the sqlite databases when operating in WAL mode, used as an index of
the WAL file. You can read more about it the [sqlite tempfiles
documentation](here: https://www.sqlite.org/tempfiles.html)

So that required actually fetching both of those extra files in order to get a
complete picture of the current database state.

# Opening the database

As per the blog, we need to commit the changes from the `wal` file to the
database file, in order to do that we run the command:

```
sqlite3 app.db "PRAGMA wal_checkpoint"
```

Afterwards, we can open the database using an interactive sqlite session,
select all the entries from the `account` table:

```
$ sqlite3 ./databases
SQLite version 3.31.1 2019-12-10 19:55:54
Enter ".help" for usage hints.
sqlite> select * from accounts;
```

Or simply save all the values without an interactive session to another file:

```
sqlite3 databases 'select * from accounts' > 2fa_accounts
```

Which will write a table representation that looks like this:

```
$ cat 2fa_accounts
1|Github|asdf12345asdf|0|0|0||Github
2|Twitch|deadbeef101010|0|0|0||Twitch
```

And the 2FA private keys would be the **deadbeef101010** and the
**asdf12345asdf** in this case, which you can then now use in case you move to
a different phone.

That's it! Hope that helps you in some way!

PS: These are **not real 2FA private keys** in case you're wondering, potential hacker.
