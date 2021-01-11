void main() {
	char* video_memory = (char*) 0xB8000;
	*video_memory = 'X';
}
