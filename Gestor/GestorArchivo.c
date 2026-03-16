#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 64
#define MAX_CHILDREN 32

typedef struct Folder {
    char name[MAX_NAME_LEN];
    struct Folder *parent;
    struct Folder *children[MAX_CHILDREN];
    int child_count;
} Folder;

// Create a new folder
Folder* create_folder(const char *name, Folder *parent) {
    Folder *folder = (Folder*)malloc(sizeof(Folder));
    strncpy(folder->name, name, MAX_NAME_LEN);
    folder->parent = parent;
    folder->child_count = 0;
    memset(folder->children, 0, sizeof(folder->children));
    return folder;
}

// Find child folder by name
Folder* find_child(Folder *folder, const char *name) {
    for (int i = 0; i < folder->child_count; i++) {
        if (strcmp(folder->children[i]->name, name) == 0)
            return folder->children[i];
    }
    return NULL;
}

// Print current path
void print_path(Folder *folder) {
    if (folder->parent != NULL) {
        print_path(folder->parent);
        printf("/%s", folder->name);
    } else {
        printf("/");
    }
}

// List folders in current directory
void list_folders(Folder *folder) {
    for (int i = 0; i < folder->child_count; i++) {
        printf("%s\n", folder->children[i]->name);
    }
}

// Main loop
int main() {
    Folder *root = create_folder("", NULL);
    Folder *current = root;
    char command[128], arg[MAX_NAME_LEN];

    printf("Simple Folder System. Type 'help' for commands.\n");
    while (1) {
        printf("\n");
        print_path(current);
        printf("> ");
        fgets(command, sizeof(command), stdin);

        if (sscanf(command, "mkdir %s", arg) == 1) {
            if (current->child_count >= MAX_CHILDREN) {
                printf("Max folders reached.\n");
                continue;
            }
            if (find_child(current, arg)) {
                printf("Folder already exists.\n");
                continue;
            }
            current->children[current->child_count++] = create_folder(arg, current);
        } else if (sscanf(command, "cd %s", arg) == 1) {
            if (strcmp(arg, "..") == 0) {
                if (current->parent)
                    current = current->parent;
            } else {
                Folder *child = find_child(current, arg);
                if (child)
                    current = child;
                else
                    printf("Folder not found.\n");
            }
        } else if (strncmp(command, "ls", 2) == 0) {
            list_folders(current);
        } else if (strncmp(command, "help", 4) == 0) {
            printf("Commands:\n");
            printf("  mkdir <name>   - Create folder\n");
            printf("  cd <name>      - Change folder\n");
            printf("  cd ..          - Go up\n");
            printf("  ls             - List folders\n");
            printf("  exit           - Quit\n");
        } else if (strncmp(command, "exit", 4) == 0) {
            break;
        } else {
            printf("Unknown command. Type 'help'.\n");
        }
    }

    // TODO: Free memory (not implemented for brevity)
    return 0;
}