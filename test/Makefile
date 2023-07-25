CXX=g++
CXXFLAGS=-g -Wall
LDFLAGS=-lcurl
SRCDIR=../src

defaultnametest: defaultnametest.cpp $(SRCDIR)/curlwrap.cpp $(SRCDIR)/filename.cpp $(SRCDIR)/curlwrap.hpp $(SRCDIR)/filename.hpp
	$(CXX) $(CXXFLAGS) defaultnametest.cpp $(SRCDIR)/curlwrap.cpp $(SRCDIR)/filename.cpp -o defaultnametest $(LDFLAGS)