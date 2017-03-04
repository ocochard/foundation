DROP TABLE IF EXISTS `hypervisor`;

CREATE TABLE IF NOT EXISTS `hypervisor` (
   `hyperid` int(11) unsigned NOT NULL AUTO_INCREMENT,
   `hypername` varchar(255) NOT NULL,
   `cpu` int(11) NOT NULL,
   `architecture` varchar(255) NOT NULL,
   `typevirt` varchar(255) NOT NULL,
   `bridge` varchar(255) NOT NULL,
   `totalHD`  int(11) NOT NULL,
   `useHD`  int(11) NOT NULL,
   `totalRAM`  int(11) NOT NULL,
   `useRAM` int(11) NOT NULL,
   `timestamp` int(11) NOT NULL,
   PRIMARY KEY (`hyperid`),
   KEY `hypername` (`hypername`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

DROP TABLE IF EXISTS `hypervisorusage`;

CREATE TABLE IF NOT EXISTS `hypervisorusage` (
   `time` int(11) unsigned NOT NULL,
   `hyperid` int(11) unsigned NOT NULL,
   `instances`  int(11) NOT NULL,
   `totalHD`  int(11) NOT NULL,
   `useHD`  int(11) NOT NULL,
   `totalRAM`  int(11) NOT NULL,
   `useRAM` int(11) NOT NULL,
   KEY `time` (`time`),
   KEY `hyperid` (`hyperid`)
   ) ENGINE=MyISAM DEFAULT CHARSET=latin1 ;

DROP TABLE IF EXISTS `container`;

CREATE TABLE IF NOT EXISTS `container` (
   `containerid` int(11) unsigned NOT NULL AUTO_INCREMENT,
   `containername` varchar(255) NOT NULL,
   `hyperid` int(11) unsigned NOT NULL,
   `imageid` int(11) unsigned NOT NULL,
   `cpu` int(11) NOT NULL,
   `rammemory` int(11) NOT NULL,
   `hdmemory` int(11) NOT NULL,
   `dns` varchar(255) NOT NULL,
   `bridge` varchar(255) NOT NULL,
   `extip` varchar(255) NOT NULL,
   `intip` varchar(255) NOT NULL,
   `status` tinyint(1) NOT NULL, 
   `period` varchar(255) NOT NULL,
   `startprocess` varchar(255) NOT NULL,
   `endprocess` varchar(255) NOT NULL,
   `crontab` varchar(255) NOT NULL,
   PRIMARY KEY (`containerid`),
   KEY `hyperid` (`hyperid`),
   KEY `imageid` (`imageid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ;

DROP TABLE IF EXISTS `containerusage`;

CREATE TABLE IF NOT EXISTS `containerusage` (
   `time` int(11) unsigned NOT NULL,
   `containerid` int(11) unsigned NOT NULL,
   `cputime`   int(11) unsigned NOT NULL,
   `memory` int(11) unsigned NOT NULL,
   `bloc`   int(11) unsigned NOT NULL,
   `tx`  int(11) unsigned NOT NULL,
   `rx`  int(11) unsigned NOT NULL,
   KEY `time` (`time`),
   KEY `containerid` (`containerid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ;


DROP TABLE IF EXISTS `image`;

CREATE TABLE IF NOT EXISTS `image` (
   `imageid` int(11) unsigned NOT NULL AUTO_INCREMENT,
   `imagename` varchar(255) NOT NULL,
   `path` varchar(255) NOT NULL,
   `vtype` varchar(255) NOT NULL,
   `information` varchar(255) NOT NULL,
   `architecture` varchar(255) NOT NULL,
   `command` varchar(255) NOT NULL,
   `status` tinyint(1) NOT NULL, 
   PRIMARY KEY (`imageid`),
   KEY `imagename` (`imagename`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

DROP TABLE IF EXISTS `profile`;

CREATE TABLE IF NOT EXISTS `profile` (
   `profileid` int(11) unsigned NOT NULL AUTO_INCREMENT,
   `profilename` varchar(255) NOT NULL,
   `information` varchar(255) NOT NULL,
   `content` varchar(255) NOT NULL,
   PRIMARY KEY (`profileid`),
   KEY `profilename` (`profilename`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;
