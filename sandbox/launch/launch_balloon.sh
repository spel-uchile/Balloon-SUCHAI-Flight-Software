#!/bin/bash
FLIGHTSOFTWARE=~/Spel/Balloon-SUCHAI-Flight-Software/build
cd $FLIGHTSOFTWARE
./SUCHAI_Flight_Software >  ~/Spel/mission-history/flight_software-"$(date "+%FT%T")".log 2>&1 &

cd ../sandbox
python3 zmqhub.py --mon  > ~/Spel/mission-history/zmq_hub-"$(date "+%FT%T")".log 2>&1 &

SUBMODULES=~/Spel/submodules/
cd $SUBMODULES
python3 bmp/bmp_com.py  > ~/Spel/mission-history/bmp_com-"$(date "+%FT%T")".log 2>&1 &
python dpl/dpl_com2.py > ~/Spel/mission-history/dpl_com-"$(date "+%FT%T")".log 2>&1 &
python gps/gps_com.py > ~/Spel/mission-history/gps_com-"$(date "+%FT%T")".log 2>&1 &
#python3 imet-xq/imet-xq_com.py > ~/Spel/mission-history/imet_com-"$(date "+%FT%T")".log 2>&1 &
#python3 iridium/ird_com.py > ~/Spel/mission-history/ird_com-"$(date "+%FT%T")".log 2>&1 &
python3 sstv/sstv_com.py > ~/Spel/mission-history/sstv_com-"$(date "+%FT%T")".log 2>&1 &
python xbee/xbee_com.py > ~/Spel/mission-history/xbee_com-"$(date "+%FT%T")".log 2>&1 &

cd  ~/Spel
