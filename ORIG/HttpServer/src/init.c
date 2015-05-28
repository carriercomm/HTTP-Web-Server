/*
File: init.c
Description: creates CRC index table and fills RAM with content of hosted files.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include "crc16.h"
#include "init.h"

//exampleOfUseCRC16: http://www.embeddedrelated.com/

unsigned int calculateCRC16(unsigned char *Data, unsigned char len) {

    //unsigned int crc;
    uint16_t crc;
    unsigned char aux = 0;

    //crc = 0x0000; //Initialization of crc to 0x0000 for DNP
    crc = 0xFFFF; //Initialization of crc to 0xFFFF for CCITT


    while (aux < len) {
        crc = crc16(crc, Data[aux]);
        aux++;
    }

    //return (~crc); //The crc value for DNP it is obtained by NOT operation

    return crc; //The crc value for CCITT
}

int initCode(struct Element *crcTable) {
    unsigned char buf[64];

    DIR *dp;
    struct dirent *ep;
    char fname[512] = "";
    char fnamecopy[512] = "";
    char fsize[64] = "";
    char cdir[256] = "./files/"; //name of the current directory
    struct stat st;

    dp = opendir(cdir);
    if (dp != NULL) {
        int i = 0;
        //int currentBurst=0;
        while (ep = readdir(dp)) {
            memset(buf, 0, sizeof (buf));
            int jk = 0;
            /* browse through the files in current directory and gets file attributes */
            int scompare1 = strcmp(ep->d_name, ".");
            int scompare2 = strcmp(ep->d_name, "..");
            if (scompare1 && scompare2) {
                strcat(fname, cdir);
                strcat(fname, ep->d_name);
                stat(fname, &st);
                int size = st.st_size;
                sprintf(fsize, " - size(bytes): %d", size);
                strcpy(fnamecopy, fname);
                strcat(fnamecopy, fsize);
                puts(fnamecopy);

                //HTTP response: status-line; header fields
                int Nfields = 4;
                int ip;
                char *responseFields[Nfields];
                responseFields[0] = "HTTP/1.0 200 OK\r\n"; //status line
                responseFields[1] = "Connection: close\r\n"; //header field //Close //keep-alive
                char thdr[50];
                sprintf(thdr, "Content-Length: %d\r\n", size); //header field
                responseFields[2] = thdr;
                //Content-Type: image/svg+xml\r\n
                responseFields[3] = "\r\n"; //CRLF
                char joined[512] = "";

                for (ip = 0; ip < Nfields; ip++) {
                    strcat(joined, responseFields[ip]);
                }

                printf("Joined: %s\n", joined);

                int size_only_headers = strlen(joined);
                // printf("Size of 'joined' = %d bytes\n", size_only_headers);

                int size_with_headers = size_only_headers + size;

                unsigned char *t1, *t2;
                t1 = buf;
                t2 = fname + 1;

                //copy string to another string without first character
                while ((*t2) != '\0') {
                    *t1 = *t2;
                    t1++;
                    t2++;
                }

//                for (jk = 0; jk < 64; jk++) {
//                    printf("buf init: buf=%c\n", buf[jk]);
//                }
                // printf("initialization: buf=%s, size(buf)=%d, strlen(buf)=%d\n", buf, sizeof (buf), strlen(buf));
                unsigned int crcValue = calculateCRC16(buf, strlen(buf)); // get crc of the current url

                char *file_content = (char *) malloc(size);
                char *file_content_write = (char *) malloc(size_with_headers);

                if (file_content == NULL | file_content_write == NULL) {
                    printf("Error with allocating memory\n");
                    exit(0);
                }

                 FILE *fp_file = fopen(fname, "rb");
                if (!fp_file) {
                    printf("Error with file\n");
                    exit(0);
                }

                fread(file_content, 1, size, fp_file); // load file content
                memcpy(file_content_write, joined, size_only_headers); // copy headers
                char *tfile = file_content_write + size_only_headers; // copy file content
                memcpy(tfile, file_content, size); // combine headers with file content

                crcTable[crcValue].pointer = (char*) calloc(size_with_headers, sizeof (char));
                crcTable[crcValue].sizeBytes = size_with_headers;
                memcpy(crcTable[crcValue].pointer, file_content_write, size_with_headers);

                printf("Checksum value: 0x%x, %d\n", crcValue, crcValue);
                printf("\n"); //
                i++;

                fname[0] = '\0'; // set empty string
                free(file_content);
                free(file_content_write);

            }
        }

        (void) closedir(dp);
    } else {
        perror("Couldn't open the directory");
        exit(-4);
    }

    return 0;
}

