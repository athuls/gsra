#!/bin/sh

vgdir=~/visiongrader/
test_db=/taf/pedestrians/daimler/daimler_pami09_evaluation_v0.01/example/pos_GroundTruth2D.db_bbox.txt
test_images=/data/pedestrians/daimler_det/DaimlerBenchmark/Data/TestData/
gtparser=eblearn

# show db
python $vgdir/main.py --input $1 --input_parser eblearn \
    --groundtruth $test_db --disp --groundtruth_parser $gtparser \
    --images_path $test_images