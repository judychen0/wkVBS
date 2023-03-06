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
    if int(isUL) == 0: pupath = "PileupWeight/hoa_PU/summer16/PU_histo_13TeV_2016_GoldenJSON_69200nb.root"
    else: pupath = "PileupWeight/UL2UL_PU/summer16/PU_histo_13TeV_2016_GoldenJSON_69200nb.root"
    if int(isUL) == 0: IDSFpath = "LphoScaleFactors/2016_PhotonsMedium.root" # Legacy MC
    else:
        if int(confg) == 0: IDSFpath = "ULphoScaleFactors/2016_PhotonsMedium_ULSF_cutbase_preVFP.root" # UL MC
        else: IDSFpath = "ULphoScaleFactors/2016_PhotonsMedium_ULSF_cutbase_postVFP.root" # UL MC
    if int(isUL) == 0: PSVpath = "LphoScaleFactors/PixelSeed_ScaleFactors_2016.root" # Legacy MC
    else:
        if int(confg) == 0: PSVpath = "ULphoScaleFactors/HasPix_SummaryPlot_UL16_preVFP.root" # UL MC
        else: PSVpath = "ULphoScaleFactors/HasPix_SummaryPlot_UL16_postVFP.root" # UL MC
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
    if int(isUL) == 0: pupath = "PileupWeight/hoa_PU/fall17_corrected/PU_correctedPU_13TeV_2017_GoldenJSON_69200nb.root"
    else: pupath = "PileupWeight/UL2UL_PU/fall17/PU_histo_13TeV_2017_GoldenJSON_69200nb.root"
    if int(isUL) == 0: IDSFpath = "LphoScaleFactors/2017_PhotonsMedium.root" # Legacy MC
    else: IDSFpath = "ULphoScaleFactors/2017_PhotonsMedium_ULSF_cutbase.root" # UL MC
    if int(isUL) == 0: PSVpath = "LphoScaleFactors/PixelSeed_ScaleFactors_2017.root" # Legacy MC
    else: PSVpath = "ULphoScaleFactors/HasPix_SummaryPlot_UL17.root" # UL MC  
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
    if int(isUL) == 0: pupath = "PileupWeight/hoa_PU/autumn18/PU_histo_13TeV_2018_GoldenJSON_69200nb.root"
    else: pupath = "PileupWeight/UL2UL_PU/autumn18/PU_histo_13TeV_2018_GoldenJSON_69200nb.root"
    if int(isUL) == 0: IDSFpath = "LphoScaleFactors/2018_PhotonsMedium.root" # Legacy MC
    else: IDSFpath = "ULphoScaleFactors/2018_PhotonsMedium_ULSF_cutbase.root" # UL MC
    if int(isUL) == 0: PSVpath = "LphoScaleFactors/PixelSeed_ScaleFactors_2018.root" # Legacy MC
    else: PSVpath = "ULphoScaleFactors/HasPix_SummaryPlot_UL18.root" # UL MC

if int(isSM) == 0: folder = "minitree_MC"
else: folder = "minitreeSM_MC"

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
        script += "mkdir /data1/GMET/"+str(folder)+"/"+str(yeardir)+"/"+str(dirname)+" \n"
        script += "returndir=/data1/GMET/"+str(folder)+"/"+str(yeardir)+"/"+str(dirname)+" \n"
        script += "datadir="+str(line)+" \n"
        script += "PUpath="+str(pupath)+" \n"
        script += "IDSFpath="+str(IDSFpath)+" \n"
        script += "PSVpath="+str(PSVpath)+" \n"
        script += "isdcount="+str(isDcount)+" \n"
        script += "isSig="+str(isSignal)+" \n"
        script += "year="+str(yearrun)+" \n"
        script += "isUL="+str(isUL)+" \n"
        script += "isSM="+str(isSM)+" \n"
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
        script += """/usr/bin/time parallel --progress --jobs 1 'root -L -l -b -q "xSkim_mc.C+(\\\"{1}\\\", \\\"{2}\\\", \\\"{3}\\\", \\\"{4}\\\", {5}, {6}, {7}, {8}, {9})"' :::: path.txt :::+ $PUpath :::+ $IDSFpath :::+ $PSVpath :::+ $isdcount :::+ $year :::+ $isSig :::+ $isUL :::+ $isSM \n"""
        script += """/usr/bin/time parallel --progress --jobs 40 'root -L -l -b -q "xSkim_mc.C+(\\\"{1}\\\", \\\"{2}\\\", \\\"{3}\\\", \\\"{4}\\\", {5}, {6}, {7}, {8}, {9})"' :::: path.txt ::: $PUpath ::: $IDSFpath ::: $PSVpath ::: $isdcount ::: $year ::: $isSig :::+ $isUL :::+ $isSM \n"""
        script += "echo Processed all $count ggtrees!!! \n"
        script += "cp mini_ggtree_mc* $returndir \n"
        script += "thiswork=$(pwd) \n"
        script += "cd $workdir; rm -r $thiswork \n"

        frun = open("run"+str(yeardir)+"_skim_"+str(isSM)+"_"+str(isDcount)+"_"+str(isSignal)+"_"+str(isUL)+"_"+".sh", "w")
        frun.write(script)
        frun.close()

        os.system( "chmod 744 run"+str(yeardir)+"_skim_"+str(isSM)+"_"+str(isDcount)+"_"+str(isSignal)+"_"+str(isUL)+"_"+".sh" )
        os.system( "./run"+str(yeardir)+"_skim_"+str(isSM)+"_"+str(isDcount)+"_"+str(isSignal)+"_"+str(isUL)+"_"+".sh" )
