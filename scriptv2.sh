#!/bin/bash 
FILE="lzw"

if [ $# -eq 0 ]; then
    if [ ! -f "$FILE" ]; then
        echo "El fichero ejecutable $FILE no está en la ruta actual"
        exit 1
    else
        MIPATH=${PWD}        
    fi
else
    MIPATH=$1
    if [ ! -d "$MIPATH" ]; then
        echo "La ruta $MIPATH no existe"
        exit 1
    elif [ ! -f "$MIPATH/$FILE" ]; then
        echo "El fichero ejecutable $FILE no está en la ruta $MIPATH"
        exit 1
    fi
fi
    
PATH="${PATH:+${PATH}:$MIPATH}"    
export PATH
echo "La ruta $MIPATH ha sido agregada al PATH"
