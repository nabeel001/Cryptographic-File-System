#ifndef CFS_header
#define CFS_header

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#define LOW 25
#define HIGH 50

using namespace std;
namespace fs = filesystem;

int UID = 1024;
map<int, string> auth;
map<string, int> keys;
map<string, bool> file_state;
map<string, int> file_auth;

void init_file_system()
{
    auto path = fs::current_path();
    for (const auto &entry : fs::directory_iterator(path))
        file_state[entry.path().filename().string()] = false;
}

void display_file_system()
{
    cout << "\nDirectory: " << fs::current_path() << endl;
    auto path = fs::current_path();
    cout << "-------------------------------------------------------" << endl;
    cout << "|    File Name                           Status       |" << endl;
    cout << "-------------------------------------------------------" << endl;
    for (const auto &entry : fs::directory_iterator(path))
    {
        cout << " " << entry.path().filename() << "\t      --->       ";
        cout << ((file_state[entry.path().filename().string()]) ? "Secured" : "Unsecured") << endl;
    }
    cout << endl;
}

int User_auth()
{
    while (true)
    {
        int uid;
        cout << "Enter your User Id if exists (ELSE enter 0): ";
        cin >> uid;
        if (uid)
        {
            if (auth.count(uid))
            {
                string pass;
                cout << "Enter your password: ";
                cin >> pass;
                if (auth[uid] == pass)
                    return uid;
                else
                    cout << "Incorrect Password.... Try again" << endl;
            }
            else
            {
                cout << "User Id entered Doesn't exist... Try Again" << endl;
            }
        }
        else
        {
            int uid = UID++;
            string pass;
            cout << "\tYour New User Id is: " << uid << endl;
            cout << "\tEnter your password: ";
            cin >> pass;
            auth[uid] = pass;
            cout << "\nSuccessfully Registered!!" << endl;
            return uid;
        }
    }
}

void encrypt_file(string file_name)
{
    srand(time(0));
    int key = LOW + (std::rand() % (HIGH - LOW + 1));
    keys[file_name] = key;

    char c;
    ifstream fin;
    ofstream fout;

    fin.open(file_name.c_str(), ios::binary);
    string temp_file_name = "temp_out";
    fout.open(temp_file_name.c_str(), ios::binary);
    while (1)
    {
        fin >> noskipws >> c;
        if (fin.eof())
            break;
        int temp = (c + key);
        fout << (char)temp;
    }
    fin.close();
    fout.close();

    remove(file_name.c_str());
    rename(temp_file_name.c_str(), file_name.c_str());

    file_state[file_name] = true;
}

void decrypt_file(string file_name)
{
    char c;
    int key = keys[file_name];

    ifstream fin;
    ofstream fout;
    fin.open(file_name.c_str(), ios::binary);
    string temp_file_name = "temp" + file_name;
    fout.open(temp_file_name.c_str(), ios::binary);
    while (1)
    {
        fin >> noskipws >> c;
        if (fin.eof())
            break;
        int temp = (c - key);
        fout << (char)temp;
    }
    fin.close();
    fout.close();

    remove(file_name.c_str());
    rename(temp_file_name.c_str(), file_name.c_str());

    file_state[file_name] = false;
    keys.erase(file_name);
}

void file_create()
{
    string file_name;
    cout << "Enter the File name to CREATE: ";
    cin >> file_name;

    if (file_state.count(file_name))
    {
        cout << "File name already exists!!";
        return;
    }

    fstream file;
    file_name += ".txt";
    file.open(file_name, ios::out);
    if (file)
    {
        cout << "File not created!";
        return;
    }
    else
    {
        file_state[file_name] = false;
        string content;
        cout << "Enter the contents of the file: ";
        cin >> content;
        file << content;
        cout << "File CREATED successfully !!" << endl;
        file.close();
    }
    char choice;
    cout << "Do you want to SECURE this file(y/n): ";
    cin >> choice;
    if (choice == 'y')
    {
        int uid = User_auth();
        file_auth[file_name] = uid;
        encrypt_file(file_name);
        cout << "The file has been SECURED with encryption !!!!" << endl;
    }
}

void file_delete()
{
    display_file_system();
    string file_name;
    cout << "Enter the file name to DELETE with extension (xxxx.yyy): ";
    cin >> file_name;

    if (file_state.count(file_name))
    {
        if (file_state[file_name])
        {
            int uid;
            string pass;

            cout << "\tEnter User Id: ";
            cin >> uid;
            cout << "\tEnter Password: ";
            cin >> pass;

            if (auth[uid] == pass)
            {
                remove(file_name.c_str());
                keys.erase(file_name);
                file_auth.erase(file_name);
            }
            else
            {
                cout << "Incorrect Credentials !!";
                return;
            }
        }
        else
            remove(file_name.c_str());

        file_state.erase(file_name);
        cout << "The File has been successfully DELETED !!" << endl;
    }
    else
        cout << "No Such File Exists" << endl;
}

void display_content(string file_name)
{
    cout << "\tThe File contents are: \n\n";
    fstream file;
    file.open(file_name, ios::in);
    char ch;
    while (1)
    {
        file >> noskipws >> ch;
        if (file.eof())
            break;
        cout << ch;
    }
    file.close();
    cout << endl;
}

void file_open()
{
    display_file_system();
    string file_name;
    cout << "Enter the file name to OPEN with extension (xxxx.yyy): ";
    cin >> file_name;

    if (!file_state.count(file_name))
    {
        cout << "File doesn't exist !!";
        return;
    }

    if (file_state[file_name])
    {
        char choice;
        display_content(file_name);
        cout << "\nDo you want to view DECRYPTED CONTENTS of this file(y/n): ";
        cin >> choice;
        if (choice == 'y')
        {
            int uid;
            string pass;

            cout << "\tEnter User Id: ";
            cin >> uid;
            cout << "\tEnter Password: ";
            cin >> pass;

            if (auth[uid] == pass)
            {
                decrypt_file(file_name);
                display_content(file_name);
                encrypt_file(file_name);
            }
            else
            {
                cout << "Incorrect Credentials!!";
                return;
            }
        }
    }
    else
        display_content(file_name);
}

void file_secure()
{
    display_file_system();
    string file_name;
    cout << "Enter the file name to SECURE with extension (xxxx.yyy): ";
    cin >> file_name;

    if (file_state.count(file_name))
    {
        if (file_state[file_name])
            cout << "This File is AlREADY SECURED" << endl;
        else
        {
            int uid = User_auth();
            file_auth[file_name] = uid;
            encrypt_file(file_name);
            cout << "The file has been SECURED with encryption !!!!" << endl;
        }
    }
    else
        cout << "No Such File Exists" << endl;
}

void file_unsecure()
{
    display_file_system();
    string file_name;
    cout << "Enter the file name to UNSECURE with extension (xxxx.yyy): ";
    cin >> file_name;

    if (file_state.count(file_name))
    {
        if (file_state[file_name])
        {
            int uid;
            string pass;

            cout << "\tEnter User Id: ";
            cin >> uid;
            cout << "\tEnter Password: ";
            cin >> pass;

            if (auth[uid] == pass)
            {
                decrypt_file(file_name);
                file_auth.erase(file_name);
                cout << "The File has been DECRYPTED successfully !!!!" << endl;
            }
            else
                cout << "Incorrect Credentials !!" << endl;
        }
        else
            cout << "This File is ALREADY UNSECURE !!" << endl;
    }
    else
        cout << "No Such File Exists" << endl;
}

#endif