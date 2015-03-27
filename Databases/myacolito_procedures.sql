-- Funcion para crear nuevos grupos

CREATE FUNCTION %1.nuevoGrupo()
RETURNS INT
BEGIN
    DECLARE resultado INT DEFAULT -1;

    INSERT INTO Grupo(nombre) VALUES(' ');

    SET resultado=LAST_INSERT_ID();

    RETURN resultado;
END

-- END

-- Funcion para calcular el numero de plazas

CREATE FUNCTION %1.numeroPlazas(nombre VARCHAR(100))
RETURNS INT
BEGIN
    DECLARE resultado INT DEFAULT 0;

    SELECT count(tipo) FROM %1.plaza WHERE tipo=nombre INTO resultado;

    IF resultado=-1 THEN
        SET resultado=0;
    END IF;
    RETURN resultado;
END

-- END

-- Procedimiento para limpiar grupos vacios

CREATE PROCEDURE %1.LimpiarGruposVacios()
BEGIN
    DELETE FROM %1.Grupo WHERE idGrupo NOT IN
    (SELECT DISTINCT IDgrupo FROM %1.Asistente);
END

-- END

-- Procedimiento para asignar personas a grupos

CREATE PROCEDURE %1.AsignarAsistenteGrupo(IN idPersona INT,IN idGrupo INT)
BEGIN
    UPDATE %1.Asistente SET IDgrupo=idGrupo WHERE idAsistente=idPersona;
END

-- END

-- Procedimiento para separar personas en nuevos grupos

CREATE PROCEDURE %1.SepararAsistenteGrupo(IN idPersona INT)
BEGIN
    UPDATE %1.Asistente SET IDgrupo=%1.nuevoGrupo() WHERE idAsistente=idPersona;
END

-- END

-- Procedimiento para separar personas en nuevos grupos por DNI

CREATE PROCEDURE %1.SepararAsistenteGrupoDNI(IN dniPersona TEXT)
BEGIN
    DECLARE idAsis INT DEFAULT -1;

    SELECT idAsistente FROM %1.Asistente WHERE dni=dniPersona INTO idAsis;
    UPDATE %1.Asistente SET IDgrupo=%1.nuevoGrupo() WHERE idAsistente=idAsis;
END

-- END

-- Procedimiento para asignar personas a grupos mediante DNI

CREATE PROCEDURE %1.AsignarAsistenteGrupoDNI(IN dniPersona TEXT,IN idGrupo INT)
BEGIN
    UPDATE %1.Asistente SET IDgrupo=idGrupo WHERE DNI=dniPersona;
END

-- END

-- Procedimiento para asignar una plaza a un asistente

CREATE PROCEDURE %1.AsignarPlaza(IN idPersona INT,IN nCategoria VARCHAR(100))
BEGIN
    DECLARE identPlaza INT DEFAULT -1;

    INSERT INTO %1.Plaza(tipo) VALUES(nCategoria);
    SET identPlaza=LAST_INSERT_ID();

    INSERT INTO %1.reservas(idAsistente,idPlaza)
    VALUES(idPersona,identPlaza);
END

-- END

-- Procedimiento para asignar una plaza a un asistente mediante DNI

CREATE PROCEDURE %1.AsignarPlazaDNI(IN dniPersona TEXT,IN nCategoria VARCHAR(100))
BEGIN
    DECLARE identPlaza INT DEFAULT -1;
    DECLARE identAsis INT DEFAULT -1;

    SELECT idAsistente FROM %1.Asistente WHERE DNI=dniPersona
    INTO identAsis;

    IF identAsis != -1 THEN

        INSERT INTO %1.Plaza(tipo) VALUES(nCategoria);
        SET identPlaza=LAST_INSERT_ID();

        INSERT INTO %1.reservas(idAsistente,idPlaza)
        VALUES(identAsis,identPlaza);
    END IF;
END

-- END

-- Procedimiento para borrar un asistente

CREATE PROCEDURE %1.BorrarAsistente(IN idPersona INT)
BEGIN
    DECLARE identPlaza INT DEFAULT -1;

    SELECT idPlaza FROM %1.reservas WHERE idAsistente=idPersona INTO identPlaza;

    DELETE FROM %1.reservas WHERE idAsistente=idPersona;
    DELETE FROM %1.Plaza WHERE idPlaza=identPlaza;
    DELETE FROM %1.Asistente WHERE idAsistente=idPersona;
END

-- END

-- Procedimiento para borrar un asistente mediante DNI

CREATE PROCEDURE %1.BorrarAsistenteDNI(IN dniPersona TEXT)
BEGIN
    DECLARE identPlaza INT DEFAULT -1;
    DECLARE identAsis INT DEFAULT -1;

    SELECT idAsistente FROM %1.Asistente WHERE DNI=dniPersona
    INTO identAsis;

    SELECT idPlaza FROM %1.reservas WHERE idAsistente=identAsis INTO identPlaza;

    DELETE FROM %1.reservas WHERE idAsistente=identAsis;
    DELETE FROM %1.Plaza WHERE idPlaza=identPlaza;
    DELETE FROM %1.Asistente WHERE idAsistente=identAsis;
END

-- END

-- Procedimiento para cambiar el pago de un asistente mediante DNI

CREATE PROCEDURE %1.SetPagoDNI(IN dniPersona TEXT,IN pago BOOL)
BEGIN
    DECLARE identAsis INT DEFAULT -1;

    SELECT idAsistente FROM %1.Asistente WHERE DNI=dniPersona
    INTO identAsis;

    IF identAsis != -1 THEN

        UPDATE %1.reservas SET haPagado=pago WHERE idAsistente=identAsis;

    END IF;
END

-- END

-- Procedimiento para cambiar la asistencia de un asistente mediante DNI

CREATE PROCEDURE %1.SetActivaDNI(IN dniPersona TEXT,IN estado BOOL)
BEGIN
    DECLARE identAsis INT DEFAULT -1;

    SELECT idAsistente FROM %1.Asistente WHERE DNI=dniPersona
    INTO identAsis;

    IF identAsis != -1 THEN

        UPDATE %1.reservas SET estaActiva=estado WHERE idAsistente=identAsis;

    END IF;
END

-- END

-- Procedimiento para cambiar el estado de expiracion mediante DNI

CREATE PROCEDURE %1.SetExpiradaDNI(IN dniPersona TEXT,IN estado BOOL)
BEGIN
    DECLARE identAsis INT DEFAULT -1;

    SELECT idAsistente FROM %1.Asistente WHERE DNI=dniPersona
    INTO identAsis;

    IF identAsis != -1 THEN

        UPDATE %1.reservas SET haExpirado=estado WHERE idAsistente=identAsis;

    END IF;
END

-- END

-- Procedimiento para establecer los datos de una reserva mediante DNI

CREATE PROCEDURE %1.SetEstadoReservaDNI(IN dniPersona TEXT,IN pago BOOL,IN estado BOOL,IN expirado BOOL)
BEGIN
    DECLARE identAsis INT DEFAULT -1;

    SELECT idAsistente FROM %1.Asistente WHERE DNI=dniPersona
    INTO identAsis;

    IF identAsis != -1 THEN

        UPDATE %1.reservas SET haPagado=pago,haExpirado=expirado,estaActiva=estado WHERE idAsistente=identAsis;

    END IF;
END

-- END

-- Procedimiento para crear una nueva habitacion

CREATE PROCEDURE %1.NuevaHabitacion(IN codigo TEXT,IN tipo TEXT)
BEGIN

    INSERT INTO habitacion(codHab,tipoHab,comentario,llaveEntregada,estaReservada,conceptoReserva)
    VALUES(codigo,tipo,' ',0,0,'Escribe el concepto de reserva');

END

-- END

-- Procedimiento para crear un nuevo tipo de habitacion

CREATE PROCEDURE %1.NuevoTipoHabitacion(IN nombre TEXT,IN nPlazas INT,IN cPermitidas TEXT)
BEGIN

    INSERT INTO tipoHab(nombreTipo,plazas)
    VALUES(nombre,nPlazas);

    INSERT INTO permitidas(nombreTipo,categoria)
    VALUES(nombre,cPermitidas);

END

-- END
