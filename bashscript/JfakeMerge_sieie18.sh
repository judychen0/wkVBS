#!/bin/bash

dir=/data1/GMET/Jfake_Data/autumn18/
cd $dir; hadd -f output_ggtree.root job*/output*;

dir=/data1/GMET/Jfake_MC/autumn18/
cd $dir; hadd -f output_ggtree.root job*/output*;
