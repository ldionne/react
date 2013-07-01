
# Simple Makefile forwarding to the build directory.

.PHONY: all gen-cmake test doc

# Suppress the output of the forwarding of commands.
${VERBOSE}.SILENT:

all:
	make -C build $@ $(args)

%:
	make -C build $@ $(args)

test:
	make -C build $@ $(args)

doc:
	make -C build $@ $(args)

gen-cmake:
	rm -rf build && mkdir build && cd build && cmake .. $(args)
