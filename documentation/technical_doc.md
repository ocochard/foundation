# A quick Installation

All tools are located in the *ansible* directory.
```
cd foundation/ansible
```


## Before to start

Before to run any scripts, be sure that you have an account on server you want to install a function and sent your key ssh. Don't forget to enable the *sudo* for this account.

To create a ssh key, run  *ssh-keygen -b 4096* and send the key to the server.
```
# example with spike account on  192.168.20.45
$ cat .ssh/id_rsa.pub | ssh spike@192.168.20.45 "cat - >> .ssh/authorized_keys"
```

To enable *sudo*, install the package and add the *spike* account in the *sudo* group 
```
# sudo package install
$ apt-get install -y sudo
# add user in group
$ adduser spike sudo
```
Test the sudo with the account
```
# Test the update for example
sudo apt-get update
```

 We have to define the different actors
 * The Web Server **[webservers]**
 * The Database Server **[dbservers]**
 * One or many Hypervisors **[hypervisors]**

This information is located in the [hosts](https://github.com/gandalf-the-white/foundation/blob/master/ansible/hosts) file

First example. I want to configure my maas. The MaaS is made of 2 servers, the web and data servers. In this example, *192.168.10.32* and *192.168.10.56*. The configuration is pretty simple.
```
# car ./hosts
[webserver]
192.168.10.32

[dbserver]
192.168.10.56
```

After your modification, run **ansible-playbook maas.yml -i hosts --ask-sudo-pass**

Other example. I want to configure 2 **hypervisors** defined by the IP address *192.168.1.10* and *92.156.48.30*. 
```
# cat ./hosts
[hypervisors]
192.168.1.10
92.156.48.30
```

Like the first example, you run **ansible-playbook maas.yml -i hosts --ask-sudo-pass**

For all hypervisors, and because of the **SSH TUNNEL**, we have to add in the *./.ssh/config* the values relatives to the new hypervisors. For example, we added a new hypervisor with the IP/port address *93.52.45.21:2222*.
```
# cat ~/.ssh/config
...
Host new-hypervisor-tunnel
   HostName             93.52.45.21
   User                 spike
   Port                 2222
   RemoteForward         44443 maas_address:4443
   ServerAliveInterval  30
   ServerAliveCountMax  3
```
And we start the tunnel with *autossh -M 0 -f -T -N new-hypervisor-tunnel*.

# Technical Architecture

### The Server

1. First job: A simple Drupal server to *communicate* with the global architecture in asynchronous mode. 
2. Seconde job: To collecte all data sent by all function probes.
3. Third job: To show easily data collected

### The hypervisor

1. First job: To collect data of configuration (delete, create, stop container)
2. Second job; To manage iptables rules on many bridges connected on containers

## Installation

### The hypervisor

#### List of packages

* lxc (to manage containers)
* git (to update engine)
* bridge-utils (to manage bridges)
* iptables-persistent (to memorise iptables rules)
* isc-dhcp-server (to attribute IP Address to containers)
* autossh

The LXC memory management is disable. We have to enable the parameter like this
```
# /etc/default/grub
...
GRUB_CMDLINE_LINUX="cgroup_enable=memory"
...
```
This parameter is enable after a `update-grub`

#### The cron

Because all transactions are unidirectionnal and asynchrones between the server and the hypervisors, we use 3+1 cron commands

```
SHELL=/bin/bash
PATH=/usr/local/bin:/usr/local/sbin:/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/bin
URL_MAAS='-k https://192.168.0.3:4443'

*/5    *   *   *   *   /opt/foundation/scripts/update_hypervisor.sh    > /dev/null
*/5    *   *   *   *   /opt/foundation/scripts/update_containers.sh    > /dev/null
*/5    *   *   *   *   /opt/foundation/scripts/add_containers.sh       > /dev/null
0      0   *   *   *   /opt/foundation/scripts/update_git.sh           > /dev/null
``` 
The variable `URL_MAAS` gives the maas server FQDN address.

+ `update_hypervisor.sh` initialize the hypervisor on the maas server.
+ `update_containers.sh` update the differents containers (start, stop and delete) and update iptables rules depending the containers features.
+ `add_containers.sh` upload its own data, creates new containers, downloads new images in case of.
+ `update_git.sh` updates the engine

### The MaaS Server

#### List of packages

* nginx (simple to install)
* drupal (CMS)
* mariadb
* iptables


#### Mariadb Installation
Let's started with the database server.

```
apt-get -y install mariadb-server mariadb-client
```



#### Nginx Installation
Let's started by nginx and fpm
```
# apt-get install nginx php5-fpm 
```
1. In the directory _/etc/nginx/sites-available/, we duplicate the *default* file by *cdpweather* and we modify it like this
```
server {
       listen 443 ssl default_server;
       root /var/www/html;
       server_name _;
       ssl_certificate /etc/nginx/ssl/nginx.crt;
       ssl_certificate_key /etc/nginx/ssl/nginx.key;

       location @rewrite {
       		rewrite ^/cdpweather/(.*)$ /cdpweather/index.php?q=$1;
		}
       location / {
       		try_files $uri /cdpweather/index.php?$query_string;
		}
       location ~ \.php$ {
       		include snippets/fastcgi-php.conf;
		fastcgi_pass 127.0.0.1:9000;
		}
       location ~ /\.ht {
       		deny all;
		}
}

```
We enable the new configuration in the _/etc/nginx/sites-enabled_ directory. Don't forget to delete the default configuration.
```
# cd /etc/nginx/sites-enabled
...
# ln -s ../sites-available/cdpweather
```
Et voila
2. Don't forget to generate keys

```
# mkdir /etc/nginx/ssl
...
# openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout /etc/nginx/ssl/nginx.key -out /etc/nginx/ssl/nginx.crt
```
3. We finish by the php configuration
```
# vi /etc/php5/fpm/php.ini
```
Set these parameters
cgi.fix_pathinfo=0
post_max_size = 200M

and the www.conf
```
# vi /etc/php5/fpm/pool.d/www.conf 
```
Set the parameter
listen = 127.0.0.1:9000
and comment "listen = /var/run/php5-fpm.sock"

#### Drupal Installation



## Network communication

### The tunnel

#### First part, the communication with the hypervisrs and the maas server

And because the MaaS server can't use a pull usage, a friendly solution concists to use a simple tunnel ssh started by the MaaS server on the remote hypervisors to keep the main process of data push made by the different clients
We propose the command

```
user@maas $: ssh -R 44443:192.168.0.3:4443 192.168.0.3 -p 3333
```

The MaaS server run the command
* *44443*, port open on the hypervisor
* *192.168.0.3*, hypervisor IP Address
* *4443*, server port on the MaaS (443 or 80 more simply)
* *3333*, ssh port access on hypervisor

More friendly, we create a `~/.ssh/conf` with these parameters

```
# ~/.ssh/config
Host hypervisor-tunnel
   HostName             192.168.0.3
   User                 spike
   Port                 3333
   RemoteForward         44443 192.168.0.3:4443
   ServerAliveInterval  30
   ServerAliveCountMax  3
```

And we run the command `autossh -M 0 -f -T -N hypervisor-tunnel`
We don't forget to install `autossh`
In this case, we modify th `URL_MAAS` variable

```
URL_MAAS='-k https://127.0.0.1:44443'
```

A good recommendation to start the set of tunnels,a cron at the reboot like this
```
SHELL=/bin/bash
PATH=/usr/local/bin:/usr/local/sbin:/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/bin
HOME=/home/spike
@reboot  /home/spike/foundation/reventlov/scripts/start_tunnels.sh > start.log 2>&1
```
#### Second part, the communication with containers and the maas server
The first remote tunnel is enable only for the host but not for the containers. In this case, we create a tunnel to access to the port 44443 like this
```
# ssh -N -f -L 0.0.0.0:64443:127.0.0.1:44443 spike@127.0.0.1
```
or 
```
#vi .ssh/config
Host Tunnel
	Hostname 127.0.0.1
	User	spike
	LocalForward   0.0.0.0:64443 localhost:44443
```
The problem is that all client can access to this redirection. So we 're going to create a second tunnel like this
```
#vi .ssh/config
Host Tunnel
	Hostname 	localhost
	User		spike
	Port 		22
	identifyFile	~/.ssh/id_rsa
	LocalForward   	64443 localhost:44443
	LocalForward   	192.168.49.49:64443 localhost:44443
```
and we add a ip address to lo. (We don't forget to create our key)
```
auto lo:0
iface lo:0 inet static
	address	192.168.49.49
	netmask	255.255.255.0
```
Here we go, if a container execute a simple curl like
```
# curl -k http://hypervisor:64443
```
the request goes through the tunnels 

#### Autostart 
We begin by the service
```
# vi /etc/systemd/system/autossh-tunnel.service
[Unit]
Description=AutoSSH tunnel service on local port 64443
After=network.target

[Service]
Environment="AUTOSSH_GATETIME=0"
ExecStart=/usr/bin/autossh -M 0 -T -N tunnel
User=spike

[Install]
WantedBy=multi-user.target
```
We don't forget to test
```
systemctl start autossh-tunnel.service
systemctl enable autossh-tunnel.service
```

#### Interesting links
* https://www.everythingcli.org/ssh-tunnelling-for-fun-and-profit-autossh/
* http://blog.arkey.fr/2016/12/05/make-docker-container-access-host-ssh-tunnel/
* http://nerderati.com/2011/03/17/simplify-your-life-with-an-ssh-config-file/
* https://www.garron.me/en/linux/add-secondary-ip-linux.html
* http://baturin.org/docs/iproute2/#Set%20human-readable%20link%20description

#### How to create keys with ssh
Let's started by `ssh-keygen` and  yours id_rsa and id_rsa.pub will be created in your _.ssh_ directory. Send your public key to yours hypervisors.
```
$ cat .ssh/id_rsa.pub | ssh spike@IP_HYPERVISOR  "cat - >> .ssh/authorized_keys"
```
Here we are.


### Security

The connection is based on a simple public key to provide a direct but secure access to hypervisors. The PermitLoginAccess is disable indeed (in the hyper.). To increase the security, we'll permit only IP address to connect on hypervisors.
A simple rule on the hypervisors iptables rules must be added.
```
iptables -A INPUT -p tcp -s MaaSServerIP --dport 4443 -j ACCEPT
```
Et voila...

Reference to systemd and per user
* https://www.everythingcli.org/ssh-tunnelling-for-fun-and-profit-autossh/
* https://bbs.archlinux.org/viewtopic.php?id=162297

## How to create a LXC image 

* Shutdown the container
```
# lxc-stop -n $NAME
```

* Archive container rootfs & config
```
# cd /var/lib/lxc/$NAME/
# tar --numeric-owner -czvf container_fs.tar.gz ./*
```

* Copy the file to your new server
```
# rsync -avh container_fs.tar.gz user@newserver:/var/lib/lxc/
```

* Extract rootfs
```
# mkdir /var/lib/lxc/$NAME/
# cd /var/lib/lxc/$NAME/
# tar --numeric-owner -xzvf container_fs.tar.gz ./*
```
## How to create a DOCKER image

So simple. I suppose that you have created on your docker server an image

* Firstly, check if there is no container running and based on this image
```
# docker ps
```

* Verify the correct name of your great image
```
# docker image
```

* Save the image
```
# docker save yourimage | gzip > yourimage.tar.gz
```

* For our maas server we have to rename the *tar.gz* in *maas*
```
# mv yourimage.tar.gz yourimage.maas
```

That'all.
