#!/bin/bash

#rocksteady:
echo "Rocksteady:"
ssh rocksteady df | grep 'lv' | awk '{print $5}'

#sardukar:
echo "Sardukar:"
ssh sardukar df | grep 'vg00-root' | awk '{print $5}'

#Orion:
echo "Orion:"
ssh orion df | grep '/dev/md127' | awk '{print $5}'
