-- phpMyAdmin SQL Dump
-- version 4.6.6deb5
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Mar 05, 2020 at 12:26 PM
-- Server version: 10.3.22-MariaDB-0+deb10u1
-- PHP Version: 7.3.14-1~deb10u1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `Project`
--

DELIMITER $$
--
-- Procedures
--
CREATE DEFINER=`root`@`localhost` PROCEDURE `pump` ()  NO SQL
BEGIN

SET @var101 := (SELECT AVG(`Sensor Value`) FROM (SELECT `Sensor Value` FROM EventTable WHERE `Sensor ID` = 1001 OR `Sensor ID` = 1002 order by `Event ID` desc limit 6) as s);
SET @var102 := (SELECT AVG(`Sensor Value`) FROM (SELECT `Sensor Value` FROM EventTable WHERE `Sensor ID` = 1003 OR `Sensor ID` = 1004 order by `Event ID` desc limit 6) as s);
SET @var102 := (SELECT AVG(`Sensor Value`) FROM (SELECT `Sensor Value` FROM EventTable WHERE `Sensor ID` = 1005 OR `Sensor ID` = 1006 order by `Event ID` desc limit 6) as s);

SET @var200 := (SELECT AVG(`Sensor Value`) FROM EventTable Where `Sensor ID` > 2000 AND `Sensor ID` < 2010 order by `Event ID` desc limit 30);

SET @var300 := (SELECT AVG(`Sensor Value`) FROM EventTable Where `Sensor ID` > 3000 AND `Sensor ID` < 3010 order by `Event ID` desc limit 30);

SET @var700 := (SELECT `Sensor Value` FROM EventTable Where `Sensor ID` = 4001 order by `Event ID` desc limit 1);

IF (@var101 > 800) THEN
    UPDATE ActionTable SET `Action Value` = 0 WHERE `Action ID` = 6001;
ELSEIF (@var101 <= 400) THEN
    UPDATE ActionTable SET `Action Value` = 1 WHERE `Action ID` = 6001;
ELSE 
	SET @var000 := (SELECT `User ID` FROM UserTable WHERE 1 ORDER BY `User ID` ASC LIMIT 1);
END IF;

IF (@var102 > 800) THEN
    UPDATE ActionTable SET `Action Value` = 0 WHERE `Action ID` = 6002;
ELSEIF (@var102 <= 400) THEN
    UPDATE ActionTable SET `Action Value` = 1 WHERE `Action ID` = 6002;
ELSE 
	SET @var000 := (SELECT `User ID` FROM UserTable WHERE 1 ORDER BY `User ID` ASC LIMIT 1);
END IF;

IF (@var103 > 800) THEN
    UPDATE ActionTable SET `Action Value` = 0 WHERE `Action ID` = 6003;
ELSEIF (@var103 <= 400) THEN
    UPDATE ActionTable SET `Action Value` = 1 WHERE `Action ID` = 6003;
ELSE 
	SET @var000 := (SELECT `User ID` FROM UserTable WHERE 1 ORDER BY `User ID` ASC LIMIT 1);
END IF;

IF (@var700 > 200) THEN
	UPDATE ActionTable SET `Action Value` = 0 WHERE `Action ID` = 7001;
ELSEIF (@var700 < 200) THEN
	UPDATE ActionTable SET `Action Value` = 1 WHERE `Action ID` = 7001;
ELSE 
	SET @var000 := (SELECT `User ID` FROM UserTable WHERE 1 ORDER BY `User ID` ASC LIMIT 1);
END IF;

END$$

DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `ActionTable`
--

CREATE TABLE `ActionTable` (
  `Action ID` int(11) NOT NULL,
  `Action Event` varchar(20) NOT NULL,
  `Action Value` int(11) NOT NULL,
  `Action Time` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `ActionTable`
--

INSERT INTO `ActionTable` (`Action ID`, `Action Event`, `Action Value`, `Action Time`) VALUES
(6001, 'pump1', 0, '2020-02-25 06:06:59'),
(6002, 'pump2', 0, '2020-02-25 06:06:59'),
(6003, 'pump3', 0, '2020-02-25 06:06:59'),
(7001, 'HCSR04Security1', 0, '2020-03-03 11:37:12'),
(8001, 'CameraSecurity', 0, '2020-03-03 11:37:12');

-- --------------------------------------------------------

--
-- Table structure for table `EventTable`
--

CREATE TABLE `EventTable` (
  `Event ID` int(11) NOT NULL,
  `Sensor ID` int(11) NOT NULL,
  `Sensor Value` float NOT NULL,
  `EventTime` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Triggers `EventTable`
--
DELIMITER $$
CREATE TRIGGER `trig1` BEFORE INSERT ON `EventTable` FOR EACH ROW BEGIN
call pump();
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `SensorTable`
--

CREATE TABLE `SensorTable` (
  `Sensor ID` int(11) NOT NULL,
  `Sensor Name` varchar(25) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `SensorTable`
--

INSERT INTO `SensorTable` (`Sensor ID`, `Sensor Name`) VALUES
(1001, 'Soil Moisture Sensor 1'),
(1002, 'Soil Moisture Sensor 2'),
(1003, 'Soil Moisture Sensor 3'),
(1004, 'Soil Moisture Sensor 4'),
(1005, 'Soil Moisture Sensor 5'),
(1006, 'Soil Moisture Sensor 6'),
(2001, 'Light Sensor 1'),
(2002, 'Light Sensor 2'),
(2003, 'Light Sensor 3'),
(2004, 'Light Sensor 4'),
(3001, 'Temperature Sensor 1'),
(3002, 'Temperature Sensor 2'),
(3003, 'Temperature Sensor 3'),
(6001, 'Pump 1'),
(6001, 'Pump 2'),
(6001, 'Pump 3'),
(7001, 'Ultrasonic Sensor 1'),
(8001, 'Camera 1');

-- --------------------------------------------------------

--
-- Table structure for table `UserTable`
--

CREATE TABLE `UserTable` (
  `User ID` int(11) NOT NULL,
  `User Name` text NOT NULL,
  `User Password` varchar(10) NOT NULL,
  `User Email` varchar(100) NOT NULL,
  `User Phone` bigint(12) NOT NULL,
  `Last Access` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `UserTable`
--

INSERT INTO `UserTable` (`User ID`, `User Name`, `User Password`, `User Email`, `User Phone`, `Last Access`) VALUES
(1, 'alpha', 'alpha', 'support@prodot.in',+917975317160, '2020-02-26 05:42:20');

-- --------------------------------------------------------

--
-- Stand-in structure for view `view5`
-- (See below for the actual view)
--
CREATE TABLE `view5` (
`Event ID` int(11)
,`Sensor ID` int(11)
,`Sensor Value` float
,`EventTime` timestamp
);

-- --------------------------------------------------------

--
-- Stand-in structure for view `view6`
-- (See below for the actual view)
--
CREATE TABLE `view6` (
`Event ID` int(11)
,`Sensor ID` int(11)
,`Sensor Value` float
,`EventTime` timestamp
);

-- --------------------------------------------------------

--
-- Structure for view `view5`
--
DROP TABLE IF EXISTS `view5`;

CREATE ALGORITHM=UNDEFINED DEFINER=`username`@`localhost` SQL SECURITY DEFINER VIEW `view5`  AS  (select `EventTable`.`Event ID` AS `Event ID`,`EventTable`.`Sensor ID` AS `Sensor ID`,`EventTable`.`Sensor Value` AS `Sensor Value`,`EventTable`.`EventTime` AS `EventTime` from `EventTable` where `EventTable`.`Sensor ID` > 1000 and `EventTable`.`Sensor ID` < 4000 order by `EventTable`.`Event ID` desc limit 13) ;

-- --------------------------------------------------------

--
-- Structure for view `view6`
--
DROP TABLE IF EXISTS `view6`;

CREATE ALGORITHM=UNDEFINED DEFINER=`username`@`localhost` SQL SECURITY DEFINER VIEW `view6`  AS  (select `EventTable`.`Event ID` AS `Event ID`,`EventTable`.`Sensor ID` AS `Sensor ID`,`EventTable`.`Sensor Value` AS `Sensor Value`,`EventTable`.`EventTime` AS `EventTime` from `EventTable` where `EventTable`.`Sensor ID` > 4000 and `EventTable`.`Sensor ID` < 5000 order by `EventTable`.`Event ID` desc limit 4) ;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `EventTable`
--
ALTER TABLE `EventTable`
  ADD PRIMARY KEY (`Event ID`);

--
-- Indexes for table `UserTable`
--
ALTER TABLE `UserTable`
  ADD PRIMARY KEY (`User ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `EventTable`
--
ALTER TABLE `EventTable`
  MODIFY `Event ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1346;
--
-- AUTO_INCREMENT for table `UserTable`
--
ALTER TABLE `UserTable`
  MODIFY `User ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
