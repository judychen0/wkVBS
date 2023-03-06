#!/bin/bash

mkdir Wgamma; cd Wgamma;
cp -r ../SMTrue/job* .
cp -r ../SMFake/job* .

mkdir SMFake
mv job_autumn18_ZvvJets_HT* SMFake/
mv job_autumn18_WJetsToLNu_MG_HT* SMFake/
mv job_autumn18_DYJetsToLL_m50_MG_HT* SMFake/
mv job_autumn18_TT_aMCatNLO* SMFake/
cd SMFake; hadd -f output_ggtree_SMFake.root job*/output*;
cp output_ggtree_SMFake.root ../; cd ../

mkdir WgTrue
mv job_UL18_Wg_* WgTrue/
cd WgTrue; hadd -f output_ggtree_WgTrue.root job*/output*;
cp output_ggtree_WgTrue.root ../; cd ../

mkdir SMTrue
mv job_autumn18_* SMTrue/
mv job_UL18_GJets_HT* SMTrue/
mv job_UL18_Zg_aMCatNLO SMTrue/
cd SMTrue; hadd -f output_ggtree_SMTrue.root job*/output*;
cp output_ggtree_SMTrue.root ../; cd ../

cp job_UL18_Znunug_VBS/output* ./output_ggtree_VBS.root
