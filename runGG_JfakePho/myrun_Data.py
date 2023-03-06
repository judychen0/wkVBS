#!/usr/bin/env python
import os, string
import sys

yearrun = raw_input("Year of run : ")
isMET = raw_input("Is MET data? (T=1:F=0): ")

if int(yearrun) == 2016:
    if int(isMET)==0: remotepath = "UL_30path/summer16_data.txt"
    else: remotepath = "UL_30path/summer16_data_MET.txt"
    yeardir = "summer16"
elif int(yearrun) == 2017:
    if int(isMET)==0: remotepath = "UL_30path/fall17_data.txt"
    else: remotepath = "UL_30path/fall17_data_MET.txt"
    #remotepath = "legacypath/fall17_data.txt"
    yeardir = "fall17"
elif int(yearrun) == 2018:
    if int(isMET)==0: remotepath = "UL_30path/autumn18_data.txt"
    else: remotepath = "UL_30path/autumn18_data_MET.txt"
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
        script += "workdir=/home/jou/wkVBS/work/runGG_JfakePho \n"
        script += "homedir=/home/jou/wkVBS/runGG_JfakePho \n"
        script += "mkdir /data1/GMET/Jfake_Data/"+str(yeardir)+"/"+str(dirname)+" \n"
        script += "returndir=/data1/GMET/Jfake_Data/"+str(yeardir)+"/"+str(dirname)+" \n"
        script += "datadir="+str(line)+" \n"
        script += "year="+str(yearrun)+" \n"
        script += "isMET="+str(isMET)+" \n"
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
        script += """/usr/bin/time parallel --progress --jobs 1 'root -L -l -b -q "xSkim_data.C+(\\\"{1}\\\", {2}, {3})"' :::: path.txt :::+ $year :::+ $isMET \n"""
        script += """/usr/bin/time parallel --progress --jobs 40 'root -L -l -b -q "xSkim_data.C+(\\\"{1}\\\", {2}, {3})"' :::: path.txt ::: $year :::+ $isMET \n"""
        script += "echo Processed all $count ggtrees!!! \n"
        script += "for i in {0..9}; do hadd output_$i.root Jfake_ggtree_*$i.root; done \n"
        script += "rm -r *ggtree*  \n"
        script += "hadd tmp1.root output_*{0..4}.root; hadd tmp2.root output_*{5..9}.root; rm -r output*; hadd output_ggtree.root tmp1.root tmp2.root \n"
        script += "cp output_ggtree.root $returndir \n"
        script += "thiswork=$(pwd) \n"
        script += "cd $workdir; rm -r $thiswork \n"

        frun = open("run"+str(yeardir)+"_"+str(isMET)+"_skimD.sh", "w")
        frun.write(script)
        frun.close()

        os.system( "chmod 744 run"+str(yeardir)+"_"+str(isMET)+"_skimD.sh" )
        os.system( "./run"+str(yeardir)+"_"+str(isMET)+"_skimD.sh" )
