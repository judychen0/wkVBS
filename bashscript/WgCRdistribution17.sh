#!/bin/bash

dir=/data1/GMET/WgCR_Data/fall17/
cd $dir
hadd -f output_ggtree.root job_*/output_ggtree.root

dir=/data1/GMET/WgCR_MC/fall17/
cd $dir

###
mkdir allmc
cp -r job_*/ allmc; cd allmc;

mkdir GJet
mv job_UL17_GJets_HT* GJet/
cd GJet
hadd output_merge_GJet.root job_*/output_ggtree.root; cd ../

mkdir VJet
mv job_fall17_WJetsToLNu_MG_HT* VJet/
mv job_fall17_ZvvJets_HT* VJet/
mv job_fall17_DYJetsToLL_m50_MG_HT* VJet/
#mv job_fall17_TT_aMCatNLO VJet/
cd VJet
hadd output_merge_VJet.root job_*/output_ggtree.root; cd ../

mkdir VG
#mv job_UL17_Zg_pt130/ VG/
mv job_UL17_Zg_aMCatNLO/ VG/
mv job_UL17_Wg_pt*/ VG/
#mv job_UL17_Znunug_pt130/ VG/
mv job_fall17_ZnunugJets_pt130/ VG/
mv job_fall17_ZnunugJets_pt40to130/ VG/
cd VG
hadd output_merge_Wg.root job_UL17_Wg*/output_ggtree.root;
hadd output_merge_Zg.root job_UL17_Zg*/output_ggtree.root job_fall17_ZnunugJets*/output*;
hadd output_merge_VG.root job_*/output_ggtree.root; cd ../

mkdir monoboson
mv job_fall17_EWK_* monoboson/
cd monoboson
hadd output_merge_monoBoson.root job_*/output_ggtree.root; cd ../

mkdir diboson
mv job_fall17_WW diboson/
mv job_fall17_WZ diboson/
mv job_fall17_ZZ diboson/
cd diboson
hadd output_merge_diBoson.root job_*/output_ggtree.root; cd ../

mkdir triboson
mv job_fall17_WWG/ triboson/
mv job_fall17_WZG/ triboson/
cd triboson
hadd output_merge_triBoson.root job_*/output_ggtree.root; cd ../

mkdir TT
mv job_*_TTG* TT/
cd TT
hadd output_merge_TT.root job_*/output_ggtree.root; cd ../
cd ../

###
mkdir WGJet
mv job_UL17_Wg_pt*/ WGJet/
cd WGJet
hadd output_merge_WG.root job_*/output_ggtree.root;
cp output_merge_WG.root ../; cd ../

mkdir SMFake
mv job_fall17_ZvvJets_HT* SMFake/
mv job_fall17_WJetsToLNu_MG_HT* SMFake/
mv job_fall17_DYJetsToLL_m50_MG_HT* SMFake/
#mv job_fall17_TT_aMCatNLO* SMFake/
cd SMFake
hadd -f output_merge_SMFake.root job*/output*;
cp output_merge_SMFake.root ../; cd ../

mkdir SMTrue
mv job_fall17_* SMTrue/
mv job_UL17_GJets_HT* SMTrue/
mv job_UL17_Zg_aMCatNLO SMTrue/
cd SMTrue
rm -r job_fall17_TT_*;
hadd -f output_merge_SMTrue.root job*/output*;
cp output_merge_SMTrue.root ../; cd ../

cp job_UL17_Znunug_VBS/output* ./output_merge_VBS.root

