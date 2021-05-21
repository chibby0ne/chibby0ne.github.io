---
title: Things to backup before doing a factory reset of an Android Phone
date: 2021-01-31T23:46:55+01:00
featured_image: "/img/android_loading.jpg"
tags: [backups, mental notes, android]
category: backups
---

There's always something that I miss whenever I do a factory reset of my
personal phone, whether it's the signal backup (which is now enabled by
the usage of a PIN), or some new 2FA code. This post (as many of them are)
are mental (and written) notes for future me to remember.


## Signal backup

Signal now allows you to backup chats and media into a local directory in the
phone. When a Signal is reinstalled it prompts you for restoring from a local
backup. Remember to write down the backup code otherwise you will not be able
to restore.

To enable backup of the Chats you must go to `Settings -> Chats -> Chat
backups` and click the `Turn On` button and write down the backup code.


## WhatsApp backup

WhatsApp backups up the conversations to Google Drive by default, but does so
on a weekly basis, therefore in order to not miss out on potentially 6 days of
messages, you should manually trigger a backup.

In order to trigger one, you must go to `Settings -> Chats -> Chat backup ->
Back up`

## Browser profile sync

For Firefox, Go to `Settings -> YOUR_ACCOUNT -> Sync Now`
For Chrome, there's no a manual sync now button that I could tell but you
could disable and enable sync and that should trigger at least a check.


## 2FA Authentication code backups

If you must do one backup **this is the one you must do**; you'll be in a
world of pain when you can't access your accounts due to missing 2FA.
Fortunately there are several ways on how to backup these now for each one of
the famous apps like Google Authenticator, Authy, and others.


## Important notes

* Unlocking ***and*** locking the bootloader resets the phone.
* Very difficult to get rid of Google Play services completely if choosing the
  red pill and going for a custom ROM.

As you can see most everything is backed up by default to the cloud either
associated to an Gmail account or the apps themselves maintain state in the
cloud, so that in both cases a simple sign in would restore your profile to
its last state.

Hooray for the cloud?!
