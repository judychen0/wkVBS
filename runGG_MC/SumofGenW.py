#!/usr/bin/env python
import os, string
import sys

yearrun = raw_input("Year of run : ")
if int(yearrun) == 2016: confg = raw_input("VFP type (preVFP==0; postVFP==1): ")
isSM = raw_input("SM samples? (T==1; F==0): ")
isDcount = raw_input("Is VJet samples: (T==1; F==0): ")
isSignal = raw_input("Is Signal? (T==1; F==0): ")
isUL = raw_input("Is ULsamples? (T==1; F==0): ")

if int(yearrun) == 2016:
    if int(isSM)==0:
        if int(isUL)==0:
            if int(isDcount)==0: remotepath = "bkg_path/summer16_bkg.txt"
            else: remotepath = "VJet_path/summer16_VJet.txt"
        else:
            if int(isSignal)==0:
                if int(confg)==0: remotepath = "VGJet_path/summer16_preVFP_VGJet.txt"
                else: remotepath = "VGJet_path/summer16_postVFP_VGJet.txt"
            else: remotepath = "VGJet_path/summer16_mc.txt"
    else:
        if int(isUL)==0:
            if int(isDcount)==0: remotepath = "bkg_path/summer16_bkgSM.txt"
            else: remotepath = "VJet_path/summer16_VJet.txt"
            if int(isSignal)==1: remotepath = "VJet_path/summer16_mcSM.txt"
        else:
            if int(confg) == 0: remotepath = "VGJet_path/summer16_preVFP_VGJet.txt"
            else: remotepath = "VGJet_path/summer16_postVFP_VGJet.txt"
    yeardir = "summer16"
elif int(yearrun) == 2017:
    if int(isSM)==0:
        if int(isUL)==0:
            if int(isDcount)==0: remotepath = "bkg_path/fall17_bkg.txt"
            else: remotepath = "VJet_path/fall17_VJet.txt"
        else:
            if int(isSignal)==0: remotepath = "VGJet_path/fall17_VGJet.txt"
            else: remotepath = "VGJet_path/fall17_mc.txt"
    else:
        if int(isUL)==0:
            if int(isDcount)==0: remotepath = "bkg_path/fall17_bkgSM.txt"
            else: remotepath = "VJet_path/fall17_VJet.txt"
            if int(isSignal)==1: remotepath = "VJet_path/fall17_mcSM.txt"
        else: remotepath = "VGJet_path/fall17_VGJet.txt"
    yeardir = "fall17"
elif int(yearrun) == 2018:
    if int(isSM)==0:
        if int(isUL)==0:
            if int(isDcount)==0: remotepath = "bkg_path/autumn18_bkg.txt"
            else: remotepath = "VJet_path/autumn18_VJet.txt"
        else:
            if int(isSignal)==0: remotepath = "VGJet_path/autumn18_VGJet.txt"
            else: remotepath = "VGJet_path/autumn18_mc.txt"
    else:
        if int(isUL)==0:
            if int(isDcount)==0: remotepath = "bkg_path/autumn18_bkgSM.txt"
            else: remotepath = "VJet_path/autumn18_VJet.txt"
            if int(isSignal)==1: remotepath = "VJet_path/autumn18_mcSM.txt"
        else: remotepath = "VGJet_path/autumn18_VGJet.txt"
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
        script += "workdir=/home/jou/wkVBS/work/runGG_MC \n"
        script += "homedir=/home/jou/wkVBS/runGG_MC \n"
        script += "mkdir /data1/GMET/minitree_MC/"+str(yeardir)+"/histo/"+str(dirname)+" \n"
        script += "returndir=/data1/GMET/minitree_MC/"+str(yeardir)+"/histo/"+str(dirname)+" \n"
        script += "datadir="+str(line)+" \n"
        script += "\n"
        script += "cd $workdir \n"
        script += "mkdir "+str(dirname)+" \n"
        script += "cd "+str(dirname)+" \n"
        script += "cp -r $homedir/* . \n"
        script += "\n"
        script += "count=$(ls $datadir | wc -l) \n"
        script += """echo "There are tot $count ggtrees!!!" \n"""
        script += "\n"
        script += "hadd -T histo.root $datadir/* \n"
        script += "echo Processed all $count ggtrees!!! \n"
        script += "cp histo.root $returndir \n"
        script += "thiswork=$(pwd) \n"
        script += "cd $workdir; rm -r $thiswork \n"
        script += " \n"

        frun = open("run"+str(yeardir)+"_"+str(isSM)+"_"+str(isDcount)+"_"+str(isSignal)+"_"+str(isUL)+"_hadd.sh", "w")
        frun.write(script)
        frun.close()

        os.system( "chmod 744 run"+str(yeardir)+"_"+str(isSM)+"_"+str(isDcount)+"_"+str(isSignal)+"_"+str(isUL)+"_hadd.sh" )
        os.system( "./run"+str(yeardir)+"_"+str(isSM)+"_"+str(isDcount)+"_"+str(isSignal)+"_"+str(isUL)+"_hadd.sh" )
