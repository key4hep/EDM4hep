# Detect os
# Disabled as currently there is only one platform supported
#TOOLSPATH=/cvmfs/fcc.cern.ch/sw/0.8.3/tools/
#OS=`python $TOOLSPATH/hsf_get_platform.py --get os`

# source LCG releases
export BINARY_TAG=x86_64-${OS-centos7}-${COMPILER_VERSION-gcc8}-opt
source /cvmfs/sft.cern.ch/lcg/views/${LCG_RELEASE-LCG_97a_FCC_4}/$BINARY_TAG/setup.sh




