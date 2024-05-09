g++ -std=c++17 ./main.cpp -o turtle.exe
./turtle.exe
# ppmtojpeg is part of netpbm
ppmtojpeg ./turtle.ppm > turtle.jpg   
rm turtle.ppm
rm turtle.exe