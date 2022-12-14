#!/bin/bash

dir=/data1/GMET/ana/220728/autumn18/mc

cd $dir
mkdir GJet
#mv job_autumn18_GJets_MG_HT* GJet/
mv job_UL18_GJets_MG_HT* GJet/
mkdir VJet
mv job_autumn18_WJetsToLNu_MG_HT* VJet/
mv job_autumn18_ZvvJets_HT* VJet/
mv job_autumn18_DYJetsToLL_m50_MG_HT* VJet/
mkdir VG
mv job_UL18_Zg_pt130/ VG/
mv job_UL18_Wg_pt130/ VG/
mv job_UL18_Znunug_pt130/ VG/
mkdir monoboson
mv job_autumn18_EWK_* monoboson/
mkdir diboson
mv job_autumn18_WW diboson/
mv job_autumn18_WZ diboson/
mv job_autumn18_ZZ diboson/
mkdir triboson
mv job_autumn18_WWG/ triboson/
mv job_autumn18_WZG/ triboson/
mkdir TT
mv job_autumn18_TT* TT/
cd VJet/
cp job_autumn18_DYJetsToLL_m50_MG_HT400to600_ext2/output_ggtree.root job_autumn18_DYJetsToLL_m50_MG_HT400to600/output_ggtree2.root
cd job_autumn18_DYJetsToLL_m50_MG_HT400to600
mv output_ggtree.root output_ggtree0.root
hadd output_ggtree.root output_ggtree0.root output_ggtree2.root
cd ~/wkVBS

