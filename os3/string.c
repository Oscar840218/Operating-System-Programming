

int strlen(char *s) { //return length of string
    int index = 0;
    while (s[index]!='\0') index++;
    return index;
}

int strcmp(char *s1, char *s2) { //compare string return 0 if same
    if (strlen(s1)!=strlen(s2)) return 1;
    int index = 0;
    while (s1[index]!='\0') {
        if (s1[index]!=s2[index]) return 1;
        index++;
    }
    return 0;
}

int strcpy(char *dest, char *src) { //copy string form src to dest
    int index = 0;
    while (src[index]!='\0') {
        dest[index] = src[index];
        index++;
    }
    dest[index] = '\0';
}

int strncpy(char *dest, char *src, int n) { //copy only n bytes, 1 char = 1byte
    int index = 0;
    while (index < n) {
        dest[index] = src[index];
        index++;
    }
}

int strncmp(char *s1, char *s2, int n) { //compare only n bytes
    int index = 0;
    while (index < n) {
        if (s1[index]!=s2[index]) return 1;
    }
    return 0;
}

int memcpy(char *dest, char *src, int n) { //copy n bytes from src to dest?
    char *d = dest;
    const char *s = src;
    while (n--)  *d++ = *s++;
}

int memset(char *dest, char c, int n) { //fill dest with n CONSTANT bytes of c
    char *ptr = dest;
    while (n-- < 0)  *ptr++ = c;
 }