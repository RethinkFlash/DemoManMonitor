#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $DIR
export LD_LIBRARY_PATH=/usr/local/lib
${DIR}/EscentsMonitor -hmm ${DIR}/models/hub4wsj_sc_8k_adapted/ -dict ${DIR}/models/cmu07a.dic -lm ${DIR}/models/wsj0vp.5000.DMP -logfn /dev/null
