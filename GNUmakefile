# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.3 (http://www.bakefile.org)
#     Do not modify, all changes will be overwritten!
# =========================================================================



# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------

# C++ compiler 
CXX := g++

# Standard flags for C++ 
CXXFLAGS := 

# Standard preprocessor flags (common for CC and CXX) 
CPPFLAGS := 

# Standard linker flags 
LDFLAGS := 

# Location and arguments of wx-config script 
WX_CONFIG := wx-config

# C++ flags to use with wxWidgets code 
WX_CXXFLAGS := `$(WX_CONFIG) --cxxflags`



# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

### Variables: ###

CPPDEPS = -MT$@ -MF`echo $@ | sed -e 's,\.o$$,.d,'` -MD
COUGAR_CXXFLAGS =  -Iinclude  $(WX_CXXFLAGS) $(CPPFLAGS) $(CXXFLAGS)
COUGAR_OBJECTS =  \
	cougar_cougarapp.o \
	cougar_cougarframe.o \
	cougar_cadmodel.o \
	cougar_facet.o \
	cougar_point.o \
	cougar_controlpanel.o \
	cougar_modelcanvas.o \
	cougar_paradialog.o \
	cougar_charvalidator.o \
	cougar_line.o \
	cougar_layer.o \
	cougar_pathcanvas.o \
	cougar_utility.o
TESTS_CXXFLAGS =  -Iinclude  $(WX_CXXFLAGS) $(CPPFLAGS) $(CXXFLAGS)
TESTS_OBJECTS =  \
	tests_tests.o \
	tests_testfacet.o \
	tests_testpoint.o \
	tests_testline.o \
	tests_testlayer.o \
	tests_testcadmodel.o \
	tests_facet.o \
	tests_cadmodel.o \
	tests_point.o \
	tests_line.o \
	tests_layer.o \
	tests_utility.o \
	tests_cougarframe.o \
	tests_controlpanel.o \
	tests_modelcanvas.o \
	tests_pathcanvas.o \
	tests_paradialog.o \
	tests_charvalidator.o

### Conditionally set variables: ###



### Targets: ###

all: cougar tests

install: all

uninstall: 

clean: 
	rm -f ./*.o
	rm -f ./*.d
	rm -f cougar
	rm -f tests

cougar: $(COUGAR_OBJECTS)
	$(CXX) -o $@ $(COUGAR_OBJECTS) $(LDFLAGS)  `$(WX_CONFIG) --libs gl,adv,core,base`

tests: $(TESTS_OBJECTS)
	$(CXX) -o $@ $(TESTS_OBJECTS) $(LDFLAGS)  `$(WX_CONFIG) --libs gl,adv,core,base`

cougar_cougarapp.o: ./src/cougarapp.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

cougar_cougarframe.o: ./src/cougarframe.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

cougar_cadmodel.o: ./src/cadmodel.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

cougar_facet.o: ./src/facet.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

cougar_point.o: ./src/point.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

cougar_controlpanel.o: ./src/controlpanel.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

cougar_modelcanvas.o: ./src/modelcanvas.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

cougar_paradialog.o: ./src/paradialog.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

cougar_charvalidator.o: ./src/charvalidator.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

cougar_line.o: ./src/line.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

cougar_layer.o: ./src/layer.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

cougar_pathcanvas.o: ./src/pathcanvas.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

cougar_utility.o: ./src/utility.cpp
	$(CXX) -c -o $@ $(COUGAR_CXXFLAGS) $(CPPDEPS) $<

tests_tests.o: ./test/tests.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_testfacet.o: ./test/testfacet.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_testpoint.o: ./test/testpoint.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_testline.o: ./test/testline.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_testlayer.o: ./test/testlayer.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_testcadmodel.o: ./test/testcadmodel.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_facet.o: ./src/facet.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_cadmodel.o: ./src/cadmodel.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_point.o: ./src/point.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_line.o: ./src/line.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_layer.o: ./src/layer.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_utility.o: ./src/utility.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_cougarframe.o: ./src/cougarframe.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_controlpanel.o: ./src/controlpanel.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_modelcanvas.o: ./src/modelcanvas.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_pathcanvas.o: ./src/pathcanvas.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_paradialog.o: ./src/paradialog.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

tests_charvalidator.o: ./src/charvalidator.cpp
	$(CXX) -c -o $@ $(TESTS_CXXFLAGS) $(CPPDEPS) $<

.PHONY: all install uninstall clean


# Dependencies tracking:
-include ./*.d
