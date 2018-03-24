#!/bin/bash

./genData.sh 10 1000000 brdata
./brTest brdata
diff brdata brdata.out

if [ $? -ne 0 ]
    then
    echo "Buffered read test failed."
else 
    echo "Buffered read test passed."
    rm brdata brdata.out
fi
