## The subject
I propose to build a simple platform composed of 2 servers:
- A LXC hypervisor
- The server MaaS composed of 2 containers including or only one server
    - the Database
    - the Web server
    
![Diagram](/Diagram.svg)

### Initialisation

We prepare the first server:
- Debian Install based on a Stretch version
- Run the ansible process provided in our git package

#### Debian Install based on a Stretch version
I recommand to install in first a jessie version of Debian and finally upgrade to a stretch version. We have 
created a simple user (*login: spike*).
We only have to replace in the */etc/apt/sources.lst* file the word **jessie** by **stretch** and finally, we 
update and dist-upgrade the server like this
```sh
apt-get update && apt-get -y dist-upgrade
```
We don't forget to clean our server
```sh
for i in `dpkg -l | grep '^rc' | cut -d" " -f3`; do dpkg --purge $i; done
```
#### Mariadb, Apache, php and Drupal Install on the same server

If we want to install all unities on the same server (Mariadb, Apache, php and Drupal),
go to the **Database Install on dbserver section**
otherwise ;)

#### The LXC hypervisor 
It's time to work on the LXC hypervisor.
We install the *sudo* package, important for the deployement of the server and we add the *spike* account to 
the *sudo* group.
```sh
root@lxc:~# apt-get install sudo
root@lxc:~# adduser spike sudo
```
We connect on the server with the login *spike* and create a **SSH** key.
```sh
spike@lxc~$ ssh-keygen
```
That all for the LXC hypervisor.

#### The MaaS server
We propose to create the 2 containers
- Web server based on *drupal* and *apache*
- The data server based on *mariadb*

A good oportunity to start with the LXC environment. Let's started with the main packages.
```sh
root@maas:~# apt -y install lxc bridge-utils iptables-persistent
```
##### The bridge in the MaaS server
Let's started to create the bridge *br0*
We modify the */etc/network/interfaces* file:
```sh
...
auto br0
iface br0 inet static
   bridge_ports none
   bridge_fd 0
   bridge_maxwait 0
   address 192.168.1.1
   netmask 255.255.255.0
```
and activate the **net.ipv4.ip_forward**. Edit the */etc/sysctl.conf*
```sh
net.ipv4.ip_forward=1
net.bridge.bridge-nf-call-ip6tables = 0
net.bridge.bridge-nf-call-iptables = 0
net.bridge.bridge-nf-call-arptables = 0
```

##### The containers in the MaaS server
It's time to create our 2 containers.
```sh
root@maas:~# lxc-create -n dbserver -t debian -- -r jessie
root@maas:~# lxc-create -n webserver -t debian -- -r jessie
```
Before to start them, we modify the configuration like this.
Edit */var/lib/lxc/dbserver/config* and replace *lxc.network.type = empty* by
```sh
...
lxc.network.type = veth
lxc.network.flags = up
lxc.network.link = br0
lxc.network.ipv4 = 192.168.1.10/24
lxc.network.ipv4.gateway = 192.168.1.1
...
lxc.start.auto = 1
lxc.start.delay = 5
```
idem for */var/lib/lxc/webserver/config*.
```sh
...
lxc.network.type = veth
lxc.network.flags = up
lxc.network.link = br0
lxc.network.ipv4 = 192.168.1.20/24
lxc.network.ipv4.gateway = 192.168.1.1
...
lxc.start.auto = 1
lxc.start.delay = 10
```
##### Firewalling rules in the MaaS server
A good idea that the *iptables-persistent* package is installed.
We propose to add theses rules (example MaaS server IP address: 90.12.56.21@eth0 )
```sh
iptables -t nat -A POSTROUTING -o eth0 -j SNAT --to 90.12.56.21
iptables -t nat -A PREROUTING -p tcp --dport 443 -i eth0 -j DNAT --to 192.168.1.20:443
iptables -t nat -A PREROUTING -p tcp --dport 2222 -i eth0 -j DNAT --to 192.168.1.10:22
iptables -t nat -A PREROUTING -p tcp --dport 3333 -i eth0 -j DNAT --to 192.168.1.20:22
```
I allowed an access on each container with ssh (ports **2222** and **3333**) and of course a direct access to the web server
We don't forget to save this set of rules.
```sh
root@maas:~# iptables-save > /etc/iptables/rules.v4
```

##### Database Install on dbserver
All communication beetween dbserver and webserver are finished, it's time to install each server and we're going to start with dbserver. dbServer will contain data relative to the web UI (*drupal*), but data collected by probes too.

We connect on the dbserver and start by install the main packages
```sh
root@dbserver:~# apt install mariadb-server git
```
Now that *mariadb* is installed, we have to open the 3306 port to the webserver with */etc/mysql/my.cnf*
We have to replace *bind-address = 127.0.0.1* by the dbserver IP address
```sh
[mysqld]
bind-address            = 90.12.56.21
```
We don't forget to reload the daemon 
```sh
root@dbserver:~# systemctl restart mysql
```

The MaaS prepository
We 're going to clone the *MaaS* project.

```sh
spike@dbserver:~# git clone https://github.com/gandalf-the-white/foundation.git
```
Few information on the topology project.
The directory (*foundation*) is composed of 13 directories
- amaryl, demerzel, hardin, reventlov, seldon, venabili (drupal modules)
- ansible (deployement scripts)
- docker 
- documentation
- maasapi, newapi (php script for the REST Api)
- scripts (scripts included in the hypervisors to synchronise containers with the MaaS server)
- sql (SQL scripts for Drupal, the MaaS database and CDP weather)

Configuration of the databases
We have to do many operation before to leave the database server
- create a user (*spike*) in mysql
- create 3 databases (drupaldb, content_delivery_weather and hyperdb)
- recovery the databases scheme of content_delivery_weather and hyperdb

To create the user
```sh
root@dbserver:~# mysql -uroot -p
...
MariaDB [(none)]> create user spike identified by 'password';
```

To create the 3 databases.
```sh
root@dbserver:~# mysql -uroot -p
...
MariaDB [(none)]> create database drupaldb character set utf8 collate utf8_general_ci;
MariaDB [(none)]> create database content_delivery_weather;
MariaDB [(none)]> create database hyperdb;
MariaDB [(none)]> grant all privileges on drupaldb.* to 'spike'@'%' identified by 'password';
MariaDB [(none)]> grant all privileges on content_delivery_weather.* to 'spike'@'%' identified by 'password';
MariaDB [(none)]> grant all privileges on hyperdb.* to 'spike'@'%' identified by 'password';
```

To recovery the database scheme of content_delivery_weather and hyperdb
We go in the *foudation/scripts* directory and
```sh
root@dbserver:~# cd foundation/sql/
```
and we start the recorvery of content_delivery_weather firstly
```sh
root@dbserver:~/foundation/sql# mysql -uspike -D content_delivery_weather -p < content_delivery_weather.sql
```

and finally hyperdb
```sh
root@dbserver:~/foundation/sql# mysql -uspike -D hyperdb -p < maas.sql
```

We reload mysql (*systemctl restart mysql*) and we leave *dbserver*.
##### Apache, php and Drupal Install on webserver
Firstly, we'll add in */etc/hosts* we add the *IP address* of **dbserver**.

Common packages to install before the real job
```sh
root@webserver:~# apt install curl git vim
```

###### APACHE
```sh
root@webserver:~# apt install apache2
```
We have to enable the rewrite module and restart the daemon.
```sh
root@webserver:~# a2enmod rewrite
root@webserver:~# systemctl restart apache2
```
###### PHP
```sh
root@webserver:~# apt install php php-xml php-mysql php-gd php-curl php-apcu php-mbstring
```

###### DRUSH
Simple like **drush**

Of course we'll use drush for all install (Drupal, Plugin etc...)
Let's started by drush
```sh
root@webserver:~# php -r "readfile('https://s3.amazonaws.com/files.drush.org/drush.phar');" > drush
root@webserver:~# chmod +x drush
root@webserver:~# mv drush /usr/local/bin
```
We go in the */var/www/html* directory and run 
```sh
root@webserver /var/www/html # drush dl drupal-7 --drupal-project-rename=cdpweather
```
We finalise the install in the right directory */var/www/html/cdpweather*
```sh
root@webserver:~# /var/www/html/cdpweather # drush site-install standard --site-name="shorty's bar" --account-name=spike --account-pass="newpassword" --db-url=mysql://spike:password@dbserver/drupaldb
```
###### SSL
```sh
root@webserver:~# /etc/ssl/private # openssl req -new -x509 -days 365 -nodes -out /etc/ssl/certs/server.crt -keyout /etc/ssl/private/server.key
```
```sh
root@webserver:~# /etc/ssl/private # a2enmod ssl
root@webserver:~# /etc/ssl/private # systemctl restart apache2
```
We write in the file */etc/apache2/sites-enabled/cdpweather.cdpweather*
```sh
<IfModule mod_ssl.c>
	<VirtualHost *:443>
	ServerName cdpweather
	DocumentRoot /var/www/html/
	SSLEngine on
	SSLCertificateFile /etc/ssl/certs/server.crt
	SSLCertificateKeyFile /etc/ssl/private/server.key
		<Directory "/var/www/html/">
			Options FollowSymLinks
			AllowOverride All
			Require all granted
		</Directory>
	</VirtualHost>
</IfModule>
```

We disable the default site and enable our new ssl website

```sh
root@webserver:~# /etc/apache2/sites-enabled # a2dissite 000-default.conf
root@webserver:~# /etc/apache2/sites-enabled # a2ensite cdpweather.conf
```
##### Modules in Drupal
We are in front of a simple Drupal install
It's time to add all of our new modiles
We go in the */var/www/html/cdpweather/sites/all/modules/* directory to clone our project and dependencies
```sh
root@webserver:~# /var/www/html/cdpweather/sites/all/modules # git clone https://github.com/gandalf-the-white/foundation.git
root@webserver:~# /var/www/html/cdpweather/sites/all/modules # curl https://ftp.drupal.org/files/projects/libraries-7.x-2.3.tar.gz | tar xvzf -
root@webserver:~# /var/www/html/cdpweather/sites/all/modules # curl https://ftp.drupal.org/files/projects/date-7.x-2.9.tar.gz | tar xvzf -
root@webserver:~# /var/www/html/cdpweather/sites/all/modules # curl https://ftp.drupal.org/files/projects/css_editor-7.x-1.0.tar.gz | tar xvzf -
```
Now, we goin the */var/www/html/cdpweather/sites/all/libraries* directory to dowload all dependencies
```sh
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # wget http://code.highcharts.com/zips/Highcharts-5.0.6.zip
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # unzip Highcharts-5.0.6.zip -d Highcharts
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # wget http://code.highcharts.com/zips/Highmaps-5.0.6.zip
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # unzip Highmaps-5.0.6.zip -d Highmaps
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # wget https://datatables.net/releases/DataTables-1.10.13.zip
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # unzip DataTables-1.10.13.zip
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # mv DataTables-1.10.13 DataTables
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # wget https://jqueryui.com/resources/download/jquery-ui-1.10.4.zip
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # unzip jquery-ui-1.10.4.zip
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # wget https://jqueryui.com/resources/download/jquery-ui-themes-1.10.4.zip
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # unzip jquery-ui-themes-1.10.4.zip
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # wget https://github.com/trentrichardson/jQuery-Timepicker-Addon/archive/master.zip
root@webserver:~# /var/www/html/cdpweather/sites/all/libraries # unzip master.zip
```

##### Theme in Drupal
We download this theme **corporateclean** in the right directory (*/var/www/html/cdpweather/sites/all/themes*)
```sh
root@webserver:~# /var/www/html/cdpweather/sites/all/themes # curl https://ftp.drupal.org/files/projects/corporateclean-7.x-2.3.tar.gz | tar xvzf -
```

And finally, to connect all databases to drupal, we'll edit */var/www/html/cdpweather/sites/default/settings.php*
```sh
$databases = array (
	'default' =>
	array (
		'default' =>
		array (
			'database' => 'drupaldb',
			'username' => 'spike',
			'password' => 'valentine',
			'host' => 'password',
			'port' => '',
			'driver' => 'mysql',
			'prefix' => '',
		),
	),
	'cdpweather' =>
	array (
		'default' =>
		array (
			'database' => 'content_delivery_weather',
			'username' => 'spike',
			'password' => 'password',
			'host' => 'localhost',
			'port' => '',
			'driver' => 'mysql',
			'prefix' => '',
		),
	),
	'lxc' =>
	array (
		'default' =>
		array (
			'database' => 'hyperdb',
			'username' => 'spike',
			'password' => 'password',
			'host' => 'localhost',
			'port' => '',
			'driver' => 'mysql',
			'prefix' => '',
		),
	),
);
```

Et voila
