#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

//ekrana son dizini yazd�r�r
void printWorkingDirectory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s>", cwd);
    } else {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }
}

//cat komutunu kullanarak komuttan sonra yaz�lan dosyay�n�n i�eri�ini ekrana yazd�r�r
void cat(const char *dosyaAdi) {
    FILE *dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL) {
        printf("Dosya bulunamadi: %s\n", dosyaAdi);
        printWorkingDirectory();
        return;
    }

    char karakter;
    while ((karakter = fgetc(dosya)) != EOF) {
        putchar(karakter);
    }
    printf("\n");
	printWorkingDirectory();
    fclose(dosya);
}

//rm komutunu kullanarak komuttan sonra yaz�lan dosyay� siler
void rm(const char *dosyaAdi) {
    if (unlink(dosyaAdi) == 0) {
        printf("%s dosyasi basariyla silindi.\n", dosyaAdi);
        printWorkingDirectory();
    } else {
        perror("Silme hatasi");
        printWorkingDirectory();
    }
}

int main() {
	printf("Microsoft Windows [Version 10.0.19045.2846]\n(c) Microsoft Corporation. Tum haklari CMSE'e aittir.\n\n");
    char directory[100]; // kullan�c�dan al�nacak dizin ad� i�in bir karakter dizisi
    char* value; // al�nacak dizin ad�
    chdir("C:\\Users");
    printWorkingDirectory();
    while (1) { // sonsuz d�ng�
        fgets(directory, sizeof(directory), stdin); // kullan�c�n�n girdi�i de�er okunur
        directory[strcspn(directory, "\n")] = 0; // \n karakterini kald�r
        // "cd " ile ba�layan bir giri� varsa
        if (strncmp(directory, "cd ", 3) == 0) {
            value = directory + 3; // "cd " k�sm�n� atlar ve geri kalan�n� directory de�i�kenine atar
            int status = chdir(value); // dizin de�i�tirme i�lemi ger�ekle�tirilir
            if (status == -1) { // hata durumunda
                printf("Error: Unable to change directory\n"); 
                printWorkingDirectory();                      
            } else { // ba�ar�l� olursa
                printWorkingDirectory();
            }
        }
        // "ls" komutu girildi�inde
        else if (strcmp(directory, "ls") == 0) {
            DIR *d;
            struct dirent *dir;
            d = opendir(".");
            if (d) {
                while ((dir = readdir(d)) != NULL) {
                    printf("%s\n", dir->d_name);               
                }
                closedir(d);
                printWorkingDirectory();
            }
        }
        // "cat" komutu girildi�inde
        else if (strncmp(directory, "cat ", 4) == 0) {
        	value = directory + 4; // "cat" k�sm�n� atlar ve geri kalan�n� directory de�i�kenine atar       	
            const char *dosyaAdi = value;
    		cat(dosyaAdi);
    		
        }
        // "rm" komutu girildi�inde
        else if (strncmp(directory, "rm ", 3) == 0) {
        	value = directory + 3; // "rm" k�sm�n� atlar ve geri kalan�n� directory de�i�kenine atar       	
            const char *dosyaAdi = value;
    		rm(dosyaAdi);
    		
        }
        // "q" komutu girildi�inde d�ng�den ��k�l�r
        else if (strcmp(directory, "q") == 0) {
            break;
        }
        // hi�bir �arta uymayan girdilerde hata mesaj� verilir
        else {
            printf("Error: Invalid input\n");
            printWorkingDirectory();
        }
    }
    return 0;
}

