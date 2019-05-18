#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <malloc.h>

typedef struct record{
	char *numberOfContract;
	char *clientName;
	char *placeOfRegistration;
	char *dateOfConclusionOfTheContract;
	char *timeConclusionOfTheContract;
	char *lease;
} structRecord;

FILE *dataFile;
FILE *openedFileStream;
char **listOfFiles, *nameOfOpenedFile;
unsigned char numberOfFiles=0, fileOpened=0;
unsigned char numberOfRecordsInBase = 0;
structRecord *Base = NULL;

inline void deleteRecord(unsigned char recordNumber){
	Base[recordNumber].numberOfContract = NULL;
	free(Base[recordNumber].numberOfContract);
	Base[recordNumber].clientName = NULL;
	free(Base[recordNumber].clientName);
	Base[recordNumber].placeOfRegistration = NULL;
	free(Base[recordNumber].placeOfRegistration);
	Base[recordNumber].dateOfConclusionOfTheContract = NULL;
	free(Base[recordNumber].dateOfConclusionOfTheContract);
	Base[recordNumber].timeConclusionOfTheContract = NULL;
	free(Base[recordNumber].timeConclusionOfTheContract);
	Base[recordNumber].lease = NULL;
	free(Base[recordNumber].lease);
}

void deleteBase(){
    for (unsigned char i = 0; i < numberOfRecordsInBase; i++){
		deleteRecord(i);
    }
	Base = NULL;
    free(Base);
}

inline void printRecord(const structRecord rec){
	printf("%s %s %s %s %s %s\n", 
		rec.numberOfContract, 
		rec.clientName,  
		rec.placeOfRegistration, 
		rec.dateOfConclusionOfTheContract, 
		rec.timeConclusionOfTheContract, 
		rec.lease);
}

inline void printBase(){
	if(numberOfRecordsInBase > 1) 
		printf("There are %d records in File: \n", numberOfRecordsInBase);
	else if (numberOfRecordsInBase == 1)
		printf("There is 1 record in File: \n");
	else printf("There is no records in File.\n");
	for(unsigned char i=0; i<numberOfRecordsInBase; ++i) {
		printf("\t %d - ", i+1); printRecord(Base[i]);}
}

unsigned char loadBase(char *nameOfFile){
	unsigned char y=0, k=0;
	char *buffer1 = NULL, *buffer2 = NULL;
	size_t m;
	int len=0;
	
	if (openedFileStream==NULL) return 0;
	Base = (structRecord*)realloc(Base, 
			(numberOfRecordsInBase+1)*sizeof(structRecord));
	
	for(unsigned char i=0; i<255; ++i){
		m = getline(&buffer1, &len, openedFileStream);
		if (m!=-1){
			if (buffer1[strlen(buffer1)-1] == '\n'){
				++numberOfRecordsInBase; 
				Base = (structRecord*)realloc(Base, 
					(numberOfRecordsInBase+1)*sizeof(structRecord)); 
			}
			buffer2 = strtok(buffer1, " ");
			if (buffer2) { 
				Base[numberOfRecordsInBase-1].numberOfContract = buffer2;
				buffer2 = strtok(NULL, " ");
				Base[numberOfRecordsInBase-1].clientName = buffer2;
				buffer2 = strtok(NULL, " ");
				Base[numberOfRecordsInBase-1].placeOfRegistration = buffer2;
				buffer2 = strtok(NULL, " ");
				Base[numberOfRecordsInBase-1].dateOfConclusionOfTheContract = 
					buffer2;
				buffer2 = strtok(NULL, " ");
				Base[numberOfRecordsInBase-1].timeConclusionOfTheContract = 
					buffer2;
				buffer2 = strtok(NULL, "\n");
				Base[numberOfRecordsInBase-1].lease = buffer2;
			}
			buffer1 = NULL;
			free(buffer1);
			buffer2 = NULL;
			free(buffer2);
		}
	}
	fclose(openedFileStream);
	openedFileStream = fopen(nameOfFile, "r+b");
	
	return 1;
}

inline void saveFile(unsigned char number){
	short count;
	while ( (count=getc(openedFileStream)) != EOF) continue;
	fprintf(openedFileStream, Base[number].numberOfContract);
	fprintf(openedFileStream, " ");
	fprintf(openedFileStream, Base[number].clientName);
	fprintf(openedFileStream, " ");
	fprintf(openedFileStream, Base[number].placeOfRegistration);
	fprintf(openedFileStream, " ");
	fprintf(openedFileStream, Base[number].dateOfConclusionOfTheContract);
	fprintf(openedFileStream, " ");
	fprintf(openedFileStream, Base[number].timeConclusionOfTheContract);
	fprintf(openedFileStream, " ");
	fprintf(openedFileStream, Base[number].lease);
	fprintf(openedFileStream, "\n");
}

inline void showMenu(){
	printf("Menu: \n");
	printf("\t0 - Exit;\n");
	printf("\t1 - Make new File;\n");
	printf("\t2 - Open File;\n");
	printf("\t3 - Show File;\n");
	printf("\t4 - Edit name of File;\n");
	printf("\t5 - Delete File;\n");
	printf("\t6 - Make record in File;\n");
	printf("\t7 - Edit record in File;\n");
	printf("\t8 - Delete record in File;\n");
	printf("\t9 - Sort File;\n");
	printf("\t10 - Search record in File.\n");
	printf("\t11 - Print records in File.\n");
	printf("Your choice is: ");
}

char* choosingFile(char *str){
	if (numberOfFiles == 0) return NULL;
	else {
		int numberOfFile;
		printf("%s", str);
		for(unsigned char i=0; i<numberOfFiles; ++i) 
			printf("\t%d - %s\n", i, listOfFiles[i]);
		if (scanf("%d", &numberOfFile) == EOF)
			printf("Mistake with choosing!\n");
		printf("Choosing %s success.\n", listOfFiles[numberOfFile]);
		return listOfFiles[numberOfFile];
	}
}

void makeFile(){	
	char *name = NULL;
	name = (char*)malloc(100*sizeof(char));
	printf("Enter the File name: ");
	if(scanf("%s", name) == 0)
		printf("Mistake with reading name!\n");
	strncat(name, ".txt", 4);
	FILE *newFile = fopen(name, "a");
	if (newFile!=NULL) 	{
		printf("File '%s' successfully created!\n", name);
		listOfFiles[numberOfFiles] = name;
		++numberOfFiles;
		fclose(newFile);
	} else printf("Can't create this file!\n"); 
	name = NULL;
	free(name); 
}

unsigned char openFile(){
	if(fileOpened == 1){
		fclose(openedFileStream);
		deleteBase();
		numberOfRecordsInBase = 0;
		nameOfOpenedFile = NULL;
		fileOpened = 0;
	}
	char *name = choosingFile("What file do you want to open?\n");
	if (name == NULL) {
		printf("You have no files to open.\n");
		free(name);
		return 0;
	} else {
		openedFileStream = fopen(name, "r+b");
		if (openedFileStream==NULL) {
			printf("File was not opened.\n");
			return 0;
		} else printf("File opened! ");
		if (loadBase(name) == 1) printf("Base loaded!\n");
			else printf("Base wasn't load. Maybe, it doesn't exist.\n");
		nameOfOpenedFile = name;
		name = NULL;
		free(name);
		return 1;
	}
}

void showFile(){
	char *name = choosingFile("What file do you want to see?\n");
	if (name == NULL) printf("You have no files to see.\n");
	else {
		char *text = NULL;
		unsigned char i=0;
		int c;
		FILE *fileToSee;
		if(fileOpened == 1) fileToSee = freopen(name, "r", openedFileStream);
			else fileToSee = fopen(name, "r");
		while( (c=getc(fileToSee)) != EOF) {
			text = (char*)realloc(text, (i+1)*sizeof(char));
			text[i] = c; ++i;
		}
		text = (char*)realloc(text, (i+1)*sizeof(char));
		text[i] = '\0';
		printf("\n%s\n", text);
		fclose(fileToSee);
		text = NULL;
		free(text);
	}
	name = NULL;
	free(name);
}

void editNameOfFile(){
	char *oldName = choosingFile("What file do you want to rename?\n");
	if (oldName == NULL) printf("You have no files to rename.\n");
	else {
		char *newName;
		newName = (char*)malloc(100*sizeof(char));
		printf("Enter a new name of File: ");
		if(scanf("%s", newName) == 0)
			printf("Mistake with reading new name!\n");
		strncat(newName, ".txt", 4);
		rename(oldName, newName);
		for(unsigned char i=0; i<numberOfFiles; ++i)
			if ( (strcmp(listOfFiles[i], oldName)) == 0)
				listOfFiles[i] = newName;
		newName = NULL;
		free(newName);
	}
	oldName = NULL;
	free(oldName);
}

void deleteFile(){
	char *name = choosingFile("What file do you want to delete?\n"); 
	if ( name == NULL) printf("You have no files to delete.\n");
	else {
		if (remove(name) == -1) printf ("Deleting File Fileed.\n");
		else {
			printf ("Succes! ");
			for(unsigned char i=0; i<numberOfFiles; ++i)
				if ((strcmp(listOfFiles[i], name)) == 0){
					--numberOfFiles;
					if (i == numberOfFiles) listOfFiles[i] = NULL;
					free(listOfFiles[i]);
					for(unsigned char j=i; j<numberOfFiles; ++j)
						listOfFiles[j] = listOfFiles[j+1];
					printf("File deleted from base.\n");
				}
		}
	}
	name = NULL;
	free(name);
}

inline void createRecord(unsigned char number){
	char buffer[40];

	printf("Enter number of contract, please: \n");
	if(scanf("%s", buffer) == 0) printf("Mistake with reading.\n");
	Base[number].numberOfContract = (char*)malloc(strlen(buffer)*sizeof(char));
	strncpy(Base[number].numberOfContract, buffer, strlen(buffer));
	Base[number].numberOfContract[strlen(buffer)] = '\0';
	
	printf("Enter client's name, please: \n");
	if(scanf("%s", buffer) == 0) printf("Mistake with reading.\n");
	Base[number].clientName = (char*)malloc(strlen(buffer)*sizeof(char));
	strncpy(Base[number].clientName, buffer, strlen(buffer));
	Base[number].clientName[strlen(buffer)] = '\0';
	
	printf("Enter place of registration, please: \n");
	if(scanf("%s", buffer) == 0) printf("Mistake with reading.\n");
	Base[number].placeOfRegistration = 
		(char*)malloc(strlen(buffer)*sizeof(char));
	strncpy(Base[number].placeOfRegistration, buffer, strlen(buffer));
	Base[number].placeOfRegistration[strlen(buffer)] = '\0';
	
	printf("Enter date of conclusion of the contract, please: \n");
	if(scanf("%s", buffer) == 0) printf("Mistake with reading.\n");
	Base[number].dateOfConclusionOfTheContract = 
		(char*)malloc(strlen(buffer)*sizeof(char));
	strncpy(Base[number].dateOfConclusionOfTheContract, buffer, strlen(buffer));
	Base[number].dateOfConclusionOfTheContract[strlen(buffer)] = '\0';
	
	printf("Enter time conclusion of the contract, please: \n");
	if(scanf("%s", buffer) == 0) printf("Mistake with reading.\n");
	Base[number].timeConclusionOfTheContract = 
		(char*)malloc(strlen(buffer)*sizeof(char));
	strncpy(Base[number].timeConclusionOfTheContract, buffer, strlen(buffer));
	Base[number].timeConclusionOfTheContract[strlen(buffer)] = '\0';
	
	printf("Enter lease, please: \n");
	if(scanf("%s", buffer) == 0) printf("Mistake with reading.\n");
	Base[number].lease = (char*)malloc(strlen(buffer)*sizeof(char));
	strncpy(Base[number].lease, buffer, strlen(buffer));
	Base[number].lease[strlen(buffer)] = '\0';
}

void makeRecordInFile(){
	if(fileOpened == 1){
		char buffer[40];
		short count=0;
		
		Base = (structRecord*)realloc(Base, 
			(numberOfRecordsInBase+1)*sizeof(structRecord));
		
		createRecord(numberOfRecordsInBase);
		++numberOfRecordsInBase;
		
		saveFile(numberOfRecordsInBase-1);
		
		printRecord(Base[numberOfRecordsInBase-1]);
		printf("New record was made.\n");
	} else printf("Open someone file at first!\n");
}

void editRecordInFile(){
	short unsigned int choose=0;
	
	printf("What record do you want to edit: \n");
	printBase();
	if(scanf("%hu", &choose) == 0)
		printf("Mistake with reading choice!\n");
	
	createRecord(choose-1);
	
	fclose(openedFileStream);
	openedFileStream = fopen(nameOfOpenedFile, "w");
	fclose(openedFileStream);
	openedFileStream = fopen(nameOfOpenedFile, "r+b");
	for (unsigned char i=0; i<numberOfFiles; ++i) saveFile(i);
	
	printf("Record was changed.\n");
}

void deleteRecordInFile(){
	short unsigned int choose=0;
	
	printf("What record do you want to delete: \n");
	printBase();
	if(scanf("%hu", &choose) == 0)
		printf("Mistake with reading choice!\n");
	
	if(numberOfRecordsInBase == choose) deleteRecord(choose);
	else {
		for(unsigned char i=choose; i<numberOfRecordsInBase-1; ++i){
			Base[i].numberOfContract = Base[i+1].numberOfContract;
			Base[i].clientName = Base[i+1].clientName;
			Base[i].placeOfRegistration = Base[i+1].placeOfRegistration;
			Base[i].dateOfConclusionOfTheContract = 
				Base[i+1].dateOfConclusionOfTheContract;
			Base[i].timeConclusionOfTheContract = 
				Base[i+1].timeConclusionOfTheContract;
			Base[i].lease = Base[i+1].lease;
		}
		deleteRecord(numberOfRecordsInBase);
	}
	
	fclose(openedFileStream);
	openedFileStream = fopen(nameOfOpenedFile, "w");
	fclose(openedFileStream);
	openedFileStream = fopen(nameOfOpenedFile, "r+b");
	for (unsigned char i=0; i<numberOfFiles; ++i) saveFile(i);
	
	printf("Record was deleted!\n");
}

inline short unsigned int chooseTagRecord(char* str){
	short unsigned int answer=0;
	printf("By what character do you want to %s records in File: \n", str);
	printf("\t 1 - by number of contract;\n");
	printf("\t 2 - by client's name;\n");
	printf("\t 3 - by place of registration;\n");
	printf("\t 4 - by date of conclusion of the contract;\n");
	printf("\t 5 - by time conclusion of the contract;\n");
	printf("\t 6 - by lease;\n");
	if(scanf("%hu", &answer) == 0)
		printf("Mistake with reading choice!\n");
	return answer;
}

int CMP_1 (const structRecord *point1, const structRecord *point2){
	return strcmp (point1->numberOfContract, point2->numberOfContract);}

int CMP_2 (const structRecord *point1, const structRecord *point2){
	return strcmp (point1->clientName, point2->clientName);}

int CMP_3 (const structRecord *point1, const structRecord *point2){
	return strcmp (point1->placeOfRegistration, point2->placeOfRegistration);}

int CMP_4 (const structRecord *point1, const structRecord *point2){
	return strcmp (point1->dateOfConclusionOfTheContract, 
				point2->dateOfConclusionOfTheContract);}

int CMP_5 (const structRecord *point1, const structRecord *point2){
	return strcmp(point1->timeConclusionOfTheContract, 
				point2->timeConclusionOfTheContract);}

int CMP_6 (const structRecord *point1, const structRecord *point2){
	return strcmp (point1->lease, point2->lease);}

unsigned char sortFile(){
	
	if(fileOpened == 0){
		printf("Open someone file at first!\n");
		return 0;
	}
	
	short unsigned int choose = chooseTagRecord("sort");
	
	switch(choose){
		case 1:
			qsort(Base, numberOfRecordsInBase, sizeof(structRecord), CMP_1);
			break;
		case 2:
			qsort(Base, numberOfRecordsInBase, sizeof(structRecord), CMP_2);
			break;
		case 3:
			qsort(Base, numberOfRecordsInBase, sizeof(structRecord), CMP_3);
			break;
		case 4:
			qsort(Base, numberOfRecordsInBase, sizeof(structRecord), CMP_4);
			break;
		case 5:
			qsort(Base, numberOfRecordsInBase, sizeof(structRecord), CMP_5);
			break;
		case 6:
			qsort(Base, numberOfRecordsInBase, sizeof(structRecord), CMP_6);
			break;
		default:
			printf("I don't know this character!\n");
			return 0;
	}
	
	fclose(openedFileStream);
	openedFileStream = fopen(nameOfOpenedFile, "w");
	for (unsigned char i=0; i<numberOfFiles; ++i) saveFile(i);
	fclose(openedFileStream);
	openedFileStream = fopen(nameOfOpenedFile, "r+b");
	
	return 1;
}

unsigned char searchRecordInFile(){
	char strSearch[40];
	structRecord target, *result;
	
	if(fileOpened == 0){
		printf("Open someone file at first!\n");
		return 0;
	}

	short unsigned int choose = chooseTagRecord("search");
	printf("Enter a string to search, please: \n");
	if (scanf("%s", strSearch) == 0)
		printf("Mistake with reading.\n");
	
	switch(choose){
		case 1:
			target.numberOfContract = strSearch;
			result = bsearch (&target, Base, numberOfRecordsInBase, 
				sizeof(structRecord), CMP_1);
			break;
		case 2:
			target.clientName = strSearch;
			result = bsearch (&target, Base, numberOfRecordsInBase, 
				sizeof(structRecord), CMP_2);
			break;
		case 3:
			target.placeOfRegistration = strSearch;
			result = bsearch (&target, Base, numberOfRecordsInBase, 
				sizeof(structRecord), CMP_3);
			break;
		case 4:
			target.dateOfConclusionOfTheContract = strSearch;
			result = bsearch (&target, Base, numberOfRecordsInBase, 
				sizeof(structRecord), CMP_4);
			break;
		case 5:
			target.timeConclusionOfTheContract = strSearch;
			result = bsearch (&target, Base, numberOfRecordsInBase, 
				sizeof(structRecord), CMP_5);
			break;
		case 6:
			target.lease = strSearch;
			result = bsearch (&target, Base, numberOfRecordsInBase, 
				sizeof(structRecord), CMP_6);
			break;
		default:
			printf("I don't know this character!\n");
			break;
	}
	if (result){
		printRecord(*result);
		return 1;
	} else return 0;
}

inline void makeData(){
	unsigned char k=0;
	char *dataFileText, ch, *name = NULL;
	
	dataFile = fopen("DataFile.txt", "a");
	fclose(dataFile);
	dataFile = fopen("DataFile.txt", "r");
	
	while (!feof(dataFile))
		if (fgetc(dataFile) == '\n')
			numberOfFiles++;
	fclose(dataFile);
	dataFile = fopen("DataFile.txt", "r");
	
	listOfFiles = (char**)malloc(numberOfFiles*400*sizeof(char*));
	
	if(numberOfFiles>0) printf("Now you have %d files: \n", numberOfFiles);
	else if(numberOfFiles == 1) printf("Now you have one file: \n");
	else printf("You have no files yet.\n");
	for (unsigned char i=0; i<numberOfFiles; ++i){
		while ((ch=fgetc(dataFile)) != '\n'){
			name = (char*)realloc(name, (k+1)*sizeof(char));
			name[k] = ch;
			++k;
		}
		name[k] = '\0';
		printf("\t %d - %s;\n", i+1, name);
		listOfFiles[i] = name;
		name = NULL; k = 0;
	}
	free(name);
	fclose(dataFile);
}

void main(int argc, char *argv[]) {
	setlocale(LC_ALL, "rus");
	
	short unsigned int choice;
	unsigned char k=0;
	makeData();
	
	while(1==1) {		
		showMenu();
		if(scanf("%hu", &choice) == 0)
			printf("Mistake with reading choice!\n");
		switch (choice) {
			case 0: 
				dataFile = fopen("DataFile.txt", "w");
				while(numberOfFiles>0){
					fputs(listOfFiles[k], dataFile);
					listOfFiles[k] = NULL;
					free(listOfFiles[k]);
					fputc('\n', dataFile);
					--numberOfFiles; ++k;
				}
				listOfFiles = NULL;
				free(listOfFiles);
				deleteBase();
				fclose(dataFile);
				if(fileOpened == 1) fclose(openedFileStream);
				printf("Thanks for using!\n");
				exit(0);
				break;
			case 1: 
				makeFile();
				break;
			case 2: 
				fileOpened = openFile();
				break;
			case 3: 
				showFile();
				break;
			case 4: 
				editNameOfFile();
				break;
			case 5: 
				deleteFile();
				break;
			case 6: 
				makeRecordInFile();
				break;
			case 7: 
				editRecordInFile();
				break;
			case 8: 
				deleteRecordInFile();
				break;
			case 9:
				if(sortFile() == 1) printf("File was sorted!\n");
					else printf("File was not sorted!\n");
				break;
			case 10:
				if(searchRecordInFile() == 1) 
					printf("That's your record, isn't it?\n");
				else {
					printf("I can't find your record. ");
					printf("Check your search information.\n");}
				break;
			case 11: 
				printBase();
				break;
			default:
				printf("I don't know this command. Please, try again.\n");
				break;
		}
		
	}
	
}
