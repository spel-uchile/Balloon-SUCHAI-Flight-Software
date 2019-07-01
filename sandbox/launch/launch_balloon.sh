#!/bin/bash
FLIGHTSOFTWARE=~/Spel/Balloon-SUCHAI-Flight-Software/build
cd $FLIGHTSOFTWARE
./SUCHAI_Flight_Software >  ~/Spel/mission-history/flight_software-"$(date "+%FT%T")".log 2>&1 &

cd ../sandbox
/usr/bin/python3 zmqhub.py --mon  > ~/Spel/mission-history/zmq_hub-"$(date "+%FT%T")".log 2>&1 &

SUBMODULES=~/Spel/submodules/
cd $SUBMODULES
/usr/bin/python3 bmp/bmp_com.py  > ~/Spel/mission-history/bmp_com-"$(date "+%FT%T")".log 2>&1 &
/usr/bin/python2 dpl/dpl_com2.py > ~/Spel/mission-history/dpl_com-"$(date "+%FT%T")".log 2>&1 &
/usr/bin/python2 gps/gps_com.py > ~/Spel/mission-history/gps_com-"$(date "+%FT%T")".log 2>&1 &
#python3 imet-xq/imet-xq_com.py > ~/Spel/mission-history/imet_com-"$(date "+%FT%T")".log 2>&1 &
#python3 iridium/ird_com.py > ~/Spel/mission-history/ird_com-"$(date "+%FT%T")".log 2>&1 &
#/usr/bin/python3 sstv/sstv_com.py > ~/Spel/mission-history/sstv_com-"$(date "+%FT%T")".log 2>&1 &
/usr/bin/python2 xbee/xbee_com.py > ~/Spel/mission-history/xbee_com-"$(date "+%FT%T")".log 2>&1 &
/home/pi/miniconda3/envs/python_eclipse/bin/python eclipse_payload/eclipse_deps/eclipse_payload.py > ~/Spel/mission-history/eclipse-"$(date "+%FT%T")".log 2>&1 &

cd  ~/Spel
