CFLAGS = -g # Flag for implicit rules. Turn on debug info
CXXFLAGS = -g
OBJS = binarycalculator.o pbyte.o parser.o
OBJS2 = pbytemain.o pbyte.o

$(OBJS): %.o: %.cpp
$(OBJS2): %.o: %.cpp


binarycalculator.o: binarycalculator.cpp pbyte.h parser.h
pbyte.o: pbyte.cpp pbyte.h
parser.o: parser.cpp pbyte.h parser.h
pbytemain.o: pbytemain.cpp pbyte.h

binarycalculator: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

pbytemain: $(OBJS2)
	$(CXX) $(CFLAGS) $(OBJS2) $(LDFLAGS) -o $@
