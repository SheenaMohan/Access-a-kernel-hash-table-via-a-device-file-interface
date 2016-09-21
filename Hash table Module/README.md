The hashtable object is accessed both by the user and driver module. This structure is persent in the pro.h file.

In the test program, four threads are generated which totally performs 200 write operation into the hash table. 

The KEY and DATA are randomly given by incrementing their values for each write operation.

The following conditions are checked while writing into the hashtable in "ht530_driver_write" function in ht530_drv.c file
- If the input data field is non-zero, a ht_object is created and added it to the hash table. 
- If an old object with the same key already exist in the hash table, it is replaced with the new one. 
- If the data field is zero, any existing object with the input key is deleted from the table.

Simultaneously the threads also perform the read operation to read data from the hash table.
For this operation the Key is passed by the user through "ht_530_read_key" function into the kernel space and the hash table is
searched for the corresponding data which is sent to the user. 

After the read-write operations 8 objects from all the 128 buckets are displayed on the console. 


STEPS FOR TESTING THE MODULE

1) chmod +x module_test.sh

2) gcc -pthread -o test main.c -Wall

3) ./module_test.sh

4) dmesg
    The dmesg will be similar to the output_dmesg file