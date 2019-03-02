#!/bin/bash
DIR=""
FILE=".bashrc"
COMPIL=0
RUN=0
while getopts d:f:cr OPTION ; do
    case $OPTION in
        \?) echo "dont know";;

        :) echo "opttrag"${OPTARG};;

        d)
            if [ -n "${OPTARG}" ]; then
                DIR=${OPTARG}
            fi;;
 
        f)
            if [ -n "${OPTARG}" ]; then
                FILE=${OPTARG}
            fi;;
            
        c)
            COMPIL=1;;
        
        r)
            RUN=1;;
    esac
done

if [ -n $DIR ]; then
    echo "--> Directory: "
    ls $DIR
fi

echo "--> File "$FILE":"
cat $FILE
echo ""

if [ $COMPIL -eq 1 ]; then
    echo "--> Compiling file "$FILE"..."
    gcc $FILE
    if [ $RUN -eq 1 ]; then
        echo "--> Running compiled file"
        ./a.out
    fi
fi