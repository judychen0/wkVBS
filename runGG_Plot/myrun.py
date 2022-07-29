#!/usr/bin/env python
import os, string
import sys

yearrun = raw_input("Year of run : ")
isData = 0
isUL = raw_input("Is UL? (T==1; F==0) : ")

#if int(yearrun) == 2016:
#    if int(isData) == 1: remotepath = "dataMC_path/summer16_data.txt"
#    if int(isUL) == 1: remotepath = "dataMC_path/summer16_mc.txt"
#    if int(isDcount) == 0 and int(isData) == 0 and int(isUL) == 0: remotepath = "dataMC_path/summer16_bkg.txt"
#    yeardir = "summer16"
if int(yearrun) == 2017:
    if int(isData) == 1: remotepath = "dataMC_path/fall17_data.txt"
    if int(isUL) == 1: remotepath = "dataMC_path/fall17_mc.txt"
    if int(isData) == 0 and int(isUL) == 0: remotepath = "dataMC_path/fall17_bkg.txt"
    yeardir = "fall17"
elif int(yearrun) == 2018:
    if int(isData) == 1: remotepath = "dataMC_path/autumn18_data.txt"
    if int(isUL) == 1: remotepath = "dataMC_path/autumn18_mc.txt"
    if int(isData) == 0 and int(isUL) == 0: remotepath = "dataMC_path/autumn18_bkg.txt"
    yeardir = "autumn18"
    

with open(remotepath, "r") as filepath:
    path = filepath.read().splitlines()
    for line in path:
        alist = line.split("/")
        namelist = []
        namelist.append(alist[-2])
        dirname = "".join(namelist)#string without brackets
        
        script = ""
        script += "#!/bin/bash \n"
        script += "\n"
        script += "workdir=/home/jou/wkVBS/work/runGG_Plot \n"
        script += "homedir=/home/jou/wkVBS/runGG_Plot \n"
        script += "mkdir /data1/GMET/ana/output_VBS/"+str(yeardir)+"/"+str(dirname)+" \n"
        script += "returndir=/data1/GMET/ana/output_VBS/"+str(yeardir)+"/"+str(dirname)+" \n"
        script += "datadir="+str(line)+" \n"
        if int(isUL) == 1: script += "isUL="+str(isUL)+" \n"
        else: script += "isUL=0 \n"
        script += "year="+str(yearrun)+" \n"
        script += "isData="+str(isData)+" \n"
        script += "\n"
        script += "cd $workdir \n"
        script += "mkdir "+str(dirname)+" \n"
        script += "cd "+str(dirname)+" \n"
        script += "cp -r $homedir/* . \n"
        script += "\n"
        script += "realpath $datadir/*.root > path.txt \n"
        script += "count=$(ls $datadir | wc -l) \n"
        script += """echo "There are tot $count ggtrees!!!" \n"""
        script += "\n"
        script += """/usr/bin/time parallel --progress --jobs 1 'root -L -l -b -q "xZg.C+(\\\"{1}\\\", {2})"' :::: path.txt :::+ $isUL \n"""
        script += """/usr/bin/time parallel --progress --jobs 45 'root -L -l -b -q "xZg.C+(\\\"{1}\\\", {2})"' :::: path.txt ::: $isUL \n"""
        script += "echo Processed all $count ggtrees!!! \n"
        script += "for i in {0..9}; do hadd output_$i.root output_ggtree_*$i.root; done \n"
        script += "rm -r *ggtree*  \n"
        script += "hadd tmp1.root output_*{0..4}.root; hadd tmp2.root output_*{5..9}.root; rm -r output*; hadd output_ggtree.root tmp1.root tmp2.root \n"
        script += "cp output_ggtree.root $returndir \n"
        script += "thiswork=$(pwd) \n"
        script += "cd $workdir; rm -r $thiswork \n"
        
        frun = open("run"+str(yeardir)+"_Zg_"+str(isData)+"_"+str(isUL)+".sh", "w")
        frun.write(script)
        frun.close()
        
        os.system( "chmod 744 run"+str(yeardir)+"_Zg_"+str(isData)+"_"+str(isUL)+".sh" )
        os.system( "./run"+str(yeardir)+"_Zg_"+str(isData)+"_"+str(isUL)+".sh")


