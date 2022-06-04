CC         := g++
CPPFLAGS     := -fopenmp -O3 -fopenmp-simd -mavx2 -falign-loops=32 -D$(TYPE) -D$(INP) -g -ftree-vectorize
# CPPFLAGS     := -qopenmp -O3 -qopenmp-simd -xCORE-AVX2 -D$(TYPE) -D$(INP) -g

BUILD_DIR := build

all: dirs bitap
	echo "Making all"

dirs:
	mkdir -p $(BUILD_DIR)

bitap: $(BUILD_DIR)/driver.o $(BUILD_DIR)/baseline.o $(BUILD_DIR)/partition.o $(BUILD_DIR)/helper.o
	$(CC) $(CPPFLAGS) $^ -o bitap

$(BUILD_DIR)/%.o : %.cpp
		$(CC) -c $(CPPFLAGS) $< -o $@

clean:
	rm $(BUILD_DIR)/*.o
	rm bitap
