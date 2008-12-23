#! /bin/sh
# CRAB related Stuff
export CRABDIR=/uscms/home/wclarida/CRAB
export EXTERNALDIR=/uscms/home/wclarida/CRAB/external

CRABPATH=${CRABDIR}/python
CRABDLSAPIPATH=${EXTERNALDIR}/DLSAPI/bin
export CRABPYTHON=${CRABDIR}/python
export CRABDBSAPIPYTHON=${EXTERNALDIR}/DBSAPI
export CRABDLSAPIPYTHON=${EXTERNALDIR}/DLSAPI/lib
export CRABPRODCOMMONPYTHON=${EXTERNALDIR}


if [ -z "$PATH" ]; then
export PATH=${CRABPATH}:${CRABDLSAPIPATH}
else
export PATH=${CRABPATH}:${CRABDLSAPIPATH}:${PATH}
fi
if [ -z "$PYTHONPATH" ]; then
export PYTHONPATH=${CRABPYTHON}:${CRABDBSAPIPYTHON}:${CRABDLSAPIPYTHON}:${CRABPRODCOMMONPYTHON}
else
export PYTHONPATH=${PYTHONPATH}:${CRABPYTHON}:${CRABDBSAPIPYTHON}:${CRABDLSAPIPYTHON}:${CRABPRODCOMMONPYTHON}
fi


## sqlite
export CRABSQLITE=${EXTERNALDIR}/sqlite/
export CRABSQLITE_VERSION="3.3.5"
export CRABSQLITE_ROOT=$CRABSQLITE/$CRABSQLITE_VERSION

export PATH=${CRABSQLITE_ROOT}/bin:${PATH}
export LD_LIBRARY_PATH=${CRABSQLITE_ROOT}/lib:${LD_LIBRARY_PATH}


## py2sql
export CRABPYSQLITE=${EXTERNALDIR}/py2-pysqlite
export CRABPY2_PYSQLITE_VERSION="2.3.2"
export CRABPY2_PYSQLITE_REVISION="1008"
export CRABPY2_PYSQLITE_ROOT=$CRABPYSQLITE/$CRABPY2_PYSQLITE_VERSION

export LD_LIBRARY_PATH=$CRABPY2_PYSQLITE_ROOT/lib:$LD_LIBRARY_PATH

export PYTHON_VERSION=2.3.4 
export PYTHONPATH=${CRABPY2_PYSQLITE_ROOT}/lib/python2.4/site-packages:$PYTHONPATH
## partially hardcoded path for python version "2.4"
## need to do something like:
#export PYTHONPATH=${CRABPY2_PYSQLITE_ROOT}/lib/python/site-packages:${PYTHONPATH}

## pyOpenssl
export PyOpenSSLPYTHON=${EXTERNALDIR}
export PYTHONPATH=${PYTHONPATH}:${PyOpenSSLPYTHON}

