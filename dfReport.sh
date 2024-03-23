USAGE=$(df | grep 'lv' | awk '{print $5}')
#echo $USAGE
ssh meteopi "echo $USAGE > /home/robotto/gauges/rocksteadyReport"
