DROP TABLE IF EXISTS `new_v4_container`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `new_v4_container` (
  `containerid` varchar(255) NOT NULL,
  `hyperid` int(11) unsigned NOT NULL,
  `imageid` int(11) unsigned NOT NULL,
  `dockerid` varchar(255) NOT NULL,
  PRIMARY KEY (`containerid`),
  KEY `hyperid` (`hyperid`),
  KEY `imageid` (`imageid`),
  KEY `dockerid` (`dockerid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `new_v4_fqdnuser`
--

DROP TABLE IF EXISTS `new_v4_fqdnuser`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `new_v4_fqdnuser` (
  `urlid` int(11) unsigned NOT NULL,
  `probeid` int(11) unsigned NOT NULL,
  `status` tinyint(1) NOT NULL,
  UNIQUE KEY `name_index` (`urlid`,`probeid`),
  KEY `urlid` (`urlid`),
  KEY `probeid` (`probeid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `new_v4_sonde`
--

DROP TABLE IF EXISTS `new_v4_sonde`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `new_v4_sonde` (
  `probeid` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `probename` varchar(255) NOT NULL,
  `ip` varchar(255) NOT NULL,
  `ext` varchar(255) NOT NULL,
  `dns` varchar(255) NOT NULL,
  `newdns` varchar(255) DEFAULT NULL,
  `uptime` int(11) unsigned NOT NULL,
  `firsttime` int(11) unsigned DEFAULT NULL,
  `lasttime` int(11) unsigned NOT NULL,
  `mac` varchar(255) NOT NULL,
  `cc` varchar(255) NOT NULL,
  `country` varchar(255) NOT NULL,
  `version` varchar(255) NOT NULL,
  `status` tinyint(1) NOT NULL,
  PRIMARY KEY (`probeid`),
  KEY `probename` (`probename`)
) ENGINE=MyISAM AUTO_INCREMENT=59 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `new_v4_times`
--

DROP TABLE IF EXISTS `new_v4_times`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `new_v4_times` (
  `time` int(11) unsigned NOT NULL,
  `probeid` int(11) unsigned NOT NULL,
  `urlid` int(11) unsigned NOT NULL,
  `contentname` varchar(255) NOT NULL,
  `contenttype` varchar(255) NOT NULL,
  `contentaddress` char(15) NOT NULL,
  `firstbyte` tinyint(1) unsigned NOT NULL,
  `prefix` varchar(255) NOT NULL,
  `asnum` varchar(255) NOT NULL,
  `asname` varchar(255) NOT NULL,
  `cc` char(2) NOT NULL,
  `country` varchar(255) NOT NULL,
  `duration` int(11) unsigned NOT NULL,
  `octet` int(11) unsigned NOT NULL,
  `starttime` int(11) unsigned NOT NULL,
  `wait` int(11) unsigned NOT NULL,
  `receive` int(11) unsigned NOT NULL,
  `har` mediumtext,
  KEY `time` (`time`),
  KEY `probeid` (`probeid`),
  KEY `urlid` (`urlid`),
  KEY `urlprobeid` (`urlid`,`probeid`),
  KEY `contentaddress` (`contentaddress`),
  KEY `firstbyte` (`firstbyte`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;



DROP TABLE IF EXISTS `new_v4_url`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `new_v4_url` (
  `urlid` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `urlname` varchar(255) NOT NULL,
  PRIMARY KEY (`urlid`),
  KEY `urlname` (`urlname`)
) ENGINE=MyISAM AUTO_INCREMENT=78 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `new_v4_users_probes`
--

DROP TABLE IF EXISTS `new_v4_users_probes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `new_v4_users_probes` (
  `userid` int(10) unsigned NOT NULL,
  `probeid` int(10) unsigned NOT NULL,
  KEY `userid` (`userid`,`probeid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `new_v4_users_probes_edit`
--

DROP TABLE IF EXISTS `new_v4_users_probes_edit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `new_v4_users_probes_edit` (
  `userid` int(10) unsigned NOT NULL,
  `probeid` int(10) unsigned NOT NULL,
  `phone` varchar(255) DEFAULT NULL,
  `email` varchar(255) DEFAULT NULL,
  `ref` tinyint(1) DEFAULT NULL,
  KEY `userid` (`userid`,`probeid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
