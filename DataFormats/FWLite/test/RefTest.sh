#!/bin/sh
rm -f ${LOCAL_TEST_DIR}/good.root ${LOCAL_TEST_DIR}/good2.root ${LOCAL_TEST_DIR}/empty.root
rm -f good.root good2.root empty.root
cmsRun ${LOCAL_TEST_DIR}/RefTest_cfg.py
cmsRun ${LOCAL_TEST_DIR}/RefTest2_cfg.py
cmsRun ${LOCAL_TEST_DIR}/EmptyFile_cfg.py
