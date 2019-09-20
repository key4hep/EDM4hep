# Detect os
TOOLSPATH=/cvmfs/fcc.cern.ch/sw/0.8.3/tools/
OS=`python $TOOLSPATH/hsf_get_platform.py --get os`

# source FCC externals
source /cvmfs/fcc.cern.ch/sw/views/releases/externals/96b.0.0/x86_64-$OS-gcc8-opt/setup.sh

export PODIO=$FCCVIEW


