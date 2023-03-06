#!/usr/bin/env python
import os, string
import sys

yearrun = raw_input("Year of run : ")
if int(yearrun) == 2016: confg = raw_input("VFP type (preVFP==0; postVFP==1): ")
isDcount = 0
isSignal = 0
isUL = 1

if int(yearrun) == 2016:
    if int(isSignal) == 1: remotepath = "VGJet_path/summer16_mc.txt"
    else:
        if int(confg) == 0: remotepath = "VGJet_path/summer16_preVFP_VGJet_True.txt"
        else: remotepath = "VGJet_path/summer16_postVFP_VGJet_True.txt"
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
    if int(isSignal) == 1: remotepath = "VGJet_path/fall17_mc.txt"
    else: remotepath = "VGJet_path/fall17_VGJet_True.txt"    
    yeardir = "fall17"
    if int(isUL) == 0: pupath = "PileupWeight/hoa_PU/fall17_corrected/PU_correctedPU_13TeV_2017_GoldenJSON_69200nb.root"
    else: pupath = "PileupWeight/UL2UL_PU/fall17/PU_histo_13TeV_2017_GoldenJSON_69200nb.root"
    if int(isUL) == 0: IDSFpath = "LphoScaleFactors/2017_PhotonsMedium.root" # Legacy MC
    else: IDSFpath = "ULphoScaleFactors/2017_PhotonsMedium_ULSF_cutbase.root" # UL MC
    if int(isUL) == 0: PSVpath = "LphoScaleFactors/PixelSeed_ScaleFactors_2017.root" # Legacy MC
    else: PSVpath = "ULphoScaleFactors/HasPix_SummaryPlot_UL17.root" # UL MC  
elif int(yearrun) == 2018:
    if int(isSignal) == 1: remotepath = "VGJet_path/autumn18_mc.txt"
    else: remotepath = "VGJet_path/autumn18_VGJet_True.txt"    
    yeardir = "autumn18"
    if int(isUL) == 0: pupath = "PileupWeight/hoa_PU/autumn18/PU_histo_13TeV_2018_GoldenJSON_69200nb.root"
    else: pupath = "PileupWeight/UL2UL_PU/autumn18/PU_histo_13TeV_2018_GoldenJSON_69200nb.root"
    if int(isUL) == 0: IDSFpath = "LphoScaleFactors/2018_PhotonsMedium.root" # Legacy MC
    else: IDSFpath = "ULphoScaleFactors/2018_PhotonsMedium_ULSF_cutbase.root" # UL MC
    if int(isUL) == 0: PSVpath = "LphoScaleFactors/PixelSeed_ScaleFactors_2018.root" # Legacy MC
    else: PSVpath = "ULphoScaleFactors/HasPix_SummaryPlot_UL18.root" # UL MC

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
        script += "mkdir /data1/GMET/Jfake_MC/"+str(yeardir)+"/"+str(dirname)+" \n"
        script += "returndir=/data1/GMET/Jfake_MC/"+str(yeardir)+"/"+str(dirname)+" \n"
        script += "datadir="+str(line)+" \n"
        script += "PUpath="+str(pupath)+" \n"
        script += "IDSFpath="+str(IDSFpath)+" \n"
        script += "PSVpath="+str(PSVpath)+" \n"
        script += "isdcount="+str(isDcount)+" \n"
        script += "isSig="+str(isSignal)+" \n"
        script += "year="+str(yearrun)+" \n"
        script += "isUL="+str(isUL)+" \n"
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
        script += """/usr/bin/time parallel --progress --jobs 1 'root -L -l -b -q "xSkim_mc.C+(\\\"{1}\\\", \\\"{2}\\\", \\\"{3}\\\", \\\"{4}\\\", {5}, {6}, {7}, {8})"' :::: path.txt :::+ $PUpath :::+ $IDSFpath :::+ $PSVpath :::+ $isdcount :::+ $year :::+ $isSig :::+ $isUL \n"""
        script += """/usr/bin/time parallel --progress --jobs 30 'root -L -l -b -q "xSkim_mc.C+(\\\"{1}\\\", \\\"{2}\\\", \\\"{3}\\\", \\\"{4}\\\", {5}, {6}, {7}, {8})"' :::: path.txt ::: $PUpath ::: $IDSFpath ::: $PSVpath ::: $isdcount ::: $year ::: $isSig :::+ $isUL \n"""
        script += "echo Processed all $count ggtrees!!! \n"
        script += "for i in {0..9}; do hadd output_$i.root Jfake_ggtree_*$i.root; done \n"
        script += "rm -r *ggtree*  \n"
        script += "hadd tmp1.root output_*{0..4}.root; hadd tmp2.root output_*{5..9}.root; rm -r output*; hadd output_ggtree.root tmp1.root tmp2.root \n"
        script += "cp output_ggtree.root $returndir \n"
        script += "thiswork=$(pwd) \n"
        script += "cd $workdir; rm -r $thiswork \n"

        frun = open("run"+str(yeardir)+"_skim_"+str(isDcount)+"_"+str(isSignal)+".sh", "w")
        frun.write(script)
        frun.close()

        os.system( "chmod 744 run"+str(yeardir)+"_skim_"+str(isDcount)+"_"+str(isSignal)+".sh" )
        os.system( "./run"+str(yeardir)+"_skim_"+str(isDcount)+"_"+str(isSignal)+".sh" )
