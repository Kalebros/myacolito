-- CREAR SCHEMA

CREATE SCHEMA `%1` ;

-- END

-- Crear la tabla de categorias

CREATE  TABLE IF NOT EXISTS `%1`.`Categoria` (
  `nombre` VARCHAR(100) NOT NULL ,
  `precio` FLOAT NOT NULL DEFAULT 0 ,
  `maximo` INT(11) NOT NULL DEFAULT 0 ,
  `fechaInicio` DATE NOT NULL ,
  `fechaLimite` DATE NOT NULL ,
  PRIMARY KEY (`nombre`)
  )
ENGINE = InnoDB;

-- END

-- Introducir datos basicos de categoria nula

INSERT INTO `%1`.`Categoria`(`nombre`,`precio`,`maximo`,`fechaInicio`,`fechaLimite`)
VALUES('SIN_CATEGORIA',0,0,CURRENT_DATE(),CURRENT_DATE());

-- END

-- INICIO ALOJAMIENTO: Crear la tabla de grupos

CREATE  TABLE IF NOT EXISTS `%1`.`Grupo` (
  `idGrupo` INT NOT NULL AUTO_INCREMENT ,
  `nombre` TEXT NOT NULL ,
  PRIMARY KEY (`idGrupo`)
  )
ENGINE = InnoDB;

-- END

-- Crear la tabla de tipos de habitacion

CREATE  TABLE IF NOT EXISTS `%1`.`TipoHab` (
  `nombreTipo` VARCHAR(100) NOT NULL ,
  `plazas` SMALLINT NULL ,
  `categoriaPermitida` VARCHAR(100) NOT NULL,
  PRIMARY KEY (`nombreTipo` ),
  CONSTRAINT `fk_TipoHab_Categoria`
    FOREIGN KEY (`categoriaPermitida` )
    REFERENCES `%1`.`Categoria` (`nombre` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
  ENGINE = InnoDB;

-- END

-- Datos por defecto en la tabla de tipos

INSERT INTO `%1`.`TipoHab`(`nombreTipo`,`plazas`,`categoriaPermitida`)
VALUES('SIN_TIPO',-1,'SIN_CATEGORIA');

-- END

-- Crear la tabla de habitaciones

CREATE  TABLE IF NOT EXISTS `%1`.`Habitacion` (
  `codHab` VARCHAR(100) NOT NULL ,
  `tipoHab` VARCHAR(100) NOT NULL ,
  `comentario` TEXT NOT NULL ,
  `llaveEntregada` TINYINT(1) NOT NULL DEFAULT 0 ,
  `estaReservada` TINYINT(1) NOT NULL DEFAULT 0 ,
  `conceptoReserva` VARCHAR(100) NOT NULL DEFAULT 'Sin concepto' ,
  PRIMARY KEY (`codHab`) ,
  INDEX `fk_Habitacion_TipoHab_INDEX` (`tipoHab`(40) ASC) ,
  CONSTRAINT `fk_Habitacion_TipoHab`
    FOREIGN KEY (`tipoHab` )
    REFERENCES `%1`.`TipoHab` (`nombreTipo` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;

-- END

-- Alojamiento por defecto

INSERT INTO `%1`.`Habitacion`(`codHab`,`tipoHab`,`comentario`)
VALUES('Sin asignar','SIN_TIPO','Alojamiento por defecto');

-- END

-- Crear la tabla de alojamientos

CREATE  TABLE IF NOT EXISTS `%1`.`alojado_en` (
  `idGrupo` INT NOT NULL ,
  `codHab` VARCHAR(100) NOT NULL DEFAULT 'Sin asignar' ,
  INDEX `fk_alojado_en_Habitacion1` (`codHab` ASC) ,
  PRIMARY KEY (`idGrupo`, `codHab`) ,
  CONSTRAINT `fk_alojado_en_Habitacion1`
    FOREIGN KEY (`codHab` )
    REFERENCES `%1`.`Habitacion` (`codHab` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_alojado_en_Habitacion2`
    FOREIGN KEY(`idGrupo`)
    REFERENCES `%1`.`Grupo`(`idGrupo`)
    ON UPDATE CASCADE)
ENGINE = InnoDB;

-- END

-- INICIO ASISTENTES: Crear la tabla de asistentes

CREATE  TABLE IF NOT EXISTS `%1`.`Asistente` (
  `idAsistente` INT NOT NULL AUTO_INCREMENT ,
  `nombre` TEXT NOT NULL ,
  `dni` TEXT NOT NULL ,
  `mail` TEXT NOT NULL ,
  `telefono` TEXT NOT NULL ,
  `comentarios` TEXT NOT NULL ,
  `IDgrupo` INT NOT NULL DEFAULT -1 ,
  PRIMARY KEY (`idAsistente`) ,
  UNIQUE INDEX `dni_UNIQUE` (`dni`(30) ASC) ,
  INDEX `fk_Asistente_Grupo1` (`IDgrupo` ASC) ,
  CONSTRAINT `fk_Asistente_Grupo1`
    FOREIGN KEY (`IDgrupo` )
    REFERENCES `%1`.`Grupo` (`idGrupo` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE
    )
ENGINE = InnoDB;

-- END

-- Crear la tabla de plazas

CREATE  TABLE IF NOT EXISTS `%1`.`Plaza` (
  `idPlaza` INT NOT NULL AUTO_INCREMENT ,
  `tipo` VARCHAR(100) NOT NULL ,
  PRIMARY KEY (`idPlaza`) ,
  INDEX `fk_Plaza_Categoria1` (`tipo` ASC) ,
  CONSTRAINT `fk_Plaza_Categoria1`
    FOREIGN KEY (`tipo` )
    REFERENCES `%1`.`Categoria` (`nombre` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE
    )
ENGINE = InnoDB;

-- END

-- Crear la tabla de reservas

CREATE  TABLE IF NOT EXISTS `%1`.`reservas` (
  `idAsistente` INT NOT NULL ,
  `idPlaza` INT NOT NULL ,
  `haPagado` TINYINT(1) NOT NULL DEFAULT false ,
  `estaActiva` TINYINT(1) NOT NULL DEFAULT false ,
  `haExpirado` TINYINT(1) NOT NULL DEFAULT false ,
  PRIMARY KEY (`idAsistente`, `idPlaza`) ,
  UNIQUE INDEX `idAsistente_UNIQUE` (`idAsistente` ASC) ,
  UNIQUE INDEX `idPlaza_UNIQUE` (`idPlaza` ASC) )
ENGINE = InnoDB;

-- END

-- INICIO ACTIVIDADES: Crear la tabla de localizaciones

CREATE  TABLE IF NOT EXISTS `%1`.`localizaciones` (
  `lugar` VARCHAR(100) NOT NULL ,
  PRIMARY KEY (`lugar`) )
ENGINE = InnoDB;

-- END

-- Crear tabla de fechas permitidas

CREATE  TABLE IF NOT EXISTS `%1`.`fechasPermitidas` (
  `fechaEvento` DATE NOT NULL ,
  PRIMARY KEY (`fechaEvento`) )
ENGINE = InnoDB;

-- END

-- Crear tabla de tipos de actividades

CREATE  TABLE IF NOT EXISTS `%1`.`tiposActividad` (
  `tipo` VARCHAR(100) NOT NULL ,
  PRIMARY KEY (`tipo`) )
ENGINE = InnoDB;

-- END

-- Valores por defecto dentro de localizaciones

INSERT INTO `%1`.`localizaciones`(lugar) VALUES('Sin asignar');

-- END

-- Crear tabla de actividades

CREATE  TABLE IF NOT EXISTS `%1`.`Actividad` (
  `idActividad` INT NOT NULL AUTO_INCREMENT ,
  `nombre` VARCHAR(100) NOT NULL ,
  `asociacion` VARCHAR(45) NULL ,
  `responsable` VARCHAR(100) NULL ,
  `correo` VARCHAR(100) NULL ,
  `coorganizadores` TEXT NULL ,
  `materialPropio` TEXT NULL ,
  `materialOrganizacion` TEXT NULL ,
  `necesidadesEspacio` TEXT NULL ,
  `dia` DATE NULL ,
  `todaJornada` TINYINT(1) NULL DEFAULT false ,
  `horaInicio` TIME NULL ,
  `horaFin` TIME NULL ,
  `minimoParticipantes` INT NULL ,
  `maximoParticipantes` INT NULL ,
  `tActividad` VARCHAR(100) NULL ,
  `descripcion` TEXT NULL ,
  `requisitos` TEXT NULL ,
  `observaciones` TEXT NULL ,
  `lugar` VARCHAR(100) NOT NULL DEFAULT 'Sin asignar' ,
  PRIMARY KEY (`idActividad`) ,
  INDEX `fk_tiposActividad_index` (`tActividad`(50) ASC) ,
  INDEX `fk_fechasPermitidas_index` (`dia` ASC) ,
  INDEX `fk_localizaciones_index` (`lugar`(50) ASC) ,
  CONSTRAINT `fk_tiposActividad`
    FOREIGN KEY (`tActividad` )
    REFERENCES `%1`.`tiposActividad` (`tipo` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_fechasPermitidas`
    FOREIGN KEY (`dia` )
    REFERENCES `%1`.`fechasPermitidas` (`fechaEvento` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_localizaciones`
    FOREIGN KEY (`lugar` )
    REFERENCES `%1`.`localizaciones` (`lugar` )
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = InnoDB;

-- END

-- Tabla de identificadores de correo de gmail

CREATE  TABLE IF NOT EXISTS `%1`.`gmail_UID` (
  `UID` INT NOT NULL ,
  `idActividad` INT NULL ,
  PRIMARY KEY (`UID`) ,
  INDEX `fk_identificadorActividad` (`idActividad` ASC) ,
  CONSTRAINT `fk_identificadorActividad`
    FOREIGN KEY (`idActividad` )
    REFERENCES `%1`.`Actividad` (`idActividad` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;

-- END
