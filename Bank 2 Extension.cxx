#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

const string Filename = "Clients1.txt";

enum enLest
{
	ShowClents = 1,
	FoundClient,
	DeletaClient,
	UpdetaClint,
	AddClient,
	Transactions,
	Exit
};
enum enTransList
{
	Deposit = 1,
	Withdraw,
	TotalBalance,
	ToInfoLest
};
struct sClient
{
	string accountnumber;
	string pincode;
	string name;
	string phone;
	double accountbalance = 0;
	bool marketfordelete = false;
};

void ShowMainScreen();
void ShowTransactionsScreen();

short ReadUserChooce()
{
	short Chooce;
	cout << "Enter your chooce :";
	cin >> Chooce;
	return Chooce;
}
string ReadAccountNumber()
{
	string Accountnumber;
	cout << "Enter accountnumber :";
	cin >> Accountnumber;
	return Accountnumber;
}
vector<string> SplitString(string text, string delim)
{
	string Word;
	short Pos = 0;
	vector<string> vWord;
	while ((Pos = text.find(delim)) != std::string::npos)
	{
		Word = text.substr(0, Pos);
		if (Word != "")
			vWord.push_back(Word);

		text.erase(0, Pos + delim.length());
	}
	if (text != "")
		vWord.push_back(text);

	return vWord;
}
sClient ConvertLineToRecord(string Line, string Delim = "#//#")
{
	sClient client;
	vector<string> vWord;
	vWord = SplitString(Line, Delim);

	client.accountnumber = vWord[0];
	client.pincode = vWord[1];
	client.name = vWord[2];
	client.phone = vWord[3];
	client.accountbalance = stod(vWord[4]);

	return client;
}
vector<sClient> LoadDataFromFile(string Filename)
{
	string Line;
	sClient Client;
	vector<sClient> vData;
	fstream file;
	file.open(Filename, ios::in);
	if (file.is_open())
	{
		while (getline(file, Line))
		{
			Client = ConvertLineToRecord(Line);
			vData.push_back(Client);
		}
		file.close();
	}
	return vData;
}
void PrintClient(sClient info)
{
	cout << left;
	cout << setw(15) << info.accountnumber << "|";
	cout << setw(10) << info.pincode << "|";
	cout << setw(20) << info.name << "|";
	cout << setw(15) << info.phone << "|";
	cout << setw(15) << info.accountbalance << endl;
}
void ShowScreenClientAndPrintClient()
{
	vector<sClient> vData = LoadDataFromFile(Filename);

	cout << "\t\t\tClient List (" << vData.size() << ")";

	cout << "\n\n-------------------------------------------------------------------\n\n";

	cout << left;
	cout << setw(15) << "accountnumber"
		 << "|";
	cout << setw(10) << "pincode"
		 << "|";
	cout << setw(20) << "name"
		 << "|";
	cout << setw(15) << "phone"
		 << "|";
	cout << setw(15) << "accountbalance" << endl;

	cout << "\n-------------------------------------------------------------------\n";

	for (sClient &Data : vData)
	{
		PrintClient(Data);
	}
}
bool FoundClientByAccountNumber(vector<sClient> vData, string Accountnumber)
{
	for (sClient &C : vData)
	{
		if (C.accountnumber == Accountnumber)
		{
			PrintClient(C);
			return true;
		}
	}
	cout << "\nSoryy is not found.";
	return false;
}
void ShowFoundClient()
{
	cout << "===============================\n";
	cout << "\tFound Client.\n";
	cout << "===============================\n";
	string Accountnumber = ReadAccountNumber();
	vector<sClient> vData = LoadDataFromFile(Filename);
	FoundClientByAccountNumber(vData, Accountnumber);
}
bool MarketClientForDelete(vector<sClient> &vData, string Accountnumber)
{
	for (sClient &c : vData)
	{
		if (c.accountnumber == Accountnumber)
		{
			c.marketfordelete = true;
			return true;
		}
	}
}
string ConvertRecordToLine(sClient vClient, string Delim = "#//#")
{
	string Line;
	Line = vClient.accountnumber + Delim;
	Line += vClient.pincode + Delim;
	Line += vClient.name + Delim;
	Line += vClient.phone + Delim;
	Line += to_string(vClient.accountbalance);
	return Line;
}
void SaveClientDataToFile(vector<sClient> vClient, string Filename)
{
	string Line;
	fstream file;
	file.open(Filename, ios::out);
	if (file.is_open())
	{
		for (sClient &c : vClient)
		{
			if (c.marketfordelete == false)
			{
				Line = ConvertRecordToLine(c);
				file << Line << endl;
			}
		}
		file.close();
	}
}
bool DeleteClient(vector<sClient> &vClient, string Accountnumber)
{
	char Answer = 'N';

	if (FoundClientByAccountNumber(vClient, Accountnumber))
	{
		cout << "\nAre you sure you deleted the client ? Y / N :";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			MarketClientForDelete(vClient, Accountnumber);
			SaveClientDataToFile(vClient, Filename);
			vClient = LoadDataFromFile(Filename);
			cout << "\n\nDeleted successfully.";
			return true;
		}
	}
}
void ShowDeleteClient()
{
	cout << "==============================\n";
	cout << "\tDeleta Client.\n";
	cout << "==============================\n";
	string Accountnumber = ReadAccountNumber();
	vector<sClient> vData = LoadDataFromFile(Filename);
	DeleteClient(vData, Accountnumber);
}
bool EditClientData(vector<sClient> &vClient, string Accountnumber)
{
	for (sClient &c : vClient)
	{
		if (c.accountnumber == Accountnumber)
		{
			cout << "Enter Pin code :";
			getline(cin >> ws, c.pincode);
			cout << "\nEnter Name ;";
			getline(cin, c.name);
			cout << "\nEnter Phone :";
			getline(cin, c.phone);
			cout << "\nEnter Account balance :";
			cin >> c.accountbalance;
			return true;
		}
	}
	return false;
}
void LoadClientDataToFile(vector<sClient> vUpdate)
{
	string Line;
	fstream file;
	file.open(Filename, ios::out);
	if (file.is_open())
	{
		for (sClient &c : vUpdate)
		{
			Line = ConvertRecordToLine(c);
			file << Line << endl;
		}
		file.close();
	}
}
bool UpdateClientData(vector<sClient> &vClient, string Accountnumber)
{
	char Answer = 'N';

	if (FoundClientByAccountNumber(vClient, Accountnumber))
	{
		cout << "\n\nAre you sure you updated Client";
		cout << " ? enter Y / N :";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			EditClientData(vClient, Accountnumber);
			LoadClientDataToFile(vClient);
		}
		cout << "\n\nUpdated successfully.";
	}
}
void ShowUpdateClient()
{
	cout << "=============================\n";
	cout << "\tUpdated Client.\n";
	cout << "=============================\n";
	string Accountnumber = ReadAccountNumber();
	vector<sClient> vData = LoadDataFromFile(Filename);
	UpdateClientData(vData, Accountnumber);
}
sClient AddClientInfo(string Accountnumber)
{
	sClient info;
	info.accountnumber = Accountnumber;
	cout << "Enter pincode :";
	getline(cin >> ws, info.pincode);
	cout << "Enter name :";
	getline(cin, info.name);
	cout << "Enter phone :";
	getline(cin, info.phone);
	cout << "Enter accountbalance :";
	cin >> info.accountbalance;
	return info;
}
void AddNewClient(vector<sClient> &vData, string Accountnumber)
{
	sClient info;
	char Answer = 'N';
	do
	{
		for (sClient &C : vData)
		{
			while (Accountnumber == C.accountnumber)
			{
				cout << "\nThe accountnumber is used.\n";
				cout << "Enter accountnumber :";
				cin >> Accountnumber;
			}
		}
		info = AddClientInfo(Accountnumber);
		vData.push_back(info);
		SaveClientDataToFile(vData, Filename);
		cout << "\n\nDo you want add more client ? enter [Y] / [N] :";
		cin >> Answer;
	} while (Answer == 'Y' || Answer == 'y');
}
void ShowAddNewClient()
{
	cout << "==============================\n";
	cout << "\tAdd Nwe Clients.\n";
	cout << "==============================\n";
	string Accountnumber = ReadAccountNumber();
	vector<sClient> vData = LoadDataFromFile(Filename);
	AddNewClient(vData, Accountnumber);
}
void GoBackToLestScrean()
{
	cout << "\n\nPress any kye to back :" << endl;
	system("pause>0");
	ShowMainScreen();
}
void ShowEndScreen()
{
	cout << "\tProgram End.\n";
	cout << "––––––––––––––––––––––––––––––\n\n";
}
/*void WithdrawClient(vector<sClient>&vData, string Accountnumber)
{
  char Answer;
  double Howmuch = 0;
  if(FoundClientByAccountNumber(vData, Accountnumber))
  {
    cout << "\nAre you sure do you want to deposit enter, Y / N :";
    cin >> Answer;
    cout << "Enter how much do you want to withdraw :";
    cin >> Howmuch;
    if(Answer == 'Y' || Answer == 'y')
    {
      for(sClient & C : vData)
        {
          if(C.accountnumber == Accountnumber)
          (C.accountbalance += Howmuch) * -1;
        }
      SaveClientDataToFile(vData, Filename);
      cout << "\nwithdraw successfully.\n";
    }
  }
}*/
void DepositClient(vector<sClient> &vData, string Accountnumber, double Amount)
{
	char Answer;
	if (FoundClientByAccountNumber(vData, Accountnumber))
	{
		cout << "\nAre you sure do you want to deposit enter, Y / N :";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			for (sClient &c : vData)
			{
				if (c.accountnumber == Accountnumber)
				{
					c.accountbalance += Amount;
					break;
				}
			}
			SaveClientDataToFile(vData, Filename);
			cout << "\nDone successfully.\n";
		}
	}
}
void ShowWithdrawClient()
{
	cout << "==============================\n";
	cout << "\tWithdraw.\n";
	cout << "==============================\n";
	vector<sClient> vData = LoadDataFromFile(Filename);
	string Accountnumber = ReadAccountNumber();
	double Amount = 0;
	cout << "Enter howmuch do you want to withdraw :";
	cin >> Amount;

	for (sClient &c : vData)
	{
		if (Accountnumber == c.accountnumber)
		{
			while (Amount > c.accountbalance)
			{
				cout << "The amount is not enough, you can withdraw " << c.accountbalance << " :";
				cin >> Amount;
			}
		}
	}
	DepositClient(vData, Accountnumber, Amount * -1);
}
void ShowDepositClient()
{
	cout << "===============================\n";
	cout << "\tDeposit.\n";
	cout << "===============================\n";
	vector<sClient> vData = LoadDataFromFile(Filename);
	string Accountnumber = ReadAccountNumber();
	double Amount = 0;
	cout << "Enter howmuch do you want to deposit :";
	cin >> Amount;

	DepositClient(vData, Accountnumber, Amount);
}
void GoBackToTransScreen()
{
	cout << "\n\nPress any kye to back :" << endl;
	system("pause>0");
	ShowTransactionsScreen();
}
void ShowTotalBalance()
{
	vector<sClient> vData = LoadDataFromFile(Filename);
	double SumTotalBalance = 0;
	cout << "\t\t\tClient List (" << vData.size() << ")";
	cout << "\n\n-------------------------------------------------------------------\n";
	cout << left;
	cout << setw(15) << "accountnumber"
		 << "|";
	cout << setw(20) << "name"
		 << "|";
	cout << setw(15) << "accountbalance" << endl;
	cout << "-------------------------------------------------------------------\n";
	for (sClient &info : vData)
	{
		cout << left;
		cout << setw(15) << info.accountnumber << "|";
		cout << setw(20) << info.name << "|";
		cout << setw(15) << info.accountbalance << endl;
		SumTotalBalance += info.accountbalance;
	}
	cout << "-------------------------------------------------------------------\n";

	cout << "\t\tTotal balance (" << SumTotalBalance << ")\n";
}
void TransactionsList(enTransList Trans)
{
	switch (Trans)
	{
	case enTransList::Deposit:
		system("cls");
		ShowDepositClient();
		GoBackToTransScreen();
		break;
	case enTransList::Withdraw:
		system("cls");
		ShowWithdrawClient();
		GoBackToTransScreen();
		break;
	case enTransList::TotalBalance:
		system("cls");
		ShowTotalBalance();
		GoBackToTransScreen();
		break;
	case enTransList::ToInfoLest:
		ShowMainScreen();
		break;
	}
}
void ShowTransactionsScreen()
{
	system("cls");
	cout << "================================\n";
	cout << "\tTransactions lest\n";
	cout << "================================\n";
	cout << "For deposit enter           [1]:\n";
	cout << "For withdraw enter          [2]:\n";
	cout << "For total balance enter     [3]:\n";
	cout << "For exit to info lest enter [4]:\n";
	cout << "================================\n";
	TransactionsList((enTransList)ReadUserChooce());
}
void LestScrean(enLest UserChooce)
{
	switch (UserChooce)
	{
	case enLest::ShowClents:
		system("cls");
		ShowScreenClientAndPrintClient();
		GoBackToLestScrean();
		break;

	case enLest::FoundClient:
		system("cls");
		ShowFoundClient();
		GoBackToLestScrean();
		break;

	case enLest::DeletaClient:
		system("cls");
		ShowDeleteClient();
		GoBackToLestScrean();
		break;

	case enLest::UpdetaClint:
		system("cls");
		ShowUpdateClient();
		GoBackToLestScrean();
		break;

	case enLest::AddClient:
		system("cls");
		ShowAddNewClient();
		GoBackToLestScrean();
		break;

	case enLest::Transactions:
		system("cls");
		ShowTransactionsScreen();
		break;

	case enLest::Exit:
		system("cls");
		ShowEndScreen();
	}
}
void ShowMainScreen()
{
	system("cls");
	cout << "=============================\n";
	cout << "\tInfo lest\n";
	cout << "=============================\n";
	cout << "For show clients enter   [1]:\n";
	cout << "For found client enter   [2]:\n";
	cout << "For deleta client enter  [3]:\n";
	cout << "For updeta clint enter   [4]:\n";
	cout << "For add new client enter [5]:\n";
	cout << "For transactions enter   [6]:\n";
	cout << "Exit enter               [7]:\n";
	cout << "============================\n";

	LestScrean((enLest)ReadUserChooce());
}
int main()
{
	ShowMainScreen();

	return 0;
}