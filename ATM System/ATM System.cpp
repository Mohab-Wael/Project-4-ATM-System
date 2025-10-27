#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;


struct stClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance=0;
};

const string ClientsFileName = "Clients.txt";
stClient CurrentClient;

enum enMainMenueOptions
{
	eQuickWithdraw = 1,
	eNormalWithdraw = 2,
	eDeposit = 3,
	eCheckBalance = 4,
	eExit = 5
};


 
void ShowMainMenueScreen();
void ShowQuickWithdrawMenueScreen();
void ShowNormalWithdrawSceen();
void ShowDepositMenueScreen();
void Login();

vector <string> SplitString(string s1, string Delim)
{
	vector <string> vString;

	short pos = 0;
	string sWord;

	while ((pos = s1.find(Delim)) != std::string::npos)
	{
		sWord = s1.substr(0, pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		s1.erase(0, pos + Delim.length());
	}
	if (s1 != "")
	{
		vString.push_back(s1);
	}

	return vString;
}

stClient ConvertLineToRecord(string S1, string Seperator = "#//#")
{
	stClient Client;
	vector <string> vString;

	vString = SplitString(S1, Seperator);

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.Phone = vString[3];
	Client.AccountBalance = stod(vString[4]);
	return Client;
}

string ConvertRecordToLine(stClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";

	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

vector <stClient>  LoadClientsDataFromFile(string FileName)
{
     fstream MyFile;
	vector <stClient> vClients;

	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return vClients;
}

vector <stClient> SaveClientsDataToFile(string FileName, vector <stClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;

	if (MyFile.is_open())
	{

		for (stClient& C : vClients)
		{
			
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
	
		}
		MyFile.close();
	}
	return vClients;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <stClient>& vClients)
{
	char Answer = 'n';
	cout << "\n\nAre you sure you want perform this transaction ? y/n ? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		for (stClient& C : vClients)
		{
			if(C.AccountNumber== AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully, New Balance is: " << C.AccountBalance << endl;
				return true;
			}
		}
	}
	return false;
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber,string PinCode,stClient &Client)
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	for (stClient &C : vClients)
	{
		if (C.AccountNumber == AccountNumber && C.PinCode== PinCode)
		{
			Client = C;
			return true;
		}
	}
	return false;
}



short ReadQuickWithdrawOption()
{
	short Choice = 0;

	while (Choice < 1 || Choice >9)
	{
		cout << "\nChoose the amount to withdraw ? [1 to 9] ? ";
		cin >> Choice;
	}

	return Choice;
}
 
short getQuickWithdrawAmount(short QuickWithdrawOption)
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
	case 7 :
		return 800;
	case 8:
		return 1000;
	default:
		return 0;
	}
}

void PerformQuickWithdraw(short QuickWithdrawOptions)
{
	if(QuickWithdrawOptions == 9)
	{
		return;
	}

	short WithdrawAmount = getQuickWithdrawAmount(QuickWithdrawOptions);

	if (WithdrawAmount > CurrentClient.AccountBalance)
	{
		cout << "\n\nThe amount exeeds your balance, make another choice.\n";
		cout << "Press any key to continue...";
		system("pause>0");
		ShowQuickWithdrawMenueScreen();
		return;
	}
 
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, -WithdrawAmount, vClients);
	CurrentClient.AccountBalance -= WithdrawAmount;
}

void GoBackToMainMenue()
{
	cout << "\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenueScreen();
}

void ShowQuickWithdrawMenueScreen()
{
	cout << "=================================\n";
	cout << "\t\tQuick Withdraw";
	cout << "\n=================================\n";
	cout << "\t[1] 20\t\t[2] 50\n";
	cout << "\t[3] 100\t\t[4] 200\n";
	cout << "\t[5] 400\t\t[6] 600\n";
	cout << "\t[7] 800\t\t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << "=================================\n";

	cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;
	PerformQuickWithdraw(ReadQuickWithdrawOption());
}

int ReadWithdrawAmount()
{
	int Amount = 0;
	cout << "\nEnter an amount to multiple of 5`s  ? ";
	cin >> Amount;

	while(Amount % 5 != 0)
	{
		cout << "\nError: Amount must be multiple of 5`s, Enter Amount again ? ";
		cin >> Amount;
	}

	return Amount;
}

void PerformNormalWithdrawOption()
{
	int Amount = ReadWithdrawAmount();

	if(Amount > CurrentClient.AccountBalance)
	{
		cout << "\n\nThe amount exeeds your balance, make another choice.\n";
		cout << "Press any key to continue...";
		system("pause>0");
		ShowNormalWithdrawSceen();
		return;
	}

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, -Amount, vClients);
	CurrentClient.AccountBalance -= Amount;
}

void ShowNormalWithdrawSceen()
{
	system("cls");
	cout << "=================================\n";
	cout << "\t\tNormal Withdraw";
	cout << "\n=================================\n";
	PerformNormalWithdrawOption();
}
	
int ReadPositiveDepositAmount()
{
	int Amount = 0;
	cout << "\nEnter a positive Deposit Amount ? ";
	cin >> Amount;

	while (Amount <= 0)
	{
		cout << "\nError: Amount must be positive, Enter Amount again ? ";
		cin >> Amount;
	}
	return Amount;
}

void PerformDepositOption()
{
	int Amount = ReadPositiveDepositAmount();

        vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
		DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount, vClients);
		CurrentClient.AccountBalance += Amount;
	
}

void ShowDepositMenueScreen()
{
	system("cls");
	cout << "=================================\n";
	cout << "\t\tDeposit";
	cout << "\n=================================\n";
	PerformDepositOption();
}


void ShowCheckBalanceScreen()
{
	system("cls");
	cout << "=================================\n";
	cout << "\t\tCheck Balance Screen";
	cout << "\n=================================\n";
	cout << "Your Balance is: " << fixed<<CurrentClient.AccountBalance << endl;
}

short ReadMainMenueOption()
{
	short Choice = 0;

	cout << "\nChoose what do you want to do ? [1 to 5] ? ";
	cin >> Choice;
	return Choice;
}

void PerformMainManueOption(enMainMenueOptions ATMMainManueOption)
{
	switch (ATMMainManueOption)
	{
	case enMainMenueOptions::eQuickWithdraw:
	    	system("cls");
			ShowQuickWithdrawMenueScreen();
			GoBackToMainMenue();
			break;
    case enMainMenueOptions::eNormalWithdraw:
		    system("cls");
			ShowNormalWithdrawSceen();
			GoBackToMainMenue();
			break;
	case enMainMenueOptions::eDeposit:
		ShowDepositMenueScreen();
		GoBackToMainMenue();
		break;
		case enMainMenueOptions::eCheckBalance:
			system("cls");
			ShowCheckBalanceScreen();
			GoBackToMainMenue();
			break;
		case enMainMenueOptions::eExit:
			system("cls");
			Login();
			break;
	}
}

void ShowMainMenueScreen()
{
	system("cls");
	cout << "=================================\n";
	cout << "\t\tMain Main Menue";
	cout << "\n=================================\n";
	cout << "\t[1] Quick Withdraw\n";
	cout << "\t[2] Normal Withdraw\n";
	cout << "\t[3] Deposit\n";
	cout << "\t[4] Check Balance\n";
	cout << "\t[5] Logout\n";
	cout << "=================================\n";
	PerformMainManueOption((enMainMenueOptions)ReadMainMenueOption());
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
	return FindClientByAccountNumberAndPinCode(AccountNumber, PinCode,CurrentClient);
}

void Login()
{
	bool LoginFailed = false;
	string AccountNumber ,PinCode;
	do
	{
		cout << "--------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n--------------------------------\n";

		if(LoginFailed)
		{
			cout << "\nError: Invalid Account Number or PinCode, Try Again.\n";
		}

		cout << "\nEnter Account Number ? ";
		cin >> AccountNumber;

		cout << "\nEnter PinCode ? ";
		cin >> PinCode;

		LoginFailed = !LoadClientInfo(AccountNumber, PinCode);

	} while (LoginFailed);

	ShowMainMenueScreen();
}



int main()
{

	Login();


	system("pause");
	return 0;
}