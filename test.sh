clear 
echo "请输入文件名:"
read filename 
ls -l $filename

# 将文件信息定义为变量
h=$(ls -l $filename)

# 通过cut命令将文件信息中文件大小的字符部分进行截取
size=`echo $h | cut -d " " -f 5`
csize=`echo $h | cut -d " " -f 5`

# 记录文件保持不变以及变化的次数
nochange=0
change=0

# 当被检测的文件或者已累计改变了两次大小，或者已连续被检测了十次还未改变大小时就结束程序
while [ $change -lt 2 -a $nochange -lt 10 ]
do 
	echo "该文件原始大小为: $size "
	echo "该文件当前大小为: $csize"

	# 初始文件大和当前文件大小一致，说明文件没有变化
	if [ $size -eq $csize ]
	then 
		echo "检测到文件$filename的大小没有发生变化"
		nochange=`expr $nochange + 1`
		echo "文件$filename的大小保持不变的次数为: $nochange"
	# 文件大小发生变化
	else
		echo "检测到文件$filename的大小发生变化"
		change=`expr $change + 1`
		echo "文件$filename的大小发生变化的次数为: $change"
		
		#更新文件大小信息
		size=$csize
	fi
	h=$(ls -l $filename)
	csize=`echo $h | cut -d " " -f 5`
	sleep 5
done 

#判断程序结束的原因
if [ $change -eq 2]
then 
	clear
	echo "文件发生了两次改变，程序运行结束！"
else 
	clear
	echo "文件已累计10次没有发生改变，程序运行结束！"
fi
