#!/bin/bash

#drawdir=/home/judychen/Draw/chipDraw/
#mcdir=/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/
drawdir=/home/jou/wkVBS/Draw/Merge/
mcdir=/data1/GMET/ana/220519/fall17/mc/

cp $drawdir/xMerge_GJet.C .; root -l -q "xMerge_GJet.C(2018)"
cp output_merge_GJet.root $mcdir/GJet
cp $drawdir/xMerge_VJet.C .; root -l -q "xMerge_VJet.C(2018)"
cp output_merge_VJet.root $mcdir/VJet
cp $drawdir/xMerge_VG.C .; root -l -q "xMerge_VG.C(2018)"
cp output_merge_VG.root $mcdir/VG
cp $drawdir/xMerge_monoboson.C .; root -l -q "xMerge_monoboson.C(2018)"
cp output_merge_monoBoson.root $mcdir/monoboson
cp $drawdir/xMerge_diboson.C .; root -l -q "xMerge_diboson.C(2018)"
cp output_merge_diBoson.root $mcdir/diboson
cp $drawdir/xMerge_triboson.C .; root -l -q "xMerge_triboson.C(2018)"
cp output_merge_triBoson.root $mcdir/triboson
cp $drawdir/xMerge_TT.C .; root -l -q "xMerge_TT.C(2018)"
cp output_merge_TT.root $mcdir/TT

