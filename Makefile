TARGET=gitdown
SOURCE=gitdown.cpp git_clone.cpp
OBJECT=gitdown.obj git_clone.obj
OUTPUT=gitdown
CPP=g++
MSVC=cl

make:
	$(CPP) --std=c++17 $(SOURCE) -o $(OUTPUT)
	strip $(OUTPUT)

gnu:
	$(CPP) --std=c++17 $(SOURCE) -o $(OUTPUT).exe
	strip $(OUTPUT).exe
	upx -9 $(OUTPUT).exe

msvc:
	$(MSVC) /std:c++17 /EHsc /Fe$(OUTPUT).exe $(SOURCE)
	strip $(OUTPUT).exe
	del $(OBJECT)
	upx -9 $(OUTPUT).exe

install:
	upx -9 $(OUTPUT)
	cp $(OUTPUT) /usr/bin/$(OUTPUT)

clean:
	rm $(OUTPUT)

winclean:
	del $(OUTPUT).exe
