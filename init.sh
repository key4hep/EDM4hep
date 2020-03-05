# Detect os
# Disabled as currently there is only one platform supported
#TOOLSPATH=/cvmfs/fcc.cern.ch/sw/0.8.3/tools/
#OS=`python $TOOLSPATH/hsf_get_platform.py --get os`

# source LCG releases
source /cvmfs/sft.cern.ch/lcg/views/LCG_96c_LS/x86_64-centos7-gcc8-opt/setup.sh

# workaround for ROOT requiring a higher version of CMake than is provided by lcg
export PATH=/cvmfs/sft.cern.ch/lcg/releases/CMake/3.11.1-773ff/x86_64-centos7-gcc8-opt/bin/:$PATH


