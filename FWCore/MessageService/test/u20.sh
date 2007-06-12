#!/bin/bash

pushd $LOCAL_TMP_DIR

status=0
  
rm -f u20_cerr.log u20_cout.log FrameworkJobReport.xml

cmsRun -e -p $LOCAL_TEST_DIR/u20.cfg 1> u20_cout.log 2> u20_cerr.log
 
for file in u20_cerr.log u20_cout.log FrameworkJobReport.xml   
do
  diff $LOCAL_TEST_DIR/unit_test_outputs/$file $LOCAL_TMP_DIR/$file  
  if [ $? -ne 0 ]  
  then
    echo The above discrepancies concern $file 
    status=1
  fi
done

popd

exit $status
