#!/bin/bash

dir=/data1/GMET/ana/220728/fall17/mc

cd $dir
mkdir GJet
#mv job_fall17_GJets_MG_HT* GJet/
mv job_UL18_GJets_MG_HT* GJet/
mkdir VJet
mv job_fall17_WJetsToLNu_MG_HT* VJet/
mv job_fall17_ZvvJets_HT* VJet/
mv job_fall17_DYJetsToLL_m50_MG_HT* VJet/
mkdir VG
mv job_UL17_Zg_pt130/ VG/
mv job_UL17_Wg_pt130/ VG/
mv job_UL17_Znunug_pt130/ VG/
mkdir monoboson
mv job_fall17_EWK_* monoboson/
mkdir diboson
mv job_fall17_WW diboson/
mv job_fall17_WZ diboson/
mv job_fall17_ZZ diboson/
mkdir triboson
mv job_fall17_WWG/ triboson/
mv job_fall17_WZG/ triboson/
mkdir TT
mv job_fall17_TT* TT/
cd VJet/
cp job_fall17_ZvvJets_HT1200to2500_ext1/output_ggtree.root job_fall17_ZvvJets_HT1200to2500/output_ggtree1.root
cd job_fall17_ZvvJets_HT1200to2500
mv output_ggtree.root output_ggtree0.root
hadd output_ggtree.root output_ggtree0.root output_ggtree1.root
cd ../
cp job_fall17_DYJetsToLL_m50_MG_HT100to200_ext1/output_ggtree.root job_fall17_DYJetsToLL_m50_MG_HT100to200/output_ggtree1.root
cd job_fall17_DYJetsToLL_m50_MG_HT100to200
mv output_ggtree.root output_ggtree0.root
hadd output_ggtree.root output_ggtree0.root output_ggtree1.root
cd ../
cp job_fall17_DYJetsToLL_m50_MG_HT200to400_ext1/output_ggtree.root job_fall17_DYJetsToLL_m50_MG_HT200to400/output_ggtree1.root
cd job_fall17_DYJetsToLL_m50_MG_HT200to400
mv output_ggtree.root output_ggtree0.root
hadd output_ggtree.root output_ggtree0.root output_ggtree1.root
cd ../
cp job_fall17_DYJetsToLL_m50_MG_HT400to600_ext1/output_ggtree.root job_fall17_DYJetsToLL_m50_MG_HT400to600/output_ggtree1.root
cd job_fall17_DYJetsToLL_m50_MG_HT400to600
mv output_ggtree.root output_ggtree0.root
hadd output_ggtree.root output_ggtree0.root output_ggtree1.root
cd ~/wkVBS
