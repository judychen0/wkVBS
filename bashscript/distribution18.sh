#!/bin/bash

dir=/data1/GMET/ana/230222/autumn18/data
cd $dir
hadd output_ggtree.root job_*/output_ggtree.root

dir=/data1/GMET/ana/230222/autumn18/mc
cd $dir
mkdir GJet
#mv job_autumn18_GJets_MG_HT* GJet/
mv job_UL18_GJets_HT* GJet/
cd GJet
hadd output_merge_GJet.root job_*/output_ggtree.root; cd ../

mkdir VJet
mv job_autumn18_WJetsToLNu_MG_HT* VJet/
mv job_autumn18_ZvvJets_HT* VJet/
mv job_autumn18_DYJetsToLL_m50_MG_HT* VJet/
mv job_autumn18_TT_aMCatNLO* VJet/
cd VJet
hadd output_merge_VJet.root job_*/output_ggtree.root; cd ../

mkdir VG
#mv job_UL18_Zg_pt130/ VG/
mv job_UL18_Zg_aMCatNLO/ VG/
mv job_UL18_Wg_pt*/ VG/
#mv job_UL18_Znunug_pt130/ VG/
mv job_autumn18_ZnunugJets_pt130/ VG/
mv job_autumn18_ZnunugJets_pt40to130/ VG/
cd VG
hadd output_merge_Wg.root job_UL18_Wg_pt*/output_ggtree.root;
hadd output_merge_Zg.root job_UL18_Zg*/output_ggtree.root job_autumn18_ZnunugJets_*/output*;
hadd output_merge_VG.root job_*/output_ggtree.root; cd ../

mkdir monoboson
mv job_autumn18_EWK_* monoboson/
cd monoboson
hadd output_merge_monoBoson.root job_*/output_ggtree.root; cd ../

mkdir diboson
mv job_autumn18_WW diboson/
mv job_autumn18_WZ diboson/
mv job_autumn18_ZZ diboson/
cd diboson
hadd output_merge_diBoson.root job_*/output_ggtree.root; cd ../

mkdir triboson
mv job_autumn18_WWG/ triboson/
mv job_autumn18_WZG/ triboson/
cd triboson
hadd output_merge_triBoson.root job_*/output_ggtree.root; cd ../

mkdir TT
mv job_*_TT* TT/
cd TT
hadd output_merge_TT.root job_*/output_ggtree.root; cd ../

### SM True photon (without Wgamma)
hadd output_merge_SMTrue.root GJet/output_merge_GJet.root monoboson/output_merge_monoBoson.root diboson/output_merge_diBoson.root triboson/output_merge_triBoson.root TT/output_merge_TT.root
#cd VJet/
#cp job_autumn18_DYJetsToLL_m50_MG_HT400to600_ext2/output_ggtree.root job_autumn18_DYJetsToLL_m50_MG_HT400to600/output_ggtree2.root
#cd job_autumn18_DYJetsToLL_m50_MG_HT400to600
#mv output_ggtree.root output_ggtree0.root
#hadd output_ggtree.root output_ggtree0.root output_ggtree2.root
#cd ~/wkVBS

