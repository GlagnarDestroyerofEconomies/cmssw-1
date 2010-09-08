/*
	create_led_system
	created by Giovanni.Organtini@roma1.infn.it 2010

	stores data about LED system power and control

        Version: 20100901 (partitioned tables)
*/

/* channels */
DECLARE
	LOGIC_ID INTEGER := 1050000000;
	Y NUMBER := 1;
	X NUMBER := 1;
	CH NUMBER := 0;
BEGIN
	INSERT INTO VIEWDESCRIPTION VALUES ('LED_PS_CHANNEL', 
				     'LED Power Supplies CHANNEL NUMBER',
				     'ENDCAP', 'DEE', NULL);
	INSERT INTO VIEWDESCRIPTION VALUES ('LED_PM_CHANNEL', 
				     'LED Power Monitor CHANNEL NUMBER',
				     'ENDCAP', 'DEE', 'CHANNEL');
	INSERT INTO VIEWDESCRIPTION VALUES ('LED_CB_CHANNEL', 
				     'LED Control Boxes CHANNEL NUMBER',
				     'ENDCAP', 'DEE', NULL);
	INSERT INTO VIEWDESCRIPTION VALUES ('LED_LED_CHANNEL', 
				     'LED Boxes CHANNEL NUMBER',
				     'ENDCAP', 'DEE', 'CHANNEL');

	FOR X IN 1..2 LOOP
	  INSERT INTO CHANNELVIEW VALUES('LED_PS_CHANNEL', +1, X, NULL,
	    		           'LED_PS_CHANNEL', LOGIC_ID);
	  LOGIC_ID := LOGIC_ID + 1;
	  INSERT INTO CHANNELVIEW VALUES('LED_PS_CHANNEL', -1, X, NULL,
	  		           'LED_PS_CHANNEL', LOGIC_ID);
	  LOGIC_ID := LOGIC_ID + 1;
        END LOOP;

	FOR X IN 1..4 LOOP
	    INSERT INTO CHANNELVIEW VALUES('LED_PM_CHANNEL', +1, 2, X,
	    	   		           'LED_PM_CHANNEL', LOGIC_ID);
            LOGIC_ID := LOGIC_ID + 1;
	    INSERT INTO CHANNELVIEW VALUES('LED_PM_CHANNEL', -1, 2, X,
	    	   		           'LED_PM_CHANNEL', LOGIC_ID);
       
            LOGIC_ID := LOGIC_ID + 1;
	END LOOP;
	FOR X IN 5..8 LOOP
	    INSERT INTO CHANNELVIEW VALUES('LED_PM_CHANNEL', +1, 1, X,
	    	   		           'LED_PM_CHANNEL', LOGIC_ID);
            LOGIC_ID := LOGIC_ID + 1;
	    INSERT INTO CHANNELVIEW VALUES('LED_PM_CHANNEL', -1, 1, X,
	    	   		           'LED_PM_CHANNEL', LOGIC_ID);
       
            LOGIC_ID := LOGIC_ID + 1;
	END LOOP;

	FOR X IN 1..2 LOOP
	    INSERT INTO CHANNELVIEW VALUES('LED_CB_CHANNEL', +1, X, NULL,
		  		           'LED_CB_CHANNEL', LOGIC_ID);
	    LOGIC_ID := LOGIC_ID + 1;
	    INSERT INTO CHANNELVIEW VALUES('LED_CB_CHANNEL', -1, X, NULL,
		  		           'LED_CB_CHANNEL', LOGIC_ID);
	    LOGIC_ID := LOGIC_ID + 1;
	END LOOP;

	FOR X IN 1..2 LOOP
	  FOR Y IN 1..4 LOOP
            CH := 166 + 2 * Y;
	    INSERT INTO CHANNELVIEW VALUES('LED_LED_CHANNEL', +1, X, CH,
		    		           'LED_LED_CHANNEL', LOGIC_ID);
	    LOGIC_ID := LOGIC_ID + 1;
  	    INSERT INTO CHANNELVIEW VALUES('LED_LED_CHANNEL', -1, X, CH,
		  		           'LED_LED_CHANNEL', LOGIC_ID);
	    LOGIC_ID := LOGIC_ID + 1;
	  END LOOP;
        END LOOP;
END;
/

@update_iov_dates_procedure_new

/* Tags */
CREATE TABLE LED_SYSTEM_TAG (
	TAG_ID		NUMBER(10) NOT NULL,
	GEN_TAG		VARCHAR(100) NOT NULL,
	VERSION		INTEGER NOT NULL
);

CREATE SEQUENCE LED_SYSTEM_TAG_SQ INCREMENT BY 1 START WITH 1 NOCACHE;

ALTER TABLE LED_SYSTEM_TAG ADD CONSTRAINT LED_SYSTEM_TAG_PK PRIMARY 
	KEY (TAG_ID);
ALTER TABLE LED_SYSTEM_TAG ADD CONSTRAINT LED_SYSTEM_TAG_UK UNIQUE (GEN_TAG,
	VERSION);

/* The following table associates a unique id to each table or group of
   tables used in IOV table to define IOV per group of tables */
CREATE TABLE LED_DATTABLE_INDEX (
	ID		NUMBER(3) NOT NULL,
	TABLE_NAME	VARCHAR(25) NOT NULL,
	DESCRIPTION     VARCHAR(255)
);

ALTER TABLE LED_DATTABLE_INDEX ADD CONSTRAINT LED_DATTABLE_INDEX_PK
        PRIMARY KEY (TABLE_NAME);
ALTER TABLE LED_DATTABLE_INDEX ADD CONSTRAINT LED_DATTABLE_INDEX_UK
	UNIQUE (ID, TABLE_NAME);

INSERT INTO LED_DATTABLE_INDEX VALUES (1, 'LED_POWER_SUPPLY_MON', '');
INSERT INTO LED_DATTABLE_INDEX VALUES (1, 'LED_POWER_MONBOX_MON', '');
INSERT INTO LED_DATTABLE_INDEX VALUES (1, 'LED_POWER_CTRLBOX_MON', '');
INSERT INTO LED_DATTABLE_INDEX VALUES (1, 'LED_POWER_LED_MON', '');
INSERT INTO LED_DATTABLE_INDEX VALUES (2, 'LED_POWER_SUPPLY_STATUS', '');
INSERT INTO LED_DATTABLE_INDEX VALUES (2, 'LED_POWER_MONBOX_STATUS', '');
INSERT INTO LED_DATTABLE_INDEX VALUES (2, 'LED_POWER_CTRLBOX_STATUS', '');

/* Tables to store interval of validities */
CREATE TABLE LED_SYSTEM_IOV (
	IOV_ID		NUMBER(10),
	MASK		NUMBER(3) NOT NULL,
	TAG_ID		NUMBER(10) NOT NULL,
	SINCE		DATE NOT NULL,
	TILL		DATE NOT NULL,
	DB_TIMESTAMP	TIMESTAMP DEFAULT SYSTIMESTAMP NOT NULL
)
PARTITION BY RANGE ("IOV_ID")
(PARTITION "LED_SYSTEM_IOV_0" VALUES LESS THAN (MAXVALUE))
;

CREATE SEQUENCE LED_SYSTEM_IOV_SQ INCREMENT BY 1 START WITH 1 NOCACHE;

ALTER TABLE LED_SYSTEM_IOV ADD CONSTRAINT LED_SYSTEM_IOV_PK 
	PRIMARY KEY (IOV_ID);
ALTER TABLE LED_SYSTEM_IOV ADD CONSTRAINT LED_SYSTEM_IOV_FK1
	FOREIGN KEY (TAG_ID) REFERENCES LED_SYSTEM_TAG (TAG_ID);

CREATE INDEX LED_SYSTEM_IOV_IX ON LED_SYSTEM_IOV (SINCE, TILL);

CREATE OR REPLACE TRIGGER LED_SYSTEM_IOV_TG
	BEFORE INSERT ON LED_SYSTEM_IOV
	REFERENCING NEW AS newiov
	FOR EACH ROW
	CALL update_iov_dates_new('LED_SYSTEM_IOV', :newiov.mask, 
        'SINCE', 'TILL', 
        :newiov.since,
	:newiov.till, :newiov.tag_id)
/

/* low frequency status tables */
CREATE TABLE LED_POWER_SUPPLY_MON (
	IOV_ID 			NUMBER(10) NOT NULL,
	LOGIC_ID		NUMBER(10),
	PS_VOLTAGE_SET_POINT	FLOAT,
	PS_STATUS		INTEGER,
	PS_STANDEVT_STATUS	INTEGER,
	PS_EXEC_ERR		INTEGER,
	PS_QUERY_ERR		INTEGER
)
PARTITION BY RANGE ("IOV_ID")
(PARTITION "LED_PWRSPPL_M_0" VALUES LESS THAN (MAXVALUE))
;

ALTER TABLE LED_POWER_SUPPLY_MON ADD CONSTRAINT LED_POWER_SUPPLY_MON_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_SUPPLY_MON ADD CONSTRAINT LED_POWER_SUPPLY_MON_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);
 
CREATE TABLE LED_POWER_MONBOX_MON (
	IOV_ID 			NUMBER,
	LOGIC_ID		NUMBER(10),
	PM_VOLTAGE_LIM		FLOAT,
	PM_CURRENT_LIMIT	FLOAT
)
PARTITION BY RANGE ("IOV_ID")
(PARTITION "LED_PWRMNBX_M_0" VALUES LESS THAN (MAXVALUE))
;

ALTER TABLE LED_POWER_MONBOX_MON ADD CONSTRAINT LED_POWER_MONBOX_MON_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_MONBOX_MON ADD CONSTRAINT LED_POWER_MONBOX_MON_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);

CREATE TABLE LED_POWER_CTRLBOX_MON (
	IOV_ID			NUMBER,
	LOGIC_ID		NUMBER(10),
	CB_OUT_DELAY_COARSE	INTEGER,
	CB_OUT_DELAY_FINE	FLOAT,
	CB_OUT_PULSE_WIDTH	INTEGER	
)
PARTITION BY RANGE ("IOV_ID")
(PARTITION "LED_PWRCTRLBX_M_0" VALUES LESS THAN (MAXVALUE))
;

ALTER TABLE LED_POWER_CTRLBOX_MON ADD CONSTRAINT LED_POWER_CTRLBOX_MON_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_CTRLBOX_MON ADD CONSTRAINT LED_POWER_CTRLBOX_MON_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);

CREATE TABLE LED_POWER_LED_MON (
	IOV_ID		NUMBER,
	LOGIC_ID	NUMBER(10),
	STATUS		NUMBER(1)
)
PARTITION BY RANGE ("IOV_ID")
(PARTITION "LED_PWRLD_M_0" VALUES LESS THAN (MAXVALUE))
;

ALTER TABLE LED_POWER_LED_MON ADD CONSTRAINT LED_POWER_LED_MON_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_LED_MON ADD CONSTRAINT LED_POWER_LED_MON_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);

/* status tables */
CREATE TABLE LED_POWER_SUPPLY_STATUS (
	IOV_ID           NUMBER(10) NOT NULL,
	LOGIC_ID         NUMBER(10),
	PS_CURRENT	 NUMBER,
	PS_VOLTAGE	 NUMBER,
	PS_DELTAV	 NUMBER,
	PS_CURRENT_LIMIT NUMBER
)
PARTITION BY RANGE ("IOV_ID")
(PARTITION "LED_PWRSPPL_S_0" VALUES LESS THAN (MAXVALUE))
;

ALTER TABLE LED_POWER_SUPPLY_STATUS ADD CONSTRAINT LED_POWER_SUPPLY_STATUS_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_SUPPLY_STATUS ADD CONSTRAINT LED_POWER_SUPPLY_STATUS_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);

CREATE TABLE LED_POWER_MONBOX_STATUS (
	IOV_ID		NUMBER(10) NOT NULL,
	LOGIC_ID	INTEGER,
	PM_VOLTAGE	FLOAT,
	PM_CURRENT 	FLOAT,
	PM_STATE	INTEGER /* This is an 8 bit integer encoding
				   4 2-bit status words */	
)
PARTITION BY RANGE ("IOV_ID")
(PARTITION "LED_PWRMNBX_S_0" VALUES LESS THAN (MAXVALUE))
;

ALTER TABLE LED_POWER_MONBOX_STATUS ADD CONSTRAINT LED_POWER_MONBOX_STATUS_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_MONBOX_STATUS ADD CONSTRAINT LED_POWER_MONBOX_STATUS_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);
 
CREATE TABLE LED_POWER_CTRLBOX_STATUS (
	IOV_ID		NUMBER(10) NOT NULL,
	LOGIC_ID	NUMBER(10),
	CB_EXT_COUNT	FLOAT,
	CB_EXT_FREQ	FLOAT,
	CB_INT_COUNT	FLOAT,
	CB_INT_FREQ	FLOAT,
	CB_OUT_COUNT	FLOAT,
	CB_OUT_FREQ	FLOAT
)
PARTITION BY RANGE ("IOV_ID")
(PARTITION "LED_PWRCTRLBX_S_0" VALUES LESS THAN (MAXVALUE))
;

ALTER TABLE LED_POWER_CTRLBOX_STATUS ADD CONSTRAINT LED_POWER_CTRLBOX_STATUS_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_CTRLBOX_STATUS ADD CONSTRAINT 
	LED_POWER_CTRLBOX_STATUS_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);

SET HEADING OFF
SET ECHO OFF
SPOOL led_grants.sql
SELECT 'GRANT SELECT ON ', TABLE_NAME, ' TO CMS_ECAL_R;' FROM USER_TABLES;
SELECT 'GRANT SELECT ON ', TABLE_NAME, ' TO CMS_WBM_R;' FROM USER_TABLES;
SPOOL OFF;

@led_grants

SPOOL led_synonyms.sql
SELECT 'CREATE OR REPLACE SYNONYM ', TABLE_NAME, ' FOR CMS_ECAL_COND.', TABLE_NAME, ';' FROM USER_TABLES WHERE TABLE_NAME LIKE 'LED%';
SPOOL OFF

@led_synonyms

SET HEADING ON
