BOOST_PATH=../../boost_1_54_0
CC       = gcc
CXX	= g++
CFLAGS   = -O0 -g -ggdb3 -std=gnu99 -Wall -Wextra
CXXFLAGS = -O0 -g -ggdb3 -Wall -Wextra -I${BOOST_PATH} -Icxx/ -Icxx/generated/
CFLAGS_PG= -g -pg -std=gnu99 -Wall -Wextra
INCLUDE  = -I. -I../ -I../src/lib/eo/ -I../build
CONFIG   = -DHAVE_CONFIG_H=1 -DEFL_EO_API_SUPPORT=1 -DEFL_BETA_API_SUPPORT=1
EINA_CFLAGS = `pkg-config --cflags eina`
EINA_LIBS   = `pkg-config --libs eina`
EO_SRC  = ../src/lib/eo/eo_ptr_indirection.c ../src/lib/eo/eo.c ../src/lib/eo/eo_class_class.c ../src/lib/eo/eo2_class_class.c  ../src/lib/eo/eo_base_class.c ../src/lib/eo/eo2_base_class.c
EO_OBJS  = eo_ptr_indirection.o eo.o eo_class_class.o eo2_class_class.o eo_base_class.o eo2_base_class.o
BENCH_SRC  = eo2_simple.c eo3_simple.c eo_simple.c eo2_inherit.c eo3_inherit.c eo_inherit.c eo2_composite.c eo_composite.c eo2-bench.c
BENCH_OBJS  = eo2_simple.o eo3_simple.o eo_simple.o eo2_inherit.o eo3_inherit.o eo_inherit.o eo2_composite.o eo_composite.o eo2-bench.o

all: eo2-bench test simple1 simple2 simple3 simple4 simple5 simple6 simple7 container1 mixin1 mixin2

eo.o: ../src/lib/eo/eo.c ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	$(CC) $(CFLAGS) -fPIC -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $< -o $@

eo_ptr_indirection.o: ../src/lib/eo/eo_ptr_indirection.c ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	$(CC) $(CFLAGS) -fPIC -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $< -o $@

eo_class_class.o: ../src/lib/eo/eo_class_class.c ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	$(CC) $(CFLAGS) -fPIC -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $< -o $@

eo2_class_class.o: ../src/lib/eo/eo2_class_class.c ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	$(CC) $(CFLAGS) -fPIC -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $< -o $@

eo_base_class.o: ../src/lib/eo/eo_base_class.c ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	$(CC) $(CFLAGS) -fPIC -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $< -o $@

eo2_base_class.o: ../src/lib/eo/eo2_base_class.c ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	$(CC) $(CFLAGS) -fPIC -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $< -o $@

eo3_inherit.o: eo3_inherit.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) -save-temps $< -o $@

eo3_simple.o: eo3_simple.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) -save-temps $< -o $@

eo3_interface.o: eo3_interface.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) -save-temps $< -o $@

eo3_inherit_interface.o: eo3_inherit_interface.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) -save-temps $< -o $@

libeo2.so: $(EO_OBJS)
	$(CC) $(CFLAGS) -shared -Wl,-soname,libeo2.so $(INCLUDE) $(CONFIG) $(EO_OBJS) $(EINA_LIBS) -o $@

cxx/samples/simple1.o: cxx/samples/simple1.cpp cxx/generated/eo3_simple.hpp
	$(CXX) -save-temps -std=c++11 $(CXXFLAGS) -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $< -o $@

cxx/samples/simple3.o: cxx/samples/simple3.cpp cxx/generated/eo3_simple.hpp
	$(CXX) -save-temps $(CXXFLAGS) -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $< -o $@

cxx/samples/container1.o: cxx/samples/container1.cpp
	$(CXX) -save-temps $(CXXFLAGS) -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $< -o $@

simple1: libeo2.so eo3_simple.o cxx/samples/simple1.o ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	$(CXX) $(CFLAGS) $(INCLUDE) $(CONFIG) cxx/samples/simple1.o eo3_simple.o $(EINA_LIBS) -L. -leo2  -o $@

simple2: libeo2.so eo3_simple.o cxx/samples/simple2.o ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	$(CXX) $(CFLAGS) $(INCLUDE) $(CONFIG) cxx/samples/simple2.o eo3_simple.o eo3_inherit.o $(EINA_LIBS) -L. -leo2  -o $@

simple3: libeo2.so eo3_simple.o cxx/samples/simple3.o ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	$(CXX) $(CFLAGS) $(INCLUDE) $(CONFIG) cxx/samples/simple3.o eo3_simple.o eo3_inherit.o $(EINA_LIBS) -L. -leo2  -o $@

simple4: libeo2.so cxx/samples/simple4.o eo3_simple.o ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	$(CXX) $(CFLAGS) $(INCLUDE) $(CONFIG) cxx/samples/simple4.o  eo3_simple.o $(EINA_LIBS) -L. -leo2  -o $@

simple5: libeo2.so cxx/samples/simple5.o cxx/generated/eo3_interface.hpp cxx/generated/eo3_inherit_interface.hpp eo3_interface.o eo3_inherit_interface.o ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	$(CXX) $(CFLAGS) $(INCLUDE) $(CONFIG) cxx/samples/simple5.o eo3_interface.o eo3_inherit_interface.o $(EINA_LIBS) -L. -leo2  -o $@

simple6: libeo2.so cxx/samples/simple6.o cxx/generated/eo3_inherit.hpp cxx/generated/eo3_simple.hpp cxx/generated/eo3_interface.hpp
	$(CXX) $(CFLAGS) $(INCLUDE) $(CONFIG) cxx/samples/simple6.o eo3_simple.o eo3_interface.o $(EINA_LIBS) -L. -leo2  -o $@

simple7: libeo2.so cxx/samples/simple7.o cxx/generated/eo3_simple.hpp
	$(CXX) $(CFLAGS) $(INCLUDE) $(CONFIG) cxx/samples/simple7.o eo3_simple.o $(EINA_LIBS) -L. -leo2  -o $@

container1: libeo2.so cxx/samples/container1.o eo3_container.o eo3_simple.o #cxx/generated/eo3_simple.hpp
	$(CXX) $(CFLAGS) $(INCLUDE) $(CONFIG) $^ $(EINA_LIBS) -L. -leo2  -o $@

mixin1: libeo2.so cxx/samples/mixin1.o eo3_mixin_mixin.o eo3_mixin_inherit.o eo3_simple.o
	$(CXX) $(CFLAGS) $(INCLUDE) $(CONFIG) $^ $(EINA_LIBS) -L. -leo2  -o $@

mixin2: libeo2.so cxx/samples/mixin2.o eo3_mixin_mixin.o eo3_mixin_inherit.o eo3_simple.o
	$(CXX) $(CFLAGS) $(INCLUDE) $(CONFIG) $^ $(EINA_LIBS) -L. -leo2  -o $@

.c.o:
	$(CC) $(CFLAGS) -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $< -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $< -o $@

eo2-bench: libeo2.so $(BENCH_OBJS) ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	$(CC) $(CFLAGS) $(INCLUDE) $(CONFIG) $(BENCH_OBJS) $(EINA_LIBS) -L. -leo2  -o $@

#eo2-bench-pg: $(EO_OBJS) $(BENCH_SRC) ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
#	$(CC) $(CFLAGS_PG) $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $(EINA_LIBS) $(EO_SRC) $(BENCH_SRC) -o $@

.cc.o:
	g++ -O2 -std=c++0x -c $(INCLUDE) $(CONFIG) $(EINA_CFLAGS) $< -o $@

test: libeo2.so simplesignal.o ../src/lib/eo/Eo.h ../src/lib/eo/eo_private.h
	g++ -O2 -std=c++0x $(INCLUDE) $(CONFIG) simplesignal.o $(EINA_LIBS) -L. -leo2 -o $@

clean:
	rm -f *~ *.o eo2-bench eo2-bench-pg test libeo2.so gmon.out simple1 simple2 simple3 simple4 simple5 simple6 simple7 container1 mixin1 mixin2 cxx/samples/*.o *.i *.s *.ii
	rm -rf cxx/doc/html cxx/doc/latex cxx/doc/man

doc:
	echo "entering cxx/doc/"
	$(MAKE) $(AM_MAKEFLAGS) -C cxx/doc all

