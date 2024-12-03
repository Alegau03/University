#!/bin/sh

rootdir="plots"
source="log.txt"
target="plot.gpl"
listofvars="p.x p.y K.x K.y"
listofvars1="p.y K.y"
listofvars2="p.y"
FirstIteration=true

if [ ! -d ${rootdir} ] ; then
    mkdir ${rootdir}
else
    echo "${rootdir} already exists"
fi



echo "set grid" > ${rootdir}/${target}

for varname in ${listofvars}
do

if [ "${FirstIteration}" = true ] ; then
    FirstIteration=false
    echo -n "plot \"${varname}.csv\" with steps title \"${varname}\" " >> ${rootdir}/${target}
else    
    echo -n ", \"${varname}.csv\" with steps title \"${varname}\" " >> ${rootdir}/${target}  
fi

    echo "BEGIN { print \"# time ${varname}\"; }" > ${rootdir}/tmp.awk
    echo "(\$1 == \"${varname}\")  { printf \"%s %s\\\n\", \$2, \$3; }" >> ${rootdir}/tmp.awk
    awk -f ${rootdir}/tmp.awk ${source} > ${rootdir}/${varname}.csv

    
done

