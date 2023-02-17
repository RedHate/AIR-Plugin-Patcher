
all: x86_64 arm-linux-gnueabihf

x86_64:
	gcc -w mpc-plugin-patcher.c -o AIR-Plugin-Patcher-Linux-x86_64
	strip AIR-Plugin-Patcher-Linux-x86_64
	chmod a+x AIR-Plugin-Patcher-Linux-x86_64
	
arm-linux-gnueabihf:
	arm-linux-gnueabihf-gcc -w mpc-plugin-patcher.c -o AIR-Plugin-Patcher-Linux-ARMHF
	arm-linux-gnueabihf-strip AIR-Plugin-Patcher-Linux-ARMHF
	chmod a+x AIR-Plugin-Patcher-Linux-ARMHF



test:
	gcc -w mpc-plugin-patcher.c -o mpc-plugin-patcher
	strip mpc-plugin-patcher
	chmod a+X mpc-plugin-patcher
	rm -rf test.txt MPC-PATCHED
	./mpc-plugin-patcher MPC 2
	strings MPC-PATCHED > test.txt
	upx MPC-PATCHED

clean:
	rm -rf test.txt MPC-PATCHED
	rm -rf AIR-Plugin-Patcher-Linux-x86_64
	rm -rf AIR-Plugin-Patcher-Linux-ARMHF
