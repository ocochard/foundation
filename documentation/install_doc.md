## Introduction

This document present the automatic install process to build 
* The Common Process
* The Web Server
* The Data Server
* A Lxc Hypervisor
* A Docker hypervisor

If we want to discover the automatic process with more details, see [the complete manuel](complete_doc.md)

## The Common Process

All configurations need a base

* Server with Debian OS (STRETCH)
* A account to access and executes different commands (SUDO)

### Install Debian OS with STRETCH

We recommand to install a debian JESSIE and after the step, we upgrade the JESSIE to STRETCH like this
```sh
root@ansible:/etc/apt# sed -i'.orig' 's/jessie/stretch/gi' sources.list
```

It's time to *update/upgrade* our server.
```sh
root@ansible:/etc/apt# apt-get update
...
root@ansible:/etc/apt# apt-get -y dist-update
...
root@ansible:/etc/apt# apt-get -y autoremove
```
We need some interesting packages, so
```sh
root@ansible:/etc/apt# apt install sudo
```

We suppose our account is *login:spike*
We have add this account to th *sudo* group.
```sh
root@ansible:/etc/apt# adduser spike sudo
```

### Install the account

We are connected with  *spike* account.
Firstly, we test the sudo.
```sh
sudo ls
```
ok, now it's time to create the *.ssh* directory.
```sh
spike@ansible:~$ mkdir -m=700 .ssh
```

## DataBase Server Install

The Database server is ready, it's time to clone the main project on our computer
All commands are ran from my desktop (a mac in this case).

firstly, I send my ssh key to the database server.
```sh
┌─[spikespiegel@spike-iMac]─[~]
└──╼ cat ~/.ssh/id_rsa.pub | ssh spike@dbserver "cat - >> .ssh/authorized_keys"
```
In second, I clone the project
```sh
┌─[spikespiegel@spike-iMac]─[~/Projects]
└──╼ git clone https://github.com/gandalf-the-white/foundation.git
```

Perfect, we go in the *ansible* directory and we edit the *hosts* file where we put the IP address of the database server in the [dbserver] section. in my case, i wrote all ip address in my */etc/hosts* (in the mac)
```sh
[dbserver]
dbserver ansible_user=spike
```
 Great, we can run the process
 ```sh
 ┌─[spikespiegel@spike-iMac]─[~/Projects/foundation/ansible]
 └──╼ ansible-playbook maas.yml -i hosts --ask-sudo-pass -u spike --extra-vars "password=mypassword"
 ```
The password *mypassword* is the **spike** password.

## Web Server Install

The same process that the database server.

the key.
```sh
┌─[spikespiegel@spike-iMac]─[~]
└──╼ cat ~/.ssh/id_rsa.pub | ssh spike@webserver "cat - >> .ssh/authorized_keys"
```
The hosts file.
```sh
[webserver]
webserver ansible_user=spike
```

Short difference, the web server **must** known the dbserver **IP address**, so  for example
```sh
┌─[spikespiegel@spike-iMac]─[~/Projects/foundation/ansible]
└──╼ ansible-playbook maas.yml -i hosts --ask-sudo-pass -u spike --extra-vars "password=mypassword ipdbserver=10.0.0.56"
```
The password *mypassword* is the **spike** password. Drupal must access to the database managed by the user (spike in this example)

In the case of a dirty proxy, we have to modify the **maas.yml** to set the **environment**. The **environment** parameter is currently commented.
That's all.

## Lxc Hypervisor

The same process that the database and web servers.

the key.
```sh
┌─[spikespiegel@spike-iMac]─[~]
└──╼ cat ~/.ssh/id_rsa.pub | ssh spike@lxcserver "cat - >> .ssh/authorized_keys"
```

The hosts file.
```sh
[lxc]
lxcserver ansible_user=spike
```

And we run the process
```sh
┌─[spikespiegel@spike-iMac]─[~/Projects/foundation/ansible]
└──╼ ansible-playbook maas.yml -i hosts --ask-sudo-pass -u spike 
```

Before to use this LXC server, we have to coonect on it with the user account, in my case **spike** and enable the localhots *known_hosts* (only once)
```sh
spike@lxcserver:~/ssh localhost
```

That's all.

## Docker Hypervisor

The same process that the others servers.

the key.
```sh
┌─[spikespiegel@spike-iMac]─[~]
└──╼ cat ~/.ssh/id_rsa.pub | ssh spike@dockerserver "cat - >> .ssh/authorized_keys"
```

The hosts file.
```sh
[docker]
dockeserver ansible_user=spike
```

And we run the process
```sh
┌─[spikespiegel@spike-iMac]─[~/Projects/foundation/ansible]
└──╼ ansible-playbook maas.yml -i hosts --ask-sudo-pass -u spike 
```
like the Lxc server,we enable the *known_hosts* 

## Important

For each LXC or Docker server,we must add in the spike *.ssh/config* file a new enter like this in the MaaS server to connect each server to the MaaS server before to run *./foundation/scripts/run_tunnel_ssh.sh*.

Example: 
* a LXC server dorothy IP address 92.13.25.45
* the MaaS server IP address 154.23.56.89

```sh
...
Host dorothy
   HostName             92.13.25.45
   User                 spike
   Port                 3333
   RemoteForward        44443 154.23.56.89:443
   ServerAliveInterval  30
   ServerAliveCountMax  3
```

Of course, the webserver public key must be included in the LXC server *authorized_keys*
```sh
spike@webserver:~/.ssh$  cat ~/.ssh/id_rsa.pub | ssh spike@lxcserver "cat - >> .ssh/authorized_keys"
```
