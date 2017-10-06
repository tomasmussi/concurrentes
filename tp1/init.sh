

for i in `seq 1 10`; do
	cmd=$(echo kill -10 $1)
	echo $cmd
	kill -10 $1
done

