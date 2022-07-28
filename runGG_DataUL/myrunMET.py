#!/usr/bin/env python
import os, string
import sys

yearrun = raw_input("Year of run : ")

if int(yearrun) == 2016:
    remotepath = "./summer16_data.txt"
    yeardir = "summer16"
elif int(yearrun) == 2017:
    remotepath = "./fall17_VJet.txt"
    yeardir = "fall17"
elif int(yearrun) == 2018:
    remotepath = "./autumn18_VJet.txt"
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
        script += "workdir=/home/jou/work/runGG \n"
        script += "homedir=/home/jou/runGG \n"
        script += "mkdir /home/jou/output/"+str(yeardir)+"/MET/"+str(dirname)+" \n"
        script += "returndir=/home/jou/output/"+str(yeardir)+"/MET/"+str(dirname)+" \n"
        script += "datadir="+str(line)+" \n"
        script += "year="+str(yearrun)+" \n"
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
        script += """/usr/bin/time parallel --progress --jobs 1 'root -l -b -q "xZgMET.C+(\\\"{1}\\\")"' :::: path.txt :::+ $year  \n"""
        script += """/usr/bin/time parallel --progress --jobs 30 'root -l -b -q "xZgMET.C+(\\\"{1}\\\")"' :::: path.txt  \n"""
        script += "echo Processed all $count ggtrees!!! \n"
        script += "for i in {0..9}; do hadd METcorr_$i.root METcorr_ggtree_*$i.root; done \n"
        script += "rm -r *ggtree*  \n"
        script += "hadd tmp1.root METcorr_*{0..4}.root; hadd tmp2.root METcorr_*{5..9}.root; rm -r MET*; hadd METcorr_ggtree.root tmp1.root tmp2.root \n"
        script += "cp METcorr_ggtree.root $returndir \n"
        script += "thiswork=$(pwd) \n"
        script += "cd $workdir; rm -r $thiswork \n"
        
        frun = open("run"+str(yeardir)+"_ZgMET.sh", "w")
        frun.write(script)
        frun.close()
        
        os.system( "chmod 744 run"+str(yeardir)+"_ZgMET.sh" )
        os.system( "./run"+str(yeardir)+"_ZgMET.sh")
