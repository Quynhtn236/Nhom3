#include <stdio.h>
#include <string.h>
#include "rwfile.h"



void writeToFile(const char* filename, const Person* person) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        printf("Không thể mở file.\n");
        return;
    }

    fprintf(file, "%s %s\n", person->UID, person->name);
    fclose(file);
}

void readFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Không thể mở file.\n");
        return;
    }

    Person person;
    while (fscanf(file, " %s %[^\n]", &person.UID, person.name) != EOF) {
        printf("ID: %s, Tên: %s\n", person.UID, person.name);
    }

    fclose(file);
}

int searchById(const char* filename, const char* UID, Person* person) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Không thể mở file.\n");
        return -1;
    }

    while (fscanf(file, " %s %[^\n]", person->UID, person->name) != EOF) {
        if (strcmp(person->UID, UID) == 0) {
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return -2;
}

/*int main() {
    Person person1 = {'A', "John"};
    Person person2 = {'B', "Alice"};
    Person person3 = {'C', "Bob"};

    // Ghi dữ liệu vào file
    writeToFile("data.txt", &person1);
    writeToFile("data.txt", &person2);
    writeToFile("data.txt", &person3);

    // Đọc dữ liệu từ file
    readFromFile("data.txt");

    // Tìm kiếm và in ra tên theo id
    searchById("data.txt", 'B');

    return 0;
}*/

