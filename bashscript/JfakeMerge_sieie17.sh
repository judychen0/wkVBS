#!/bin/bash

dir=/data1/GMET/Jfake_Data/fall17/
cd $dir; hadd -f output_ggtree.root job*/output*;

dir=/data1/GMET/Jfake_MC/fall17/
cd $dir; hadd -f output_ggtree.root job*/output*;
