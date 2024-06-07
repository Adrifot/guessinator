#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *question;
    struct Node *no;
    struct Node *yes;
} Node;

int yes_or_no(char *question) {
    char answer[3];
    printf("%s? (y/n):  ", question);
    fgets(answer, 3, stdin);
    return answer[0] == 'y';
}

Node* create_new_node(char *question) {
    Node *new_node = (struct Node*)malloc(sizeof(Node));
    new_node->question = strdup(question);
    new_node->no = NULL;
    new_node->yes = NULL;
    return new_node;
}

void release(Node *n) {
    if (n) {
        if (n->no) release(n->no);
        if (n->yes) release(n->yes);
        if (n->question) free(n->question);
        free(n);
    }
}

void save_to_file(struct Node* n, FILE* savefile) {
    if(n == NULL) return;
    int hasChildren = (n->yes && n->no);
    fprintf(savefile, "%s | %i ", n->question, hasChildren);
    save_to_file(n->yes, savefile);
    save_to_file(n->no, savefile);
}

int read_next_node(FILE* file, char* question, int* hasChildren) {
    return fscanf(file, " %[^|] | %i ", question, hasChildren);
}

struct Node* load_from_file(FILE* savefile) {
    char question[80];
    int hasChildren;
    if(read_next_node(savefile, question, &hasChildren) != 2) return NULL;
    struct Node* n = create_new_node(question);
    if(hasChildren) {
        n->yes = load_from_file(savefile);
        n->no = load_from_file(savefile);
    }
    return n;
}

int main() {
    char question[80];
    char person[20];

    FILE* loadfile = fopen("save.dat", "r");
    Node* loaded_root = load_from_file(loadfile);
    fclose(loadfile);

    if(loaded_root == NULL) {
        loaded_root = create_new_node("Is a man");
        loaded_root->yes = create_new_node("John Doe");
        loaded_root->no = create_new_node("Joanne Doe");
    }

    Node *current_node;

    do {
        current_node = loaded_root;
        while (1) {
            if (yes_or_no(current_node->question)) {
                if (current_node->yes) current_node = current_node->yes;
                else {
                    printf("Person guessed\n");
                    break;
                }
            } else if (current_node->no) current_node = current_node->no;
            else {
                printf("Who is the person? ");
                fgets(person, 20, stdin);
                strtok(person, "\n");
                Node *yes_node = create_new_node(person);
                current_node->yes = yes_node;
                Node *no_node = create_new_node(current_node->question);
                current_node->no = no_node;
                printf("What would be TRUE for %s but FALSE for %s?\n", person, current_node->question);
                fgets(question, 80, stdin);
                strtok(question, "\n");
                free(current_node->question);
                current_node->question = strdup(question);
                break;
            }
        }
    } while (yes_or_no("Run again"));

    FILE* savefile = fopen("save.dat", "w");
    save_to_file(loaded_root, savefile);
    fclose(savefile);
    release(loaded_root);
    return 0;
}
