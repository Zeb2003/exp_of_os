#! /usr/bin/bash

clear


if [ ! $1 ]
then
echo "Usage: test5 username"
exit 1
fi

echo "The user you are searching for: [$1]"
echo "List of users already login in"

who > name1.txt
awk '{print $1 "\t"}' name1.txt
awk '{print $1 "\t"}' name1.txt > name2.txt

while read line
do
		if [ $1 = $line ]
				then
				echo "user [$1] have already logged in"
				exit

		fi
done < name2.txt

while true
do 
		echo "Waiting for user [$1] ..."
		who > name1.txt
		awk '{print $1 "\t"}' name1.txt > name2.txt
		while read line		
		do
        		if [ $1 = $line ]
                		then
                		echo "user [$1] have already logged in"
                		exit
				fi
		done < name2.txt
		sleep 5
done
