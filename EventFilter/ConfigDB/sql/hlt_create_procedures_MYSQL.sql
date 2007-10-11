--
-- MySQL Stored Procedure: load templates for a release
--

-- drop the procedure if it already exists
USE hltdb;
DROP PROCEDURE IF EXISTS load_templates;
DROP PROCEDURE IF EXISTS load_parameters;
DROP PROCEDURE IF EXISTS load_parameter_value;
DROP PROCEDURE IF EXISTS get_parameters;
DROP PROCEDURE IF EXISTS get_boolean_values;
DROP PROCEDURE IF EXISTS get_int_values;
DROP PROCEDURE IF EXISTS get_real_values;
DROP PROCEDURE IF EXISTS get_string_values;


-- set delimiter to '//'
DELIMITER //


--
-- PROCEDURE load_templates
--
CREATE PROCEDURE load_templates(release_id BIGINT UNSIGNED)
proc:
BEGIN
  DECLARE temporary_table_exists BOOLEAN;
  BEGIN
    DECLARE CONTINUE HANDLER FOR SQLEXCEPTION BEGIN END;
    DROP TABLE IF EXISTS tmp_template_table;
    DROP TABLE IF EXISTS tmp_parameter_table;
    DROP TABLE IF EXISTS tmp_boolean_table;
    DROP TABLE IF EXISTS tmp_int_table;
    DROP TABLE IF EXISTS tmp_real_table;
    DROP TABLE IF EXISTS tmp_string_table;
  END;
  BEGIN
    /* variables */
    DECLARE v_template_id     INT;
    DECLARE v_template_type   CHAR(64);
    DECLARE v_template_name   CHAR(128);
    DECLARE v_template_cvstag CHAR(32);
    DECLARE done               BOOLEAN DEFAULT FALSE;

    /* cursor for edsource templates */
    DECLARE cur_edsource_templates CURSOR FOR
      SELECT EDSourceTemplates.superId,
             EDSourceTemplates.name,
             EDSourceTemplates.cvstag
      FROM EDSourceTemplates
      JOIN SuperIdReleaseAssoc
      ON EDSourceTemplates.superId = SuperIdReleaseAssoc.superId
      WHERE SuperIdReleaseAssoc.releaseId = release_id;

    /* cursor for essource templates */
    DECLARE cur_essource_templates CURSOR FOR
      SELECT ESSourceTemplates.superId,
             ESSourceTemplates.name,
             ESSourceTemplates.cvstag
      FROM ESSourceTemplates
      JOIN SuperIdReleaseAssoc
      ON ESSourceTemplates.superId = SuperIdReleaseAssoc.superId
      WHERE SuperIdReleaseAssoc.releaseId = release_id;

    /* cursor for esmodule templates */
    DECLARE cur_esmodule_templates CURSOR FOR
      SELECT ESModuleTemplates.superId,
             ESModuleTemplates.name,
             ESModuleTemplates.cvstag
      FROM ESModuleTemplates
      JOIN SuperIdReleaseAssoc
      ON ESModuleTemplates.superId = SuperIdReleaseAssoc.superId
      WHERE SuperIdReleaseAssoc.releaseId = release_id;

    /* cursor for service templates */
    DECLARE cur_service_templates CURSOR FOR
      SELECT ServiceTemplates.superId,
             ServiceTemplates.name,
             ServiceTemplates.cvstag
      FROM ServiceTemplates
      JOIN SuperIdReleaseAssoc
      ON ServiceTemplates.superId = SuperIdReleaseAssoc.superId
      WHERE SuperIdReleaseAssoc.releaseId = release_id;

    /* cursor for module templates */
    DECLARE cur_module_templates CURSOR FOR
      SELECT ModuleTemplates.superId,
             ModuleTemplates.name,
             ModuleTemplates.cvstag,
             ModuleTypes.type
      FROM ModuleTemplates
      JOIN ModuleTypes
      ON   ModuleTemplates.typeId = ModuleTypes.typeId
      JOIN SuperIdReleaseAssoc
      ON ModuleTemplates.superId = SuperIdReleaseAssoc.superId
      WHERE SuperIdReleaseAssoc.releaseId = release_id;

    /* error handlers */
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

    /* temporary template table */
    CREATE TEMPORARY TABLE tmp_template_table
    (
      template_id	BIGINT UNSIGNED,
      template_type    CHAR(64),
      template_name    CHAR(128),
      template_cvstag  CHAR(32)
    );
    SET temporary_table_exists = TRUE;

    /* temporary parameter table */
    CREATE TEMPORARY TABLE tmp_parameter_table
    (
      parameter_id	BIGINT UNSIGNED,
      parameter_type    CHAR(64),
      parameter_name    CHAR(128),
      parameter_trkd    BOOLEAN,
      parameter_seqnb   INT,
      parent_id         BIGINT UNSIGNED
    );

    /* temporary bool parameter-value table */
    CREATE TEMPORARY TABLE tmp_boolean_table
    (
      parameter_id	BIGINT UNSIGNED,
      parameter_value   BOOLEAN
    );

    /* temporary int32 parameter-value table */
    CREATE TEMPORARY TABLE tmp_int_table
    (
      parameter_id	BIGINT UNSIGNED,
      parameter_value   BIGINT,
      sequence_nb       INT
    );

    /* temporary double parameter-value table */
    CREATE TEMPORARY TABLE tmp_real_table
    (
      parameter_id	BIGINT UNSIGNED,
      parameter_value   REAL,
      sequence_nb       INT
    );

    /* temporary string parameter-value table */
    CREATE TEMPORARY TABLE tmp_string_table
    (
      parameter_id	BIGINT UNSIGNED,
      parameter_value   VARCHAR(512),
      sequence_nb       INT
    );

    /* load edsource templates */
    OPEN cur_edsource_templates;
    FETCH cur_edsource_templates
      INTO v_template_id,v_template_name,v_template_cvstag;
    WHILE done=FALSE DO
      INSERT INTO tmp_template_table
        VALUES(v_template_id,'EDSource',v_template_name,v_template_cvstag);
      FETCH cur_edsource_templates
        INTO v_template_id,v_template_name,v_template_cvstag;
    END WHILE;
    CLOSE cur_edsource_templates;
    SET done=FALSE;

    /* load essource templates */
    OPEN cur_essource_templates;
    FETCH cur_essource_templates
      INTO v_template_id,v_template_name,v_template_cvstag;
    WHILE done=FALSE DO
      INSERT INTO tmp_template_table
        VALUES(v_template_id,'ESSource',v_template_name,v_template_cvstag);
      CALL load_parameters(v_template_id);
      FETCH cur_essource_templates
        INTO v_template_id,v_template_name,v_template_cvstag;
    END WHILE;
    CLOSE cur_essource_templates;
    SET done=FALSE;

    /* load esmodule templates */
    OPEN cur_esmodule_templates;
    FETCH cur_esmodule_templates
      INTO v_template_id,v_template_name,v_template_cvstag;
    WHILE done=FALSE DO
      INSERT INTO tmp_template_table
        VALUES(v_template_id,'ESModule',v_template_name,v_template_cvstag);
      CALL load_parameters(v_template_id);
      FETCH cur_esmodule_templates
        INTO v_template_id,v_template_name,v_template_cvstag;
    END WHILE;
    CLOSE cur_esmodule_templates;
    SET done=FALSE;

    /* load service templates */
    OPEN cur_service_templates;
    FETCH cur_service_templates
      INTO v_template_id,v_template_name,v_template_cvstag;
    WHILE done=FALSE DO
      INSERT INTO tmp_template_table
        VALUES(v_template_id,'Service',v_template_name,v_template_cvstag);
      CALL load_parameters(v_template_id);
      FETCH cur_service_templates
        INTO v_template_id,v_template_name,v_template_cvstag;
    END WHILE;
    CLOSE cur_service_templates;
    SET done=FALSE;

    /* load module templates */
    OPEN cur_module_templates;
    FETCH cur_module_templates
      INTO v_template_id,v_template_name,
           v_template_cvstag,v_template_type;
    WHILE done=FALSE DO
      INSERT INTO tmp_template_table
        VALUES(v_template_id,v_template_type,
               v_template_name,v_template_cvstag);
      CALL load_parameters(v_template_id);
      FETCH cur_module_templates
        INTO v_template_id,v_template_name,
             v_template_cvstag,v_template_type;
    END WHILE;
    CLOSE cur_module_templates;
    SET done=FALSE;

    /* generate the final result set by selecting the temporary table */
    SELECT template_id,template_type,template_name,template_cvstag
    FROM tmp_template_table;
  END;  

  /* if the temporary table was created, drop it now */
  IF temporary_table_exists THEN
    DROP TEMPORARY TABLE tmp_template_table;
  END IF;
END;
//


--
-- PROCEDURE load_parameters
--
CREATE PROCEDURE load_parameters(parent_id BIGINT UNSIGNED)
proc:
BEGIN
  DECLARE v_parameter_id    BIGINT UNSIGNED;
  DECLARE v_parameter_type  CHAR(64);
  DECLARE v_parameter_name  CHAR(128);
  DECLARE v_parameter_trkd  BOOLEAN;
  DECLARE v_parameter_seqnb INT;
  DECLARE done              BOOLEAN DEFAULT FALSE;

  /* cursor for parameters */
  DECLARE cur_parameters CURSOR FOR
    SELECT Parameters.paramId,
           ParameterTypes.paramType,
           Parameters.name,
           Parameters.tracked,
           SuperIdParameterAssoc.sequenceNb
    FROM Parameters
    JOIN ParameterTypes
    ON ParameterTypes.paramTypeId = Parameters.paramTypeId
    JOIN SuperIdParameterAssoc
    ON SuperIdParameterAssoc.paramId = Parameters.paramId
    WHERE SuperIdParameterAssoc.superId = parent_id
    ORDER BY SuperIdParameterAssoc.sequenceNb ASC;

  /* cursor for psets */
  DECLARE cur_psets CURSOR FOR
    SELECT ParameterSets.superId,
           ParameterSets.name,
           ParameterSets.tracked,
           SuperIdParamSetAssoc.sequenceNb
    FROM ParameterSets
    JOIN SuperIdParamSetAssoc
    ON SuperIdParamSetAssoc.psetId = ParameterSets.superId
    WHERE SuperIdParamSetAssoc.superId = parent_id
    ORDER BY SuperIdParamSetAssoc.sequenceNb ASC;

  /* cursor for vpsets */
  DECLARE cur_vpsets CURSOR FOR
    SELECT VecParameterSets.superId,
           VecParameterSets.name,
           VecParameterSets.tracked,
           SuperIdVecParamSetAssoc.sequenceNb
    FROM VecParameterSets
    JOIN SuperIdVecParamSetAssoc
    ON SuperIdVecParamSetAssoc.vpsetId = VecParameterSets.superId
    WHERE SuperIdVecParamSetAssoc.superId = parent_id
    ORDER BY SuperIdVecParamSetAssoc.sequenceNb ASC;

  /* error handlers */
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

  /* load the parameters and fill them into temporary param table */
  OPEN cur_parameters;
  FETCH cur_parameters
    INTO v_parameter_id,v_parameter_type,
         v_parameter_name,v_parameter_trkd,v_parameter_seqnb;
  WHILE done=FALSE DO
    INSERT INTO tmp_parameter_table
      VALUES(v_parameter_id,v_parameter_type,
             v_parameter_name,v_parameter_trkd,v_parameter_seqnb,parent_id);
    CALL load_parameter_value(v_parameter_id,v_parameter_type);
    FETCH cur_parameters
      INTO v_parameter_id,v_parameter_type,
           v_parameter_name,v_parameter_trkd,v_parameter_seqnb;
  END WHILE;
  CLOSE cur_parameters;
  SET done = FALSE;

  /* load psets and fill them into temporary param table */
  OPEN cur_psets;
  FETCH cur_psets
    INTO v_parameter_id,v_parameter_name,v_parameter_trkd,v_parameter_seqnb;
  WHILE done=FALSE DO
    INSERT INTO tmp_parameter_table
      VALUES(v_parameter_id,'PSet',
             v_parameter_name,v_parameter_trkd,v_parameter_seqnb,parent_id);
    CALL load_parameters(v_parameter_id);
    FETCH cur_psets
      INTO v_parameter_id,
           v_parameter_name,v_parameter_trkd,v_parameter_seqnb;
  END WHILE;
  CLOSE cur_psets;
  SET done = FALSE;

  /* load vpsets and fill them into temporary param table */
  OPEN cur_vpsets;
  FETCH cur_vpsets
    INTO v_parameter_id,
         v_parameter_name,v_parameter_trkd,v_parameter_seqnb;
  WHILE done=FALSE DO
    INSERT INTO tmp_parameter_table
      VALUES(v_parameter_id,'VPSet',
             v_parameter_name,v_parameter_trkd,v_parameter_seqnb,parent_id);
    CALL load_parameters(v_parameter_id);
    FETCH cur_vpsets
      INTO v_parameter_id,
           v_parameter_name,v_parameter_trkd,v_parameter_seqnb;
  END WHILE;
  CLOSE cur_vpsets;
  SET done = FALSE;

END;
//


--
-- PROCDEDURE load_parameter_value
--
CREATE PROCEDURE load_parameter_value(parameter_id   BIGINT UNSIGNED,
                                      parameter_type CHAR(64))
proc:
BEGIN
  DECLARE v_bool_value   BOOLEAN;
  DECLARE v_int32_value  BIGINT;
  DECLARE v_double_value REAL;
  DECLARE v_string_value VARCHAR(512);
  DECLARE v_sequence_nb  INT;
  DECLARE done           BOOLEAN DEFAULT FALSE;

  /* declare cursors */
  DECLARE cur_bool CURSOR FOR
    SELECT value FROM BoolParamValues
    WHERE paramId=parameter_id;
  DECLARE cur_int32 CURSOR FOR
    SELECT value FROM Int32ParamValues
    WHERE paramId=parameter_id;
  DECLARE cur_vint32 CURSOR FOR
    SELECT value,sequenceNb FROM VInt32ParamValues
    WHERE paramId=parameter_id
    ORDER BY sequenceNb ASC;
  DECLARE cur_uint32 CURSOR FOR
    SELECT value FROM UInt32ParamValues
    WHERE paramId=parameter_id;
  DECLARE cur_vuint32 CURSOR FOR
    SELECT value,sequenceNb FROM VUInt32ParamValues
    WHERE paramId=parameter_id
    ORDER BY sequenceNb ASC;
  DECLARE cur_double CURSOR FOR
    SELECT value FROM DoubleParamValues
    WHERE paramId=parameter_id;
  DECLARE cur_vdouble CURSOR FOR
    SELECT value,sequenceNb FROM VDoubleParamValues
    WHERE paramId=parameter_id
    ORDER BY sequenceNb ASC;
  DECLARE cur_string CURSOR FOR
    SELECT value FROM StringParamValues
    WHERE paramId=parameter_id;
  DECLARE cur_vstring CURSOR FOR
    SELECT value,sequenceNb FROM VStringParamValues
    WHERE paramId=parameter_id
    ORDER BY sequenceNb ASC;
  DECLARE cur_inputtag CURSOR FOR
    SELECT value FROM InputTagParamValues
    WHERE paramId=parameter_id;
  DECLARE cur_vinputtag CURSOR FOR
    SELECT value,sequenceNb FROM VInputTagParamValues
    WHERE paramId=parameter_id
    ORDER BY sequenceNb ASC;
  DECLARE cur_eventid CURSOR FOR
    SELECT value FROM EventIDParamValues
    WHERE paramId=parameter_id;
  DECLARE cur_veventid CURSOR FOR
    SELECT value,sequenceNb FROM VEventIDParamValues
    WHERE paramId=parameter_id
    ORDER BY sequenceNb ASC;
  DECLARE cur_fileinpath CURSOR FOR
    SELECT value FROM FileInPathParamValues
    WHERE paramId=parameter_id;

  /* declare error handlers */
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

  /** load bool values */
  IF parameter_type='bool' THEN
    OPEN cur_bool;
    FETCH cur_bool INTO v_bool_value;
    IF done=FALSE THEN
      INSERT INTO tmp_boolean_table VALUES(parameter_id,v_bool_value);
    END IF;
    CLOSE cur_bool;
    LEAVE proc;
  END IF;

  /** load int32 values */
  IF parameter_type='int32' THEN
    OPEN cur_int32;
    FETCH cur_int32 INTO v_int32_value;
    IF done=FALSE THEN
      INSERT INTO tmp_int_table VALUES(parameter_id,v_int32_value,NULL);
    END IF;
    CLOSE cur_int32;
    LEAVE proc;
  END IF;

  /** load vint32 values */
  IF parameter_type='vint32' THEN
    OPEN cur_vint32;
    FETCH cur_vint32 INTO v_int32_value,v_sequence_nb;
    WHILE done=FALSE DO
      INSERT INTO tmp_int_table
        VALUES(parameter_id,v_int32_value,v_sequence_nb);
      FETCH cur_vint32 INTO v_int32_value,v_sequence_nb;
    END WHILE;
    CLOSE cur_vint32;
    LEAVE proc;
  END IF;

  /** load uint32 values */
  IF parameter_type='uint32' THEN
    OPEN cur_uint32;
    FETCH cur_uint32 INTO v_int32_value;
    IF done=FALSE THEN
      INSERT INTO tmp_int_table VALUES(parameter_id,v_int32_value,NULL);
    END IF;
    CLOSE cur_uint32;
    LEAVE proc;
  END IF;

  /** load vuint32 values */
  IF parameter_type='vuint32' THEN
    OPEN cur_vuint32;
    FETCH cur_vuint32 INTO v_int32_value,v_sequence_nb;
    WHILE done=FALSE DO
      INSERT INTO tmp_int_table
        VALUES(parameter_id,v_int32_value,v_sequence_nb);
      FETCH cur_vuint32 INTO v_int32_value,v_sequence_nb;
    END WHILE;
    CLOSE cur_vuint32;
    LEAVE proc;
  END IF;

  /** load double values */
  IF parameter_type='double' THEN
    OPEN cur_double;
    FETCH cur_double INTO v_double_value;
    IF done=FALSE THEN
      INSERT INTO tmp_real_table VALUES(parameter_id,v_double_value,NULL);
    END IF;
    CLOSE cur_double;
    LEAVE proc;
  END IF;

  /** load vdouble values */
  IF parameter_type='vdouble' THEN
    OPEN cur_vdouble;
    FETCH cur_vdouble INTO v_double_value,v_sequence_nb;
    WHILE done=FALSE DO
      INSERT INTO tmp_real_table
        VALUES(parameter_id,v_double_value,v_sequence_nb);
      FETCH cur_vdouble INTO v_double_value,v_sequence_nb;
    END WHILE;
    CLOSE cur_vdouble;
    LEAVE proc;
  END IF;

  /** load string values */
  IF parameter_type='string' THEN
    OPEN cur_string;
    FETCH cur_string INTO v_string_value;
    IF done=FALSE THEN
      INSERT INTO tmp_string_table VALUES(parameter_id,v_string_value,NULL);
    END IF;
    CLOSE cur_string;
    LEAVE proc;
  END IF;

  /** load vstring values */
  IF parameter_type='vstring' THEN
    OPEN cur_vstring;
    FETCH cur_vstring INTO v_string_value,v_sequence_nb;
    WHILE done=FALSE DO
      INSERT INTO tmp_string_table
        VALUES(parameter_id,v_string_value,v_sequence_nb);
      FETCH cur_vstring INTO v_string_value,v_sequence_nb;
    END WHILE;
    CLOSE cur_vstring;
    LEAVE proc;
  END IF;

  /** load inputtag values */
  IF parameter_type='InputTag' THEN
    OPEN cur_inputtag;
    FETCH cur_inputtag INTO v_string_value;
    IF done=FALSE THEN
      INSERT INTO tmp_string_table VALUES(parameter_id,v_string_value,NULL);
    END IF;
    CLOSE cur_inputtag;
    LEAVE proc;
  END IF;

  /** load vinputtag values */
  IF parameter_type='VInputTag' THEN
    OPEN cur_vinputtag;
    FETCH cur_vinputtag INTO v_string_value,v_sequence_nb;
    WHILE done=FALSE DO
      INSERT INTO tmp_string_table
        VALUES(parameter_id,v_string_value,v_sequence_nb);
      FETCH cur_vinputtag INTO v_string_value,v_sequence_nb;
    END WHILE;
    CLOSE cur_vinputtag;
    LEAVE proc;
  END IF;

  /** load eventid values */
  IF parameter_type='EventID' THEN
    OPEN cur_eventid;
    FETCH cur_eventid INTO v_string_value;
    IF done=FALSE THEN
      INSERT INTO tmp_string_table VALUES(parameter_id,v_string_value,NULL);
    END IF;
    CLOSE cur_eventid;
    LEAVE proc;
  END IF;

  /** load veventid values */
  IF parameter_type='VEventID' THEN
    OPEN cur_veventid;
    FETCH cur_veventid INTO v_string_value,v_sequence_nb;
    WHILE done=FALSE DO
      INSERT INTO tmp_string_table
        VALUES(parameter_id,v_string_value,v_sequence_nb);
      FETCH cur_veventid INTO v_string_value,v_sequence_nb;
    END WHILE;
    CLOSE cur_veventid;
    LEAVE proc;
  END IF;

  /** load fileinpath values */
  IF parameter_type='FileInPath' THEN
    OPEN cur_fileinpath;
    FETCH cur_fileinpath INTO v_string_value;
    IF done=FALSE THEN
      INSERT INTO tmp_string_table VALUES(parameter_id,v_string_value,NULL);
    END IF;
    CLOSE cur_fileinpath;
    LEAVE proc;
  END IF;

  SELECT 'UNKNOWN PARAMETER TYPE',parameter_type;
END;
//


--
-- PROCEDURE get_parameters
--
CREATE PROCEDURE get_parameters()
BEGIN
  SELECT parameter_id,parameter_type,
         parameter_name,parameter_trkd,parameter_seqnb,parent_id
  FROM tmp_parameter_table;
  DROP TEMPORARY TABLE IF EXISTS tmp_parameter_table;
END;
//


--
-- PROCEDURE get_boolean_values
--
CREATE PROCEDURE get_boolean_values()
BEGIN
  SELECT parameter_id,parameter_value FROM tmp_boolean_table;
  DROP TEMPORARY TABLE IF EXISTS tmp_boolean_table;
END;
//


--
-- PROCEDURE get_int_values
--
CREATE PROCEDURE get_int_values()
BEGIN
  SELECT parameter_id,parameter_value,sequence_nb FROM tmp_int_table;
  DROP TEMPORARY TABLE IF EXISTS tmp_int_table;
END;
//


--
-- PROCEDURE get_real_values
--
CREATE PROCEDURE get_real_values()
BEGIN
  SELECT parameter_id,parameter_value,sequence_nb FROM tmp_real_table;
  DROP TEMPORARY TABLE IF EXISTS tmp_real_table;
END;
//


--
-- PROCEDURE get_string_values
--
CREATE PROCEDURE get_string_values()
BEGIN
  SELECT parameter_id,parameter_value,sequence_nb FROM tmp_string_table;
  DROP TEMPORARY TABLE IF EXISTS tmp_string_table;
END;
//


-- reset delimiter to ';'
DELIMITER ; //