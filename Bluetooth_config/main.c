#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define DATA_SIZE 56
#define WORD_LENGTH 16
#define MAX_SEGMENTS 64

#define TYPE_TEXT 0
#define TYPE_NUMERIC 1
#define TYPE_BYTE 2

#define device_name_SEG 0
#define device_name_POS 0
#define rom_revision_SEG 0
#define rom_revision_POS 1
#define serial_number_SEG 0
#define serial_number_POS 2
#define bd_addr_part0_SEG 1
#define bd_addr_part0_POS 0
#define bd_addr_part1_SEG 1
#define bd_addr_part1_POS 1
#define bd_pass_part0_SEG 1
#define bd_pass_part0_POS 2
#define serial_baudrate_SEG 2
#define serial_baudrate_POS 0
#define audio_bitrate_SEG 2
#define audio_bitrate_POS 1
#define sleep_period_SEG 2
#define sleep_period_POS 2
#define serial_parity_SEG 3
#define serial_parity_POS 0
#define serial_data_bit_SEG 3
#define serial_data_bit_POS 1
#define serail_stop_bit_SEG 3
#define serail_stop_bit_POS 2
#define bd_pass_part1_SEG 4
#define bd_pass_part1_POS 0
#define rom_checksum_part0_SEG 4
#define rom_checksum_part0_POS 1
#define rom_checksum_part1_SEG 4
#define rom_checksum_part1_POS 2

int stringToInt(char* number){
	if (number==NULL){
		return -1;
	}
	int result=0;
	int counter=0;
	while(number[counter]){
		result *= 10;
		result += number[counter++] - '0';	
	}
	return result;
}
struct Segment {
	char type;
	char meta[7];
	union Data {
		char bytes[DATA_SIZE];
		int numeric[DATA_SIZE / sizeof(int)];
		char text[DATA_SIZE/WORD_LENGTH][WORD_LENGTH];
	}data;
};

struct Segment seg[MAX_SEGMENTS];
int segment_count=0;
void readFile(char* filename){
	int fd = open(filename,O_RDONLY);
	if(fd==-1){
		fprintf(stdout,"Error opening file %s",filename);
		exit(1);
	}

	int rd = read(fd,&seg[0],sizeof(struct Segment) * MAX_SEGMENTS);
	close(fd);

	segment_count=rd / sizeof(struct Segment);

	if(rd % sizeof(struct Segment) != 0){
		fprintf(stderr,"Invalid size of conf file");
	}
}

void Write(char* filename){
        int fd=open(filename,O_CREAT | O_TRUNC | O_RDWR,0666);
        if(fd==-1){
                fprintf(stderr,"Error opening file");
                exit(1);
        }

        int wr=write(fd,&seg,sizeof(seg));
        if(wr!= sizeof(seg)){
                fprintf(stderr,"Error writing to file");
                exit(1);
        }
	close(fd);
}

void parameterToSegmentPosition(char* parameter,int* segment,int* position){
    if (strcmp("device_name", parameter) == 0) {
        *segment = device_name_SEG;
        *position = device_name_POS;
    } else if (strcmp("rom_revision", parameter) == 0) {
        *segment = rom_revision_SEG;
        *position = rom_revision_POS;
    } else if (strcmp("serial_number", parameter) == 0) {
        *segment = serial_number_SEG;
        *position = serial_number_POS;
    } else if (strcmp("bd_addr_part0", parameter) == 0) {
        *segment = bd_addr_part0_SEG;
        *position = bd_addr_part0_POS;
    } else if (strcmp("bd_addr_part1", parameter) == 0) {
        *segment = bd_addr_part1_SEG;
        *position = bd_addr_part1_POS;
    } else if (strcmp("bd_pass_part0", parameter) == 0) {
        *segment = bd_pass_part0_SEG;
        *position = bd_pass_part0_POS;
    } else if (strcmp("serial_baudrate", parameter) == 0) {
        *segment = serial_baudrate_SEG;
        *position = serial_baudrate_POS;
    } else if (strcmp("audio_bitrate", parameter) == 0) {
        *segment = audio_bitrate_SEG;
        *position = audio_bitrate_POS;
    } else if (strcmp("sleep_period", parameter) == 0) {
        *segment = sleep_period_SEG;
        *position = sleep_period_POS;
    } else if (strcmp("serial_parity", parameter) == 0) {
        *segment = serial_parity_SEG;
        *position = serial_parity_POS;
    } else if (strcmp("serial_data_bit", parameter) == 0) {
        *segment = serial_data_bit_SEG;
        *position = serial_data_bit_POS;
    } else if (strcmp("serial_stop_bit", parameter) == 0) {
        *segment = serail_stop_bit_SEG;
        *position = serail_stop_bit_POS;
    } else {
        *segment = -1;
        *position = -1;
    }	
}
int segmentType(int segment){
	if(segment < 0 || segment > 64){
		fprintf(stderr,"Invalid segment");
		exit(1);
	}
	
	int type=seg[segment].type;
	return type;
}
void setParameterText(int segment, int position, char* value){
	if(segment < 0 || segment > 56){
                fprintf(stderr,"Invalid segment");
                exit(1);
        }

	if(position < 0 || position > 56){
		fprintf(stderr,"Invalid position");
		exit(1);
	}
	if(sizeof(value) > WORD_LENGTH){
		fprintf(stderr,"New value cannot be correctly changed");
	}
	strncpy(&seg[segment].data.text[position][0],value,WORD_LENGTH);
}

void setParameterInt(int segment, int position, char* value){
        if(segment < 0 || segment > 56){
                fprintf(stderr,"Invalid segment");
                exit(1);
        }

        if(position < 0 || position > 56){
                fprintf(stderr,"Invalid position");
                exit(1);
        }

	if(sizeof(value) > sizeof(int)){
                fprintf(stderr,"New value cannot be correctly changed");
        }


        seg[segment].data.numeric[position]=stringToInt(value);
}

void setParameterByte(int segment, int position, char* value){
        if(segment < 0 || segment > 56){
                fprintf(stderr,"Invalid segment");
                exit(1);
        }

        if(position < 0 || position > 56){
                fprintf(stderr,"Invalid position");
                exit(1);
        
        seg[segment].data.bytes[position]=value[0];
	}
}

void setParameter(char* parameter,char* value){
	int segment;
	int position;
	parameterToSegmentPosition(parameter,&segment,&position);
	
	int type=segmentType(segment);
	if(type == 0 ){
		setParameterText(segment,position,value);
	}
	else if(type == 1){
		setParameterInt(segment,position,value);
	}
	else if(type == 2){
		setParameterByte(segment,position,value);
	}
}

void setMetaOptionBit(char* parameter,char value){
	if(value != '0' && value != '1'){
		fprintf(stderr,"Invalid value");
		exit(1);
	}
	int segment=-1;
        int position=-1;
	char byte;
	char bit;
        parameterToSegmentPosition(parameter,&segment,&position);
	byte=seg[segment].meta[position/8];
	bit=byte >> position%8;
	seg[segment].meta[position/8] |= value << bit;
}

char* getParameterValueText(int segment, int position){
	char* value=malloc(WORD_LENGTH);
        strncpy(value,seg[segment].data.text[position],WORD_LENGTH);
	return value;
}

int getParameterValueInt(int segment,int position){
        return seg[segment].data.numeric[position];
}

char getParameterValueByte(int segment, int position){
        return seg[segment].data.bytes[position];
}
void printParameterValue(char * parameter){
	int segment;
	int position;
	parameterToSegmentPosition(parameter,&segment,&position);
	int type=segmentType(segment);
	if(type == 0 ){
                char* value = getParameterValueText(segment,position);
		printf("%s %s\n ",parameter,value);
        }
        else if(type == 1){
               int value =  getParameterValueInt(segment,position);
	       printf("%s %d\n ",parameter,value);
        }
        else if(type == 2){
                char value = getParameterValueByte(segment,position);
		printf("%s %c\n ",parameter,value);
        }
}
char getMetaOptionBit(char* parameter){
	int segment=-1;
	int position=-1;
	parameterToSegmentPosition(parameter,&segment,&position);
	if(segment == -1 || position == -1){
		fprintf(stderr,"Error converting parameter to segment/position");
		exit(1);
	}
	int byte=seg[segment].meta[position/8];
        int bit=byte >> position%8;
	return bit;

}
int segmentParameters(int segment){
	if( seg[segment].type == TYPE_TEXT ){
		return DATA_SIZE / WORD_LENGTH;
	}
	else if ( seg[segment].type == TYPE_NUMERIC){
		return DATA_SIZE / sizeof(int);
	}
	else if ( seg[segment].type == TYPE_BYTE) {
		return DATA_SIZE;
	}
	return -1;
}
char letterType(char letter){
	int type;
	if(letter=='t'){
		type=TYPE_TEXT;
	}
	else if(letter=='n'){
		type=TYPE_NUMERIC;
	}
	else if(letter=='b'){
		type=TYPE_BYTE;
	}
	return type;
}
void createConfig(char* filename,int* segments,char* types,int segmentNumber){
	int* end=segments+segmentNumber;
	while(segments < end){;
		char typeN=letterType(*types);
		seg[*segments].type=typeN;
		segments++;
		types++;
	}
	
	Write(filename);
}
	
void printActiveParameters(){
	//TODO	
}

void printAllParameters(){ 
	for(int segment=0;segment<segment_count;segment++){
		int max_position=segmentParameters(segment);
		for(int position=0;position<max_position;position++){
			//TODO				
		}
	} 
	
}
//[a-zA-Z0-9_-]
bool isLowerCase(char value){
	return value>='a' && value <='z';
}

bool isUpperCase(char value){
         return value>='A' && value <='Z';
}

bool isNumber(char value){
	return value>=0 && value<=9;
}
//[a-zA-Z0-9_-]
bool validationOne(char* value){
	while(value){
		if(isLowerCase(*value) || isUpperCase(*value) || isNumber(*value) || 
		*value=='_' || *value=='-'){
			value++;
			continue;	
		}
		else{
			fprintf(stderr,"Incorrect value");
			exit(1);
		}

	}
	return true;
}
//[A-Z0-9]
bool validationTwo(char* value){
	while(value){
                if( isUpperCase(*value) || isNumber(*value)){
                        value++;
                        continue;
                }
                else{
                        fprintf(stderr,"Incorrect value");
                        exit(1);
                }

        }
        return true;
}
//[a-z0-9]
bool validationThree(char* value){
        while(value){
                if( isLowerCase(*value) || isNumber(*value)){
                        value++;
                        continue;
                }
                else{
                        fprintf(stderr,"Incorrect value");
                        exit(1);
                }

        }
        return true;
}
//[a-z0-9:]
bool validationFour(char* value){
        while(value){
                if( isLowerCase(*value) || isNumber(*value) || *value==':'){
                        value++;
                        continue;
                }
                else{
                        fprintf(stderr,"Incorrect value");
                        exit(1);
                }

        }
        return true;
}

bool validateSerialBaudrate(char* value){
	int valueInt=stringToInt(value);
	if(valueInt != 1200 && valueInt != 2400 &&  valueInt != 4800 && valueInt != 9600 &&
	   valueInt != 19200 && valueInt != 115200){
		fprintf(stderr,"Incorrect value");
		exit(1);	
	}
	return true;
}

bool validateAudioBitrate(char* value){
        int valueInt=stringToInt(value);
        if(valueInt != 32 && valueInt != 96 &&  valueInt != 128 && valueInt != 160 &&
           valueInt != 192 && valueInt != 256 && valueInt != 320){
                fprintf(stderr,"Incorrect value");
                exit(1);
        }
        return true;
}

bool validateSleepPeriod(char* value){
        int valueInt=stringToInt(value);
        if(valueInt != 100 && valueInt != 500 &&  valueInt != 1000 && valueInt != 5000 &&
           valueInt != 10000){
                fprintf(stderr,"Incorrect value");
                exit(1);
        }
        return true;
}

bool validateSerialParity(char *value){
	if(*value != 'N' && *value != 'E' && *value != 'O'){
		fprintf(stderr,"Incorrect value");
                exit(1);
	}
	return true;
}

bool validateSerialDataBit(char *value){
        if(*value != '1' && *value != '5' && *value != '6' && *value != '7' && *value != '8'){
                fprintf(stderr,"Incorrect value");
                exit(1);
        }
        return true;
}

bool validateSerialStopBit(char *value){
        if(*value != '1' && *value != '0'){
                fprintf(stderr,"Incorrect value");
                exit(1);
        }
        return true;
}

bool validationAll(char* parameter, char* value){
	if(strcmp(parameter,"device_name")){
		return validationOne(value);
	}
	else if(strcmp(parameter,"rom_revision")){
		return validationOne(value);
	}
	else if(strcmp(parameter,"serial_number")){
                return validationTwo(value);
        }
	else if(strcmp(parameter,"bd_addr_part0")){
                return validationFour(value);
        }
	else if(strcmp(parameter,"bd_addr_part1")){
                return validationFour(value);
        }
	else if(strcmp(parameter,"bd_pass_part0")){
                return validationThree(value);
        }
	else if(strcmp(parameter,"serial_baudrate")){
                return validateSerialBaudrate(value);
        }
	else if(strcmp(parameter,"sleep_period")){
                return validateSleepPeriod(value);
        }
	else if(strcmp(parameter,"audio_bitrate")){
                return validateAudioBitrate(value);
        }
	else if(strcmp(parameter,"serial_parity")){
                return validateSerialParity(value);
        }
	else if(strcmp(parameter,"serial_data_bit")){
                return validateSerialDataBit(value);
        }
	else if(strcmp(parameter,"serial_stop_bit")){
                return validateSerialStopBit(value);
        }
	else if(strcmp(parameter,"bd_pass_part1")){
                return validationThree(value);
        }
	else if(strcmp(parameter,"rom_checksum_part0")){
                return validationThree(value);
        }
	else if(strcmp(parameter,"rom_checksum_part1")){
                return validationThree(value);
        }
	else return false;
}




int main(int argc, char** argv){
	//argv[1] - file
	//argv[2] - option
	printf("%d",argc);

	//./bk8000l_conf conf.bin -s device_name "DeviceN1"
	if(strcmp(argv[2],"-s")==0){
		if(argc !=5){
			fprintf(stderr,"Invalid arguments count");
			exit(1);
		}
		readFile(argv[1]);
		setParameter(argv[3],argv[4]);
		setMetaOptionBit(argv[3],'1');
		Write(argv[1]);
	}
	//./bk8000l_conf conf.bin -S device_name "DeviceN1"
	else if(strcmp(argv[2],"-S")==0){
                if(argc !=5){
                        fprintf(stderr,"Invalid arguments count");
                        exit(1);
                }
		readFile(argv[1]);
                setParameter(argv[3],argv[4]);
		Write(argv[1]);
        }
	//./bk8000l_conf conf.bin -g device_name
	else if(strcmp(argv[2],"-g")){
		if(argc != 4){
                        fprintf(stderr,"Invalid arguments count");
                        exit(1);
		}
		readFile(argv[1]);
		if(getMetaOptionBit(argv[3])){
			printParameterValue(argv[3]);
		}
	}
	//./bk8000l_conf conf.bin -G device_name
	else if(strcmp(argv[2],"-G")){
		if(argc != 4){
                        fprintf(stderr,"Invalid arguments count");
                        exit(1);
                }
		readFile(argv[1]);
		printParameterValue(argv[3]);
	}
	//./bk8000l_conf conf.bin -l
	else if(strcmp(argv[2],"-l")){
		if(argc>3){
			readFile(argv[1]);
			for(int i=3;i<argc;i++){
				printParameterValue(argv[i]);
			}
		}
		else if(argc==3){
			//TODO
		}

	}
	//./bk8000l_conf conf.bin -L
	else if(strcmp(argv[2],"-L")){
                if(argc>3){
                        readFile(argv[1]);
                        for(int i=3;i<argc;i++){
				 if(getMetaOptionBit(argv[3])){
                       			 printParameterValue(argv[3]);
               			 }
                       	 }
                }
                else if(argc==3){
                        //TODO
                }
	}
	//./bk8000l_conf conf.bin -b device_name 1
	else if(strcmp(argv[2],"-b")){
		if(argc!=5){
			fprintf(stderr,"Invalid arguments count");
               		exit(1);
		}
		readFile(argv[1]);
		setMetaOptionBit(argv[3],*argv[4]);
		Write(argv[1]);
	}
	//./bk8000l_conf -h
	else if(strcmp(argv[2],"-h")){
		if(argc!=2){
			fprintf(stderr,"Invalid arguments count");
                        exit(1);
			//TODO
		}
	}
}
