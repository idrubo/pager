CFLAGS := -Wall -g -O2

check  := check
dows   := dows

all: $(check) $(dows)

$(check):

$(dows): $(dows).c
	@echo !!!!!!!!!!!!!!!!
	@echo We compile here to find the errors and warnings of $(dows)
	@echo !!!!!!!!!!!!!!!!
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	rm -f core
	rm -f $(check)

