#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

const string ClientsFileName = "Clients.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

sClient CurrentClient;
void ShowQuickWithdrawScreen();
void ShowMainMenu();
void Login();
void ShowNormalWithdrawScreen();

enum enMainMenuOptions {
    eQuickWithdraw = 1, eNormalWithDraw = 2, eDeposit = 3,
    eCheckBalance = 4, eChangePassword = 5, eExit = 6
};
vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}

vector <sClient> LoadClientsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Client)
{

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }

    }
    return false;

}
bool LoadUserInfo(string Username, string Password)
{
    if (FindClientByAccountNumberAndPinCode(Username, Password, CurrentClient))
        return true;
    else
        return false;
}
void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenu();
}
short ReadQuickWithdrawOption()
{
    short Choice = 0;
    while (Choice < 1 || Choice > 9)
    {
        cout << "\nChoose what to do from [1] to [9] ? ";
        cin >> Choice;
    }
    return Choice;
}
short GetQuickWithdrawAmount(short QuickWithdrawOption)
{
    switch (QuickWithdrawOption)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}
vector <sClient> SaveClientsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}
bool SaveClientDataToFile(vector <sClient> vClients, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            string DataLine = "";
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
        return true;
    }
    return false;
}
bool UpdateClientDataByAccountNumber(vector <sClient> &vClients, sClient &CurrentClient, string AccountNumber, string NewPinCode)
{
    
    for (sClient &C : vClients)
    {
        if (CurrentClient.AccountNumber == AccountNumber)
        {
            CurrentClient.PinCode = NewPinCode;
            C = CurrentClient;
            return true;
        }
    }
    return false;

}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }
        return false;
    }

}
void PerformQuickWithdrawOption(short QuickWithdrawOption)
{
    if (QuickWithdrawOption == 9)//Exit
        return;
    short WithDrawBalance = GetQuickWithdrawAmount(QuickWithdrawOption);
    while (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }
    vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WithDrawBalance;

}
void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tQuick Withdraw Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "\nYour Balance is " << CurrentClient.AccountBalance;
    cout << "\n";
    PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}
int ReadWithDrawAmont()
{
    int Amount;
    cout << "\nEnter an amount multiple of 5's ? ";

    cin >> Amount;

    while (Amount % 5 != 0)
    {
        cout << "\nEnter an amount multiple of 5's ? ";
        cin >> Amount;
    }
    return Amount;
}

void PerformNormalWithdrawOption()
{
    int WithdrawBalance = ReadWithDrawAmont();
    if (WithdrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowNormalWithdrawScreen();
        return;

    }
    vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawBalance, vClients);
    CurrentClient.AccountBalance -= WithdrawBalance;

}
void ShowNormalWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "===========================================\n";
    PerformNormalWithdrawOption();
}
double ReadDepositAmount()
{
    double Amount = 0;

    while (Amount <= 0)
    {
        cout << "\nEnter a positive Deposit Amount? ";
        cin >> Amount;
    }
    return Amount;
}
void ShowDepositScreen()
{
    double  DepositAmount = ReadDepositAmount();
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
    CurrentClient.AccountBalance += DepositAmount;
}
void  ShowCheckBalanceScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===========================================\n";
    cout << "Your Balance is: " << CurrentClient.AccountBalance << "\n";
}
string ReadNewPinCode()
{
    string PinCode = "";
    cout << "\nPlease enter the new pincode/password";
    cin >> PinCode;
    return PinCode;
}

void ShowChangePasswordScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tChange PassWord Screen\n";
    cout << "===========================================\n";
    //CurrentClient.PinCode = ReadNewPinCode();
    
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    cout << "\nYour password is " << CurrentClient.PinCode;
    char Answer = 'n';
    cout << "\n\nAre you sure do you want to change your password y/n? ";
    cin >> Answer;
    if (Answer == 'Y' || Answer == 'y')
    {
        if (UpdateClientDataByAccountNumber(vClients, CurrentClient, CurrentClient.AccountNumber, ReadNewPinCode()))
        {

            if (SaveClientDataToFile(vClients, ClientsFileName))
                cout << "\nUpdate succefully, New Password is: " << CurrentClient.PinCode << endl;
        }
    }
    
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOption)
{
    switch (MainMenuOption)
    {
    case enMainMenuOptions::eQuickWithdraw:
        ShowQuickWithdrawScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eNormalWithDraw:
        ShowNormalWithdrawScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eDeposit:
        ShowDepositScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eCheckBalance:
        ShowCheckBalanceScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eChangePassword:
        ShowChangePasswordScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eExit:
        system("cls");
        Login();
        break;
    }
}
short ReadMainMenuOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void ShowMainMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Change My Password.\n";
    cout << "\t[6] Logout.\n";
    cout << "===========================================\n";
    PerformMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}
bool FindClientByAccountNumberAndPincode(string AccountNumber, string PinCode, sClient& Client)
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    for (const sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
bool LoadClientInfo(string Username, string Pincode)
{
    if (FindClientByAccountNumberAndPincode(Username, Pincode, CurrentClient))
        return true;
    else
        return false;
}
void Login()
{
    bool LoginFaild = false;
    string AccountNumber, Pincode;
    do
    {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invalid Account Number/PinCode!\n";
        }

        cout << "Enter Account Number? ";
        cin >> AccountNumber;

        cout << "Enter Pin? ";
        cin >> Pincode;
        LoginFaild = !LoadClientInfo(AccountNumber, Pincode);
    } while (LoginFaild);

    ShowMainMenu();
}
int main()
{
    Login();

    system("pause>0");
    return 0;
}