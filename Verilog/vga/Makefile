CC := iverilog

BENCH_DIRECTORY   := benches
INCLUDE_DIRECTORY := includes
MODULE_DIRECTORY  := modules
BUILD_DIRECTORY   := build

BENCHES := $(wildcard $(BENCH_DIRECTORY)/*.v)
TARGETS := $(notdir $(basename $(BENCHES)))

vpath %.v $(BENCH_DIRECTORY)

$(TARGETS):
	$(CC) $(BENCH_DIRECTORY)/$@.v -DVCD_FILE="$(BUILD_DIRECTORY)/$@.vcd" -l $(wildcard $(MODULE_DIRECTORY)/*.v) -I$(INCLUDE_DIRECTORY) -o $(BUILD_DIRECTORY)/$@
