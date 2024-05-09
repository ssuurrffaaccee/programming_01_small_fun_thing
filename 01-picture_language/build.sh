g++ -std=c++17 ./main.cpp -o pic.exe
./pic.exe
# ppmtojpeg is part of netpbm
ppmtojpeg ./square_limit.ppm > square_limit.jpg   
rm square_limit.ppm
rm pic.exe