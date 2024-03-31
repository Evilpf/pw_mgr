#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <iomanip>
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <ctime>





using namespace std;


vector<string> vec;

map <string, string> records;
map <string, string> loginCreds;

string email, pw, fileUser, fileLogin;



//Function prototypes
void read();
void write();
void display();
void generatePW();
bool login();
bool registerUser();

//Function prototypes




int main() {

    // Read login creds from file
    ifstream inFile("loginCreds.txt");

    if (!inFile.is_open()) {
        cout << "Error: Unable to open login credentials file." << endl;
        // Exit program if file cannot be opened
        return 1;
    }

    string storedUsername, storedPassword;

    while (inFile >> storedUsername >> storedPassword) {
        loginCreds[storedUsername] = storedPassword;
    }

    inFile.close();



    // Attempt to login or register
    if (!login() && !registerUser()) {
        cout << "Login or registration failed. Exiting...." << endl;
        return 1;
    }


    string menuChoice;

    do {
        string repeat;
        bool leave = true;

        read();

        cout << "[A]dd, [D]elete, [E]dit, [P]rint, [G]enerate, [EX]it \n";
        cin >> menuChoice;
        cin.ignore();

        for (char& c : menuChoice) {
            c = toupper(c);
        }

        if (menuChoice == "A") {
            while (leave) {
                cout << "\nEnter Email ";
                cin >> email;

                cout << "\nEnter Password ";
                cin >> pw;

                if (records.find(email) != records.end() && records.find(pw) != records.end()) {
                    cout << "Error! Login already saved! ";
                    continue;
                }

                records.insert(pair<string, string>(email, pw));

                cout << "\nRecords.......\n";

                write();
                display();

                cout << "\nDo you want to enter another set of credentials? (Y/N) ";
                cin >> repeat;

                for (char& s : repeat) {
                    s = toupper(s);
                }

                if (repeat == "N") {
                    cout << "Data is Saved....";
                    break;
                }
            }
        }

        if (menuChoice == "D") {
            cout << "Enter then email of the login ";
            cin >> email;

            auto it = records.find(email);
            if (it != records.end()) {
                records.erase(it);
                write();
            }
        }

        if (menuChoice == "E") {
            string pwNew;
            cout << "Enter the email ";
            cin >> email;

            auto it = records.find(email);
            if (it != records.end()) {
                cout << "Enter the password: ";
                cin >> pwNew;
                it->second = pwNew;
                write();
                cout << "Records updated............";
            }
            else {
                cout << "Error! Email not found!" << endl;
            }
        }

        if (menuChoice == "P") {
            display();
        }

        if (menuChoice == "EX") {
            return 0;
        }

        if (menuChoice == "G") {
            generatePW();
        }
    } while (menuChoice != "EX");

    return 0;
}


void read() {

    ifstream file("login.txt");

    if (!file.is_open()) {
        cout << "Error: Could not open the file! " << endl;
        return;
    }

    while (file >> email >> pw) {
        records[email] = pw;
    }

    file.close();

    if (records.empty()) {

        cout << "There are no records";
        return;
    }

    display();
}

void write() {

    ofstream file;

    string s;

    file.open("login.txt", ofstream::out | ofstream::trunc);
    
    file.close();

    file.open("login.txt", ofstream::out | ofstream::app);

    map <string, string>::iterator it;

    for (it = records.begin(); it != records.end(); ++it) {

        s = it->first + " " + it->second + "\n";

        file << s;
    }
    
    file.close();
}

void display() {

    cout << "You have " << records.size() << " login(s) in your manager! " << endl;

    map <string, string>::iterator it;

    cout << setw(23) << "Logins\n" << "**************************************\n" << endl;

    for (it = records.begin(); it != records.end(); ++it) {
 
        cout  << "Email: " << it->first << "\n" << "Password: " << it->second << endl;
    }
    cout << "\n**************************************" << endl;

}

void generatePW(){

    const char str[] = "!#$%^&*()_+-={}[]|\\:;<>,.?/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    string sizeinput;

    cout << "How many characters? ";
    cin >> sizeinput;

    int number = stoi(sizeinput);
    string randompw;

    
    for (int i = 0; i < number; i++) {
        
        int randomIndex = rand() % sizeof(str);
        
        randompw += str[randomIndex];
    }

    cout << "Random Password: " << randompw << "\n" << endl;

  
}

bool login() {
    
    string username, password;
    
    cout << "Enter username: ";
    cin >> username;

    cout << "Enter password: ";
    cin >> password;

    // Read login creds from file
    ifstream inFile("loginCreds.txt");

    if (!inFile.is_open()) {

        cout << "Error: Unable to open login credentials file. " << endl;
        
        return false;
    }

    string storedUsername, storedPassword;

    bool found = false;

    while (inFile >> storedUsername >> storedPassword) {

        if (storedUsername == username && storedPassword == password) {

            cout << "Login Succesful! Welcome!" << endl;

            // Clear the screen
            #ifdef _WIN32
           
            system("cls");
            
            #else
            system("clear");
            
            #endif

            found = true;

            break;
        }
    }

    inFile.close();

    if (!found) {

        cout << "Invalid username or password." << endl;
        
        exit(EXIT_FAILURE);
    }

    return found;
}

bool registerUser() {

    string username, password;

    cout << "New user registration\n";
    cout << "Enter username: ";
    cin >> username;

    if (loginCreds.find(username) != loginCreds.end()) {

        cout << "Username already exists. Registration failed." << endl;

        return false;
    }

    cout << "Enter password: ";
    cin >> password;

    // Debug output
    cout << "Attempting to open login credentials file..." << endl;

    ofstream outFile("loginCreds.txt", ios::app);

    if (outFile.is_open()) {

        cout << "File opened successfully." << endl;

        outFile << username << " " << password << endl;

        outFile.close();

        cout << "Login credentials written to file." << endl;

    }
    else {

        cout << "Error: Unable to open file for writing." << endl;
    }

    return true;
}

//3.29.2024