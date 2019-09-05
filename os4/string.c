/********************************************************************
Copyright 2010-2017 K.C. Wang, <kwang@eecs.wsu.edu>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/
// write YOUR OWN
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

int memcpy(char *dest, char *src, int n) { //copy n bytes from src to dest?
    char *d = dest;
    const char *s = src;
    while (n--)  *d++ = *s++;
}

int geti(char *msg) {
    int index = 0;
    int res = 0;
    int n;
    while (msg[index]!=0) {
        n = msg[index] - '0';
        if (index==0) {
            res += n;
        } else {
            res *= 10;
            res += n;
        } 
        index++;
    }
    *msg = 0;
    return res;
}

// strlen(), strcmp(), strcpy(), memcpy(), geti() functions
// where
//   geti() returns an integer from KBD
//   HINT: gets(char *s); then retrun atoi(s);
// }

