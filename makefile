# CC = gcc
CC = g++
# STD = -std=c99
STD = -std=c++17
SRC = src/
BUILD = build/

# all: $(BUILD)3_6_fib.out
# all : $(BUILD)3_10_fib_shm.out $(BUILD)3_6_fib.out $(BUILD)3_27_filecopy.out $(BUILD)3_11_shm_ds.out $(BUILD)4_9_prime.out $(BUILD)6_11_sleep_barber.out
all : $(BUILD)6_13_bound_buffer_monitor.out
$(BUILD)6_13_bound_buffer_monitor.out : $(SRC)6_13_bound_buffer_monitor.cpp
	$(CC) $(STD) $(SRC)6_13_bound_buffer_monitor.cpp -o $(BUILD)6_13_bound_buffer_monitor.out

$(BUILD)3_6_fib.out: $(SRC)3_6_fib.c
	$(CC) $(STD) $(SRC)3_6_fib.c -o $(BUILD)3_6_fib.out
	
$(BUILD)3_10_fib_shm.out: $(SRC)3_10_fib_shm.c
	$(CC) $(STD) $(SRC)3_10_fib_shm.c -o $(BUILD)3_10_fib_shm.out

$(BUILD)3_27_filecopy.out: $(SRC)3_27_filecopy.c
	$(CC) $(STD) $(SRC)3_27_filecopy.c -o $(BUILD)3_27_filecopy.out

$(BUILD)3_11_shm_ds.out: $(SRC)3_11_shm_ds.c
	$(CC) $(STD) $(SRC)3_11_shm_ds.c -o $(BUILD)3_11_shm_ds.out

$(BUILD)4_9_prime.out: $(SRC)4_9_prime.c
	$(CC) $(STD) $(SRC)4_9_prime.c -o $(BUILD)4_9_prime.out -lm -lpthread

$(BUILD)6_11_sleep_barber.out: $(SRC)6_11_sleep_barber.c
	$(CC) $(STD) $(SRC)6_11_sleep_barber.c -o $(BUILD)6_11_sleep_barber.out -lm -lpthread

.PHONY:
clean:
	rm -r $(BUILD)*