---
layout: post
date: 2018-08-11
title: Using a credential store for DockerHub login
math: true
---

Whenever there's a need to push images (public or private) or pull images from
privates repositories from DockerHub you need to login first. With the docker
client CLI that's achieved with:

```
docker login
```

It then asks for the credentials to use for logging in.
The problem with this approach is explicitly mentioned in output of that
command.

In my case:
```
Login with your Docker ID to push and pull images from Docker Hub. If you don't have a Docker ID, head over to https://hub.docker.com to create one.
Username: chibby0ne
Password: 
WARNING! Your password will be stored unencrypted in /home/turing/.docker/config.json.
Configure a credential helper to remove this warning. See
https://docs.docker.com/engine/reference/commandline/login/#credentials-store
```

The password is stored unencrypted (although not in plain-text).

Visiting the link you will find there are a number of docker-credentials
helpers in [docker-credentials-helpers] for every Operating
System.

For Linux, I decided to go with [pass] and use the appropriate
docker-credential-helper for it.

Pass is a password store that keeps passwords in a GPG encrypted file.

The steps to set it up are very straightforward:


1. Install `pass`

   Install `pass` using your package manager if its available, otherwise download
   a release and install it using `make install`

   You can find all the options in the official website: [pass]

2. Install `docker-credential-pass`

   You need a Go installation to build the binaries. If you don't have one yet
   just install it using your package manager, and set the `GOPATH` environment
   variable to the place where you want to have your go projects. For more
   information visit:

   Assuming you have a Go installation then:
    
   * Fetch the sources with:
      ```
      go get github.com/docker/docker-credential-helpers
      ```
      This will download the repo and place it in
      `$GOPATH/src/github.com/docker/docker-credential-helpers`.

   * Build the helper that uses pass by running:
      ```
      make pass
      ```

      The binary `docker-credential-pass` will be located in the `bin/` directory.
      You need to copy the binary to a directory included in the `PATH` environment so
      that the docker client CLI can actually find it.

3. Log out 

   Run ``docker logout``.
   
   If you where already logged in you should see:
   
   ```
   Removing login credentials for https://index.docker.io/v1/
   ```
   
   If you where not logged in, you should see:
   
   ```
   Not logged in to https://index.docker.io/v1/
   ```
   
4. Modify the `~/.docker/config.json` to use the `docker-credential-pass`

   Add the following key/value pair:
   
   ```
       "credsStore": "pass"
   ```
   
   By now your `~/.docker/config.json` should look something like this
   (User-Agent might be different):
   
   ```
   {
   	"auths": {},
   	"HttpHeaders": {
   		"User-Agent": "Docker-Client/18.05.0-ce (linux)"
   	},
   	"credsStore": "pass"
   }
   ```
   
5. Initialize `pass`

   You need to tell pass which GPG keypair you're going to use to encrypt/decrypt
   the password.
   
   Run:
   
   ```
   pass init GPG_ID
   ```
   
   Where GPG_ID is the fingerprint or email address that identifies the key.
   
   If you don't have a GPG keypair yet, then proceed to create it using:
   `gpg --full-gen-key` and configure the settings using the interactive prompt.

6. Login

   Run `docker login` and enter your credentials, but this time after entering
   the username and password, you should see a prompt to enter the password for
   the selected GPG key.
   
   Important to note is that the output should not have the warning message
   anymore.


7. Done!

   That's it. Now the you are logged in but the credentials are not stored
   unencrypted in the `~/.docker/config.json` file.


[docker-credentials-helpers]: https://github.com/docker/docker-credential-helpers
[pass]: https://www.passwordstore.org/
[setting_gopath]: https://github.com/golang/go/wiki/SettingGOPATH
