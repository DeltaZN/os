#define SCREEN_SIZE 25*80*2
#define VIDEO_MEM 0xB8000

void clear(void);

char* video_memory = (char*) VIDEO_MEM;

void kernel() {
	//char* video_memory = (char*) VIDEO_MEM;
	*video_memory = 'X';
	clear();
}

void clear(void){
	//char* video_memory = (char*) VIDEO_MEM;
	unsigned int i = 0;
	while(i < SCREEN_SIZE){
		*(video_memory + (i++)) = ' ';
		*(video_memory + (i++)) = 0x0f;
	}
}
