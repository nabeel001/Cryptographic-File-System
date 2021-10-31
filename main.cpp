#include "CFS_headers.h"

int main()
{
    cout << endl;
    cout << "\t\t------------------------------" << endl;
    cout << "\t\t|  Crytographic file system  |" << endl;
    cout << "\t\t------------------------------" << endl;
    init_file_system();

    while (true)
    {
        cout << endl;
        cout << "\t1. Create a File" << endl;
        cout << "\t2. Delete a File" << endl;
        cout << "\t3. Open a File" << endl;
        cout << "\t4. Secure File with Encryption" << endl;
        cout << "\t5. Unsecure an Encrpyted File " << endl;
        cout << "\t6. Open File Explorer" << endl;
        cout << "\t7. Exit" << endl;

        int choice;
        cout << "\nEnter Your Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            file_create();
            break;
        case 2:
            file_delete();
            break;
        case 3:
            file_open();
            break;
        case 4:
            file_secure();
            break;
        case 5:
            file_unsecure();
            break;
        case 6:
            display_file_system();
            break;
        case 7:
            return 0;
            break;
        default:
            cout << "Enter a Valid Choice" << endl;
            break;
        }
    }
    return 0;
}
