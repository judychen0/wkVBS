#!/bin/bash

dir=/data1/GMET/Jfake_pseudoData/autumn18/
cd $dir

cp job_UL18_Znunug_VBS/output_ggtree.root ./output_ggtree_VBS.root

mkdir SMFake
mv job_autumn18_ZvvJets_HT* SMFake/
mv job_autumn18_WJetsToLNu_MG_HT* SMFake/
mv job_autumn18_DYJetsToLL_m50_MG_HT* SMFake/
#mv job_autumn18_TT_aMCatNLO* SMFake/
cd SMFake; hadd -f output_ggtree_SMFake.root job*/output*;
cp output_ggtree_SMFake.root ../; cd ../

mkdir SMTrue
cp -r job_autumn18_* SMTrue/
mv job_UL18_GJets_HT* SMTrue/
mv job_UL18_Wg_* SMTrue/
mv job_UL18_Zg_aMCatNLO SMTrue/
cd SMTrue;
rm -r job_autumn18_TT_*;
hadd -f output_ggtree_SMTrue.root job*/output*;
cp output_ggtree_SMTrue.root ../; cd ../

###
mkdir allMCs; cd allMCs;
cp -r ../SMTrue/job* .
cp -r ../SMFake/job* .

mkdir GJet
#mv job_autumn18_GJets_MG_HT* GJet/
mv job_UL18_GJets_HT* GJet/
cd GJet
hadd -f output_merge_GJet.root job_*/output_ggtree.root; cd ../

mkdir JetFake
mv job_autumn18_WJetsToLNu_MG_HT* JetFake/
mv job_autumn18_ZvvJets_HT* JetFake/
mv job_autumn18_DYJetsToLL_m50_MG_HT* JetFake/
#mv job_autumn18_TT_aMCatNLO* JetFake/
cd JetFake
hadd -f output_merge_JetFake.root job_*/output_ggtree.root; cd ../

mkdir VG
#mv job_UL18_Zg_pt130/ VG/
mv job_UL18_Zg_aMCatNLO/ VG/
mv job_UL18_Wg_pt*/ VG/
#mv job_UL18_Znunug_pt130/ VG/
mv job_autumn18_ZnunugJets_pt130/ VG/
mv job_autumn18_ZnunugJets_pt40to130/ VG/
cd VG
hadd output_merge_Wg.root job_UL18_Wg*/output_ggtree.root;
hadd output_merge_Zg.root job_UL18_Zg*/output_ggtree.root job_autumn18_ZnunugJets*/output*;
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
mv job_*_TTG* TT/
cd TT
hadd output_merge_TT.root job_*/output_ggtree.root; cd ../

hadd output_merge_allbkg.root GJet/output_merge_GJet.root JetFake/output_merge_JetFake.root VG/output_merge_VG.root monoboson/output_merge_monoBoson.root diboson/output_merge_diBoson.root triboson/output_merge_triBoson.root TT/output_merge_TT.root

# SM True photon (without Wgamma)
hadd output_merge_SMTrue.root GJet/output_merge_GJet.root monoboson/output_merge_monoBoson.root diboson/output_merge_diBoson.root triboson/output_merge_triBoson.root TT/output_merge_TT.root
cd ../

###
mkdir Wgamma; cd Wgamma
cp -r ../SMTrue/job* .
cp -r ../SMFake/job* .
cp -r ../job_UL18_Znunug_VBS .

mkdir WJet
mv job_UL18_Wg_pt*/ WJet/
cd WJet; hadd output_ggtree_WG.root job*/output*;
cp output_ggtree_WG.root ../; cd ../

mkdir SMFake
mv job_autumn18_ZvvJets_HT* SMFake/
mv job_autumn18_WJetsToLNu_MG_HT* SMFake/
mv job_autumn18_DYJetsToLL_m50_MG_HT* SMFake/
#mv job_autumn18_TT_aMCatNLO* SMFake/
cd SMFake; hadd -f output_ggtree_SMFake.root job*/output*;
cp output_ggtree_SMFake.root ../; cd ../

mkdir SMTrue
mv job_autumn18_* SMTrue/
mv job_UL18_GJets_HT* SMTrue/
mv job_UL18_Zg_aMCatNLO SMTrue/ 
cd SMTrue; hadd -f output_ggtree_SMTrue.root job*/output*;
cp output_ggtree_SMTrue.root ../; cd ../

cp job_UL18_Znunug_VBS/output_ggtree.root ./output_ggtree_VBS.root
