#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

//ekrana son dizini yazdýrýr
void printWorkingDirectory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s>", cwd);
    } else {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }
}

//cat komutunu kullanarak komuttan sonra yazýlan dosyayýnýn içeriðini ekrana yazdýrýr
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

//rm komutunu kullanarak komuttan sonra yazýlan dosyayý siler
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
    char directory[100]; // kullanýcýdan alýnacak dizin adý için bir karakter dizisi
    char* value; // alýnacak dizin adý
    chdir("C:\\Users");
    printWorkingDirectory();
    while (1) { // sonsuz döngü
        fgets(directory, sizeof(directory), stdin); // kullanýcýnýn girdiði deðer okunur
        directory[strcspn(directory, "\n")] = 0; // \n karakterini kaldýr
        // "cd " ile baþlayan bir giriþ varsa
        if (strncmp(directory, "cd ", 3) == 0) {
            value = directory + 3; // "cd " kýsmýný atlar ve geri kalanýný directory deðiþkenine atar
            int status = chdir(value); // dizin deðiþtirme iþlemi gerçekleþtirilir
            if (status == -1) { // hata durumunda
                printf("Error: Unable to change directory\n"); 
                printWorkingDirectory();                      
            } else { // baþarýlý olursa
                printWorkingDirectory();
            }
        }
        // "ls" komutu girildiðinde
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
        // "cat" komutu girildiðinde
        else if (strncmp(directory, "cat ", 4) == 0) {
        	value = directory + 4; // "cat" kýsmýný atlar ve geri kalanýný directory deðiþkenine atar       	
            const char *dosyaAdi = value;
    		cat(dosyaAdi);
    		
        }
        // "rm" komutu girildiðinde
        else if (strncmp(directory, "rm ", 3) == 0) {
        	value = directory + 3; // "rm" kýsmýný atlar ve geri kalanýný directory deðiþkenine atar       	
            const char *dosyaAdi = value;
    		rm(dosyaAdi);
    		
        }
        // "q" komutu girildiðinde döngüden çýkýlýr
        else if (strcmp(directory, "q") == 0) {
            break;
        }
        // hiçbir þarta uymayan girdilerde hata mesajý verilir
        else {
            printf("Error: Invalid input\n");
            printWorkingDirectory();
        }
    }
    return 0;
}

