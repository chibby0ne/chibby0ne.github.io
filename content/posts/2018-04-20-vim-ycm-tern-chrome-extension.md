---
layout: post
date: 2018-04-20
title: Using Javascript completion including adding Google Chrome API in vim
---

[YouCompleteMe][youcompleteme] is a very powerful code completion engine built
for the vim editor. It has several completion engines for many languages
including C/C++, Rust, Go, Python, Javascript among others. It contains two
parts: a vim-plugin and a daemon.

A very simple explanation of how YouCompleteMe works is by starting a server
whenever a file is opened whose filetype corresponds to one of the installed
languages engines. After this, the file is analyzed by communicating with from
vim using the vim plugin (client) to the daemon (server).
*The languages for which YouCompleteMe provides auto completion are selected
are install time, therefore if you don't explicitly install it selecting a
language, autocompletion for that language will probably be disabled.*

For Javascript, [tern][tern] is used for the code analysis. Tern itself
requires a `.tern-project` or a `.tern-config` file describing the specific
configuration in the root of the project or above or the home directory
respectively. Tern then opens a server in a unused port that listens for POST
requests from a client that asks questions about the code in the form of JSON.

When vim is started is runs tern in a separate process and starts the
querying whenever possible as described before.

In addition, tern supports server plugins, which are normal Javascript
programs that add extra functionality to the server. There are a few plugins
that come with the distribution such as doc and nodejs.

Now what if I wanted to develop a Chrome Extension and wanted the
autocompletion for the Chrome API in vim?

Well since tern supports this it would be only a matter of installing the tern
plugin and adding the plugin to the `.tern-project`. 

First installing the tern Chrome Extension: 
I found this [tern-chrome-extension][tern-chrome-extension] which worked
pretty well.

And we can't just install it in the project directory that would install it in
the `node modules` but vim(and tern) would be aware of it. We have to install it
in `node_modules` of the tern instance that's run by vim. 

To find it we run, with a vim instance reading a Javascript file:

```
ps aux | grep tern
```

We find that the process is running as:

```
/usr/bin/node /home/user/.vim/bundle/YouCompleteMe/third_party/ycmd/third_party/tern_runtime/node_modules/tern/bin/tern --port 46973 --host 127.0.0.1 --persistent --no-port-file
```

We want to navigate to this directory `/home/user/.vim/bundle/YouCompleteMe/third_party/ycmd/third_party/tern_runtime/`, because the `node_modules` is the place where we want it to be installed.

We run:

```
npm install --prefix . tern-chrome-extension
```

And we make sure everything went ok by checking if there's a directory with
that name in `node_modules` or in the `package.json`

```
$ cat package.json
{
  "description": "ycmd tern runtime area with required tern version and plugins",
  "dependencies": {
    "tern": "0.21.0",
    "tern-chrome-extension": "^46.1.20160420"
  }
}
```
Now all that's needed to create/edit the `.tern-project` file, and add the
chrome-extension to the plugins section.

```
$ cat .tern-project
{
  "libs": [
        "browser",
        "ecma5"
  ],
  "plugins": {
      "node": {},
      "chrome-extension": {}
  }
}
```
That's it! Now you can enjoy having the Chrome API completion in vim.

[youcompleteme]: https://github.com/Valloric/YouCompleteMe
[tern]: http://ternjs.net/
[tern-chrome-extension]: https://www.npmjs.com/package/tern-chrome-extension
