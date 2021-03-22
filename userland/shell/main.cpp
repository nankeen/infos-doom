#include <libcore.h>

int main(const char *)
{
    printf("This is the InfOS shell.  Path resolution is not-yet-implemented, so you\n"
            "must type the command exactly, e.g. try typing: /usr/ls.\n\n");
    
    while (true) {
        printf("$ ");

        char command_buffer[128];
        size_t n = 0;

        while (n < sizeof command_buffer - 1) {
            char c = getch();
            if (c == 0) continue;
            if (c == '\n') break;
            if (c == '\b') {
                if (n > 0) {
                    command_buffer[--n] = 0;
                    printf("\b");
                }
            } else {
                command_buffer[n++] = c;
                printf("%c", c);
            }
        }

        printf("\n");
        if (n == 0) continue;
        
        command_buffer[n] = 0;
        
        if (strcmp("exit", command_buffer) == 0) break;
        system(command_buffer);
    }
    
    return 0;
}
