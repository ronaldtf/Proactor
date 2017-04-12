.PHONY: all clean doc

all:
	cd cxx && make -f cxx.mk all
run:
	cd cxx && make -f cxx.mk run
clean:
	cd cxx && make -f cxx.mk clean
	@rm -rf doc
doc:
	@mkdir -p doc
	cd cxx && make -f cxx.mk doxygen
