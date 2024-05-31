#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char *question;
    struct node *no;
    struct node *yes;
} node;

int yes_or_no(char *question) {
    char answer[3];
    printf("%s? (y/n):  ", question);
    fgets(answer, 3, stdin);
    return answer[0] == 'y';
}

node* create_new_node(char *question) {
    node *new_node = malloc(sizeof(node));
    new_node->question = strdup(question);
    new_node->no = NULL;
    new_node->yes = NULL;
    return new_node;
}

/*
void save_to_file(node *start, FILE *savefile) {
    if (start == NULL) {
        fprintf(savefile, "# | ");
        return;
    }
    fprintf(savefile, "%s | ", start->question);
    save_to_file(start->no, savefile);
    save_to_file(start->yes, savefile);
}

node* load_from_file(FILE *loadfile) {
    char buffer[256];
    if (fscanf(loadfile, "%255[^|] | ", buffer) != 1) {
        return NULL;
    }

    if (strcmp(buffer, "#") == 0) {
        return NULL;
    }

    node *new_node = create_new_node(buffer);
    new_node->no = load_from_file(loadfile);
    new_node->yes = load_from_file(loadfile);

    return new_node;
}
*/

void release(node *n) {
    if (n) {
        if (n->no) release(n->no);
        if (n->yes) release(n->yes);
        if (n->question) free(n->question);
        free(n);
    }
}

int main() {
    char question[80];
    char person[20];
    //node *starting_node = NULL;
    //FILE *loadfile = fopen("savefile.dat", "r");
    node *starting_node = create_new_node("Is the person a man");
    starting_node->no = create_new_node("Joanne Doe");
    starting_node->yes = create_new_node("John Doe");

    /*
    if (loadfile) {
        fseek(loadfile, 0, SEEK_END);
        int filesize = ftell(loadfile);
        fseek(loadfile, 0, SEEK_SET);
        if (filesize > 0) {
            starting_node = load_from_file(loadfile);
        } else {
            starting_node = create_new_node("Is the person a man");
            starting_node->no = create_new_node("Joanne Doe");
            starting_node->yes = create_new_node("John Doe");
        }
        fclose(loadfile);
    } else {
        starting_node = create_new_node("Is the person a man");
        starting_node->no = create_new_node("Joanne Doe");
        starting_node->yes = create_new_node("John Doe");
    }
    */

    node *current_node;

    do {
        current_node = starting_node;
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
                node *yes_node = create_new_node(person);
                current_node->yes = yes_node;
                node *no_node = create_new_node(current_node->question);
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

    /*
    FILE *savefile = fopen("savefile.dat", "w");
    if (!savefile) {
        perror("Error opening the file");
        return 1;
    }
    save_to_file(starting_node, savefile);
    fclose(savefile);
    */
   
    release(starting_node);
    return 0;
}
