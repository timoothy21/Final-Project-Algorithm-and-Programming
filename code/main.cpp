#include <stdio.h>
#include <string.h> // strcmp
#include <conio.h> // getch
#include <time.h> // time
#include <unistd.h> // sleep

// Jelshen Febryand Valesco - 2502037593
// Timothy Theophilus Hartono - 2540129546

struct Customer { // struct untuk menyimpan data customer
    char ID[20];
    char pass[9];
    char name[50];
    long long int phoneNumber;
    char date[10];
    long long int money;
};

struct tm *timeinfo; //struct yang digunakan di time.h

struct Customer cs[1000]; // tempat menyimpan data customer
int userCount = 0; // menghitung banyaknya user
int currentUser; // menyimpan index, dari user yang login

void firstMenu(); // Jelshen
void importData(); // Timothy
void welcomeArt(); // Timothy
void createAcc(); // Jelshen
int checkUsername(char username[]); // Jelshen
void waktu(); // Timothy
void logIn(); // Jelshen
void secondMenu(); // Jelshen
void checkBalance(); // Jelshen
void deposit(); // Timothy
void withdraw(); // Jelshen
void transfer(); // Timothy
void accSett(); // Timothy
void accInfo(); // Timothy
void changePass(); // Timothy
void deleteACC(); // Timothy
void exit(); // Jelshen

int main() { // main program
    firstMenu(); // masuk ke first menu
    exit(); // keluar dari program
    return 0;
}

void firstMenu() { // menampilkan menu awal
    system("cls"); // clear terminal pada layar
    importData(); // mengimport data dari customer_data.txt ke dalam struct
    int choice;
    do{
        welcomeArt(); // menampilkan UI (berupa ascii art)
        puts("1. Create Bank Account");
        puts("2. Already Have Account? Sign In");
        puts("3. Exit");
        printf("\n");
        printf("Choice[1-3] >> ");
        scanf("%d", &choice); // meminta user memasukkan pilihan menu
        switch(choice) {
        case 1:
            createAcc(); // masuk ke menu create account
            break;
        case 2:
            logIn(); // masuk ke menu login
            break;
        }
    }while(choice < 0 || choice > 3);
}

void importData() { // mengambil data yang ada di customer_data.txt kemudian dimasukkan ke dalam struct
    FILE *fp = fopen("../data/customer_data.txt", "r"); // membuka file txt
    int index = 0; // declare index (mulai dari 0) untuk menyimpan data dari file txt
    userCount = 0; // menghitung banyaknya user sekarang
    // scan file txt sampai data di txt habis
    while(fscanf(fp, "%[^,],%[^,],%[^,],%lld,%[^,],%lld\n", cs[index].ID, cs[index].pass, cs[index].name, &cs[index].phoneNumber, cs[index].date, &cs[index].money) != EOF){
        userCount++; // menambah jumlah user setiap scan
        if(feof(fp)) break; // jika file habis break
        index++; // menambah jumlah index setiap scan
    }
    fclose(fp); // menuutup file
}

void welcomeArt() { // menampilkan UI (berupa ascii art)
    FILE *fpwelcome = fopen("../data/welcome.txt", "r"); // membuka file welcome.txt
    char art[255];
    while(fscanf(fpwelcome, "%[^\n]", art) != EOF){ // scan welcome.txt sampai habis
        fgetc(fpwelcome); // mengambil karakter dari file
        printf("%s\n", art); // print art
    }
    fclose(fpwelcome); // menutup file
}

void createAcc() { // membuat bank account
createMenu: // menandakan createMenu sehingga nantinya bisa kembali ke line ini
    printf("Input Username: "); 
    scanf("%s", cs[userCount].ID); // meminta user memasukan username
    int validate = checkUsername(cs[userCount].ID); // mengecek apakah ada username yang sama
    if(validate == 1){ // jika sama akan kembali ke createmenu dan user diminta memasukan ulang username dengan username baru
        goto createMenu;
    }

    printf("Input Password(must be 8 char/number): "); // user diminta memasukan password (syarat harus 8 char/number)
    int i;
    for (i = 0; i < 8; i++) {
        cs[userCount].pass[i] = getch();
        printf("*");
    }
    cs[userCount].pass[i] = '\0';

    puts("");
    getchar();
    printf("Input Name: "); scanf("%[^\n]", cs[userCount].name); // user diminta memasukan nama
    printf("Input Phone Number: "); scanf("%lld", &cs[userCount].phoneNumber); // user diminta memasukan nomor telepon

    do { // user diminta untuk menyetorkan uang setoran awal (syarat minimal Rp. 100.000)
        printf("Input Money (min. Rp100.000): "); 
        scanf("%lld", &cs[userCount].money);
    } while(cs[userCount].money < 100000); // jika kurang dari Rp. 100.000, user di minta kembali memasukkan uang setoran awal 

    waktu(); // waktu digunakan untuk mencatat tanggal pembuatan akun

    FILE *fpacc = fopen("../data/customer_data.txt", "a"); // membuka file customer_data.txt dengan mode append
    // menambahkan data baru pada customer_data.txt
    fprintf(fpacc, "%s,%s,%s,%lld,%d/%d/%d,%lld\n", cs[userCount].ID, cs[userCount].pass, cs[userCount].name, cs[userCount].phoneNumber, timeinfo->tm_mday, timeinfo->tm_mon+1, timeinfo->tm_year+1900, cs[userCount].money);
    fclose(fpacc); // menutup file

    puts(""); 
    // menampilkan seluruh data user
    printf("Your Data\n"); 
    printf("Username\t\t: %s\n", cs[userCount].ID);
    printf("Password\t\t: %s\n", cs[userCount].pass);
    printf("Name\t\t\t: %s\n", cs[userCount].name);
    printf("Phone Number\t\t: 0%lld\n", cs[userCount].phoneNumber);
    printf("Money\t\t\t: %lld\n", cs[userCount].money);
    printf("Registration Date\t: %d/%d/%d\n", timeinfo->tm_mday, timeinfo->tm_mon+1, timeinfo->tm_year+1900);
    puts("");

    // konfirmasi bahwa akun sudah di registrasi
    printf("Registration Is Succesful!\n"); 
    printf("Press Any Key to Continue\n");  
    getch();
    userCount++;

    firstMenu(); // kembali ke first menu
}

int checkUsername(char username[]) { // mengecek apakah username sama dengan username lain & untuk mengecek apakah username ada pada customer_data.txt
    int flag = 0;
    for(int i = 0; i < userCount; i++){ // menggunakan linear search untuk mencari username yang sama
        if(strcmp(username, cs[i].ID) == 0){ // jika username ada pada data
            printf("User already exists\n");
            flag = 1;
            return flag; // return nilai flag = 1
        }
    }
    return flag; // return flag = 0
}

void waktu() { // memanggil function time
    time_t now; // menyimpan waktu di kalender dengan nama now
    time(&now); // menghitung waktu kalender saat ini
    timeinfo = localtime(&now); // menyimpan zona waktu lokal
}

void logIn() { // menu login
retrylogin: //menandakan retrylogin jika password salah kembali ke retrylogin
    char user[20], pwd[9];
    printf("Enter your username: "); 
    scanf("%s", user); // meminta user masukkan username

    printf("Enter Password (must be 8 char/num): "); // meminta user memasukkan password
    int i;
    for (i = 0; i < 8; i++) {
        pwd[i] = getch();
        printf("*");
    }
    pwd[i] = '\0';
    printf("\n");

    int loginStat = 0; // memandakan status login
    for(int i = 0; i < userCount; i++){ // searching untuk username dan password
        if((strcmp(user, cs[i].ID) == 0) && (strcmp(pwd, cs[i].pass) == 0)){
            loginStat = 1; // jika benar status login akan menjadi satu
            currentUser = i; // currentuser akan menyimpan index i
        }
    }

    if(loginStat == 1){ // jika login berhasil
        printf("Login Succesful!\n");
        sleep(1.5); // delay 1.5 s
        secondMenu(); // pindah ke menu login selanjutnya
    } else if(loginStat == 0) { // jika login gagal
        printf("Wrong username/password\n");
        goto retrylogin; // kembali ke retrylogin
    }

}

void secondMenu() { // menampilkan menu setelah login
    system("cls"); // clear terminal
    int choice;
    do{ 
        welcomeArt(); // menampilkan header (ascii art)
        puts("1. Check Balance");
        puts("2. Deposit Money");
        puts("3. Withdraw Money");
        puts("4. Transfer Money");
        puts("5. Account Settings");
        puts("6. Log Out");
        printf("\n");
        printf("Choice[1-6] >> ");
        scanf("%d", &choice); // meminta user memasukkan pilihan menu
        fflush(stdin); // membuang seluruh buffer
        switch(choice) {
        case 1:
            checkBalance(); // masuk ke menu check balance
            break;
        case 2:
            deposit(); // masuk ke menu deposit
            break;
        case 3:
            withdraw(); // masuk ke menu withdraw
            break;
        case 4:
            transfer(); // masuk ke menu transfer
            break;
        case 5:
            accSett(); // masuk ke menu pengaturan akun
            break;
        case 6:
            firstMenu(); // kembali ke first menu
            break;
        }
    }while(choice < 1 || choice > 6);
}

void checkBalance() { // menu cek saldo
    system("cls"); // clear terminal
    welcomeArt(); // memanggil header (berupa ascii art)
    printf("\nYour Balance: %lld\n", cs[currentUser].money); // menampilkan saldo user saat ini

    printf("Press enter key to continue..\n");
    getchar();

    secondMenu(); // kembali ke second menu
}

void deposit() { // menu deposit
depositMenu:
    system("cls"); // clear terimnal
    welcomeArt(); // menampilkan header (berupa ascii art)
    long long int depo;
    printf("Deposit Menu\n\n");
    do{ // meminta user memasukkan nominal deposit (syarat minimal Rp. 100.000)
        printf("Deposit Amount (min. Rp100.000): "); scanf("%lld", &depo); getchar();
        if(depo < 100000){
            printf("Error! Input min. Rp100.000!\n");
            sleep(1.5);
            goto depositMenu;
        }
    }while(depo < 100000); // jika kurang dari 100.000 maka user diminta memasukkan deposit kembali

    cs[currentUser].money += depo; // nilai saldo user ditambahkan sesuai nominal deposit
    puts("");
    printf("Deposit Successful\n");
    printf("Balance: %d\n", cs[currentUser].money); // menampilkan saldo user setelah deposit

    // update saldo ke customer_data.txt
    FILE *fpupdateDepo = fopen("../data/customer_data.txt", "w");
    for(int i = 0; i < userCount; i++){
        fprintf(fpupdateDepo, "%s,%s,%s,%lld,%s,%lld\n", cs[i].ID, cs[i].pass, cs[i].name, cs[i].phoneNumber, cs[i].date, cs[i].money);
    }
    fclose(fpupdateDepo); // close file

    printf("Press enter key to continue..\n");
    getchar();

    secondMenu(); // kembali ke second menu
}

void withdraw() { // menu withdraw
withdrawMenu: // menandakan withdrawMenu
    system("cls"); // clear terminal
    welcomeArt(); // menampilkan header (berupa ascii art)
    long long int tarikTunai;
    printf("Withdraw Menu\n\n"); 
    do{ // meminta user memasukkan nominal yang akan di withdraw (syarat max Rp. 5.000.000)
        printf("Withdraw Amount (max Rp5.000.000): "); scanf("%lld", &tarikTunai); getchar();
        if(tarikTunai > 5000000){ // jika lebih dari 5jt
            printf("Error! Input max Rp5.000.000!\n");
            sleep(2);
            goto withdrawMenu; // kembali ke withdrawMenu
        } else if (tarikTunai > cs[currentUser].money){ // jika yang di nominal yang di tarik lebih besar dari saldo user
            printf("Your Balance is not enough!\n");
            sleep(2);
            goto withdrawMenu; // kembali ke withdrawMenu
        }
    }while(tarikTunai > 5000000 || tarikTunai > cs[currentUser].money);

    cs[currentUser].money -= tarikTunai; // mengurangi saldo user sesuai nominal setelah melakukan penarikan
    puts("");
    printf("Withdraw Successful\n");
    printf("Balance: %d\n", cs[currentUser].money); // menampilkan saldo user setelah penarikan

    // update saldo user ke customer_data.txt
    FILE *fpupdateWithdraw = fopen("../data/customer_data.txt", "w");
    for(int i = 0; i < userCount; i++){
        fprintf(fpupdateWithdraw, "%s,%s,%s,%lld,%s,%lld\n", cs[i].ID, cs[i].pass, cs[i].name, cs[i].phoneNumber, cs[i].date, cs[i].money);
    }
    fclose(fpupdateWithdraw); // menutup file

    printf("Press enter key to continue..\n");
    getchar();

    secondMenu(); // kembali ke second menu
}

void transfer() { // menu transfer
    system("cls"); // clear terminal
    welcomeArt(); // menampilkan header (berupa ascii art)

    int tempDestination;
    long long int tempMoney;
    
    printf("Your Balance : %lld\n", cs[currentUser].money);

    if(cs[currentUser].money == 0){
        printf("Your Balance is Empty!\n");
        sleep(2.5);
        secondMenu();
    }

    printf("Daftar Transfer: \n");
    // print seluruh daftar account
    for(int i = 0; i < userCount; i++){
        printf("%d. %s\n", i+1, cs[i].name);
    }
    puts("");

    // user diminta memilih tujuan transfer
    do{
        printf("Choose account >> "); scanf("%d", &tempDestination);
        if(tempDestination < 0 || tempDestination > userCount){
            printf("Account not Found!\n");
        }
        if(tempDestination-1 == currentUser){
            printf("You Can't Transfer to Your Own Account!\n");
        }
    } while(tempDestination <= 0 || tempDestination > userCount || tempDestination-1 == currentUser);
    int transferDestination = tempDestination - 1;

retryTransfer: // declare retryTransfer sebagai flag
    // user diminta memasukkan nominal yang akan di transfer
    printf("Amount of money: "); scanf("%lld", &tempMoney);
    if(tempMoney <= cs[currentUser].money){ //jika saldo memenuhi nominal transfer
        cs[currentUser].money -= tempMoney;
        cs[transferDestination].money += tempMoney;
    } else { //jika saldo tidak mencukupi
        printf("Your Balance is not Enough!\n");
        goto retryTransfer; // kembali ke retryTransfer
    }

    // update data ke customer_data.txt
    FILE *fpupdateTransfer = fopen("../data/customer_data.txt", "w");
    for(int i = 0; i < userCount; i++){
        fprintf(fpupdateTransfer, "%s,%s,%s,%lld,%s,%lld\n", cs[i].ID, cs[i].pass, cs[i].name, cs[i].phoneNumber, cs[i].date, cs[i].money);
    }
    fclose(fpupdateTransfer);

    system("cls"); // clear terminal
    welcomeArt(); // menampilkan header (berupa ascii art)
    printf("Transfer Successful!\n"); 
    printf("Your Balance : %lld\n", cs[currentUser].money); // menampilkan saldo setelah transfer

    sleep(2);
    secondMenu(); // kembali ke second menu
}

void accSett() { // menu account setting
    int choice1;
    do{ 
        system("cls"); // clear terminal
        welcomeArt(); // menampilkan header (ascii art)
        puts("1. Account Information");
        puts("2. Change Password");
        puts("3. Close Account");
        puts("4. Back");
        printf("\n");
        printf("Choice[1-4] >> ");
        scanf("%d", &choice1); // meminta user memasukkan pilihan menu
        fflush(stdin); // membuang seluruh buffer
        switch(choice1) {
        case 1:
            accInfo(); // masuk ke menu account information
            break;
        case 2:
            changePass(); // masuk ke menu change password
            break;
        case 3:
            deleteACC(); // masuk ke menu delete account
            break;
        case 4:
            secondMenu(); // masuk ke menu sebelumnya (back)
            break;
        }
    }while(choice1 < 1 || choice1 > 4);
}

void accInfo() { // menu account information
    system("cls"); // clear terminal
    welcomeArt(); // menampilkan header (ascii art)

    // menampilkan seluruh data user
    printf("Your Account Information:\n");
    printf("Username\t\t: %s\n", cs[currentUser].ID);
    printf("Name\t\t\t: %s\n", cs[currentUser].name);
    printf("Phone Number\t\t: 0%lld\n", cs[currentUser].phoneNumber);
    printf("Money\t\t\t: %lld\n", cs[currentUser].money);
    printf("Registration Date\t: %s\n", cs[currentUser].date);

    printf("\nPress enter key to continue..\n");
    getchar();

    accSett(); // kembali ke menu account settings
}

void changePass() { // menu change password
    system("cls"); // clear terminal
    welcomeArt(); // menampilkan header (ascii art)

    char tempPass[9];

    // user diminta memasukkan password lama
    do{
        printf("Enter your current password: ");
        int i;
        for (i = 0; i < 8; i++) {
            tempPass[i] = getch();
            printf("*");
        }
        tempPass[i] = '\0';
        if(strcmp(tempPass, cs[currentUser].pass) != 0){ // jika password salah
            printf("\nWrong Password!\n");
        }
    }while(strcmp(tempPass, cs[currentUser].pass) != 0); // jika password salah akan kembali meminta password 
    
    puts("");
    // user diminta memasukkan password baru
    printf("Enter your new password: ");
    int i;
    for (i = 0; i < 8; i++) { // sensor password
        cs[currentUser].pass[i] = getch();
        printf("*");
    }
    cs[currentUser].pass[i] = '\0';

    // update file ke customer_data.txt
    FILE *fpupdatePassword = fopen("../data/customer_data.txt", "w");
    for(int i = 0; i < userCount; i++){
        fprintf(fpupdatePassword, "%s,%s,%s,%lld,%s,%lld\n", cs[i].ID, cs[i].pass, cs[i].name, cs[i].phoneNumber, cs[i].date, cs[i].money);
    }
    fclose(fpupdatePassword); // close file

    printf("\nPassword is changed\n");

    printf("\nPress enter key to continue..\n");
    getchar();

    secondMenu(); // kembali ke menu awal
}

void deleteACC() { // menu delete account
    system("cls"); // clear terminal
    welcomeArt(); // menampilkan header (berupa ascii art)

    char deleteConfirm;
    char withdrawConfirm;

    puts("");
    // menampilkan seluruh data user
    printf("Your Data\n");
    printf("Username\t\t: %s\n", cs[currentUser].ID);
    printf("Password\t\t: %s\n", cs[currentUser].pass);
    printf("Name\t\t\t: %s\n", cs[currentUser].name);
    printf("Phone Number\t\t: 0%lld\n", cs[currentUser].phoneNumber);
    printf("Registration Date\t: %s\n", cs[currentUser].date);
    printf("Your Balance\t\t: %lld\n", cs[currentUser].money);
    puts("");

    printf("Are you sure you want to close your account? y/n\n"); // konfirmasi apakah user akan menghapus account
    printf(">> "); scanf("%c", &deleteConfirm); getchar(); 
    if(deleteConfirm == 'y'){ // jika user setuju
        if(cs[currentUser].money > 0){ // mengecek saldo user
            // jika masih ada, user diminta untuk menarik saldo
            printf("You still have money in your balance!\n");
            printf("Do you want to withdraw your money? y/n\n");
            printf(">> ");
            scanf("%c", &withdrawConfirm);
            if(withdrawConfirm == 'y'){ // jika setuju menarik saldo
                printf("Your balance is already withdrawn\n");
                printf("Your balance : 0\n"); // saldo akan menjadi 0
            } else if(withdrawConfirm == 'n'){ // jika tidak setuju menarik saldo
                printf("If you want to CLOSE ACCOUNT your balance must be \'0\'\n");
                sleep(4);
                accSett(); // user di arahkan untuk kembali ke account setting
            }
        }   
        // update file yang di delete ke customer_data.txt
        FILE *fdeleteAcc = fopen("../data/customer_data.txt", "w");
        for(int i = 0; i < userCount; i++){
            if(i == currentUser){ // jika i sama dengan current user
                continue; // diabaikan
            }
            fprintf(fdeleteAcc, "%s,%s,%s,%lld,%s,%lld\n", cs[i].ID, cs[i].pass, cs[i].name, cs[i].phoneNumber, cs[i].date, cs[i].money);
        }
        fclose(fdeleteAcc); // close file

        printf("Your Account Has Been Closed!\n");
        printf("Thank You for Using Our Bank\n");
        sleep(2);
        firstMenu(); // kembali ke first menu
    } else if(deleteConfirm == 'n'){ // jika user tidak setuju
        printf("Returning to previous menu...\n");
        sleep(2);
        accSett(); // kembali ke account setting
    }
}

void exit() { // menu exit
    system("cls"); // clear terminal
    welcomeArt(); // menampilkan header (berupa ascii art)
    printf("\t - Terima kasih - Thank you - Gracias -\n");
    printf("\t - Merci - matur nuwun -  hatur nuhun -\n");
    puts("---------------------------------------------------------");
    printf("\t  Jelshen Febryand Valesco - 2502037593\n");
    printf("\t Timothy Theophilus Hartono - 2540129546\n");
    puts("---------------------------------------------------------\n");
}
