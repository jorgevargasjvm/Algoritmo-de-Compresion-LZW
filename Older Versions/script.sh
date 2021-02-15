#!/bin/bash

INPUT= "your path to exe"

echo ""
echo "-------------------------------------------"
echo "EVALUATING $INPUT"
echo "-------------------------------------------"

if [ -e $INPUT ]; then
    PATH = $PATH:$INPUT
    export PATH
else
    echo "PATH =$INPUT does not exist"
fi
