#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>
#include <time.h>

// Function to check if a file exists
int file_exists(const char *path) {
    struct stat statbuf;
    return stat(path, &statbuf) == 0;
}

void log_message(const char *level, const char *message) {
    FILE *log_file = fopen("writer.log", "a");
    if (log_file == NULL) {
        fprintf(stderr, "Error opening log file: %s\n", strerror(errno));
        return;
    }

    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = '\0'; // Remove newline from timestamp

    fprintf(log_file, "[%s] %s: %s\n", timestamp, level, message);
    fclose(log_file);
}

// Function to write a string to a file
int write_to_file(const char *file_path, const char *write_str) {
    FILE *file = fopen(file_path, "a");
    if (file == NULL) {
        syslog(LOG_ERR, "Error opening file %s: %s", file_path, strerror(errno));
        return -1;
    }
    fprintf(file, "%s\n", write_str);
    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    const char *write_str;
    const char *file_path;

    // Open syslog with LOG_USER facility
    openlog("writer", LOG_PID | LOG_CONS, LOG_USER);

    // Check the number of arguments
    if (argc != 3) {
        syslog(LOG_ERR, "Invalid number of arguments. Usage: %s <file_path> <write_string>", argv[0]);
        fprintf(stderr, "Usage: %s <file_path> <write_string>\n", argv[0]);
        closelog();
        return 1;
    }

    file_path = argv[1];
    write_str = argv[2];

    // Check if the file exists
    if (!file_exists(file_path)) {
        FILE *new_file = fopen(file_path, "w");
        if (new_file == NULL) {
            syslog(LOG_ERR, "Error creating file %s: %s", file_path, strerror(errno));
            fprintf(stderr, "Error creating file %s\n", file_path);
            closelog();
			log_message("ERROR", "Error creating file");
            return 1;
        }
        fclose(new_file);
    }

    // Write the string to the file
    if (write_to_file(file_path, write_str) != 0) {
        fprintf(stderr, "Error writing to file %s\n", file_path);
        closelog();
		log_message("ERROR", "Error writing to file");
        return 1;
    }

    // Log the success message
	log_message("SUCCESS", "File write succeeded");
    syslog(LOG_DEBUG, "Writing %s to %s", write_str, file_path);

    // Close syslog
    closelog();
    return 0;
}
