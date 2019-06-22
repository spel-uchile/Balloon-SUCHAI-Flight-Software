#!/bin/bash
SUBMODULES=~/Spel/submodules
cd $SUBMODULES
#direwolf -t 0 -c aprs/aprs_telem.conf  > ~/Spel/mission-history/aprs-"$(date "+%FT%T")".log 2>&1
direwolf -t 0 -c aprs/aprs_telem.conf  2>&1 | tee ~/Spel/mission-history/aprs-"$(date "+%FT%T")".log
cd  ~/Spel
