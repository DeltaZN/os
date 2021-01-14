const char *ECHO_COMMAND = "echo\0";
const char *WRITE_MEMORY_COMMAND = "wmem\0";
const char *READ_MEMORY_COMMAND = "rmem\0";

void print(const char *str);

void *base_pointer = 0x00000000;

void print_newline(void);

char buffer[256];
char output[256];

void buf_cpy(char *buf, int start) {
    int current = start;
    int i = 0;
    while (buf[current] != '\0') {
        buffer[i++] = buf[current++];
    }
    buffer[i] = '\0';
}

int compare_str(const char *str1, const char *str2) {
    int i = 0;
    while (str1[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 0;
        }
        i++;
    }
    return 1;
}

void echo_cmd(char *buf_ptr) {
    print(buf_ptr);
    print_newline();
}

void write_to_mem(char* ptr, char *str){
    unsigned int i = 0;
    while (str[i] != '\0') {
        ptr[i] = str[i];
        ptr[i++] = 0x02;
    }
}

char* read_from_mem(char* ptr){
    unsigned int i = 0;
//    while (ptr[i] != '\0') {
//        output[i] = ptr[i];
//    }
    return output;
}

void handle_command(char *buf_ptr) {
    int i = 0;
    buf_cpy(buf_ptr, 5);
    if (compare_str(ECHO_COMMAND, buf_ptr)) {
        echo_cmd(buffer);
    } else if (compare_str(WRITE_MEMORY_COMMAND, buf_ptr)) {
        print("Command in dev\n");
    } else if (compare_str(READ_MEMORY_COMMAND, buf_ptr)) {
        char *result = read_from_mem(buffer);
        echo_cmd(result);
    } else {
        print("Command not found\n");
    }
}