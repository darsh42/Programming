CC := iverilog

DIRECTORY_INCLUDE := includes
DIRECTORY_SOURCES := sources
DIRECTORY_BENCHES := benches
DIRECTORY_BUILD   := build

BENCHES := $(wildcard $(DIRECTORY_BENCHES)/*.v)

TARGETS := $(DIRECTORY_BUILD)/$(notdir $(basename $(BENCHES)))

$(TARGETS): $(BENCHES)
	$(CC) $< -I $(DIRECTORY_INCLUDE) -l $(DIRECTORY_SOURCES)/*.v -o $@

.PHONY: clean

clean:
	rm $(DIRECTORY_BUILD)/*
