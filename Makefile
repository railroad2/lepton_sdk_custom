CC=gcc
FLAGS= -fPIC -c -Wall
INC=-I./Inc	
UVCFLAG=-I/storage/irc/GetThermal/source/libuvc/build/include \
        -L/storage/irc/GetThermal/source/libuvc/build \
		-luvc

CURPATH=`pwd`

INSTALLPATH=/usr/local

shared:
	mkdir Obj lib
	$(CC) $(FLAGS) -o ./Obj/LEPTON_AGC.o ./Src/LEPTON_AGC.c $(INC)
	$(CC) $(FLAGS) -o ./Obj/LEPTON_OEM.o ./Src/LEPTON_OEM.c $(INC)
	$(CC) $(FLAGS) -o ./Obj/LEPTON_RAD.o ./Src/LEPTON_RAD.c $(INC)
	$(CC) $(FLAGS) -o ./Obj/LEPTON_SDK.o ./Src/LEPTON_SDK.c $(INC) $(UVCFLAG)
	$(CC) $(FLAGS) -o ./Obj/LEPTON_SYS.o ./Src/LEPTON_SYS.c $(INC)
	$(CC) $(FLAGS) -o ./Obj/LEPTON_VID.o ./Src/LEPTON_VID.c $(INC)
	$(CC) $(FLAGS) -o ./Obj/uvc_sdk.o ./Src/uvc_sdk.c $(INC) $(UVCFLAG)
	
	ld -shared $(CURPATH)/Obj/*.o -o ./lib/liblepton.so

install:
	cp ./lib/liblepton.so $(INSTALLPATH)/lib/
	cp ./Inc/*.h $(INSTALLPATH)/include/

clean:
	rm ./Obj/*.o
	rm ./lib/liblepton.so
