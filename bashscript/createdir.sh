#!/bin/bash

dir=/data1/GMET/ana/230222
outputdir=/data1/GMET/ana/output_VBS
mkdir $dir
#mkdir summer16
#mkdir summer16/data
#mkdir summer16/mc
#mkdir fall17
#mkdir fall17/data
#mkdir fall17/mc
#mkdir fall17/OOT
cd $dir; mkdir fall17; mkdir fall17/data; mkdir fall17/mc
mv $outputdir/fall17/job_UL17* ./fall17/mc
mv $outputdir/fall17/job_fall17* ./fall17/mc
#mv $outputdir/fall17/job_SinglePho* ./fall17/data
mv $outputdir/fall17/job_MET* ./fall17/data
cd $dir; mkdir autumn18; mkdir autumn18/data; mkdir autumn18/mc
mv $outputdir/autumn18/job_UL18* ./autumn18/mc
mv $outputdir/autumn18/job_autumn18* ./autumn18/mc
#mv $outputdir/autumn18/job_EGamma* ./autumn18/data
mv $outputdir/autumn18/job_MET* ./autumn18/data
#mkdir autumn18/OOT
#mv ~/output/summer16/job_summer16* ./summer16/mc
#mv ~/Draw/chipoutput/fall17/job_fall17* ./fall17/mc
#mv ~/Draw/chipoutput/fall17/job_UL17* ./fall17/mc
#mv ~/Draw/chipoutput/autumn18/job_autumn18* ./autumn18/mc
#mv ~/Draw/chipoutput/autumn18/job_UL18* ./autumn18/mc
#mv ~/output/summer16/job_SinglePho* ./summer16/data
#mv ~/Draw/chipoutput/fall17/job_SinglePho* ./fall17/data
#mv ~/Draw/chipoutput/autumn18/job_EGamma* ./autumn18/data
#mv ~/Draw/chipoutput/autumn18/job_MET* ./autumn18/data
#mv ~/Draw/chipoutput/fall17/OOT/* ./fall17/OOT
#mv ~/Draw/chipoutput/autumn18/OOT/* ./autumn18/OOT
