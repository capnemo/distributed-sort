#!/bin/bash

./genData.sh 10 1000000 bwdata
./bwTest bwdata
diff bwdata bwdata.out

if [ $? -ne 0 ]
    then
    echo "Buffered read test failed."
else 
    echo "Buffered read test passed."
    rm bwdata bwdata.out
fi
