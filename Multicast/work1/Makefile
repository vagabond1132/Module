#target := $(object:%.o=%)
#
#.PHONY: target 
#
#sender:sender.cpp
#	g++ sender.cpp -o sender
#
#recver:recver.cpp
#	g++ sender.cpp -o recver

# $< 代表 依赖项  %.cpp    $@ 标示生成目标 %.o
$(OBJECTS):%.o:%.cpp
	g++ $< -o $@





.PHONY:clean
clean:     
	rm -f sender  recver

