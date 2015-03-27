-- Crear el esquema base

CREATE SCHEMA IF NOT EXISTS `Eventos` DEFAULT CHARACTER SET latin1 COLLATE latin1_spanish_ci;

-- END

-- Crear la tabla del listado

CREATE  TABLE IF NOT EXISTS `Eventos`.`Listado` (
  `idEvento` INT NOT NULL AUTO_INCREMENT ,
  `nombre` TEXT NULL ,
  `comentario` TEXT NULL ,
  `nombreDatabase` TEXT NULL ,
  PRIMARY KEY (`idEvento`) ,
  UNIQUE INDEX `nombre_UNIQUE` (`nombre`(100) ASC),
  UNIQUE INDEX `nombreD_UNIQUE` (`nombreDatabase`(100) ASC) )
ENGINE = InnoDB;

-- END

-- Crear la tabla de fechas

CREATE  TABLE IF NOT EXISTS `Eventos`.`Fechas` (
  `idEvento` INT NOT NULL ,
  `fechaEvento` DATE NOT NULL ,
  INDEX `fk_Fechas_Listado` (`idEvento` ASC) ,
  CONSTRAINT `fk_Fechas_Listado`
    FOREIGN KEY (`idEvento` )
    REFERENCES `Eventos`.`Listado` (`idEvento` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

-- END
