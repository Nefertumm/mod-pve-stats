-- Put only sql structure in this file (create table if exists, delete table, alter table etc...).
-- If you don't use this database, then delete this file.

DROP TABLE IF EXISTS `pve_stats`;
CREATE TABLE IF NOT EXISTS `pve_stats` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `GuildID` INT(10) UNSIGNED NOT NULL COMMENT 'Guild ID',
  `InstanceID` SMALLINT(5) UNSIGNED NOT NULL COMMENT 'MapID',
  `EncounterEntry` INT(10) UNSIGNED NOT NULL COMMENT 'EncounterEntry (from instance_encounters)',
  `EncounterName` CHAR(100) NOT NULL COMMENT 'EncounterName (from instance_encounters)',
  `Difficulty` TINYINT(3) NOT NULL COMMENT 'Difficulty',
  `KillDate` INT(10) UNSIGNED NOT NULL COMMENT 'Date',
  `RaidComposition` CHAR(10) NOT NULL COMMENT 'Raid composition (T H DPS)',
  `TotalAlive` SMALLINT(3) NOT NULL COMMENT 'Alive players',
  `TotalGuildPlayers` SMALLINT(3) NOT NULL COMMENT 'Players of the guild that participated on the encounter.',
  `TotalPlayers` SMALLINT(3) NOT NULL COMMENT 'Total players on the encounter',
  -- `TimeEmployed` INT(10) UNSIGNED NOT NULL COMMENT 'Total time of the encounter', -- not yet
  PRIMARY KEY (`id`)
) ENGINE=INNODB DEFAULT CHARSET=utf8 COMMENT='pve_stats';
