---
title: "Set a custom machine name in Vagrant"
date: 2022-06-10T01:17:07+02:00
description: "Keeping your VMs tidy"
featured_image: "/img/vagrant_virtualbox.png"
tags: [vagrant, virtualbox, VM]
category: VMs
---

This is something took me a while to found out since it is documented but not
as straight forward as "setting a custom machine name", but instead is a kind
of wanted side effect of using [multi-machine
setup](https://www.vagrantup.com/docs/multi-machine). But I'm getting ahead of
myself. Let's start with a quick review of what is Vagrant


## What is Vagrant

Vagrant is a virtual machine (VM) provisioning tool that allows us to enjoy
"Development Environment as Code" by configuring and managing VMs. It can
interface with so called "providers" such as Virtualbox, KVM, VMWare, Docker
containers, etc.
It does so by storing all the configuration in a `Vagrantfile` whose syntax is
regular is that of a regular ruby file where we specify the memory allocated,
cores allocated, provider used, provisioning, etc...

It uses a concept of "Box" which are the initial image to clone from and that
are fetched from Vagrant Cloud. Anyone can push images, although the most
downloaded ones are the ones coming from Hashicorp or known distros. You can
think of Boxes kind of like the equivalent of the images used in the `FROM`
directives in a Dockerfile, and Vagrant Cloud as Docker Hub. In Docker, the
images are then customized by copying files to it and provisioning by
installing additional packages.

In the case of Vagrant machines, we don't usually copy files as much as we
mount a directory from our local machine into the VM.

It's used by developers and operators to share reproducible virtual
environments which are stored as code as part of the project's repository.
Pretty nifty!


## Typical Vagrant Workflow


A typical workflow looks like this:

```
$ mkdir -p ~/Projects/vagrant_experiments/archlinux; cd !$
$ vagrant init arhclinux/archlinux
A `Vagrantfile` has been placed in this directory. You are now
ready to `vagrant up` your first virtual environment! Please read
the comments in the Vagrantfile as well as documentation on
`vagrantup.com` for more information on using Vagrant.
```

This then creates this `Vagrantfile` in the current working directory, with
the contents:

```
Vagrant.configure("2") do |config|
  config.vm.box = "archlinux/archlinux"
end
```

As you can see the initial box of the VM an `archlinux`.


If we run `vagrant up` it starts and provisions the vagrant environment, and
by will output something like this:

```
$ vagrant up
Bringing machine 'default' up with 'virtualbox' provider...
==> default: Importing base box 'archlinux/archlinux'...
==> default: Matching MAC address for NAT networking...
==> default: Checking if box 'archlinux/archlinux' version '20210415.20050' is up to date...
==> default: A newer version of the box 'archlinux/archlinux' for provider 'virtualbox' is
==> default: available! You currently have version '20210415.20050'. The latest is version
==> default: '20220615.61815'. Run `vagrant box update` to update.
==> default: Setting the name of the VM: archlinux_default_1655671732047_63504
==> default: Clearing any previously set network interfaces...
==> default: Preparing network interfaces based on configuration...
    default: Adapter 1: nat
==> default: Forwarding ports...
    default: 22 (guest) => 2222 (host) (adapter 1)
==> default: Booting VM...
==> default: Waiting for machine to boot. This may take a few minutes...
    default: SSH address: 127.0.0.1:2222
    default: SSH username: vagrant
    default: SSH auth method: private key
    default:
    default: Vagrant insecure key detected. Vagrant will automatically replace
    default: this with a newly generated keypair for better security.
    default:
    default: Inserting generated public key within guest...
    default: Removing insecure key from the guest if it's present...
    default: Key inserted! Disconnecting and reconnecting using new SSH key...
==> default: Machine booted and ready!
==> default: Checking for guest additions in VM...
==> default: Mounting shared folders...
    default: /vagrant => /home/chibby0ne/Projects/vagrant_experiments/archlinux
```

The output informs us that it created a VM using virtualbox, started the VM,
started a SSH daemon on the VM and created a pair of ssh key which is used for
ssh'ing to the VM, and finally that it mounted the local directory into
`/vagrant`.

It additionally mentioned that the VM name will be: `archlinux_default_1655671732047_63504`

In this line:

```
==> default: Setting the name of the VM: archlinux_default_1655671732047_63504
```

We can check all the environment and machines for the current user to see if
that name appears:

```
$ vagrant global-status
id       name         provider   state    directory
---------------------------------------------------------------------------------------------
c9924b8  default      virtualbox running  /home/tesla/Projects/vagrant_experiments/archlinux

The above shows information about all known Vagrant environments
on this machine. This data is cached and may not be completely
up-to-date (use "vagrant global-status --prune" to prune invalid
entries). To interact with any of the machines, you can go to that
directory and run Vagrant, or you can use the ID directly with
Vagrant commands from any directory. For example:
"vagrant destroy 1a2b3c4d"
```

So the name is actually `default` according to Vagrant. So where is the
`archlinux_default_1655671732047_63504` it mentioned before?

Here's the caveat: This command shows us the id and the name associated to
that ID of the vagrant machines. The name shown was the one used by the
provisioner to created the VM itself, in this case Virtualbox.

We can verify this by running:

```
$ VBoxManage list vms
"archlinux_default_1655671732047_63504" {1d8fc426-36bc-4035-a7a8-c76e1cf16128}
```

So the name of the VM in Virtualbox is
`archlinux_default_1655671732047_63504`, but the name of the machine is
`default`.

## So how do we set the machine name?

As hinted in the beginning in the case of Multi-Machines, this entails
managing multiple guest machines per Vagrantfile. That's the thing, **the
simple default case in vagrant is to manage a single machine per Vagrantfile**.
You can see from the output of the `global-status` command that it also
associates a path to the machine, but by default they are all named `default`.

To define a custom name we need to a another "define" section to the `Vagrantfile`.

```
Vagrant.configure("2") do |config|
  config.vm.define "arch" do |arch|
    arch.vm.box = "archlinux/archlinux"
end
```

Here we define machine name as "arch" and try to run `vagrant up`:

```
Bringing machine 'arch' up with 'virtualbox' provider...
==> arch: Importing base box 'archlinux/archlinux'...
==> arch: Matching MAC address for NAT networking...
==> arch: Checking if box 'archlinux/archlinux' version '20210415.20050' is up to date...
==> arch: Setting the name of the VM: archlinux_arch_1655673970260_47411
==> arch: Fixed port collision for 22 => 2222. Now on port 2200.
==> arch: Clearing any previously set network interfaces...
==> arch: Preparing network interfaces based on configuration...
    arch: Adapter 1: nat
==> arch: Forwarding ports...
    arch: 22 (guest) => 2200 (host) (adapter 1)
==> arch: Booting VM...
==> arch: Waiting for machine to boot. This may take a few minutes...
    arch: SSH address: 127.0.0.1:2200
    arch: SSH username: vagrant
    arch: SSH auth method: private key
        arch: Inserting generated public key within guest...
    arch: Removing insecure key from the guest if it's present...
    arch: Key inserted! Disconnecting and reconnecting using new SSH key...
==> arch: Machine booted and ready!
==> arch: Checking for guest additions in VM...
==> arch: Mounting shared folders...
    arch: /vagrant => /home/tesla/Projects/vagrant_experiments/archlinux
```

So the first line told us that it was bringing machine "arch" up. That's what
we want: *machine* name

And if we check the environments now we can see:

```
$ vagrant global-status
id       name         provider   state    directory
---------------------------------------------------------------------------------------------
c9924b8  default      virtualbox running  /home/tesla/Projects/vagrant_experiments/archlinux
6201c41  arch         virtualbox running  /home/tesla/Projects/vagrant_experiments/archlinux

The above shows information about all known Vagrant environments
on this machine. This data is cached and may not be completely
up-to-date (use "vagrant global-status --prune" to prune invalid
entries). To interact with any of the machines, you can go to that
directory and run Vagrant, or you can use the ID directly with
Vagrant commands from any directory. For example:
"vagrant destroy 1a2b3c4d"
```

We can see the "arch" machine now.

## Reality check

The reality of having custom-named machines, besides having a better and
easier time at disambiguating which machine which is running from a `vagrant
status` point of view, is that it's of little utility (outside of
multi-machine setups, in which case I'd say it's actually necessary), for two
reasons:

* For a single Vagrantfile with a single machine defined you can only have
  running machine.

  If we check the status of the directory after running the second `vagrant
  up` with the `arch` machine, we will see that `arch` is running whilst the
  `default` was halted.

```
$ vagrant status
Current machine states:

arch                      running (virtualbox)

```

* As hinted before, vagrant will always disambiguate machines by directory.
  In order words, you cannot start or halt a machine from a directory where
  the Vagrantfile defines a different machine, i.e: you know which machine
  you're interfacing with because of the directory you're currently in.


That said it does add a bit of order and neatness to the environments, and
doesn't hurt at all since it's just 2 extra lines to the Vagrantfile.


That's it! Quick and hopefully somewhat useful tip!
