build: main.o matrix.o math.o
	gcc $^ -lm

install: FlexirtaM.so
ifeq ($(OS), Windows_NT)
else
	cp $^ /usr/lib
endif


buildlib: FlexirtaM.so

FlexirtaM.so: matrix.o math.o
	gcc -o FlexirtaM.so -shared -fPIC $^

debug: mainDebug.o matrixDebug.o mathDebug.o
	gcc $^ -lm -g

main.o: main.c FlexirtaM.h FlexirtaM_Build.h
matrix.o: matrix.c FlexirtaM.h FlexirtaM_Build.h
math.o: math.c FlexirtaM.h FlexirtaM_Build.h

%.o: %.c
	gcc -c $<

%.o: %.s
	gcc -c $<

clean:
ifeq ($(OS), Windows_NT)
	del *.o *.a *.exe
else
	rm *.o *.a *.out
endif
