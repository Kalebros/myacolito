-- Trigger para insertar nuevos asistentes

CREATE TRIGGER %1.tr_nuevosAsistentes BEFORE INSERT ON %1.Asistente
FOR EACH ROW
BEGIN
    IF NEW.IDgrupo = -1 THEN
        INSERT INTO %1.Grupo(nombre)
        VALUES (' ');

        SET NEW.IDgrupo = LAST_INSERT_ID();
    END IF;
END

-- END

-- Trigger para eliminar grupos vacios en asistentes

CREATE TRIGGER %1.tr_updateAsistentes AFTER UPDATE ON %1.Asistente
FOR EACH ROW
BEGIN

    CALL %1.LimpiarGruposVacios();

END

-- END

-- Trigger para eliminar grupos vacios en asistentes despues de borrar

CREATE TRIGGER %1.tr_deleteAsistentes AFTER DELETE ON %1.Asistente
FOR EACH ROW
BEGIN

    CALL %1.LimpiarGruposVacios();

END

-- END

-- Trigger para eliminar entradas de reservas al eliminar grupos

CREATE TRIGGER %1.tr_eliminarReservas BEFORE DELETE ON %1.Grupo
FOR EACH ROW
BEGIN

    DELETE FROM %1.alojado_en WHERE idGrupo=OLD.idGrupo;

END

-- END

-- Trigger para asignar automaticamente al grupo al alojamiento por defecto

CREATE TRIGGER %1.tr_alojamientoPorDefecto AFTER INSERT ON %1.Grupo
FOR EACH ROW
BEGIN

    INSERT INTO %1.alojado_en(idGrupo,codHab)
    VALUES(NEW.idGrupo,'Sin asignar');

END

-- END

-- Trigger para enviar a un grupo a 'sin asignar' al eliminar una habitacion alojada

CREATE TRIGGER %1.tr_actualizarAlojamiento BEFORE DELETE ON %1.Habitacion
FOR EACH ROW
BEGIN

    UPDATE %1.alojado_en
    SET codHab='Sin asignar' WHERE codHab=OLD.codHab;

END

-- END
