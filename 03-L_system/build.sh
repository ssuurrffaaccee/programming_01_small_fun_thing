for i in {0..4}
do 
    g++ -std=c++17 ./main${i}.cpp -o l.exe
    ./l.exe
    # ppmtojpeg is part of netpbm
    ppmtojpeg ./l.ppm > l${i}.jpg   
    rm l.ppm
    rm l.exe  
done 
