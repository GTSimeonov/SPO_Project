#!/bin/sh

MATRIX_SIZE=100
TO_THREADS=4
SOURCE_DIR=..
TEST_DIR=.
FROM_THREADS=1
ANSW="g"

#1-st argument is the size of the matrix. If ommited then it's 100
[ $1 ] && [ $1 -ge 0 ] && MATRIX_SIZE=$1
[ $2 ] && [ $2 -ge 1 ] && FROM_THREADS=$2
[ $3 ] && [ $3 -ge $FROM_THREADS ] && TO_THREADS=$3

nxn="$MATRIX_SIZE\\$MATRIX_SIZE"

#make the executable
make -C $SOURCE_DIR || (echo "ERROR: COULD NOT MAKE THE EXECUTABLE" ; exit -1) || exit

#move it to testing grounds
mv $SOURCE_DIR/main.exe $TEST_DIR/executables/main.exe 

#along the heavy line generator
mv $SOURCE_DIR/make_heavy.exe $TEST_DIR/executables/make_heavy.exe 

#make matrix, dump it in a file and then execute over one thread
if [ -e $TEST_DIR/inputs/matrix$nxn ]
then
	while [ $ANSW != "y" ] && [ $ANSW != "n" ]
	do
		echo "Testing on a matrix size that has been tested already. Run tests again[y/n]?"
		read ANSW
	done
	[ $ANSW = "n" ] && exit
	ANSW="g"
	while [ $ANSW != "y" ] && [ $ANSW != "n" ]
	do
		echo "Generate new matrix?[y/n]?"
		read ANSW
	done

	echo "Testing on $FROM_THREADS thread(s)"
	[ $ANSW = "n" ] && $TEST_DIR/executables/main.exe -t $FROM_THREADS -i $TEST_DIR/inputs/matrix$nxn -o $TEST_DIR/outputs/output$nxn > $TEST_DIR/times/normal/[$nxn]$FROM_THREADS.time 
	[ $ANSW = "y" ] && $TEST_DIR/executables/main.exe -t $FROM_THREADS -dm $TEST_DIR/inputs/matrix$nxn -o $TEST_DIR/outputs/output$nxn -n $MATRIX_SIZE > $TEST_DIR/times/normal/[$nxn]$FROM_THREADS.time 
else
	echo "Testing on $FROM_THREADS thread(s)"
	$TEST_DIR/executables/main.exe -t $FROM_THREADS -dm $TEST_DIR/inputs/matrix$nxn -o $TEST_DIR/outputs/output$nxn -n $MATRIX_SIZE > $TEST_DIR/times/normal/[$nxn]$FROM_THREADS.time 
fi

cat $TEST_DIR/times/normal/[$nxn]$FROM_THREADS.time | tail -n 1 

#execute over 2 or more threads using the dumped matrix (reusing it so we don't have misleading results)
i=$((FROM_THREADS+1))
while [ $i -le $TO_THREADS ]
do
	echo "Testing on $i threads" 
	$TEST_DIR/executables/main.exe -t $i -i $TEST_DIR/inputs/matrix$nxn -o $TEST_DIR/outputs/temp_output$nxn > $TEST_DIR/times/normal/[$nxn]$i.time 

	#check for consistency between outputs
	[ $(cat $TEST_DIR/outputs/output$nxn) != $(cat $TEST_DIR/outputs/temp_output$nxn) ] && echo "ERROR OUTPUTS DO NOT MATCH" && exit

	rm $TEST_DIR/outputs/temp_output$nxn

	#print time it took for $i threads to do the task
	cat $TEST_DIR/times/normal/[$nxn]$i.time | tail -n 1 

	i=$((i+1))
done

#test over a matrix with a heavy row (This should be the worst case for the algorithm)
#make a matrix with a really heavy (MAXTHREADS - 1 to last ) row
echo "$MATRIX_SIZE $((MATRIX_SIZE - MAX_THREADS+1))" | $TEST_DIR/executables/make_heavy.exe > $TEST_DIR/inputs/matrix$nxn.heavy

#do not dump the matrix this time
#heavy matrices are very alike - do not prompt to keep matrice

echo "[HEAVY]Testing on $FROM_THREADS thread(s)"
$TEST_DIR/executables/main.exe -t $FROM_THREADS -i $TEST_DIR/inputs/matrix$nxn.heavy -o $TEST_DIR/outputs/output$nxn.heavy > $TEST_DIR/times/heavy/[$nxn]$FROM_THREADS.time 

cat $TEST_DIR/times/heavy/[$nxn]$FROM_THREADS.time | tail -n 1 

#execute over 2 or more threads using the dumped matrix (reusing it so we don't have misleading results)
i=$((FROM_THREADS+1))
while [ $i -le $TO_THREADS ]
do
	echo "[HEAVY]Testing on $i threads" 
	$TEST_DIR/executables/main.exe -t $i -i $TEST_DIR/inputs/matrix$nxn.heavy -o $TEST_DIR/outputs/temp_output$nxn.heavy > $TEST_DIR/times/heavy/[$nxn]$i.time 

	#check for consistency between outputs
	[ $(cat $TEST_DIR/outputs/output$nxn.heavy) != $(cat $TEST_DIR/outputs/temp_output$nxn.heavy) ] && echo "ERROR OUTPUTS DO NOT MATCH" && exit

	rm $TEST_DIR/outputs/temp_output$nxn.heavy

	#print time it took for $i threads to do the task
	cat $TEST_DIR/times/heavy/[$nxn]$i.time | tail -n 1 

	i=$((i+1))
done

exit 

