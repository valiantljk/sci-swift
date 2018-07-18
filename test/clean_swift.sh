#!/bin/bash
# swift -A https://ngfobjtest.nersc.gov:5443/v3 -U $LOGNAME -K $NIMPW -a
swift -A https://ngfobjtest.nersc.gov:5443/v3 -U $LOGNAME -K $NIMPW list >> .tmp_list
while read p; do
  swift -A https://ngfobjtest.nersc.gov:5443/v3 -U $LOGNAME -K $NIMPW delete $p 
done < .tmp_list
rm .tmp_list
