# Detect os
# Disabled as currently there is only one platform supported
#TOOLSPATH=/cvmfs/fcc.cern.ch/sw/0.8.3/tools/
#OS=`python $TOOLSPATH/hsf_get_platform.py --get os`

# source LCG releases
export BINARY_TAG=x86_64-${OS-centos7}-${COMPILER_VERSION-gcc8}-opt

ls /cvmfs/sft.cern.ch/lcg/views/${LCG_RELEASE-LCG_96c_LS}/$BINARY_TAG
source /cvmfs/sft.cern.ch/lcg/views/${LCG_RELEASE-LCG_96c_LS}/$BINARY_TAG/setup.sh

# workaround for ROOT requiring a higher version of CMake than is provided by lcg
export PATH=/cvmfs/sft.cern.ch/lcg/releases/CMake/3.14.3-34449/$BINARY_TAG/bin/:$PATH



