CFLAGS = -g # Flag for implicit rules. Turn on debug info
CXXFLAGS = -g
OBJS = binarycalculator.o pbyte.o parser.o

$(OBJS): %.o: %.cpp


binarycalculator.o: binarycalculator.cpp pbyte.h parser.h
pbyte.o: pbyte.cpp pbyte.h
parser.o: parser.cpp pbyte.h parser.h

binarycalculator: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
