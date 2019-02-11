#
#Author  ligang
#2016-10-15 00:41:33
#
# 865853453
# 15762522956

# dir
INCLUDEDIR=./include/
BINDIR=./bin/
LIBSDIR=./libs/
LIBDIR=./lib/
SRCDIR=./src/
OBJDIR=./obj/
LOGDIR=./log/

#find file ; *.cpp, *.h
CPPFILE=`find . -iname "*.cpp"`
OFILE=`find . -iname "*.o"`
LIBDIR=`find . -iname "*.so*"`


#g++
$(BINDIR)hask:$(OBJDIR)
	#gcc $(SRCDIR)*  $(LIBSDIR)* -o $(BINDIR)hask
	g++ $(CPPFILE) -o $(BINDIR)test  -std=c++11


.PHNOY:clean release
clean:
	rm -rf $(OBJDIR)*
	rm -rf $(LOGDIR)*
	rm -rf $(BINDIR)*


release:
	rm -rf $(BINDIR)*
	rm -rf $(LOGDIR)*
