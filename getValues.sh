#!/bin/bash

#rocksteady:
echo "Rocksteady:"
ssh robotto@rocksteady.moore.dk -p 222 df | grep 'lv' | awk '{print $5}'

#sardukar:
echo "Sardukar:"
ssh robotto@sardukar.moore.dk -p 222 df | grep 'vg00-root' | awk '{print $5}'
