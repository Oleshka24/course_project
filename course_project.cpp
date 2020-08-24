#include <windows.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <string>
#include <regex>
#include <vector>

#define ORDERS "orders.dat"
#define SERVICES "services.dat"
#define STAFF "staff.dat"
#define POSITIONS "positions.dat"


#define PHONE_NUMB_LENGHT 9
#define DATE_LENGHT 3
#define ORDER_SERV_LENGHT 5
#define EMPL_POS_LENGHT 3

using namespace std;

struct order {
	size_t	ID;
	char	fullName[20];
	size_t	phoneNumb[PHONE_NUMB_LENGHT];
	time_t	dateOfOrder[DATE_LENGHT];
	size_t	servNumbers[ORDER_SERV_LENGHT];
};

struct service {
	size_t	ID;
	size_t	employee;
	size_t	category;
	char	title[40];
	int		price;
	size_t	term;
};

struct employee {
	size_t	ID;
	char	fullName[20];
	size_t	phoneNumb[PHONE_NUMB_LENGHT];
	size_t	position[EMPL_POS_LENGHT];
};

struct position {
	size_t ID;
	char occupation[20];
};

struct table {
	size_t layoutLenght;
	string title;
};

FILE* dataFile;

FILE* ordFile;
FILE* staffFile;
FILE* servFile;
FILE* posFile;

string createPatternForMultipleNumbers(size_t, size_t, size_t, bool);
string inputCurrentVal(string*, string*);

order orderInputLayout(size_t);
service servInputLayout(size_t);
employee emplInputLayout(size_t);
position posInputLayout(size_t);

void outputTabSymb(size_t);
void outputTabLine(size_t, table*, size_t);
void outputTabHeaderRow(table*, size_t);

void navigation(size_t);
void input(size_t);
void output(size_t, size_t, size_t);

void editOrRemove(size_t, size_t);
void sorting(size_t);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	fopen_s(&dataFile, ORDERS, "a+");
	fclose(dataFile);
	fopen_s(&dataFile, SERVICES, "a+");
	fclose(dataFile);
	fopen_s(&dataFile, STAFF, "a+");
	fclose(dataFile);
	fopen_s(&dataFile, POSITIONS, "a+");
	fclose(dataFile);

	int navPos;
	do {
		cout << "\n\tМеню программы."
			<< "\n\t1 - Работа с заказами."
			<< "\n\t2 - Работа с услугами."
			<< "\n\t3 - Работа с сотрудниками."
			<< "\n\t4 - Работа с квалификациями."
			<< "\n\t0 - Закончить программу.";

		string titleVal = "\n\tВыберите пункт меню: ";
		string checkVal = createPatternForMultipleNumbers(1, 0, 4, true);

		navPos = stoi(inputCurrentVal(&checkVal, &titleVal));

		if (navPos) navigation(navPos);
		else return 0;

	} while (true);

	return 0;
}

void navigation(size_t dataType) {
	int navPos;
	do {
		cout << "\n\tМеню программы (";

		switch (dataType) {
			case 1: cout << "ЗАКАЗЫ"; break;
			case 2: cout << "УСЛУГИ"; break;
			case 3: cout << "СОТРУДНИКИ"; break;
			case 4: cout << "КВАЛИФИКАЦИИ"; break;
		}

		cout << ")."
			<< "\n\t1 - Новая запись."
			<< "\n\t2 - Вывести все записи."
			<< "\n\t3 - Редактировать запись."
			<< "\n\t4 - Удалить запись."
			<< "\n\t5 - Сортировать записи."
			<< "\n\t6 - Найти запись."
			<< "\n\t0 - Вернуться в главное меню.";

		string titleVal = "\n\tВыберите пункт меню: ";
		string checkVal = createPatternForMultipleNumbers(1, 0, 6, true);

		navPos = stoi(inputCurrentVal(&checkVal, &titleVal));

		switch (navPos) {
		case 0:
			return;

		case 1:
			input(dataType);
			break;

		case 2:
			break;

		case 3:
			editOrRemove(dataType, navPos);
			break;

		case 4:
			editOrRemove(dataType, navPos);
			break;

		case 5:
			sorting(dataType);
			break;
		}
		output(dataType, 0, dataType);
	} while (true);
	return;
}

string createPatternForMultipleNumbers(size_t lenght, size_t minNumber, size_t maxNumber, bool strictAdherence) {
	string str;
	str.clear();
	for (size_t i = 0; i < lenght; i++) {
		for (size_t j = i; j < lenght; j++) {
			str.append("(");
			if (maxNumber >= 10) {
				for (size_t n = maxNumber / 10; n > 0; n--)
					n != maxNumber / 10
					? str.append(to_string(n) + "[0-9]|")
					: str.append(to_string(n) + "[0-" + to_string(maxNumber % 10) + "]|");
				str.append("[" + to_string(minNumber) + "-9]),");
			}
			else str.append("[" + to_string(minNumber) + "-" + to_string(maxNumber % 10) + "]),");
		}
		if (!strictAdherence) {
			str.erase(str.end() - 1);
			str.append("|");
		}
		else break;
	}
	str.erase(str.end() - 1);
	return str;
}

string inputCurrentVal(string* checkVal, string* titleVal) {
	string str;
	cmatch result;
	regex valueCheck(*checkVal);

	cin.ignore(cin.rdbuf()->in_avail());
	do {
		cout << *titleVal;
		getline(cin, str);
	} while (!regex_match(str.c_str(), result, valueCheck));

	return str;
}

order orderInputLayout(size_t ID) {
	order ord;
	service serv;

	string str;
	string titleVal;
	string checkVal;

	size_t itemsLenght = 0;

	// ID
	ord.ID = ID;

	// Full Name
	titleVal = "\tФИО заказчика по шаблону \"Иванов И.И.\": ";
	checkVal = "([А-Я]{1})([а-я]+?) ([А-Я]{1})\\.([А-Я]{1})\\.";
	strcpy_s(ord.fullName, inputCurrentVal(&checkVal, &titleVal).c_str());

	// Phone Number
	titleVal = "\tНомер телефона заказчика: 380";
	checkVal = "([0-9]{" + to_string(PHONE_NUMB_LENGHT) + "})";
	str = inputCurrentVal(&checkVal, &titleVal);

	for (size_t i = 0; i < str.size(); i++)
		ord.phoneNumb[i] = stoi(str.substr(i, 1));

	// Date of ord
	titleVal = "\tВведите дату заказа по шаблону (дд/мм/гггг): ";
	checkVal = "([0-2]{1}[1-9]{1}|3[0-1]{1})/(0[1-9]{1}|1[0-2]{1})/20([0-2]{1}[0-9]{1})";
	str = inputCurrentVal(&checkVal, &titleVal);

	ord.dateOfOrder[0] = stoi(str.substr(0, 2));
	ord.dateOfOrder[1] = stoi(str.substr(3, 2));
	ord.dateOfOrder[2] = stoi(str.substr(6, 4));

	// Services Numbers
	vector <size_t> servIDs;
	output(2, 0, 2);

	fopen_s(&servFile, SERVICES, "rb");
	while (fread(&serv, sizeof(service), 1, servFile)) {
		itemsLenght++;
		servIDs.push_back(serv.ID);
	}
	fclose(servFile);

	titleVal = "\n\n\tПеречислите через запятую номера услуг (не более " + to_string(ORDER_SERV_LENGHT) + "): ";
	checkVal = createPatternForMultipleNumbers(ORDER_SERV_LENGHT, 1, itemsLenght, false);
	str = inputCurrentVal(&checkVal, &titleVal);
	if (true) {
		size_t j = 0;
		for (size_t i = 0, n = 0; i <= str.size(); i++)
			if (str.substr(i, 1) == "," || i == str.size()) {
				ord.servNumbers[j] = servIDs[stoi(str.substr(i - n, n)) - 1];
				n = 0;
				j++;
			}
			else n++;

		for (j; j < ORDER_SERV_LENGHT; j++) ord.servNumbers[j] = 0;
	}

	return ord;
}

service servInputLayout(size_t ID) {
	service serv;
	position pos;
	employee empl;

	string str;
	string titleVal;
	string checkVal;


	// ID
	serv.ID = ID;

	// Category
	vector <size_t> posIDs;
	output(4, 0, 4);

	fopen_s(&posFile, POSITIONS, "rb");
	size_t posLenght = 0;
	while (fread(&pos, sizeof(position), 1, posFile)) {
		posLenght++;
		posIDs.push_back(pos.ID);
	}
	fclose(posFile);

	titleVal = "\n\tВыберите категорию: ";
	checkVal = createPatternForMultipleNumbers(1, 1, posLenght, true);;
	serv.category = posIDs[stoi(inputCurrentVal(&checkVal, &titleVal)) - 1];

	// Title
	titleVal = "\tНазвание услуги: ";
	checkVal = "(.+?)";
	strcpy_s(serv.title, inputCurrentVal(&checkVal, &titleVal).c_str());

	// Employee
	vector <size_t> staffIDs;
	output(3, 0, 3);

	checkVal.clear();
	fopen_s(&staffFile, STAFF, "rb");
	size_t staffLenght = 0;
	while (fread(&empl, sizeof(employee), 1, staffFile)) {
		for (size_t i = 0; i < staffLenght; i++)
			if (empl.position[i] == serv.category) {
				checkVal.append("|(" + to_string(empl.ID) + ")");
			}
		staffLenght++;
		staffIDs.push_back(empl.ID);
	}
	fclose(staffFile);
	checkVal.erase(checkVal.begin());

	titleVal = "\n\tВыберите специалиста, который имеет выбранную квалификацию: ";
	serv.employee = staffIDs[stoi(inputCurrentVal(&checkVal, &titleVal)) - 1];

	// Price
	titleVal = "\tСтоимость услуги (в $): ";
	checkVal = "([0-9]+?)";
	serv.price = stoi(inputCurrentVal(&checkVal, &titleVal));

	// Term
	titleVal = "\tСрок, отведённый на выполнение задачи (в днях): ";
	checkVal = "([0-9]+?)";
	serv.term = stoi(inputCurrentVal(&checkVal, &titleVal));

	return serv;
}

employee emplInputLayout(size_t ID) {
	employee empl;
	position pos;

	string str;
	string titleVal;
	string checkVal;

	// ID
	empl.ID = ID;

	// Full Name
	titleVal = "\tФИО сотрудника по шаблону \"Иванов И.И.\": ";
	checkVal = "([А-Я]{1})([а-я]+?) ([А-Я]{1})\\.([А-Я]{1})\\.";
	strcpy_s(empl.fullName, inputCurrentVal(&checkVal, &titleVal).c_str());

	// Phone Number
	titleVal = "\tНомер телефона сотрудника: 380";
	checkVal = "([0-9]{" + to_string(PHONE_NUMB_LENGHT) + "})";
	str = inputCurrentVal(&checkVal, &titleVal);

	for (size_t i = 0; i < str.size(); i++)
		empl.phoneNumb[i] = stoi(str.substr(i, 1));

	// Positions
	vector <size_t> posIDs;
	output(4, 0, 4);

	fopen_s(&posFile, POSITIONS, "rb");
	size_t posLenght = 0;
	while (fread(&pos, sizeof(position), 1, posFile)) {
		posLenght++;
		posIDs.push_back(pos.ID);
	}
	fclose(posFile);

	titleVal = "\n\tПеречислите через запятую номера квалификаций: ";
	checkVal = createPatternForMultipleNumbers(EMPL_POS_LENGHT, 1, posLenght, false);
	str = inputCurrentVal(&checkVal, &titleVal);

	for (size_t i = 0; i < EMPL_POS_LENGHT; i++) {
		empl.position[i] = 0;
	}

	for (size_t i = 0, j = 0, n = 0; i <= str.size(); i++)
		if (str.substr(i, 1) == "," || i == str.size()) {
			empl.position[j] = posIDs[stoi(str.substr(i - n, n)) - 1];
			n = 0;
			j++;
		}
		else n++;

	return empl;
}

position posInputLayout(size_t ID) {
	position pos;

	// ID
	pos.ID = ID;

	// Title
	string str;
	string titleVal = "\tНазвание квалификации: ";
	string checkVal = "(.+?)";
	strcpy_s(pos.occupation, inputCurrentVal(&checkVal, &titleVal).c_str());

	return pos;
}

void input(size_t dataType) {
	order ord;
	service serv;
	employee empl;
	position pos;

	size_t entryNumb = 1;
	string fileName;

	switch (dataType) {
		case 1: fileName = ORDERS; break;
		case 2: fileName = SERVICES; break;
		case 3: fileName = STAFF; break;
		case 4: fileName = POSITIONS; break;
	}

	fopen_s(&dataFile, fileName.c_str(), "rb");
	switch (dataType) {
		case 1: while (fread(&ord, sizeof(order), 1, dataFile)) entryNumb++; break;
		case 2: while (fread(&serv, sizeof(service), 1, dataFile)) entryNumb++; break;
		case 3: while (fread(&empl, sizeof(employee), 1, dataFile)) entryNumb++; break;
		case 4: while (fread(&pos, sizeof(position), 1, dataFile)) entryNumb++; break;
		default: return;
	}
	fclose(dataFile);

	do {
		string titleVal;
		string checkVal;

		size_t itemsLenght = 0;

		cout << "\n\tВведите данные " << entryNumb 
			<< (entryNumb % 10 == 3 && entryNumb % 100 != 13 ? "-ей" : "-ой") 
			<< " записи:\n";


		switch (dataType) {
		case 1:
			fopen_s(&ordFile, ORDERS, "a+b");
				ord = orderInputLayout(entryNumb++);
				fwrite(&ord, sizeof(order), 1, ordFile);
			fclose(ordFile);
			break;
		case 2:
			fopen_s(&servFile, SERVICES, "a+b");
				serv = servInputLayout(entryNumb++);
				fwrite(&serv, sizeof(service), 1, servFile);
			fclose(servFile);
			break;
		case 3:
			fopen_s(&staffFile, STAFF, "a+b");
				empl = emplInputLayout(entryNumb++);
				fwrite(&empl, sizeof(employee), 1, staffFile);
			fclose(staffFile);
			break;
		case 4:
			fopen_s(&posFile, POSITIONS, "a+b");
				pos = posInputLayout(entryNumb++);
				fwrite(&pos, sizeof(position), 1, posFile);
			fclose(posFile);
			break;
		default: return;
		}

		titleVal = "\n\tДостаточно? (Д/Н), ваш вариант: ";
		checkVal = "Д|д|Н|н";
		char inputRepeat = inputCurrentVal(&checkVal, &titleVal)[0];
		if (inputRepeat == 'Д' || inputRepeat == 'д') break;

	} while (true);
}

void output(size_t dataType, size_t outputType, size_t navPos) {
	order ord;
	service serv;
	employee empl;
	position pos;

	bool fileIsEmpty;

	switch (dataType) {
	case 1:
		fopen_s(&ordFile, ORDERS, "rb"); 
		fread(&ord, sizeof(order), 1, ordFile)
			? fileIsEmpty = false
			: fileIsEmpty = true;
		fclose(ordFile);
		break;
	case 2:
		fopen_s(&servFile, SERVICES, "rb");
		fread(&serv, sizeof(service), 1, servFile)
			? fileIsEmpty = false
			: fileIsEmpty = true;
		fclose(servFile);
		break;
	case 3:
		fopen_s(&staffFile, STAFF, "rb"); 
		fread(&empl, sizeof(employee), 1, staffFile)
			? fileIsEmpty = false
			: fileIsEmpty = true;
		fclose(staffFile);
		break;
	case 4:
		fopen_s(&posFile, POSITIONS, "rb"); 
		fread(&pos, sizeof(position), 1, posFile)
			? fileIsEmpty = false
			: fileIsEmpty = true;
		fclose(posFile);
		break;
	default: return;
	}

	if (!fileIsEmpty) {
		string str;

		switch (dataType) {
		case 1: {
			//	+--------------------------------------------------------------------------------------------------------+
			//	|                                       ИНФОРМАЦИЯ О ВСЕХ ЗАКАЗАХ                                        |
			//	+---+-------------+---------------+------------+--------------------------------+-----------+------------+
			//	|   |     ФИО     |     Номер     |    Дата    |                                |           |    Дата    |
			//	| № |  заказчика  |    телефона   |   заказа   |             Услуги             | Стоимость | исполнения |
			//	+---+-------------+---------------+------------+--------------------------------+-----------+------------+
			//	|   |             |               |            | Петров П.П. - Эксклюзивный...  |           |            |
			//	|   |             |               |            | Сидоров С.С. - Framework       |           |            |
			//	| 1 | Иванов И.И. | +380987654321 | 31/12/2020 | Васечкин В.В. - Внешнее SEO... |     1500$ | 09/02/2021 |
			//	+---+-------------+---------------+------------+--------------------------------+-----------+------------+
			table title[1] = {
				{25, "ИНФОРМАЦИЯ О ВСЕХ ЗАКАЗАХ"}
			};

			const size_t positionsKol = 7;

			table tabItem[positionsKol] = {
				{ 1, "№"},
				{ 9, "ФИО\nзаказчика"},
				{12, "Номер\nтелефона"},
				{10, "Дата\nзаказа"},
				{30, "Услуги"},
				{ 9, "Стоимость"},
				{10, "Дата\nисполнения"},
			};

			fopen_s(&ordFile, ORDERS, "rb");
			size_t minServWeight = tabItem[4].layoutLenght;
			while (fread(&ord, sizeof(order), 1, ordFile)) {
				str = to_string(ord.ID);
				if (str.size() > tabItem[0].layoutLenght) tabItem[0].layoutLenght = str.size();

				str = ord.fullName;
				if (str.size() > tabItem[1].layoutLenght) tabItem[1].layoutLenght = str.size();

				if (tabItem[4].layoutLenght % 2) tabItem[4].layoutLenght++;
			}
			fclose(ordFile);

			size_t positionsSize = positionsKol * 3 + 1;
			for (size_t i = 0; i < positionsKol; i++)
				positionsSize += tabItem[i].layoutLenght;

			while (title->layoutLenght > positionsSize) {
				positionsSize = 1;
				for (size_t i = 0; i < positionsKol; i++) {
					if (tabItem[i].title != "№") {
						tabItem[i].layoutLenght += 2;
					}
					positionsSize += tabItem[i].layoutLenght + 3;
				}
			}
			title->layoutLenght = positionsSize - 4;

			outputTabLine(positionsSize, tabItem, 5);

			outputTabHeaderRow(title, 1);

			outputTabLine(positionsSize, tabItem, 3);

			outputTabHeaderRow(tabItem, positionsKol);

			size_t index = 0;
			fopen_s(&ordFile, ORDERS, "rb");
			while (fread(&ord, sizeof(order), 1, ordFile)) {

				bool flag = false;
				if (outputType) {
					switch (navPos) {
					case 2:
						for (size_t j = 0; j < ORDER_SERV_LENGHT; j++) {
							if (outputType == ord.servNumbers[j]) {
								flag = true;
								break;
							}
						}
						break;
					default:
						flag = true;
					}
				}

				if (!outputType || flag) {
					index++;

					outputTabLine(positionsSize, tabItem, 1);

					size_t servLenght = 0;
					size_t orderTerm = 0;
					int orderPrice = 0;
					for (size_t i = 0; i < ORDER_SERV_LENGHT; i++) {
						if (ord.servNumbers[i]) {
							servLenght++;
						}
					}

					for (size_t i = 0; i < servLenght; i++) {
						cout << "\n\t";
						outputTabSymb(0);
						//	ID
						str = to_string(index);
						cout << " " << right << setw(tabItem[0].layoutLenght) << (i + 1 == servLenght ? str : " ") << " ";
						outputTabSymb(0);

						//	Full name
						str = ord.fullName;
						cout << " " << left << setw(tabItem[1].layoutLenght) << (i + 1 == servLenght ? str : " ") << " ";
						outputTabSymb(0);

						//	Phone number
						str = "380";
						for (size_t j = 0; j < PHONE_NUMB_LENGHT; j++) {
							str.append(to_string(ord.phoneNumb[j]));
						}
						cout << " " << left << setw(tabItem[2].layoutLenght) << (i + 1 == servLenght ? str : " ") << " ";
						outputTabSymb(0);

						//	Date
						str.clear();
						for (size_t j = 0; j < DATE_LENGHT; j++) {
							to_string(ord.dateOfOrder[j]).size() % 10 == 2 || to_string(ord.dateOfOrder[j]).size() % 10 == 4
								? str.append(to_string(ord.dateOfOrder[j]) + "/")
								: str.append("0" + to_string(ord.dateOfOrder[j]) + "/");
						}
						str.erase(str.end() - 1);
						cout << " " << left << setw(tabItem[3].layoutLenght) << (i + 1 == servLenght ? str : " ") << " ";
						outputTabSymb(0);

						//	Services
						str.clear();

						fopen_s(&servFile, SERVICES, "rb");
						while (fread(&serv, sizeof(service), 1, servFile)) {
							if (serv.ID == ord.servNumbers[i]) {
								str.append(serv.title, 0, tabItem[4].layoutLenght - str.size() - 3);
								orderPrice += serv.price;
								orderTerm += serv.term;
								break;
							}
						}
						bool flag = false;
						while (str.at(str.size() - 1) == ' ') {
							str.erase(str.end() - 1);
							flag = true;
						}
						if (str.size() >= tabItem[4].layoutLenght - 3 || flag) str.append("...");

						fclose(servFile);

						cout << " " << left << setw(tabItem[4].layoutLenght) << str << " ";
						outputTabSymb(0);

						//	Price
						str.clear();
						str.append(to_string(orderPrice));
						str.append("$");

						cout << " " << right << setw(tabItem[5].layoutLenght) << (i + 1 == servLenght ? str : " ") << " ";
						outputTabSymb(0);

						//	Date

						time_t ts_data;
						struct tm t_data;
						ts_data = time(NULL);
						localtime_s(&t_data, &ts_data);

						t_data.tm_mday = (int)ord.dateOfOrder[0];
						t_data.tm_mon = (int)ord.dateOfOrder[1] - 1;
						t_data.tm_year = (int)ord.dateOfOrder[2] - 1900;
						ts_data = mktime(&t_data);

						ts_data += (time_t)orderTerm * 86400; // 1 day = 86 400 second
						localtime_s(&t_data, &ts_data);

						str.clear();
						to_string(t_data.tm_mday).size() % 10 == 2
							? str.append(to_string(t_data.tm_mday) + "/")
							: str.append("0" + to_string(t_data.tm_mday) + "/");

						to_string(t_data.tm_mon + 1).size() % 10 == 2
							? str.append(to_string(t_data.tm_mon + 1) + "/")
							: str.append("0" + to_string(t_data.tm_mon + 1) + "/");

						str.append(to_string(t_data.tm_year + 1900));

						cout << " " << right << setw(tabItem[6].layoutLenght) << (i + 1 == servLenght ? str : " ") << " ";
						outputTabSymb(0);
					}
				}
			}
			fclose(ordFile);

			outputTabLine(positionsSize, tabItem, 2);
			cout << "\n\t";
			break;
		}
		case 2: {
			//	+---------------------------------------------------------------------------------------------+
			//	|                                  ИНФОРМАЦИЯ О ВСЕХ УСЛУГАХ                                  |
			//	+---+---------------+------------------+-------------------------------+-----------+----------+
			//	|   |      ФИО      |                  |                               |           |   Срок   |
			//	| № |  сотрудника   |    Категория     |         Подкатегория          | Стоимость | (в днях) |
			//	+---+---------------+------------------+-------------------------------+-----------+----------+
			//	| 4 | Васечкин В.В. | Дизайн           | Эксклюзивный (с WOW-эффектом) |      400$ |        4 |
			//	+---+---------------+------------------+-------------------------------+-----------+----------+
			//	| 5 | Петров П.П.   | Программирование | WordPress                     |      500$ |        5 |
			//	+---+---------------+------------------+-------------------------------+-----------+----------+
			table title[1] = {
				{25, "ИНФОРМАЦИЯ О ВСЕХ УСЛУГАХ"}
			};

			const size_t positionsKol = 6;

			table tabItem[positionsKol] = {
				{ 1, "№"},
				{10, "ФИО\nсотрудника"},
				{ 9, "Категория"},
				{12, "Подкатегория"},
				{ 9, "Стоимость"},
				{ 8, "Срок\n(в днях)"},
			};

			fopen_s(&servFile, SERVICES, "rb");
			while (fread(&serv, sizeof(service), 1, servFile)) {
				str = to_string(serv.ID);
				if (str.size() > tabItem[0].layoutLenght) tabItem[0].layoutLenght = str.size();

				fopen_s(&staffFile, STAFF, "rb");
				while (fread(&empl, sizeof(employee), 1, staffFile)) {
					str = empl.fullName;
					if (str.size() > tabItem[1].layoutLenght) tabItem[1].layoutLenght = str.size();
					if (tabItem[1].layoutLenght % 2) tabItem[1].layoutLenght++;
				}
				fclose(staffFile);

				fopen_s(&posFile, POSITIONS, "rb");
				while (fread(&pos, sizeof(position), 1, posFile)) {
					if (serv.category == pos.ID) {
						str = pos.occupation;
						if (str.size() > tabItem[2].layoutLenght)
							tabItem[2].layoutLenght = str.size();
					}
				}
				fclose(posFile);
				if (tabItem[2].layoutLenght % 2) tabItem[2].layoutLenght++;

				str = serv.title;
				if (str.size() > tabItem[3].layoutLenght) tabItem[3].layoutLenght = str.size();
				if (tabItem[3].layoutLenght % 2) tabItem[3].layoutLenght++;
			}
			fclose(servFile);

			size_t positionsSize = positionsKol * 3 + 1;
			for (size_t i = 0; i < positionsKol; i++)
				positionsSize += tabItem[i].layoutLenght;

			while (title->layoutLenght > positionsSize) {
				positionsSize = 1;
				for (size_t i = 0; i < positionsKol; i++) {
					if (tabItem[i].title != "№") {
						tabItem[i].layoutLenght += 2;
					}
					positionsSize += tabItem[i].layoutLenght + 3;
				}
			}
			title->layoutLenght = positionsSize - 4;

			outputTabLine(positionsSize, tabItem, 5);

			outputTabHeaderRow(title, 1);

			outputTabLine(positionsSize, tabItem, 3);

			outputTabHeaderRow(tabItem, positionsKol);

			size_t index = 0;
			fopen_s(&servFile, SERVICES, "rb");
			while (fread(&serv, sizeof(service), 1, servFile)) {
				bool flag = false;
				if (outputType) {
					switch (navPos) {
					case 3:
						flag = serv.employee == outputType;
						break;
					case 4:
						flag = serv.category == outputType;
						break;
					default:
						flag = true;
					}
				}

				if (!outputType || flag) {
					index++;

					outputTabLine(positionsSize, tabItem, 1);

					cout << "\n\t";
					outputTabSymb(0);
					//	ID
					str = to_string(index);
					cout << " " << right << setw(tabItem[0].layoutLenght) << str << " ";
					outputTabSymb(0);

					//	Full name
					fopen_s(&staffFile, STAFF, "rb");
					while (fread(&empl, sizeof(employee), 1, staffFile)) {
						if (empl.ID == serv.employee) {
							str = empl.fullName;
							break;
						}
					}
					fclose(staffFile);

					cout << " " << left << setw(tabItem[1].layoutLenght) << str << " ";
					outputTabSymb(0);

					//	Category
					fopen_s(&posFile, POSITIONS, "rb");
					while (fread(&pos, sizeof(position), 1, posFile)) {
						if (serv.category == pos.ID) {
							str = pos.occupation;
							break;
						}
					}
					fclose(posFile);

					cout << " " << left << setw(tabItem[2].layoutLenght) << str << " ";
					outputTabSymb(0);

					//	Title
					str = serv.title;
					cout << " " << left << setw(tabItem[3].layoutLenght) << str << " ";
					outputTabSymb(0);

					//	Price
					str = to_string(serv.price);
					str.append("$");
					cout << " " << right << setw(tabItem[4].layoutLenght) << str << " ";
					outputTabSymb(0);

					//	Term
					str = to_string(serv.term);
					cout << " " << right << setw(tabItem[5].layoutLenght) << str << " ";
					outputTabSymb(0);
				}
			}

			fclose(servFile);

			outputTabLine(positionsSize, tabItem, 2);
			cout << "\n\t";
			break;
		}
		case 3: {
			//	+----------------------------------------------------+
			//	|            ИНФОРМАЦИЯ О ВСЕХ СОТРУДНИКАХ           |
			//	+---+-------------+---------------+------------------+
			//	|   |      ФИО    |      Номер    |                  |
			//	| № |  сотрудника |    телефона   |   Квалификации   |
			//	+---+-------------+---------------+------------------+
			//	|   |             |               | Дизайн           |
			//	|   |             |               | Программирование |
			//	| 1 | Иванов И.И. | +380987654321 | SEO-оптимизация  |
			//	+---+-------------+---------------+------------------+
			table title[1] = {
				{29, "ИНФОРМАЦИЯ О ВСЕХ СОТРУДНИКАХ"}
			};

			const size_t positionsKol = 4;

			table tabItem[positionsKol] = {
				{ 1, "№"},
				{10, "ФИО\nсотрудника"},
				{12, "Номер\nтелефона"},
				{12, "Квалификации"},
			};

			fopen_s(&staffFile, STAFF, "rb");
			while (fread(&empl, sizeof(employee), 1, staffFile)) {
				str = to_string(empl.ID);
				if (str.size() > tabItem[0].layoutLenght) tabItem[0].layoutLenght = str.size();

				str = empl.fullName;
				if (str.size() > tabItem[1].layoutLenght) tabItem[1].layoutLenght = str.size();
				if (tabItem[1].layoutLenght % 2) tabItem[1].layoutLenght++;

				fopen_s(&posFile, POSITIONS, "rb");
				for (size_t i = 0; i < EMPL_POS_LENGHT; i++) {
					while (fread(&pos, sizeof(position), 1, posFile)) {
						if (empl.position[i] == pos.ID) {
							str = pos.occupation;
							if (str.size() > tabItem[3].layoutLenght)
								tabItem[3].layoutLenght = str.size();
						}
					}
				}
				fclose(posFile);

				if (tabItem[3].layoutLenght % 2) tabItem[3].layoutLenght++;
			}
			fclose(staffFile);

			size_t positionsSize = positionsKol * 3 + 1;
			for (size_t i = 0; i < positionsKol; i++)
				positionsSize += tabItem[i].layoutLenght;

			while (title->layoutLenght > positionsSize) {
				positionsSize = 1;
				for (size_t i = 0; i < positionsKol; i++) {
					if (tabItem[i].title != "№") {
						tabItem[i].layoutLenght += 2;
					}
					positionsSize += tabItem[i].layoutLenght + 3;
				}
			}
			title->layoutLenght = positionsSize - 4;

			outputTabLine(positionsSize, tabItem, 5);

			outputTabHeaderRow(title, 1);

			outputTabLine(positionsSize, tabItem, 3);

			outputTabHeaderRow(tabItem, positionsKol);

			size_t index = 0;
			fopen_s(&staffFile, STAFF, "rb");
			while (fread(&empl, sizeof(employee), 1, staffFile)) {
				bool flag = false;
				if (outputType) {
					switch (navPos) {
					case 4:
						for (size_t j = 0; j < EMPL_POS_LENGHT; j++) {
							if (outputType == empl.position[j]) {
								flag = true;
								break;
							}
						}
						break;
					default:
						flag = true;
					}
				}

				if (!outputType || flag) {
					index++;

					outputTabLine(positionsSize, tabItem, 1);

					size_t posLenght = 0;
					for (size_t i = 0; i < EMPL_POS_LENGHT; i++) {
						if (empl.position[i]) {
							posLenght++;
						}
					}

					for (size_t i = 0; i < posLenght; i++) {
						cout << "\n\t";
						outputTabSymb(0);
						//	ID
						str = to_string(index);
						cout << " " << right << setw(tabItem[0].layoutLenght) << (i + 1 == posLenght ? str : " ") << " ";
						outputTabSymb(0);

						//	Full name
						str = empl.fullName;
						cout << " " << left << setw(tabItem[1].layoutLenght) << (i + 1 == posLenght ? str : " ") << " ";
						outputTabSymb(0);

						//	Phone number
						str = "380";
						for (size_t j = 0; j < PHONE_NUMB_LENGHT; j++) {
							str.append(to_string(empl.phoneNumb[j]));
						}
						cout << " " << left << setw(tabItem[2].layoutLenght) << (i + 1 == posLenght ? str : " ") << " ";
						outputTabSymb(0);

						//	Positions
						fopen_s(&posFile, POSITIONS, "rb");
						while (fread(&pos, sizeof(position), 1, posFile)) {
							if (pos.ID == empl.position[i]) {
								str = pos.occupation;
								break;
							}
						}
						fclose(posFile);
						cout << " " << left << setw(tabItem[3].layoutLenght) << str << " ";
						outputTabSymb(0);
					}
				}
			}
			fclose(staffFile);

			outputTabLine(positionsSize, tabItem, 2);
			cout << "\n\t";
			break;
		}
		case 4: {
			//	+-------------------------------------+
			//	|   ИНФОРМАЦИЯ О ВСЕХ КВАЛИФИКАЦИЯХ   |
			//	+---+---------------------------------+
			//	| № |      Название квалификации      |
			//	+---+---------------------------------+
			//	| 1 | Дизайн                          |
			//	+---+---------------------------------+
			//	| 2 | Программирование                |
			//	+---+---------------------------------+
			//	| 3 | SEO-оптимизация                 |
			//	+---+---------------------------------+
			table title[1] = {
				{35, "ИНФОРМАЦИЯ О ВСЕХ КВАЛИФИКАЦИЯХ"}
			};

			const size_t positionsKol = 2;

			table tabItem[positionsKol] = {
				{ 1, "№"},
				{21, "Название квалификации"}
			};

			fopen_s(&posFile, POSITIONS, "rb");
			while (fread(&pos, sizeof(position), 1, posFile)) {
				str = to_string(pos.ID);
				if (str.size() > tabItem[0].layoutLenght) tabItem[0].layoutLenght = str.size();

				str = pos.occupation;
				if (str.size() > tabItem[1].layoutLenght) tabItem[1].layoutLenght = str.size();
				if (tabItem[1].layoutLenght % 2) tabItem[1].layoutLenght++;
			}
			fclose(posFile);
			size_t positionsSize = positionsKol * 3 + 1;
			for (size_t i = 0; i < positionsKol; i++)
				positionsSize += tabItem[i].layoutLenght;

			while (title->layoutLenght > positionsSize) {
				positionsSize = 1;
				for (size_t i = 0; i < positionsKol; i++) {
					if (tabItem[i].title != "№") {
						tabItem[i].layoutLenght += 2;
					}
					positionsSize += tabItem[i].layoutLenght + 3;
				}
			}
			title->layoutLenght = positionsSize - 4;

			outputTabLine(positionsSize, tabItem, 5);

			outputTabHeaderRow(title, 1);

			outputTabLine(positionsSize, tabItem, 3);

			outputTabHeaderRow(tabItem, positionsKol);

			size_t index = 0;
			fopen_s(&posFile, POSITIONS, "rb");
			while (fread(&pos, sizeof(position), 1, posFile)) {
				index++;

				outputTabLine(positionsSize, tabItem, 1);
				cout << "\n\t";
				outputTabSymb(0);
				//	ID
				str = to_string(index);
				cout << " " << right << setw(tabItem[0].layoutLenght) << str << " ";
				outputTabSymb(0);

				//	Occupation
				str = pos.occupation;
				cout << " " << left << setw(tabItem[1].layoutLenght) << str << " ";
				outputTabSymb(0);
			}

			fclose(posFile);

			outputTabLine(positionsSize, tabItem, 2);
			cout << "\n\t";
			break;
		}
		}
	}
	else cout << "\n\tФайл с данными пуст. Попробуйте заполнить его в пункте меню: \"1 - Новая запись.\"\n";
}

void outputTabLine(size_t lenght, table *positions, size_t linePos) {
	//	linePos:
	//	0 - top
	//	1 - center
	//	2 - bottom
	//	3 - header -> center
	//	4 - center -> footer
	//	5 - header
	//	6 - footer

	cout << "\n\t";

	for (size_t i = 0, j = 0, pos = positions[0].layoutLenght; i < lenght; i++)
		if (i == 0) {
			switch (linePos) {
				case 0:
				case 5:
					outputTabSymb(3); break;
				case 1: 
				case 3: 
				case 4: 
					outputTabSymb(7); break;
				case 2:
				case 6:
					outputTabSymb(4); break;
			}
		}
		else if (i == lenght - 1) {
			switch (linePos) {
				case 0:
				case 5:
					outputTabSymb(5); break;
				case 1: 
				case 3: 
				case 4: 
					outputTabSymb(8); break;
				case 2:
				case 6:
					outputTabSymb(6); break;
			}
		}
		else if (pos + 3 == i) {
			switch (linePos) {
				case 0: 
				case 3:	
					outputTabSymb(9); break;
				case 1: 
					outputTabSymb(2); break;
				case 2:
				case 4:
					outputTabSymb(10); break;
				case 5:
				case 6:
					outputTabSymb(1); break;
			}
			pos += positions[++j].layoutLenght + 3;
		}
		else outputTabSymb(1);
}

void outputTabHeaderRow(table* item, size_t kol) {
	string	str;
	size_t	maxRowsKol = 1;
	size_t* rowsKol = new size_t[kol];

	for (size_t i = 0; i < kol; i++) {
		str = item[i].title;
		rowsKol[i] = 1;
		while (str.find_first_of("\n") >= 0 && str.find_first_of("\n") <= str.size()) {
			rowsKol[i]++;
			str = str.substr(str.find_first_of("\n") + 1);
		}
		if (maxRowsKol < rowsKol[i]) maxRowsKol = rowsKol[i];
	}

	for (size_t i = maxRowsKol; i >= 1; i--) {
		cout << "\n\t";
		outputTabSymb(0);

		for (size_t j = 0; j < kol; j++) {
			str.clear();

			if (rowsKol[j] >= i) {
				str.append(item[j].title);
				for (size_t n = maxRowsKol - i; n > 0; n--) {
					str = str.substr(str.find_first_of("\n") + 1);
				}
				str = str.substr(0, str.find_first_of("\n"));
			}
			else {
				str.clear();
				for (size_t n = 0; n < item[j].layoutLenght; n++) {
					str.append(" ");
				}
			}
			
			if (str != "№") {
				cout << setw((item[j].layoutLenght - str.size() + (str.size() % 2 ? 1 : 0)) / 2) << "";
				cout << left << ' ' << str << ' ';
				cout << setw((item[j].layoutLenght - str.size()) / 2) << "";
			}
			else cout << right << " " << setw(item[j].layoutLenght) << str << " ";

			outputTabSymb(0);
		}
	}

	delete[] rowsKol;
}

void outputTabSymb(size_t symbNumb) {
	SetConsoleCP(437);
	SetConsoleOutputCP(437);

	switch (symbNumb) {
		case  0: cout << char(186); break; // ║
		case  1: cout << char(205); break; // ═
		case  2: cout << char(206); break; // ╬
		case  3: cout << char(201); break; // ╔
		case  4: cout << char(200); break; // ╚
		case  5: cout << char(187); break; // ╗
		case  6: cout << char(188); break; // ╝
		case  7: cout << char(204); break; // ╠
		case  8: cout << char(185); break; // ╣
		case  9: cout << char(203); break; // ╦
		case 10: cout << char(202); break; // ╩
	}

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}

void editOrRemove(size_t dataType, size_t navPos) {
	order ord;
	service serv;
	employee empl;
	position pos;

	size_t lenght = 0;
	size_t entryNumb;
	string fileName;

	switch (dataType) {
	case 1: fileName = ORDERS; break;
	case 2: fileName = SERVICES; break;
	case 3: fileName = STAFF; break;
	case 4: fileName = POSITIONS; break;
	}

	string titleVal;
	string checkVal;

	vector <size_t> itemsIDs;

	fopen_s(&dataFile, fileName.c_str(), "rb");
	switch (dataType) {
	case 1: 
		while (fread(&ord, sizeof(order), 1, dataFile)) {
			lenght++;
			itemsIDs.push_back(ord.ID);
		}
		break;
	case 2:
		while (fread(&serv, sizeof(service), 1, dataFile)) {
			lenght++;
			itemsIDs.push_back(serv.ID);
		}
		break;
	case 3:
		while (fread(&empl, sizeof(employee), 1, dataFile)) {
			lenght++;
			itemsIDs.push_back(empl.ID);
		}
		break;
	case 4:
		while (fread(&pos, sizeof(position), 1, dataFile)) {
			lenght++;
			itemsIDs.push_back(pos.ID);
		}
		break;
	default: return;
	}
	fclose(dataFile);

	if (lenght) {
		do {
			output(dataType, 0, dataType);

			switch (navPos) {
			case 3: titleVal = "\n\tВведите номер записи, которую вы собираетесь отредактировать (0 для отмены): "; break;
			case 4: titleVal = "\n\tВведите номер записи, которую вы собираетесь удалить (0 для отмены): "; break;
			default: titleVal = "\n\tВведите номер записи (0 для отмены): ";
			}
			checkVal = createPatternForMultipleNumbers(1, 0, lenght, true);
			entryNumb = itemsIDs[stoi(inputCurrentVal(&checkVal, &titleVal)) - 1];

			if (!entryNumb) break;

			bool flag[2] = { false, false };
			if (navPos == 4) {
				switch (dataType) {
				case 2:
					fopen_s(&ordFile, ORDERS, "rb");
					while (fread(&ord, sizeof(order), 1, ordFile)) {
						for (size_t i = 0; i < ORDER_SERV_LENGHT; i++) {
							if (entryNumb == ord.servNumbers[i]) {
								flag[0] = true;
								flag[1] = true;
								break;
							}
						}
						if (flag[0]) break;
					}
					fclose(ordFile);
					break;
				case 3:
					fopen_s(&servFile, SERVICES, "rb");
					while (fread(&serv, sizeof(service), 1, servFile)) {
						if (entryNumb == serv.employee) {
							flag[0] = true;
							flag[1] = true;
							break;
						}
					}
					fclose(servFile);
					break;
				case 4:
					fopen_s(&servFile, SERVICES, "rb");
					while (fread(&serv, sizeof(service), 1, servFile)) {
						if (entryNumb == serv.category) {
							flag[0] = true;
							break;
						}
					}
					fclose(servFile);
					fopen_s(&staffFile, STAFF, "rb");
					while (fread(&empl, sizeof(employee), 1, staffFile)) {
						for (size_t i = 0; i < EMPL_POS_LENGHT; i++) {
							if (entryNumb == empl.position[i]) {
								flag[1] = true;
								break;
							}
						}
					}
					fclose(staffFile);
					break;
				}
			}
			if (!flag[0] && !flag[1]) {

				FILE* pfiletemp;
				fopen_s(&pfiletemp, "temp.dat", "w+b");

				switch (dataType) {
				case 1:
					fopen_s(&dataFile, fileName.c_str(), "a+b");
					while (fread(&ord, sizeof(order), 1, dataFile)) {
						switch (navPos) {
						case 3:
							if (ord.ID == entryNumb) {
								ord = orderInputLayout(entryNumb);
							}
							fwrite(&ord, sizeof(order), 1, pfiletemp);
							break;
						case 4:
							if (ord.ID != entryNumb) {
								fwrite(&ord, sizeof(order), 1, pfiletemp);
							}
							break;
						}
					}
					fclose(dataFile);
					break;
				case 2:
					fopen_s(&dataFile, fileName.c_str(), "a+b");
					while (fread(&serv, sizeof(service), 1, dataFile)) {
						switch (navPos) {
						case 3:
							if (serv.ID == entryNumb) {
								serv = servInputLayout(entryNumb);
							}
							fwrite(&serv, sizeof(service), 1, pfiletemp);
							break;
						case 4:
							if (serv.ID != entryNumb) {
								if (serv.ID > entryNumb) serv.ID--;
								fwrite(&serv, sizeof(service), 1, pfiletemp);
							}
							break;
						}
					}
					fclose(dataFile);
					break;
				case 3:
					fopen_s(&dataFile, fileName.c_str(), "a+b");
					while (fread(&empl, sizeof(employee), 1, dataFile)) {
						switch (navPos) {
						case 3:
							if (empl.ID == entryNumb) {
								empl = emplInputLayout(entryNumb);
							}
							fwrite(&empl, sizeof(employee), 1, pfiletemp);
							break;
						case 4:
							if (empl.ID != entryNumb) {
								if (empl.ID > entryNumb) empl.ID--;
								fwrite(&empl, sizeof(employee), 1, pfiletemp);
							}
							break;
						}
					}
					fclose(dataFile);
					break;
				case 4:
					fopen_s(&dataFile, fileName.c_str(), "a+b");
					while (fread(&pos, sizeof(position), 1, dataFile)) {
						switch (navPos) {
						case 3:
							if (pos.ID == entryNumb) {
								pos = posInputLayout(entryNumb);
							}
							fwrite(&pos, sizeof(position), 1, pfiletemp);
							break;
						case 4:
							if (pos.ID != entryNumb) {
								if (pos.ID > entryNumb) pos.ID--;
								fwrite(&pos, sizeof(position), 1, pfiletemp);
							}
							break;
						}
					}
					fclose(dataFile);
					break;
				}

				fclose(pfiletemp);

				remove(fileName.c_str());
				rename("temp.dat", fileName.c_str());

				if (navPos == 4)
					switch (dataType) {
					case 2:
						fopen_s(&pfiletemp, "temp.dat", "w+b");

						fopen_s(&ordFile, ORDERS, "rb");
						while (fread(&ord, sizeof(order), 1, ordFile)) {
							for (size_t i = 0; i < ORDER_SERV_LENGHT; i++)
								if (ord.servNumbers[i] > entryNumb) {
									ord.servNumbers[i]--;
								}
							fwrite(&ord, sizeof(order), 1, pfiletemp);
						}
						fclose(ordFile);

						fclose(pfiletemp);

						remove(ORDERS);
						rename("temp.dat", ORDERS);
						break;
					case 3:
						fopen_s(&pfiletemp, "temp.dat", "w+b");

						fopen_s(&servFile, SERVICES, "rb");
						while (fread(&serv, sizeof(service), 1, servFile)) {
							if (serv.employee > entryNumb) {
								serv.employee--;
							}
							fwrite(&serv, sizeof(service), 1, pfiletemp);
						}
						fclose(servFile);

						fclose(pfiletemp);

						remove(SERVICES);
						rename("temp.dat", SERVICES);
						break;
					case 4:
						fopen_s(&pfiletemp, "temp.dat", "w+b");

						fopen_s(&staffFile, STAFF, "rb");
						while (fread(&empl, sizeof(employee), 1, staffFile)) {
							for (size_t i = 0; i < EMPL_POS_LENGHT; i++)
								if (empl.position[i] > entryNumb) {
									empl.position[i]--;
								}
							fwrite(&empl, sizeof(employee), 1, pfiletemp);
						}
						fclose(staffFile);

						fclose(pfiletemp);

						remove(STAFF);
						rename("temp.dat", STAFF);

						fopen_s(&pfiletemp, "temp.dat", "w+b");

						fopen_s(&servFile, SERVICES, "rb");
						while (fread(&serv, sizeof(service), 1, servFile)) {
							if (serv.category > entryNumb) {
								serv.category--;
							}
							fwrite(&serv, sizeof(service), 1, pfiletemp);
						}
						fclose(servFile);

						fclose(pfiletemp);

						remove(SERVICES);
						rename("temp.dat", SERVICES);
						break;
					default: return;
					}
			}
			else {
				switch (dataType) {
				case 2:
					cout << "\n\tУдалить данную запись невозможно, т.к. она используется в следующих заказах:";
					output(1, entryNumb, dataType);
					break;
				case 3:
					cout << "\n\tУдалить данную запись невозможно, т.к. она используется в следующих услугах:";
					output(2, entryNumb, dataType);
					break;
				case 4:
					if (flag[0]) {
						cout << "\n\tУдалить данную запись невозможно, т.к. она используется в следующих услугах:";
						output(2, entryNumb, dataType);
					}
					if (flag[1]) {
						cout << "\n\tУдалить данную запись невозможно, т.к. она используется у следующих сотрудников:";
						output(3, entryNumb, dataType);
					}
					break;
				default:
					cout << "\n\tУдалить данную запись невозможно.";
				}
				cout << "(чтобы удалить запись, избавитесь от всех её упоминаний)\n";
			}

			titleVal = "\n\tДостаточно? (Д/Н), ваш вариант: ";
			checkVal = "Д|д|Н|н";
			char inputRepeat = inputCurrentVal(&checkVal, &titleVal)[0];
			if (inputRepeat == 'Д' || inputRepeat == 'д') break;
		} while (true);
	}
}

void sorting(size_t dataType) {
	string titleVal;
	string checkVal;

	size_t sortNav;
	bool sortVariable;

	switch (dataType) {
	case 1: {
		order ord;

		cout << "\n\tВыберите значение, по которому будет производиться сортировка:"
			<< "\n\t1 - Номер заказа."
			<< "\n\t2 - ФИО заказчика."
			<< "\n\t3 - Дата заказа."
			<< "\n\t4 - Услуги."
			<< "\n\t5 - Стоимость."
			<< "\n\t6 - Дата исполнения.";

		titleVal = "\n\tВаш вариант: ";
		checkVal = createPatternForMultipleNumbers(1, 1, 6, true);
		sortNav = stoi(inputCurrentVal(&checkVal, &titleVal));

		cout << "\n\tВыберите как будет производиться сортировка:"
			<< "\n\t1 - По возрастанию."
			<< "\n\t2 - По убыванию.";

		titleVal = "\n\tВаш вариант: ";
		checkVal = createPatternForMultipleNumbers(1, 1, 2, true);
		sortVariable = (bool)(stoi(inputCurrentVal(&checkVal, &titleVal)) - 1);

		order temp;
		size_t n;
		bool flag;
		bool sortValCurrent;

		fopen_s(&ordFile, ORDERS, "rb+");
		do {
			flag = false;
			n = 1;

			fread(&ord, sizeof(order), 1, ordFile);
			while (fread(&temp, sizeof(order), 1, ordFile) == 1) {
				n++;

				switch (sortNav) {
				case 1: {
					sortVariable
						? sortValCurrent = ord.ID < temp.ID
						: sortValCurrent = ord.ID > temp.ID;
					break;
				}
				case 2: {
					sortVariable
						? sortValCurrent = strcmp(ord.fullName, temp.fullName) < 0
						: sortValCurrent = strcmp(ord.fullName, temp.fullName) > 0;
					break;
				}
				case 3: {
					struct tm t_data;

					time_t ord_s_data;
					ord_s_data = time(NULL);
					localtime_s(&t_data, &ord_s_data);
					t_data.tm_mday = (int)ord.dateOfOrder[0];
					t_data.tm_mon = (int)ord.dateOfOrder[1] - 1;
					t_data.tm_year = (int)ord.dateOfOrder[2] - 1900;
					ord_s_data = mktime(&t_data);

					time_t temp_s_data;
					temp_s_data = time(NULL);
					localtime_s(&t_data, &temp_s_data);
					t_data.tm_mday = (int)temp.dateOfOrder[0];
					t_data.tm_mon = (int)temp.dateOfOrder[1] - 1;
					t_data.tm_year = (int)temp.dateOfOrder[2] - 1900;
					temp_s_data = mktime(&t_data);

					sortVariable
						? sortValCurrent = ord_s_data < temp_s_data
						: sortValCurrent = ord_s_data > temp_s_data;
					break;
				}
				case 4: {
					size_t ordServLenght = 0;
					size_t tempServLenght = 0;
					for (size_t i = 0; i < ORDER_SERV_LENGHT; i++) {
						if (!ord.servNumbers[i] && !temp.servNumbers[i]) break;
						if (ord.servNumbers[i]) ordServLenght++;
						if (temp.servNumbers[i]) tempServLenght++;
					}

					sortVariable
						? sortValCurrent = ordServLenght < tempServLenght
						: sortValCurrent = ordServLenght > tempServLenght;
					break;
				}
				case 5: {
					service serv;
					size_t ordPrice = 0;
					size_t tempPrice = 0;
					for (size_t i = 0; i < ORDER_SERV_LENGHT; i++) {
						if (!ord.servNumbers[i] && !temp.servNumbers[i]) break;
						fopen_s(&servFile, SERVICES, "rb");
						while (fread(&serv, sizeof(service), 1, servFile)) {
							if (ord.servNumbers[i] == serv.ID) ordPrice += serv.price;
							if (temp.servNumbers[i] == serv.ID) tempPrice += serv.price;
						}
						fclose(servFile);
					}

					sortVariable
						? sortValCurrent = ordPrice < tempPrice
						: sortValCurrent = ordPrice > tempPrice;
					break;
				}
				case 6: {
					service serv;
					struct tm t_data;

					time_t ord_s_data;
					time_t temp_s_data;

					ord_s_data = time(NULL);
					temp_s_data = time(NULL);

					localtime_s(&t_data, &ord_s_data);
					t_data.tm_mday = (int)ord.dateOfOrder[0];
					t_data.tm_mon = (int)ord.dateOfOrder[1] - 1;
					t_data.tm_year = (int)ord.dateOfOrder[2] - 1900;
					ord_s_data = mktime(&t_data);

					localtime_s(&t_data, &temp_s_data);
					t_data.tm_mday = (int)temp.dateOfOrder[0];
					t_data.tm_mon = (int)temp.dateOfOrder[1] - 1;
					t_data.tm_year = (int)temp.dateOfOrder[2] - 1900;
					temp_s_data = mktime(&t_data);

					for (size_t i = 0; i < ORDER_SERV_LENGHT; i++) {
						if (!ord.servNumbers[i] && !temp.servNumbers[i]) break;

						fopen_s(&servFile, SERVICES, "rb");
						while (fread(&serv, sizeof(service), 1, servFile)) {
							if (ord.servNumbers[i] && ord.servNumbers[i] == serv.ID)
								ord_s_data += serv.term * 86400; // 1 day = 86 400 second

							if (temp.servNumbers[i] && temp.servNumbers[i] == serv.ID)
								temp_s_data += serv.term * 86400; // 1 day = 86 400 second
						}
						fclose(servFile);
					}

					sortVariable
						? sortValCurrent = ord_s_data < temp_s_data
						: sortValCurrent = ord_s_data > temp_s_data;
					break;
				}
				default: sortValCurrent = false;
				}

				if (sortValCurrent) {
					fseek(ordFile, (n - 2) * sizeof(order), SEEK_SET);
					fwrite(&temp, sizeof(order), 1, ordFile);
					fseek(ordFile, (n - 1) * sizeof(order), SEEK_SET);
					fwrite(&ord, sizeof(order), 1, ordFile);
					flag = true;
				}
				else ord = temp;
				fseek(ordFile, n * sizeof(order), SEEK_SET);
			}
			rewind(ordFile);
		} while (flag);

		fclose(ordFile);

		break;
	}
	case 2: {
		service serv;

		cout << "\n\tВыберите значение, по которому будет производиться сортировка:"
			<< "\n\t1 - Номер услуги."
			<< "\n\t2 - ФИО сотрудника."
			<< "\n\t3 - Категории."
			<< "\n\t4 - Стоимость."
			<< "\n\t5 - Срок.";

		titleVal = "\n\tВаш вариант: ";
		checkVal = createPatternForMultipleNumbers(1, 1, 5, true);
		sortNav = stoi(inputCurrentVal(&checkVal, &titleVal));

		cout << "\n\tВыберите как будет производиться сортировка:"
			<< "\n\t1 - По возрастанию."
			<< "\n\t2 - По убыванию.";

		titleVal = "\n\tВаш вариант: ";
		checkVal = createPatternForMultipleNumbers(1, 1, 2, true);
		sortVariable = (bool)(stoi(inputCurrentVal(&checkVal, &titleVal)) - 1);

		service temp;
		size_t n;
		bool flag;
		bool sortValCurrent;

		fopen_s(&servFile, SERVICES, "rb+");
		do {
			flag = false;
			n = 1;

			fread(&serv, sizeof(service), 1, servFile);
			while (fread(&temp, sizeof(service), 1, servFile) == 1) {
				n++;

				switch (sortNav) {
				case 1: {
					sortVariable
						? sortValCurrent = serv.ID < temp.ID
						: sortValCurrent = serv.ID > temp.ID;
					break;
				}
				case 2: {
					employee empl;

					string servEmplName;
					string tempEmplName;

					fopen_s(&staffFile, STAFF, "rb");
					while (fread(&empl, sizeof(employee), 1, staffFile)) {
						if (servEmplName.size() && tempEmplName.size()) break;
						if (serv.employee == empl.ID) servEmplName = empl.fullName;
						if (temp.employee == empl.ID) tempEmplName = empl.fullName;
					}
					fclose(staffFile);

					sortVariable
						? sortValCurrent = servEmplName < tempEmplName
						: sortValCurrent = servEmplName > tempEmplName;
					break;
				}
				case 3: {
					sortVariable
						? sortValCurrent = serv.category < temp.category
						: sortValCurrent = serv.category > temp.category;
					break;
				}
				case 4: {
					sortVariable
						? sortValCurrent = serv.price < temp.price
						: sortValCurrent = serv.price > temp.price;
					break;
				}
				case 5: {
					sortVariable
						? sortValCurrent = serv.term < temp.term
						: sortValCurrent = serv.term > temp.term;
					break;
				}
				default: sortValCurrent = false;
				}

				if (sortValCurrent) {
					fseek(servFile, (n - 2) * sizeof(service), SEEK_SET);
					fwrite(&temp, sizeof(service), 1, servFile);
					fseek(servFile, (n - 1) * sizeof(service), SEEK_SET);
					fwrite(&serv, sizeof(service), 1, servFile);
					flag = true;
				}
				else serv = temp;
				fseek(servFile, n * sizeof(service), SEEK_SET);
			}
			rewind(servFile);
		} while (flag);

		fclose(servFile);

		break;
	}
	case 3: {
		employee empl;

		cout << "\n\tВыберите значение, по которому будет производиться сортировка:"
			<< "\n\t1 - Номер сотрудника."
			<< "\n\t2 - ФИО сотрудника."
			<< "\n\t3 - Квалифицированность.";

		titleVal = "\n\tВаш вариант: ";
		checkVal = createPatternForMultipleNumbers(1, 1, 3, true);
		sortNav = stoi(inputCurrentVal(&checkVal, &titleVal));

		cout << "\n\tВыберите как будет производиться сортировка:"
			<< "\n\t1 - По возрастанию."
			<< "\n\t2 - По убыванию.";

		titleVal = "\n\tВаш вариант: ";
		checkVal = createPatternForMultipleNumbers(1, 1, 2, true);
		sortVariable = (bool)(stoi(inputCurrentVal(&checkVal, &titleVal)) - 1);

		employee temp;
		size_t n;
		bool flag;
		bool sortValCurrent;

		fopen_s(&staffFile, STAFF, "rb+");
		do {
			flag = false;
			n = 1;

			fread(&empl, sizeof(employee), 1, staffFile);
			while (fread(&temp, sizeof(employee), 1, staffFile) == 1) {
				n++;

				switch (sortNav) {
				case 1: {
					sortVariable
						? sortValCurrent = empl.ID < temp.ID
						: sortValCurrent = empl.ID > temp.ID;
					break;
				}
				case 2: {
					sortVariable
						? sortValCurrent = strcmp(empl.fullName, temp.fullName) < 0
						: sortValCurrent = strcmp(empl.fullName, temp.fullName) > 0;
					break;
				}
				case 3: {
					size_t emplPosLenght = 0;
					size_t tempPosLenght = 0;
					for (size_t i = 0; i < EMPL_POS_LENGHT; i++) {
						if (!empl.position[i] && !temp.position[i]) break;
						if (empl.position[i]) emplPosLenght++;
						if (temp.position[i]) tempPosLenght++;
					}

					sortVariable
						? sortValCurrent = emplPosLenght < tempPosLenght
						: sortValCurrent = emplPosLenght > tempPosLenght;
					break;
				}
				default: sortValCurrent = false;
				}

				if (sortValCurrent) {
					fseek(staffFile, (n - 2) * sizeof(employee), SEEK_SET);
					fwrite(&temp, sizeof(employee), 1, staffFile);
					fseek(staffFile, (n - 1) * sizeof(employee), SEEK_SET);
					fwrite(&empl, sizeof(employee), 1, staffFile);
					flag = true;
				}
				else empl = temp;
				fseek(staffFile, n * sizeof(employee), SEEK_SET);
			}
			rewind(staffFile);
		} while (flag);

		fclose(staffFile);

		break;
	}
	case 4: {
		position pos;

		cout << "\n\tВыберите значение, по которому будет производиться сортировка:"
			<< "\n\t1 - Номер квалификации."
			<< "\n\t2 - Название квалификации.";

		titleVal = "\n\tВаш вариант: ";
		checkVal = createPatternForMultipleNumbers(1, 1, 2, true);
		sortNav = stoi(inputCurrentVal(&checkVal, &titleVal));

		cout << "\n\tВыберите как будет производиться сортировка:"
			<< "\n\t1 - По возрастанию."
			<< "\n\t2 - По убыванию.";

		titleVal = "\n\tВаш вариант: ";
		checkVal = createPatternForMultipleNumbers(1, 1, 2, true);
		sortVariable = (bool)(stoi(inputCurrentVal(&checkVal, &titleVal)) - 1);

		position temp;
		size_t n;
		bool flag;
		bool sortValCurrent;

		fopen_s(&posFile, POSITIONS, "rb+");
		do {
			flag = false;
			n = 1;

			fread(&pos, sizeof(position), 1, posFile);
			while (fread(&temp, sizeof(position), 1, posFile) == 1) {
				n++;

				switch (sortNav) {
				case 1: {
					sortVariable
						? sortValCurrent = pos.ID < temp.ID
						: sortValCurrent = pos.ID > temp.ID;
					break;
				}
				case 2: {
					sortVariable
						? sortValCurrent = strcmp(pos.occupation, temp.occupation) < 0
						: sortValCurrent = strcmp(pos.occupation, temp.occupation) > 0;
					break;
				}
				default: sortValCurrent = false;
				}

				if (sortValCurrent) {
					fseek(posFile, (n - 2) * sizeof(position), SEEK_SET);
					fwrite(&temp, sizeof(position), 1, posFile);
					fseek(posFile, (n - 1) * sizeof(position), SEEK_SET);
					fwrite(&pos, sizeof(position), 1, posFile);
					flag = true;
				}
				else pos = temp;
				fseek(posFile, n * sizeof(position), SEEK_SET);
			}
			rewind(posFile);
		} while (flag);

		fclose(posFile);

		break;
	}
	}
}