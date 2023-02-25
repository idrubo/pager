CFLAGS := -Wall -g -O2
LDFLAGS := -lm

alan  := alan
adder := adder

all: $(alan) $(adder)

$(alan): $(alan).c
	$(CC) $(CFLAGS) $< -o $@

$(adder): $(adder).c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f core
	rm -f $(alan)
	rm -f $(adder)

