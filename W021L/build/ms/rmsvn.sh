#!/usr/bin/sh
#write by aming 2010-0915
#���ļ�copy��cygwin��binĿ¼�£���sh rmsvn.shִ��ʵ�ֵݹ�ɾ��svn����Ϣ
ScanFolder()
{
	CURDIR=`pwd`
	FileName=`ls -a`
#	echo "current folder is $CURDIR"
	for file in $FileName
	do
		if [ $file = "." -o $file = ".." ];then
			continue
		fi
		
		Suffix=`echo $file | sed 's!.*\.!!'`
		if [ $Suffix = "svn" ];then
			rm -Rf $file
			echo "Delete svn folder $file"
		elif [ -d $file ];then
			echo "Enter folder $file"
			cd $file
			ScanFolder 
			cd ..		
		else
		:				
		fi
	done
}			

ScanFolder