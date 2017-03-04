## Introduction

This document explains how to create a image including our program.
Because the sofware will be started depending a period and more exactly from a cron, we have to know how long the process will run. 

## How to create a LXC image?


The solution consists to install our application in a LXC container.
A good recommendation is to chose a light distribution when we create the container.
Inside the new container, we define a script or a binary to load. This script or binary can be execute by the cron.

### CDPWeather containera building

We'll take example on CDPWeather. Firstly, we'll create our container.
```sh
root@lxcserver:~# lxc-create -n cdpweather -t debian -- -r jessie
```
It'will take few second, depending last last created containers.
In my case, the container has to be connected on the bridge br0 (ip:192.168.1.1), then we'll modify the *root@lxcserver:~# vi /var/lib/lxc/cdpweather/config* to enable the network.

We'll replace *lxc.network.type = empty* by these 5 lines.
```
#lxc.network.type = empty
lxc.network.type = veth
lxc.network.flags = up
lxc.network.link = br0
lxc.network.name = eth0
lxc.network.ipv4 = 0.0.0.0/24
```

Inside the container, we set the interface */var/lib/lxc/cdpweather/rootfs/etc/network/interfaces* like this.
```sh
auto lo
iface lo inet loopback

auto eth0
iface eth0 inet static
   address  192.168.1.10
   netmask  255.255.255.0
   gateway  192.168.1.1
```

Ok, my container is created, the config file is right, so it's time to start and to connect on it.
```sh
root@lxcserver:~# lxc-start -n cdpweather
root@lxcserver:~# lxc-attach -n cdpweather
root@cdpweather:~#
```

### CDPweather Engine building

We are in the cdpweather container. Great.
A update/upgrade could be a good start with some good packages.
```sh
root@cdpweather:~# apt update && apt upgrade
root@cdpweather:~# apt -y install vim git
```

Ok,let'start by clone the great cdpweather engin in the */opt* directory. We don't forget to switch on the develop branch.
```sh
root@cdpweather:/opt# git clone https://github.com/gandalf-the-white/dockbaley.git
root@cdpweather:/opt# cd dockbaley/
root@cdpweather:/opt/dockbaley$ git checkout develop
```

And because we use nodejs, here is the process to install this package
```sh
root@cdpweather:/opt/dockbaley# apt install curl build-essential libpcap-dev libfontconfig1 libicu52 python libjpeg62-turbo
root@cdpweather:/opt/dockbaley# curl -sL https://deb.nodesource.com/setup_5.x | bash -
root@cdpweather:/opt/dockbaley# apt -y install nodejs
```

Perfect, now it's time to install node modules
```sh
root@cdpweather:/opt/dockbaley# npm install
root@cdpweather:/opt/dockbaley# npm install https://github.com/mranney/node_pcap.git
```

The last element is *phantomjs*. And because we use a *jessie* release, it's very simple.
```sh
root@cdpweather:# wget https://bitbucket.org/ariya/phantomjs/downloads/phantomjs-2.1.1-linux-x86_64.tar.bz2
root@cdpweather:# bunzip2 phantomjs-2.1.1-linux-x86_64.tar.bz2
root@cdpweather:# tar xvf phantomjs-2.1.1-linux-x86_64.tar
root@cdpweather:# cp phantomjs-2.1.1-linux-x86_64/bin/phantomjs /usr/local/bin/
```

Now, we prepare the autonomous process of the container with the crontab.
```sh
root@cdpweather:/opt/dockbaley/scripts# crontab /opt/dockbaley/scripts/crontab
```

```sh
# crontab -l
0 0 * * *   	  /opt/dockbaley/scripts/gitupdate.sh
0 * * * *   	  /opt/dockbaley/scripts/rootscript.sh
*/30 * * * *   	  /opt/dockbaley/scripts/process.sh
```

The first line proposes to update our engine. The second propose to update by progammation from the central server the thirth line which starts the process of cdpweather every 30mn.

So if you modify the start of process from the central NMaaS, this the thirth line which will be modify.

It's time to clean our container.
```sh
root@cdpweather:~# apt-get remove -y build-essential python vim manpages 
root@cdpweather:~# apt-get autoremove
root@cdpweather:~# apt-get clean
root@cdpweather:~# rm -rf /var/lib/apt/lists/*
root@cdpweather:~# rm -rf /tmp/*
root@cdpweather:~# for i in `dpkg -l | grep '^rc' | cut -d" " -f3`; do dpkg --purge $i; done
```

### The package 

All is done, we stop the container including our application *lxc-stop -n cdpweather*.

firstly, we go in the container directory and we backup all the content.
```sh
root@lxcserver:/var/lib/lxc/cdpweather# tar --numeric-owner -czvf cdpweather.tar.gz ./*
```

For the MaaS, we must rename the file *cdpweather.tar.gz* by *cdpweather.maas*

That's all
