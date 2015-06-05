#!/bin/bash

#source "/network-raid/opt/maxcompiler-2014.2/settings.sh"
source /network-raid/opt/maxcompiler-ticket7202-clang2/settings.sh

export MAXELEROSDIR=$MAXCOMPILERDIR/lib/maxeleros-sim
export LD_PRELOAD=$MAXELEROSDIR/lib/libmaxeleros.so:$LD_PRELOAD
export SLIC_CONF="$SLIC_CONF;use_simulation=${USER}Sim"

# How to set a BASH variable equal to the output from a command?
# You need `, not '. This character is called "backticks" 
srcMax=`maxGuessBuildDir -s httpServer_2014_ticket7202NEW_ISCA_DFE`scratch/altera_quartus/httpServer_2014_ticket7202NEW.max
srcH=`maxGuessBuildDir -s httpServer_2014_ticket7202NEW_ISCA_DFE`scratch/httpServer_2014_ticket7202NEW.h
echo "Copying .max and .h files to the /hostname directory"
echo $srcMax
echo $srcH
cp $srcMax /home/${USER}/workspace/workingNonMaxIDE/hostcode
cp $srcH /home/${USER}/workspace/workingNonMaxIDE/hostcode
echo "Finished copying"

# 1) run 'make_initCode.py' (python fabricate)
python make_initCode.py
# 2) run 'make_httpServerCPUcode.py' (python fabricate) 
python make_DFE_httpServerCPUcode_2014_ticket7202NEW.py
