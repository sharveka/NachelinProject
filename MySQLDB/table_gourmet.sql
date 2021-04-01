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
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (1,'Korean','GalbiJib','Galbi',100000,3,1,NULL,1);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (2,'Korean','Galbee King','Prime Galbee',199000,4,1,'Peral Lee',1);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (3,'Western','New York, New York','T-bone Steak',99000,1,1,'NO Pearl',1);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (4,'Western','Thompson, CT','G. Washington Steak',99000,5,1,'1st Bonus',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (5,'Japanese','Ome','Uni sushi',29000,5,1,'with mom',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (6,'Japanese','Ninja Ramen','Miso Ramen',9000,2,1,'#scam',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (7,'Korean','Bob Kim','Gimbap',5000,5,1,'#Young감',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (8,'Snack','Bob Kim','Combo Ramyun',5500,5,1,'#Young감',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (9,'Chinese','Peking 888','Peking Duck',199000,5,1,'#with mom & dad',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (10,'Others','Saigon 84','Beef Rice Noodle',9900,3,1,'#localized',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (11,'Snack','PNU Main Gate','Koraen Street Toast',4900,2,1,'#Kiwi Juice',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (12,'Korean','Suwon Wang Galbee','Galbee Special',59000,2,1,'#oh my money!!!',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (13,'Western','Granville','Stone Grilled Prime Steak',99000,5,1,'#good wine selection',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (14,'Chinese','Hong Kong 1997','Fried Rice',12000,2,1,'#Paek is better',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (15,'Snack','PNU Main Gate','Cheese hotdog',3900,2,1,'#why I am here again?',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (16,'Western','Providence','Supreme Pizza',29000,5,1,'#extra cheese',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (17,'Western','Papa Jongs','Supreme Pizza',9000,5,1,'#I hate Jongs',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (18,'Others','Silom','Pineapple fried rice with green curry',9900,1,1,'#spicy',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (19,'Japanese','Ukai tei','Sushi Omakase',300000,5,1,'#CEO Forums',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (20,'Chinese','Hong Kong 1997','Mongolian Beef',22000,1,1,'#sorry for Genghis Khan',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (21,'Snack','SNU Station','Takoyaki',4900,2,1,'#Seoul',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (22,'Korean','River Han','Bulgogi Special',29000,3,1,'#Coex',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (23,'Korean','Yongsusan','Course A',149000,3,1,'#with mother-in-law',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (24,'Western','Papa Dons','Pepperoni Pizza',19900,4,1,'#pilsner',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (25,'Others','Deli','green curry',14900,2,1,'#Indian',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (26,'Snack','Naksung Univ','Sundae',4900,1,1,'#so fake!',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (27,'Western','Dominoms','Mushroom & Cheese Pizza',19900,1,1,'0 star!',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (28,'Chinese','Hong Kong 1997','Singaporean noodle',12000,1,1,'bye bye Hong Kong',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (29,'Japanese','Yoshiyama','Sushi set',45000,5,1,'#uni, toro, amaebi',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (30,'Western','Napoli','Mozzarella en Carrozza',14900,4,1,'good wines',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (31,'Korean','Mt. Halla','Jeju pork BBQ',29900,2,1,'#not black pork, #Jeju',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (32,'Others','Sawadee','pork in bamboo leaf',9900,3,1,'#Thai',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (33,'Chinese','Xian','Hot and sour soup',9900,1,1,'too spicy',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (34,'Chinese','Xian','Xiao Long Bao',9900,3,1,'not too bad',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (35,'Western','Philly','Sandwich',9900,1,1,'go to Subway',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (36,'Others','Berlin 1880','Lager',9900,5,1,'#Craft Beer',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (37,'Others','Berlin 1880','Pale Ale',9900,4,1,'#Craft Beer',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (38,'Others','Berlin 1880','IPA',12900,1,1,'#Craft Beer',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (39,'Others','Berlin 1880','Porter',9900,5,1,'#Craft Beer',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (40,'Western','Union Bay','Lobster',99900,5,1,'my favorite!',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (41,'Others','Pastiche','Mascarpone cheese cake',7900,5,1,'#dessert cafe',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (42,'Others','Bonne Nuit','Chablis',99000,3,1,'#birthday, #wine',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (43,'Others','Dogs & Cats','Double Espresso',4900,3,1,'#pet#cafe',1);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (44,'Japanese','Yamamoto','Yogurt Curry',12900,5,1,'#mom',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (45,'Japanese','Yamamoto','Set A',14900,5,1,'#pork with ginger',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (46,'Japanese','Yamamoto','Set C',14900,5,1,'#fried tofu',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (47,'Chinese','Panda & Bamboo','Mapo Tofu',12900,4,1,'#airport',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (48,'Korean','Parallel 38','Nang myun',14900,4,1,'#try dumpling! #TDC',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (49,'Others','Xin Chao','Bun Cha',14900,3,1,'#HanoiStyle#Vietnamese',1);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (50,'Snack','Bob Kim','BK Special',7900,5,1,'#luxury gimbap!',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (51,'Korean','Parallel 38','Nang myun',14900,5,1,'soup was good',4);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (52,'Western','Union Bay','Shrimp Cocktail',19900,4,1,'nice sauce',4);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (53,'Western','Union Bay','Raw Oyster',9900,2,1,'not fresh',4);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (54,'Western','Union Bay','Clam Chowder',14900,5,1,'for Andy',9);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (55,'Chinese','Panda & Bamboo','Mapo Tofu',12900,2,1,'served cold',4);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (56,'Korean','Bob Kim','BK Special',7900,4,1,'#Best#Gimbap!',9);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (57,'Snack','Bob Kim','BK Special',7900,4,1,'#Gimbap!',4);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (58,'Korean','Parallel 38','Nang myun',14900,5,1,'really good!',9);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (59,'Korean','Parallel 38','Steamed Dumpling',9900,2,1,'nang myun next time!',1);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (60,'Western','Union Bay','Raw Oyster',9900,4,1,'best oyster in 2020',1);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (61,'Japanese','Yamamoto','Set C',14900,5,1,'#good tofu',1);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (62,'Chinese','Panda & Bamboo','Mapo Tofu',12900,4,1,'not too bad',1);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (63,'Others','Xin Chao','Bun Cha',14900,1,1,'nasty!',4);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (64,'Others','Bonne Nuit','Aloha 2012',39000,3,1,'#wine',4);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (65,'Japanese','Yamamoto','Set C',14900,5,1,'#yummy',9);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (66,'Snack','Bob Kim','BK Special',7900,5,1,'#taking Jenn next time',1);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (67,'Others','Bonne Nuit','MPS 1975',49000,5,1,'#good wine',9);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (68,'Chinese','Panda & Bamboo','Mapo Tofu',12900,1,1,'never again!',9);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (69,'Chinese','Castle 10004','Gourmet Dumpling Set',19900,1,1,'PNU North Gate',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (70,'Others','Momos','Geisha',14900,3,1,'#cafe',9);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (71,'Others','Momos','Kona',12900,4,1,'#cafe',2);
INSERT INTO `gourmet` (`gid`,`Category`,`NameRest`,`NameMenu`,`Price`,`Rate`,`Open`,`Comment`,`OID`) VALUES (72,'Western','Oyster Bay','Oyster Cocktail Special',27900,4,1,'Very fresh oysters!',9);
