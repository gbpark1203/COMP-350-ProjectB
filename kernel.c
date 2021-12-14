// Gibeom Park COMP350

void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);

void main(){

	int startVidMem = 0xb800;
	int vidMemOffset = 0x0;
	int white = 0x7;
	char* letters = "Hello World\0";
    char line[80];
    char buffer[512];


    interrupt(0x21,0,0,0,0);

	while(*letters != 0x0){
		putInMemory(startVidMem, vidMemOffset, *letters);
		vidMemOffset++;
		putInMemory(startVidMem, vidMemOffset, white);
		vidMemOffset++;

		letters++;
	}

    readSector(buffer, 30);
    printString(buffer);
    makeInterrupt21();
    interrupt(0x21,0,"Type : ",0,0);
    interrupt(0x21,1,line,0,0);

    interrupt(0x21,0,line,0,0);

	while(1);

}

void printString(char* chars){
    int i = 0;
    while(chars[i] != '\0'){
        int charvalue = chars[i];
        interrupt(0x10, 0xe*256+charvalue, 0, 0, 0);
        i++;
    }
}

void readString(char* line){

    int enter = 0xd;
    int backspace = 0x8;

    int i = 0;

    while(1){

        int asciivalue = interrupt(0x16, 0, 0, 0, 0);

        if(asciivalue == enter){
	        line[i] = 0xd;
            interrupt(0x10, 0xe*256+0xd, 0, 0, 0);
			i++;
			line[i] = 0xa;
            interrupt(0x10, 0xe*256+0xa, 0, 0, 0);
			i++;
			line[i] = 0x0;
            interrupt(0x10, 0xe*256+0x0, 0, 0, 0);
			break;
        }
        else if(asciivalue == backspace){
            if(i>0){
                line[i] = 0x0;
                i--;
                interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
				i++;
				interrupt(0x10, 0xe*256+0x0, 0, 0, 0);
				i--;
				interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
            }
        }
        else{
            line[i] = asciivalue;
            interrupt(0x10, 0xe*256+asciivalue, 0, 0, 0);
            i++;
        }
    }
}

void readSector(char* buffer, int sector){

	int ah = 2;
	int al = 1;
	int bx = buffer;
	int ch = 0;
	int cl = sector + 1;
	int dh = 0;
	int dl = 0x80;

	int ax = ah*256+al;
	int cx = ch*256+cl;
	int dx = dh*256+dl;

	interrupt(0x13, ax, bx, cx, dx);

}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	if (ax == 0){
	    printString(bx);
    }
	else if (ax == 1){
		readString(bx);
	}
	else if (ax == 2){
		readSector(bx, 30);
	}
	else if (ax > 2){
		printString("This is Error\0");
	}
}
