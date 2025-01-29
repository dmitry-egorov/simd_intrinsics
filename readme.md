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
data size: 16k                   min                          avg
    baseline read 32x4  :        0us, 246.578647 Gb/s,        0us, 232.550493 Gb/s
    baseline read 32x8  :        0us, 294.353260 Gb/s,        0us, 247.298719 Gb/s
    baseline read 32x16 :        0us, 294.353260 Gb/s,        0us, 240.697983 Gb/s
    sum simple          :        0us, 246.578647 Gb/s,        0us, 228.111618 Gb/s
    sum simd x2         :        0us, 133.796936 Gb/s,        0us, 120.762628 Gb/s
    sum simd x4         :        0us, 246.578647 Gb/s,        0us, 227.702705 Gb/s
    sum simd x8         :        0us, 294.353260 Gb/s,        0us, 246.806544 Gb/s
    sum simd x16        :        0us, 294.353260 Gb/s,        0us, 236.527546 Gb/s
    simple swap         :        0us, 105.126164 Gb/s,        0us,  97.063714 Gb/s
    find simple         :        0us,  16.730558 Gb/s,        1us,  15.831764 Gb/s
    find simple x2      :        0us,  16.916854 Gb/s,        1us,  15.640725 Gb/s
    find simple x4      :        0us,  63.989839 Gb/s,        0us,  59.209298 Gb/s
    find simple x8      :        1us,  12.063658 Gb/s,        1us,  10.247029 Gb/s
    find simd           :        0us,  70.084109 Gb/s,        0us,  65.256660 Gb/s
    find simd 2         :        0us, 122.647192 Gb/s,        0us, 114.234936 Gb/s
    find simd x2        :        0us, 133.796936 Gb/s,        0us, 121.342335 Gb/s
    find simd x4        :        0us, 211.195164 Gb/s,        0us, 192.030481 Gb/s
    find simd x8        :        0us, 210.252328 Gb/s,        0us, 179.666056 Gb/s
    find simd x16       :        0us, 245.294383 Gb/s,        0us, 189.839192 Gb/s
    min simple          :        0us, 246.578647 Gb/s,        0us, 228.841661 Gb/s
    min simple x2       :        0us,  91.985394 Gb/s,        0us,  84.542263 Gb/s
    min simple x4       :        0us,  32.016670 Gb/s,        0us,  30.581249 Gb/s
    min simple x8       :        0us,  37.737597 Gb/s,        0us,  34.967994 Gb/s
    min simple x16      :        0us,  33.449234 Gb/s,        0us,  31.915093 Gb/s
    min simd            :        0us, 183.970787 Gb/s,        0us, 157.071678 Gb/s
    min simd x2         :        0us, 245.294383 Gb/s,        0us, 215.103584 Gb/s
    min simd x4         :        0us, 245.294383 Gb/s,        0us, 224.085569 Gb/s
    min simd x8         :        0us, 294.353260 Gb/s,        0us, 245.170806 Gb/s
    min simd x16        :        0us, 294.353260 Gb/s,        0us, 241.541468 Gb/s
    min idx simple      :        0us,  16.724617 Gb/s,        1us,  15.809575 Gb/s
    min idx simd        :        0us, 245.294383 Gb/s,        0us, 208.061419 Gb/s
    min idx simd x2     :        0us, 183.970787 Gb/s,        0us, 171.072617 Gb/s
    min idx simd x4     :        0us, 246.578647 Gb/s,        0us, 223.887892 Gb/s
    min idx simd x8     :        0us, 245.294383 Gb/s,        0us, 218.260966 Gb/s
    min idx simd x16    :        0us, 210.252328 Gb/s,        0us, 178.268759 Gb/s
data size: 384k                  min                          avg
    baseline read 32x4  :        2us, 134.834369 Gb/s,        3us, 130.818755 Gb/s
    baseline read 32x8  :        2us, 134.818287 Gb/s,        2us, 131.261460 Gb/s
    baseline read 32x16 :        2us, 134.305670 Gb/s,        3us, 129.979209 Gb/s
    sum simple          :        2us, 135.334832 Gb/s,        3us, 129.143631 Gb/s
    sum simd x2         :        2us, 133.796936 Gb/s,        3us, 128.585760 Gb/s
    sum simd x4         :        2us, 135.334832 Gb/s,        3us, 130.340446 Gb/s
    sum simd x8         :        2us, 134.305670 Gb/s,        3us, 129.423786 Gb/s
    sum simd x16        :        2us, 133.796936 Gb/s,        3us, 129.668893 Gb/s
    simple swap         :        7us,  54.594113 Gb/s,        7us,  50.888935 Gb/s
    find simple         :       23us,  16.708794 Gb/s,       24us,  15.949031 Gb/s
    find simple x2      :       23us,  16.622302 Gb/s,       24us,  15.964571 Gb/s
    find simple x4      :        5us,  66.646021 Gb/s,        6us,  63.681797 Gb/s
    find simple x8      :       32us,  12.243463 Gb/s,       36us,  10.900326 Gb/s
    find simd           :        5us,  67.280745 Gb/s,        6us,  65.264133 Gb/s
    find simd 2         :        3us, 103.281846 Gb/s,        4us,  96.389472 Gb/s
    find simd x2        :        2us, 133.307762 Gb/s,        3us, 126.794838 Gb/s
    find simd x4        :        2us, 132.806547 Gb/s,        3us, 128.212992 Gb/s
    find simd x8        :        2us, 134.305670 Gb/s,        3us, 128.988165 Gb/s
    find simd x16       :        2us, 133.307762 Gb/s,        3us, 128.636572 Gb/s
    min simple          :        2us, 131.799967 Gb/s,        3us, 128.470529 Gb/s
    min simple x2       :        4us,  88.534230 Gb/s,        4us,  84.913037 Gb/s
    min simple x4       :       12us,  32.228459 Gb/s,       12us,  30.768207 Gb/s
    min simple x8       :       10us,  37.220644 Gb/s,       10us,  35.997187 Gb/s
    min simple x16      :       11us,  33.769016 Gb/s,       12us,  32.285994 Gb/s
    min simd            :        2us, 134.305670 Gb/s,        3us, 130.558041 Gb/s
    min simd x2         :        2us, 135.351038 Gb/s,        3us, 128.772088 Gb/s
    min simd x4         :        2us, 135.334832 Gb/s,        3us, 129.789408 Gb/s
    min simd x8         :        2us, 134.305670 Gb/s,        3us, 128.789722 Gb/s
    min simd x16        :        2us, 132.293600 Gb/s,        3us, 128.767297 Gb/s
    min idx simple      :       23us,  16.693248 Gb/s,       24us,  16.047536 Gb/s
    min idx simd        :        2us, 135.334832 Gb/s,        3us, 129.916386 Gb/s
    min idx simd x2     :        2us, 133.796936 Gb/s,        3us, 124.565736 Gb/s
    min idx simd x4     :        2us, 132.790944 Gb/s,        3us, 125.987013 Gb/s
    min idx simd x8     :        2us, 134.818287 Gb/s,        3us, 127.897710 Gb/s
    min idx simd x16    :        2us, 134.305670 Gb/s,        3us, 127.762900 Gb/s
data size: 6144k                 min                          avg
    baseline read 32x4  :       55us, 112.693571 Gb/s,       61us, 102.391757 Gb/s
    baseline read 32x8  :       56us, 110.945869 Gb/s,       60us, 104.474527 Gb/s
    baseline read 32x16 :       56us, 110.924774 Gb/s,       61us, 103.107351 Gb/s
    sum simple          :       56us, 111.912526 Gb/s,       61us, 103.096491 Gb/s
    sum simd x2         :       58us, 107.220311 Gb/s,       62us, 101.010306 Gb/s
    sum simd x4         :       55us, 113.394514 Gb/s,       59us, 104.957849 Gb/s
    sum simd x8         :       55us, 112.491692 Gb/s,       59us, 105.038450 Gb/s
    sum simd x16        :       55us, 112.761025 Gb/s,       58us, 107.079705 Gb/s
    simple swap         :      123us,  50.832727 Gb/s,      160us,  39.164145 Gb/s
    find simple         :      377us,  16.686102 Gb/s,      396us,  15.855546 Gb/s
    find simple x2      :      381us,  16.508683 Gb/s,      396us,  15.860305 Gb/s
    find simple x4      :       97us,  64.310225 Gb/s,      102us,  61.657425 Gb/s
    find simple x8      :      582us,  10.794534 Gb/s,      607us,  10.353986 Gb/s
    find simd           :       94us,  66.701081 Gb/s,       98us,  64.005476 Gb/s
    find simd 2         :       68us,  92.030799 Gb/s,       74us,  84.957691 Gb/s
    find simd x2        :       58us, 108.143563 Gb/s,       62us, 101.168520 Gb/s
    find simd x4        :       57us, 110.296303 Gb/s,       60us, 104.216020 Gb/s
    find simd x8        :       58us, 107.159956 Gb/s,       61us, 101.973572 Gb/s
    find simd x16       :       57us, 109.209325 Gb/s,       61us, 101.845867 Gb/s
    min simple          :       56us, 111.427788 Gb/s,       60us, 103.900704 Gb/s
    min simple x2       :       76us,  82.673824 Gb/s,       80us,  78.591331 Gb/s
    min simple x4       :      202us,  31.057771 Gb/s,      214us,  29.344805 Gb/s
    min simple x8       :      173us,  36.356273 Gb/s,      182us,  34.439767 Gb/s
    min simple x16      :      188us,  33.453194 Gb/s,      197us,  31.927421 Gb/s
    min simd            :       56us, 111.164095 Gb/s,       61us, 102.512463 Gb/s
    min simd x2         :       56us, 110.989446 Gb/s,       60us, 104.108468 Gb/s
    min simd x4         :       56us, 110.555215 Gb/s,       61us, 102.203406 Gb/s
    min simd x8         :       55us, 113.099511 Gb/s,       59us, 106.004343 Gb/s
    min simd x16        :       56us, 110.968334 Gb/s,       60us, 103.970474 Gb/s
    min idx simple      :      381us,  16.511095 Gb/s,      401us,  15.653768 Gb/s
    min idx simd        :       57us, 108.663384 Gb/s,       62us, 101.075865 Gb/s
    min idx simd x2     :       57us, 108.663384 Gb/s,       62us, 100.880088 Gb/s
    min idx simd x4     :       58us, 107.057825 Gb/s,       63us,  99.298809 Gb/s
    min idx simd x8     :       58us, 107.444536 Gb/s,       63us,  99.785829 Gb/s
    min idx simd x16    :       59us, 105.874533 Gb/s,       62us, 100.636223 Gb/s
data size: 262144k               min                          avg
    baseline read 32x4  :     9052us,  29.652399 Gb/s,     9483us,  28.304221 Gb/s
    baseline read 32x8  :     8988us,  29.863890 Gb/s,     9264us,  28.974333 Gb/s
    baseline read 32x16 :     8996us,  29.838503 Gb/s,     9321us,  28.798195 Gb/s
    sum simple          :     9021us,  29.756465 Gb/s,     9272us,  28.948249 Gb/s
    sum simd x2         :     9087us,  29.538958 Gb/s,     9487us,  28.293044 Gb/s
    sum simd x4         :     8875us,  30.246009 Gb/s,     9156us,  29.316025 Gb/s
    sum simd x8         :     8894us,  30.178478 Gb/s,     9293us,  28.883584 Gb/s
    sum simd x16        :     8923us,  30.083374 Gb/s,     9237us,  29.059965 Gb/s
    simple swap         :    30523us,   8.794458 Gb/s,    38657us,   6.943916 Gb/s
    find simple         :    19530us,  13.744429 Gb/s,    20208us,  13.283482 Gb/s
    find simple x2      :    19408us,  13.830864 Gb/s,    19905us,  13.485165 Gb/s
    find simple x4      :    12634us,  21.246913 Gb/s,    13108us,  20.477543 Gb/s
    find simple x8      :    28182us,   9.524870 Gb/s,    28664us,   9.364789 Gb/s
    find simd           :    10183us,  26.360695 Gb/s,    10575us,  25.382244 Gb/s
    find simd 2         :    10038us,  26.741433 Gb/s,    10437us,  25.719492 Gb/s
    find simd x2        :     9114us,  29.451612 Gb/s,     9989us,  26.871678 Gb/s
    find simd x4        :     9035us,  29.709647 Gb/s,     9374us,  28.635693 Gb/s
    find simd x8        :     9061us,  29.624129 Gb/s,     9464us,  28.363363 Gb/s
    find simd x16       :     9029us,  29.728145 Gb/s,     9325us,  28.786137 Gb/s
    min simple          :     8999us,  29.828057 Gb/s,     9335us,  28.753272 Gb/s
    min simple x2       :    11033us,  24.330104 Gb/s,    11464us,  23.414940 Gb/s
    min simple x4       :    14678us,  18.287900 Gb/s,    15031us,  17.858465 Gb/s
    min simple x8       :    14277us,  18.801007 Gb/s,    14718us,  18.237773 Gb/s
    min simple x16      :    14957us,  17.946686 Gb/s,    15492us,  17.327190 Gb/s
    min simd            :     8950us,  29.990472 Gb/s,     9341us,  28.736589 Gb/s
    min simd x2         :     8971us,  29.922405 Gb/s,     9250us,  29.017434 Gb/s
    min simd x4         :     8924us,  30.077258 Gb/s,     9314us,  28.818201 Gb/s
    min simd x8         :     8909us,  30.127727 Gb/s,     9281us,  28.921593 Gb/s
    min simd x16        :     8955us,  29.975522 Gb/s,     9252us,  29.010755 Gb/s
    min idx simple      :    19522us,  13.750009 Gb/s,    20078us,  13.369411 Gb/s
    min idx simd        :     8991us,  29.855793 Gb/s,     9470us,  28.344596 Gb/s
    min idx simd x2     :     9056us,  29.639313 Gb/s,     9543us,  28.127498 Gb/s
    min idx simd x4     :     8967us,  29.935741 Gb/s,     9396us,  28.567617 Gb/s
    min idx simd x8     :     9076us,  29.575151 Gb/s,     9571us,  28.044484 Gb/s
    min idx simd x16    :     9178us,  29.246609 Gb/s,     9602us,  27.953452 Gb/s
data size: 1048576k              min                          avg
    baseline read 32x4  :    36950us,  29.058544 Gb/s,    39004us,  27.528745 Gb/s
    baseline read 32x8  :    36713us,  29.246316 Gb/s,    39020us,  27.517028 Gb/s
    baseline read 32x16 :    37170us,  28.886995 Gb/s,    37835us,  28.379529 Gb/s
    sum simple          :    36799us,  29.177817 Gb/s,    38066us,  28.207143 Gb/s
    sum simd x2         :    37383us,  28.722685 Gb/s,    38321us,  28.019557 Gb/s
    sum simd x4         :    36778us,  29.194583 Gb/s,    37287us,  28.796123 Gb/s
    sum simd x8         :    36572us,  29.358900 Gb/s,    37023us,  29.001885 Gb/s
    sum simd x16        :    36935us,  29.070420 Gb/s,    38077us,  28.198639 Gb/s
    simple swap         :   137258us,   7.822753 Gb/s,   220134us,   4.877660 Gb/s
    find simple         :    78525us,  13.673847 Gb/s,    86657us,  12.390667 Gb/s
    find simple x2      :    78422us,  13.691815 Gb/s,    82513us,  13.012905 Gb/s
    find simple x4      :    51153us,  20.990417 Gb/s,    52474us,  20.461983 Gb/s
    find simple x8      :   113817us,   9.433877 Gb/s,   118169us,   9.086453 Gb/s
    find simd           :    40807us,  26.312564 Gb/s,    42167us,  25.463939 Gb/s
    find simd 2         :    41082us,  26.136516 Gb/s,    42061us,  25.527677 Gb/s
    find simd x2        :    37980us,  28.270994 Gb/s,    39266us,  27.345186 Gb/s
    find simd x4        :    37310us,  28.778509 Gb/s,    38568us,  27.839919 Gb/s
    find simd x8        :    36863us,  29.127447 Gb/s,    37866us,  28.355980 Gb/s
    find simd x16       :    37293us,  28.791583 Gb/s,    40167us,  26.731878 Gb/s
    min simple          :    36796us,  29.180619 Gb/s,    37576us,  28.574509 Gb/s
    min simple x2       :    43905us,  24.455509 Gb/s,    46053us,  23.314999 Gb/s
    min simple x4       :    58815us,  18.256059 Gb/s,    60130us,  17.856769 Gb/s
    min simple x8       :    58125us,  18.472747 Gb/s,    61177us,  17.551346 Gb/s
    min simple x16      :    60504us,  17.746460 Gb/s,    61730us,  17.393885 Gb/s
    min simd            :    36811us,  29.168913 Gb/s,    37555us,  28.591076 Gb/s
    min simd x2         :    37070us,  28.964868 Gb/s,    37924us,  28.312276 Gb/s
    min simd x4         :    36443us,  29.463496 Gb/s,    37545us,  28.598468 Gb/s
    min simd x8         :    36519us,  29.401934 Gb/s,    37398us,  28.710470 Gb/s
    min simd x16        :    36684us,  29.269577 Gb/s,    38065us,  28.208032 Gb/s
    min idx simple      :    79364us,  13.529235 Gb/s,    80953us,  13.263715 Gb/s
    min idx simd        :    37270us,  28.809652 Gb/s,    38788us,  27.682036 Gb/s
    min idx simd x2     :    37227us,  28.842785 Gb/s,    38381us,  27.975331 Gb/s
    min idx simd x4     :    36968us,  29.044570 Gb/s,    38040us,  28.226493 Gb/s
    min idx simd x8     :    37186us,  28.874767 Gb/s,    38433us,  27.937419 Gb/s
    min idx simd x16    :    37338us,  28.757316 Gb/s,    38539us,  27.860489 Gb/s


