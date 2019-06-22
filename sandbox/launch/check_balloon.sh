#!/bin/bash
dpl_id=$(ps aux | grep dpl | grep -v grep | awk '{print $2}')
bmp_id=$(ps aux | grep bmp | grep -v grep | awk '{print $2}')
gps_id=$(ps aux | grep gps | grep -v grep | awk '{print $2}')
ird_id=$(ps aux | grep ird | grep -v grep | awk '{print $2}')
imet_id=$(ps aux | grep imet | grep -v grep | awk '{print $2}')
sstv_id=$(ps aux | grep sstv | grep -v grep | awk '{print $2}')
hub_id=$(ps aux | grep hub | grep -v grep | awk '{print $2}')
suchai_id=$(ps aux | grep SUCHAI | grep -v grep | awk '{print $2}')
direwolf_id=$(ps aux | grep direwolf | grep -v grep | awk '{print $2}')

echo dpl_id = $dpl_id
echo bmp_id = $bmp_id
echo gps_id = $gps_id
echo ird_id = $ird_id
echo imet_id = $imet_id
echo sstv_id = $sstv_id
echo hub_id = $hub_id
echo suchai_id = $suchai_id
echo direwolf_id= $direwolf_id

