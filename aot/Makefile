JL_SHARE = $(shell julia -e 'print(joinpath(Sys.BINDIR, Base.DATAROOTDIR, "julia"))')
CFLAGS   += $(shell $(JL_SHARE)/julia-config.jl --cflags)
CXXFLAGS += $(shell $(JL_SHARE)/julia-config.jl --cflags)
LDFLAGS  += $(shell $(JL_SHARE)/julia-config.jl --ldflags)
LDLIBS   += $(shell $(JL_SHARE)/julia-config.jl --ldlibs)

CXXFLAGS:=$(filter-out -std=gnu99,$(CXXFLAGS))
OPT = -O2
CXX = clang++

test: test.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $(OPT)

all: test

clean:
	rm test

juliaimage:
	julia create_package.jl