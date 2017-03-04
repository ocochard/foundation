-- --------------------------------------------------------

--
-- Table structure for table `new_v4_hyper`
--

DROP TABLE IF EXISTS `new_v4_hyper`;
CREATE TABLE IF NOT EXISTS `new_v4_hyper` (
  `hyperid` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `hypername` varchar(255) NOT NULL,
  PRIMARY KEY (`hyperid`),
  KEY `hypername` (`hypername`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `new_v4_hyperimage`
--


DROP TABLE IF EXISTS `new_v4_hyperimage`;
CREATE TABLE IF NOT EXISTS `new_v4_hyperimage` (
   `hyperid` int(11) unsigned NOT NULL,
   `imageid` int(11) unsigned NOT NULL,
   KEY `hyperid` (`hyperid`),
   KEY `imageid` (`imageid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 ;

-- --------------------------------------------------------

--
-- Table structure for table `new_v4_container`
--

DROP TABLE IF EXISTS `new_v4_container`;
CREATE TABLE IF NOT EXISTS `new_v4_container` (
  `containerid` varchar(255) NOT NULL,
  `hyperid` int(11) unsigned NOT NULL,
  `imageid` int(11) unsigned NOT NULL,
  `dockerid` varchar(255) NOT NULL,
  PRIMARY KEY (`containerid`),
  KEY `hyperid` (`hyperid`),
  KEY `imageid` (`imageid`),
  KEY `dockerid` (`dockerid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 ;

-- --------------------------------------------------------

--
-- Table structure for table `new_v4_image`
--

DROP TABLE IF EXISTS `new_v4_image`;
CREATE TABLE IF NOT EXISTS `new_v4_image` (
   `imageid` int(11) unsigned NOT NULL,
   `imagename` varchar(255) NOT NULL,
   PRIMARY KEY (`imageid`),
   KEY `imagename` (`imagename`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;   

-- --------------------------------------------------------
