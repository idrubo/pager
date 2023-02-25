CFLAGS := -Wall -g -O2

check  := check
dows   := dows

all: $(check) $(dows)

$(check):

$(dows): $(dows).c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	rm -f core
	rm -f $(check)

