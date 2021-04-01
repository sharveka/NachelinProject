DROP TABLE `user`;
CREATE TABLE `user` (
  `oid` int NOT NULL AUTO_INCREMENT,
  `IDUser` varchar(45) NOT NULL,
  `UserName` varchar(45) DEFAULT NULL,
  `PhoneNum` varchar(45) DEFAULT NULL,
  `Public` enum('Public','Protected','Private') DEFAULT 'Public',
  PRIMARY KEY (`oid`),
  UNIQUE KEY `IDUser_UNIQUE` (`IDUser`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
INSERT INTO `user` (`oid`,`IDUser`,`UserName`,`PhoneNum`,`Public`) VALUES (1,'abc','xyz','010-1588-1588','Public');
INSERT INTO `user` (`oid`,`IDUser`,`UserName`,`PhoneNum`,`Public`) VALUES (2,'GourmetHunter','Andy JW Yang','010-7777-1020','Protected');
INSERT INTO `user` (`oid`,`IDUser`,`UserName`,`PhoneNum`,`Public`) VALUES (3,'JYP Nation','Mark Cho','010-1588-1588','Public');
INSERT INTO `user` (`oid`,`IDUser`,`UserName`,`PhoneNum`,`Public`) VALUES (4,'MattDaddy','Matthew Fujieki','010-7777-0405','Protected');
INSERT INTO `user` (`oid`,`IDUser`,`UserName`,`PhoneNum`,`Public`) VALUES (5,'Mr. Nam','CW Nam','010-9939-4267','Protected');
INSERT INTO `user` (`oid`,`IDUser`,`UserName`,`PhoneNum`,`Public`) VALUES (6,'Ome1737','JN Lee','81428321737','Protected');
INSERT INTO `user` (`oid`,`IDUser`,`UserName`,`PhoneNum`,`Public`) VALUES (7,'Ome2244','NJ Kim','01019480202','Protected');
INSERT INTO `user` (`oid`,`IDUser`,`UserName`,`PhoneNum`,`Public`) VALUES (8,'Scott',NULL,NULL,'Public');
INSERT INTO `user` (`oid`,`IDUser`,`UserName`,`PhoneNum`,`Public`) VALUES (9,'Sweetie777','Karen Rizner','010-7777-1121','Protected');
