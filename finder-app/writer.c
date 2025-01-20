#include <stdio.h>
#include <syslog.h>

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Requires 2 arguments. USAGE: ./writer.c file_path write_string");
        return 1;
    }

    char* file_path = argv[1];
    char* text = argv[2];

    // Open log
    openlog("spencer741-assignment2-writer","",LOG_USER);

    FILE * filePointer = fopen(argv[1],"w");

    if (filePointer == NULL) {
        syslog(LOG_ERR,"Error opening the file provided");
        return 1;
    }
    //printf("Writing %s to %s", text, file_path);

    syslog(LOG_DEBUG, "Writing %s to %s", text, file_path);
    
    // Write to file
    fprintf(filePointer, text);

    // Close file and log.
    fclose(filePointer);
    closelog();

    return 0;
}


