#!/bin/bash

dir=/data1/GMET/Jfake_pseudoData/fall17/
cd $dir

cp job_UL17_Znunug_VBS/output_ggtree.root ./output_ggtree_VBS.root

mkdir SMFake
mv job_fall17_ZvvJets_HT* SMFake/
mv job_fall17_WJetsToLNu_MG_HT* SMFake/
mv job_fall17_DYJetsToLL_m50_MG_HT* SMFake/
mv job_fall17_TT_aMCatNLO/ SMFake/
cd SMFake; hadd -f output_ggtree_SMFake.root job*/output*;
cp output_ggtree_SMFake.root ../; cd ../

mkdir SMTrue
mv job_fall17_* SMTrue/
mv job_UL17_GJets_HT* SMTrue/
mv job_UL17_Wg_* SMTrue/
mv job_UL17_Zg_aMCatNLO SMTrue/
cd SMTrue; hadd -f output_ggtree_SMTrue.root job*/output*;
cp output_ggtree_SMTrue.root ../; cd ../

