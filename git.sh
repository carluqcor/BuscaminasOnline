#!/bin/bash
if [ $1 ];then
	$(git add *.cpp)
	$(git add *.hpp)
	$(git add CMakeLists.txt)
else
	$(git push origin master)
fi
