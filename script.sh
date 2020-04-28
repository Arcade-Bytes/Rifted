#! /bin/bash
rm -r build
cmake -H. -Bbuild
cd build
make
mv Rifted ..
cd ..
./Rifted
