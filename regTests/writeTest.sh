#!/bin/bash

./genData.sh 10 1000000 bwdata
./bwTest bwdata
diff bwdata bwdata.out

if [ $? -ne 0 ]
    then
    echo "Buffered write test failed."
else 
    echo "Buffered write test passed."
    rm bwdata bwdata.out
fi
