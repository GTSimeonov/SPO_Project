#!/bin/sh
TEST_DIR=.
JID=$(echo $@ | sed 's/ /_/g')

touch "$TEST_DIR/logs/log[$JID]"
sh ./core_script.sh $@ | tee "$TEST_DIR/logs/log[$JID]"
tail ./times/normal/* -q -n 1 | cut -f 2 -d " " | awk 'BEGIN{printf "a=c(";cnt=1}{if(cnt!=1){printf ","}printf "%s",$1; ++cnt}END{print ")"}' > $TEST_DIR/Rstuff/[$JID].R
echo "plot(1:length(a),col='red',type='l');points(1:length(a),col='red');points(a/a[1],col='green');lines(a/a[1],col='green');" >> $TEST_DIR/Rstuff/[$JID].R

