/*
 *
 *  Ultros MPC plugin patcher for Akai MPC / FORCE
 * 
 * Dependancies:
 *  *requires libc*
 * 
 * What it does:
 * 	This program will patch your MPC or FORCE executables to activate plugins using a valid token.
 * 	Say you have a registered a copy of MINID but its the only plugin you have. This will enable-
 * 	all the remaining plugins using your MINID token. Once patched, you only need to activate the
 * 	one plugin you own to enable all plugins available. (but- requires content to be put in place manually)
 * 
 * How To:
 * 	Copy the MPC exectuable from your Akai MPC or FORCE via ssh, then run this program
 * 	Next copy the resulting MPC-PATCHED or FORCE-PATCHED back to your unit, but be sure to-
 * 	rename it back to the correct name.
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

char *plugin_name[]={
	"air-fabric",
	"air-organ",
	"air-opx-4",
	"air-stage-electric-piano",
	"air-stage-piano",
	"air-stage-strings",
	"air-mini-d",
	"air-flavor-pro",
	"air-delay",
	"air-vintage-filter",
	"air-chorus",
	"air-multitap-delay"
};

char *plugin_data[]={
	"AKFB1",
	"AKOR1",
	"AKOX1",
	"AKSE1",
	"AKSP1",
	"AKSS1",
	"ARMD1",
	"ARFL2",
	"ARDL3",
	"ARVF1",
	"ARCH1",
	"ARDL2" 
};

char *plugin_block[]={
	 "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"
};

int main(int argc, char **argv){
	
	if(argc < 3){
		printf("\r\n Ultros 2023:\r\n");
		printf(" 	What it does:\r\n");
		printf(" 	This program will patch your MPC or FORCE executables to activate plugins using a valid token.\r\n");
		printf(" 	Say you have a registered a copy of MINID but its the only plugin you have. This will enable-\r\n");
		printf(" 	all the remaining plugins using your MINID token. Once patched, you only need to activate the\r\n");
		printf(" 	one plugin you own to enable all plugins available. (but- requires content to be put in place manually)\r\n\r\n");
		printf(" How To:\r\n");
		printf(" 	Copy the MPC exectuable from your Akai MPC or FORCE via ssh, then run this program\r\n");
		printf(" 	Next copy the resulting MPC-PATCHED or FORCE-PATCHED back to your unit, but be sure to-\r\n");
		printf(" 	rename it back to the correct name.\r\n");
		printf("\r\n Note:\r\n");
		printf(" 	On Hakai, all executables are compressed with upx, must unpack first.\r\n\r\n");
		printf(" 	Download upx and run: upx -d MPC.\r\n\r\n");
	
		printf("\r\n Usage:	%s <MPC/FORCE> <NUMBER>\r\n\r\n", argv[0]);
		printf(" Please select from the following using the coresponding number:\r\n (choose the one you own not the one you want)\r\n");
		int i;
		for(i=0;i<12;i++){
			printf("	%2d - %s\r\n", i, plugin_name[i]);
		}
		printf("\r\n");
		return 0;
	}
		
	int chosen = atoi(argv[2]);
	printf(" Chosen plugin token to patch against: %s\r\n", plugin_name[chosen]);
	
	int fp = open(argv[1], O_RDONLY);
	if(fp < 0){
		printf(" debug: input file open failure\r\n");
		return 0;
	}
	
	char new_buffer[128];
	sprintf(new_buffer, "%s-PATCHED", argv[1]);
	int fp_out = open(new_buffer, O_WRONLY|O_CREAT,0644);
	if(fp < 0){
		printf("debug: output file open failure\r\n");
		return 0;
	}
	
	printf("debug: open success\r\n");
	
	unsigned int filesize = lseek(fp, 0, SEEK_END);
	lseek(fp, 0, SEEK_SET);
	unsigned char *buffer = malloc(filesize);
	read(fp, buffer,filesize);
	printf("debug: buffer allocated, size: %d \r\n", filesize);
	
	unsigned char byte;
	unsigned int i = 0;
	
	for( i=0; i<filesize; i++ ){
		unsigned int j = 0;
		for( j=0; j<12; j++){
			if(memcmp(&buffer[i], plugin_data[j], 5) == 0){
				//printf("debug: plugin tag found at offset 0x%08X for %s\r\n", i, plugin_name[j]);
				memcpy(&buffer[i], plugin_data[chosen], 5);
			}
		}
	}

	for( i=0; i<filesize; i++ ){
		unsigned int j = 0;
		for( j=0; j<12; j++){
			if(memcmp(&buffer[i], plugin_block[j], 736) == 0){
				//printf("debug: plugin data found at offset 0x%08X for %s\r\n", i, plugin_block[j]);
				memcpy(&buffer[i], plugin_block[chosen], 736);
			}
		}
	}
	
	int ret = write(fp_out, buffer, filesize);
	if (ret) {
		printf("debug: %S-PATCHED written!\r\n", argv[1]);
	}
	
	free(buffer);
	close(fp);
	close(fp_out);
	
	printf("debug: exiting\r\n");
	return 0;
	
};
