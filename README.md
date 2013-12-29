### m2.4xlarge (AWS, Xen domU w/ PV, Amazon Linux, 68.4GB, one-way X5550)

    ./alloc_test F 1024
    Time for fork() to return to child:  0.241
    Time for fork() to return to parent: 0.241

    ./alloc_test F 10240
    Time for fork() to return to child:  2.421
    Time for fork() to return to parent: 2.421

    ./alloc_test F 32768
    Time for fork() to return to child:  7.713
    Time for fork() to return to parent: 7.713



### cr1.8xlarge (AWS, Xen domU w/ HVM, Amazon Linux, 244GB, two-way E5-2670)

    ./alloc_test F 1024
    Time for fork() to return to child:  0.001
    Time for fork() to return to parent: 0.001

    ./alloc_test F 10240
    Time for fork() to return to child:  0.005
    Time for fork() to return to parent: 0.005

    ./alloc_test F 32768
    Time for fork() to return to child:  0.016
    Time for fork() to return to parent: 0.016

    ./alloc_test F 102400
    Time for fork() to return to child:  0.045
    Time for fork() to return to parent: 0.045

    ./alloc_test F 117760
    Time for fork() to return to child:  0.053
    Time for fork() to return to parent: 0.053



### calumet (Bare metal, OSX 10.9, 16GB, one-way i7-3720QM)

    ./alloc_test F 1024
    Time for fork() to return to child:  0.005
    Time for fork() to return to parent: 0.005

    ./alloc_test F 10240
    Time for fork() to return to child:  0.045
    Time for fork() to return to parent: 0.045
