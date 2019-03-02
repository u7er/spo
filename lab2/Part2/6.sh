#!/bin/bash
echo "Введите комманду"
read INPUTED
if (test ${INPUTED}); then
    ${INPUTED};
else
    echo "Net";
fi;