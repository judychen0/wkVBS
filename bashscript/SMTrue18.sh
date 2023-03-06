#!/bin/bash

mkdir SMTrue
mv job_autumn18_* SMTrue/
mv job_UL18_GJets_HT* SMTrue/
mv job_UL18_Wg_* SMTrue/
mv job_UL18_Zg_aMCatNLO SMTrue/ 

cd SMTrue; hadd -f output_ggtree_SMTrue.root job*/output*;
cp output_ggtree_SMTrue.root ../; cd ../
