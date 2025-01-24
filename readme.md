# SIMD intrinsics
A set of benchmarks for some simple algorithms implemented with SIMD intrinsics.

Baseline is a sequence of reads without any calculations or writes, implemented with inline assembly.

## Build and Run
Execute build_and_run.bat in the MSVC dev terminal. Windows only.

## Example Output
Ran on ADM 6800H CPU.

```
Estimating cpu timer frequency...
Filling data buffer...
Benchmarking...
data size: 16k
baseline read 32x4  : min        0us, 245.293064 Gb/s, avg        0us, 222.572627 Gb/s
baseline read 32x8  : min        0us, 294.351677 Gb/s, avg        0us, 246.096974 Gb/s
baseline read 32x16 : min        0us, 294.351677 Gb/s, avg        0us, 240.833890 Gb/s
sum simple          : min        0us, 245.293064 Gb/s, avg        0us, 219.822590 Gb/s
sum simd x8         : min        0us, 294.351677 Gb/s, avg        0us, 244.241872 Gb/s
find simple         : min        0us,  16.542420 Gb/s, avg        1us,  15.586878 Gb/s
find simple x4      : min        0us,  63.989495 Gb/s, avg        0us,  60.380617 Gb/s
find simd           : min        0us, 105.125599 Gb/s, avg        0us,  64.536852 Gb/s
find simd x16       : min        0us, 211.194028 Gb/s, avg        0us, 193.381809 Gb/s
data size: 384k
baseline read 32x4  : min        2us, 131.799258 Gb/s, avg        3us, 126.561263 Gb/s
baseline read 32x8  : min        2us, 131.799258 Gb/s, avg        3us, 128.369491 Gb/s
baseline read 32x16 : min        2us, 131.309298 Gb/s, avg        3us, 126.990357 Gb/s
sum simple          : min        2us, 132.292889 Gb/s, avg        3us, 127.618964 Gb/s
sum simd x8         : min        2us, 132.292889 Gb/s, avg        3us, 127.346441 Gb/s
find simple         : min       23us,  16.622212 Gb/s, avg       24us,  15.758922 Gb/s
find simple x4      : min        5us,  66.649593 Gb/s, avg        6us,  60.313613 Gb/s
find simd           : min        6us,  65.532841 Gb/s, avg        6us,  63.841377 Gb/s
find simd x16       : min        3us, 130.822968 Gb/s, avg        3us, 127.577910 Gb/s
data size: 6144k
baseline read 32x4  : min       61us, 101.866478 Gb/s, avg       65us,  96.540078 Gb/s
baseline read 32x8  : min       63us,  99.833107 Gb/s, avg       67us,  93.455472 Gb/s
baseline read 32x16 : min       60us, 104.387739 Gb/s, avg       66us,  95.017782 Gb/s
sum simple          : min       61us, 101.573548 Gb/s, avg       65us,  95.992945 Gb/s
sum simd x8         : min       61us, 101.995167 Gb/s, avg       67us,  93.453815 Gb/s
find simple         : min      389us,  16.148676 Gb/s, avg      409us,  15.371010 Gb/s
find simple x4      : min       98us,  63.967767 Gb/s, avg      109us,  57.654322 Gb/s
find simd           : min       96us,  65.170113 Gb/s, avg      100us,  62.590470 Gb/s
find simd x16       : min       63us,  98.304961 Gb/s, avg       68us,  91.232328 Gb/s
data size: 262144k
baseline read 32x4  : min     9486us,  28.296968 Gb/s, avg     9908us,  27.090084 Gb/s
baseline read 32x8  : min     9481us,  28.310489 Gb/s, avg    10462us,  25.656705 Gb/s
baseline read 32x16 : min     9387us,  28.594133 Gb/s, avg     9639us,  27.848117 Gb/s
sum simple          : min     9335us,  28.752918 Gb/s, avg     9796us,  27.400193 Gb/s
sum simd x8         : min     9454us,  28.392392 Gb/s, avg     9882us,  27.162077 Gb/s
find simple         : min    19100us,  14.054196 Gb/s, avg    19589us,  13.703293 Gb/s
find simple x4      : min    12100us,  22.184584 Gb/s, avg    12765us,  21.028021 Gb/s
find simd           : min    10217us,  26.272179 Gb/s, avg    10980us,  24.446517 Gb/s
find simd x16       : min     9874us,  27.185556 Gb/s, avg    10334us,  25.973496 Gb/s
data size: 1048576k
baseline read 32x4  : min    38232us,  28.084174 Gb/s, avg    39664us,  27.070641 Gb/s
baseline read 32x8  : min    37603us,  28.554296 Gb/s, avg    38784us,  27.685051 Gb/s
baseline read 32x16 : min    37617us,  28.543717 Gb/s, avg    38867us,  27.625747 Gb/s
sum simple          : min    37674us,  28.500348 Gb/s, avg    38661us,  27.772747 Gb/s
sum simd x8         : min    38004us,  28.253062 Gb/s, avg    38889us,  27.610000 Gb/s
find simple         : min    76440us,  14.046754 Gb/s, avg    77636us,  13.830341 Gb/s
find simple x4      : min    49132us,  21.854125 Gb/s, avg    50096us,  21.433535 Gb/s
find simd           : min    41237us,  26.038132 Gb/s, avg    44160us,  24.314304 Gb/s
find simd x16       : min    38639us,  27.788611 Gb/s, avg    40123us,  26.760764 Gb/s
