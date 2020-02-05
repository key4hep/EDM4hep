# Detect os
TOOLSPATH=/cvmfs/fcc.cern.ch/sw/0.8.3/tools/
OS=`python $TOOLSPATH/hsf_get_platform.py --get os`

# source FCC externals
source /cvmfs/fcc.cern.ch/sw/latest/setup.sh
export PATH=/cvmfs/sft.cern.ch/lcg/releases/CMake/3.11.1-773ff/x86_64-centos7-gcc8-opt/bin/:$PATH




