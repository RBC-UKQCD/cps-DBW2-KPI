#!/bin/bash

. conf.sh

name=cps

echo "!!!! build CPS !!!!"

cps="$(cd $cps; pwd)"
echo "Real location is : ""$cps"

rm -rf $cps/build* >/dev/null 2>&1 || true
rm $wd/log >/dev/null 2>&1 || true

mkdir -p $cps || true
cd "$cps"

export TIMEFORMAT="%R secs"
#export LD_LIBRARY_PATH=/usr/local/lib:${LD_LIBRARY_PATH}
#export PPCF=/bgsys/drivers/ppcfloor
#export MPICH_CC=${PPCF}/gnu-linux-4.7.2/bin/powerpc64-bgq-linux-gcc
#export MPICH_CXX=${PPCF}/gnu-linux-4.7.2/bin/powerpc64-bgq-linux-g++

profile () {
    echo
    echo "$@"
    time { "$@" >>$wd/log 2>&1 ; }
}

cd "$cps"

profile rsync -avz --delete $wd/cps_pp/ ./cps_pp-qmp/
cd cps_pp-qmp
profile autoconf
cd "$cps"
mkdir build-qmp
cd build-qmp
profile ../cps_pp-qmp/configure --build=powerpc64-unknown-linux-gnu --host=powerpc64-bgq-linux --enable-target=bgq --enable-qmp=$prefix --enable-qio --enable-sse=no --enable-debug=no
profile make -j60
cd "$cps"

echo
cd $wd
echo "!!!! CPS build !!!!"

rm -rf $temp_dir
