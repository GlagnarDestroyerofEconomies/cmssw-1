CREATE TABLE CMS_COND_GENERAL."POOL_FC_PFN" ( "PFN" VARCHAR2(4000), "FID" CHAR(36) NOT NULL, "TYPE" VARCHAR2(4000), CONSTRAINT "POOL_FC_PFN_PK" PRIMARY KEY ( "PFN" ) );

CREATE INDEX CMS_COND_GENERAL."POOL_FC_PFN_IDX" ON CMS_COND_GENERAL."POOL_FC_PFN" ( "FID" );

CREATE TABLE CMS_COND_GENERAL."POOL_FC_LFN" ( "LFN" VARCHAR2(4000), "FID" CHAR(36) NOT NULL, CONSTRAINT "POOL_FC_LFN_PK" PRIMARY KEY ( "LFN" ) );

CREATE INDEX CMS_COND_GENERAL."POOL_FC_LFN_IDX" ON CMS_COND_GENERAL."POOL_FC_LFN" ( "FID" );

GRANT SELECT ON CMS_COND_GENERAL."POOL_FC_PFN" TO PUBLIC;

GRANT SELECT ON CMS_COND_GENERAL."POOL_FC_LFN" TO PUBLIC;
