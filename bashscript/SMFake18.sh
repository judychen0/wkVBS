#!/bin/bash

mkdir SMFake

mv job_autumn18_ZvvJets_HT* SMFake/
mv job_autumn18_WJetsToLNu_MG_HT* SMFake/
mv job_autumn18_DYJetsToLL_m50_MG_HT* SMFake/
mv job_autumn18_TT_aMCatNLO* SMFake/

cd SMFake; hadd -f output_ggtree_SMFake.root job*/output*;
cp output_ggtree_SMFake.root ../; cd ../
