#ifndef RWFILE_H
#define RWFILE_H

typedef struct {
    char UID[10];;
    char name[100];
} Person;



void readFromFile(const char* filename);
void writeToFile(const char* filename, const Person* person);
int searchById(const char* filename, const char* UID, Person* person);

#endif
