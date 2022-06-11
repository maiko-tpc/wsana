#!/bin/csh

set tmpdir=$PWD
cd /home/tamidaq/cern/root/v6.26.04/bin
unalias cd
source thisroot.csh
cd $tmpdir
alias cd           'cd \!*;echo $cwd'
#alias root "root -l"
