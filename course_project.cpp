#include <windows.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <string>
#include <regex>

#define ORDERS "orders.dat"
#define STAFF "staff.dat"
#define SERVICES "services.dat"

#define POS_KOL 3
#define DATE_LENGHT 3
#define PHONE_NUMB_LENGHT 9
#define ORDER_SERV_LENGHT 5

using namespace std;

struct order {
	size_t	ID;
	char	fullName[20];
	size_t	phoneNumb[PHONE_NUMB_LENGHT];
	time_t	dateOfOrder[DATE_LENGHT];
	size_t	servNumbers[ORDER_SERV_LENGHT];
};

struct employee {
	size_t	ID;
	char	fullName[20];
	size_t	phoneNumb[PHONE_NUMB_LENGHT];
	size_t	position[POS_KOL];
};

struct service {
	size_t	ID;
	size_t	employee;
	size_t	category;
	char	title[40];
	int		price;
	size_t	term;
};

struct table {
	size_t layoutLenght;
	string title;
};

struct position {
	size_t ID;
	char occupation[20];
};

position pos[POS_KOL] = {
	{1, "Дизайн"},
	{2, "Программирование"},
	{3, "SEO-оптимизация"}
};

FILE* dataFile;
FILE* ordFile;
FILE* servFile;
FILE* staffFile;

string createPatternForMultipleNumbers(size_t, size_t, size_t, bool);
string inputCurrentVal(string*, string*);

order orderInputLayout(size_t);
employee emplInputLayout(size_t);
service servInputLayout(size_t);

void outputTableLine(size_t, table*, size_t);
void outputVerticalLine(size_t);
void outputTableHeaderRow(table*, size_t);

void navigation(size_t);
void inputVal(size_t);
void output(size_t, size_t);

void editAndRemoveVal(size_t, size_t);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	fopen_s(&dataFile, ORDERS, "a+");
	fclose(dataFile);
	fopen_s(&dataFile, STAFF, "a+");
	fclose(dataFile);
	fopen_s(&dataFile, SERVICES, "a+");
	fclose(dataFile);

	int navPos;
	do {
		cout << "\n\tМеню программы."
			<< "\n\t1 - Работа с заказами."
			<< "\n\t2 - Работа с сотрудниками."
			<< "\n\t3 - Работа с услугами."
			<< "\n\t0 - Закончить программу.";

		string titleVal = "\n\tВыберите пункт меню: ";
		string checkVal = createPatternForMultipleNumbers(1, 0, 3, true);

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
			case 2: cout << "СОТРУДНИКИ"; break;
			case 3: cout << "УСЛУГИ"; break;
			case 4: cout << "СПЕЦИАЛИЗАЦИИ"; break;
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
			inputVal(dataType);
			break;

		case 2:
			break;

		case 3:
			editAndRemoveVal(dataType, navPos);
			break;

		case 4:
			editAndRemoveVal(dataType, navPos);
			break;
		}
		output(dataType, 0);
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

order orderInputLayout(size_t position) {
	order ord;
	service serv;

	string str;
	string titleVal;
	string checkVal;

	size_t itemsLenght = 0;

	// ID
	ord.ID = position;

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
	output(3, 0);

	fopen_s(&servFile, SERVICES, "rb");
	while (fread(&serv, sizeof(service), 1, servFile))
		itemsLenght++;
	fclose(servFile);

	titleVal = "\n\n\tПеречислите через запятую номера услуг (не более " + to_string(ORDER_SERV_LENGHT) + "): ";
	checkVal = createPatternForMultipleNumbers(5, 1, itemsLenght, false);
	str = inputCurrentVal(&checkVal, &titleVal);
	if (true) {
		size_t j = 0;
		for (size_t i = 0, n = 0; i <= str.size(); i++)
			if (str.substr(i, 1) == "," || i == str.size()) {
				ord.servNumbers[j] = stoi(str.substr(i - n, n));
				n = 0;
				j++;
			}
			else n++;

		for (j; j < ORDER_SERV_LENGHT; j++) ord.servNumbers[j] = 0;
	}

	return ord;
}

employee emplInputLayout(size_t position) {
	employee empl;

	string str;
	string titleVal;
	string checkVal;

	// ID
	empl.ID = position;

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
	cout << "\n\tСпециализации:\n";
	for (size_t i = 0; i < POS_KOL; i++) {
		empl.position[i] = 0;
		cout << '\t' << pos[i].ID << ". " << pos[i].occupation << '\n';
	}

	titleVal = "\tПеречислите через запятую номера специализаций: ";
	checkVal = createPatternForMultipleNumbers(POS_KOL, 1, POS_KOL, false);
	str = inputCurrentVal(&checkVal, &titleVal);

	for (size_t i = 0, j = 0, n = 0; i <= str.size(); i++)
		if (str.substr(i, 1) == "," || i == str.size()) {
			empl.position[j] = stoi(str.substr(i - n, n));
			n = 0;
			j++;
		}
		else n++;

	return empl;
}

service servInputLayout(size_t position) {
	service serv;
	employee empl;

	string str;
	string titleVal;
	string checkVal;

	size_t itemsLenght = 0;

	// ID
	serv.ID = position;

	// Category
	cout << "\n\tКатегории:\n";
	for (size_t i = 0; i < POS_KOL; i++)
		cout << '\t' << pos[i].ID << ". " << pos[i].occupation << '\n';

	titleVal = "\tВыберите категорию: ";
	checkVal = createPatternForMultipleNumbers(1, 1, POS_KOL, true);;
	serv.category = stoi(inputCurrentVal(&checkVal, &titleVal));

	// Title
	titleVal = "\tНазвание услуги: ";
	checkVal = "(.+?)";
	strcpy_s(serv.title, inputCurrentVal(&checkVal, &titleVal).c_str());

	// Employee
	fopen_s(&staffFile, STAFF, "rb");
	while (fread(&empl, sizeof(employee), 1, staffFile)) itemsLenght++;
	fclose(staffFile);

	checkVal.clear();
	fopen_s(&staffFile, STAFF, "rb");
	while (fread(&empl, sizeof(employee), 1, staffFile))
		for (size_t i = 0; i < itemsLenght; i++)
			if (empl.position[i] == serv.category) {
				checkVal.append("|(" + to_string(empl.ID) + ")");
				cout << "\n\t" << empl.ID << ". " << empl.fullName;
			}
	fclose(staffFile);
	checkVal.erase(checkVal.begin());

	titleVal = "\n\tВыберите специалиста: ";
	serv.employee = stoi(inputCurrentVal(&checkVal, &titleVal));

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

void inputVal(size_t dataType) {
	order ord;
	employee empl;
	service serv;

	size_t entryNumb = 1;
	string fileName;

	switch (dataType) {
		case 1: fileName = ORDERS; break;
		case 2: fileName = STAFF; break;
		case 3: fileName = SERVICES; break;
	}

	fopen_s(&dataFile, fileName.c_str(), "rb");
	switch (dataType) {
		case 1: while (fread(&ord, sizeof(order), 1, dataFile)) entryNumb++; break;
		case 2: while (fread(&empl, sizeof(employee), 1, dataFile)) entryNumb++; break;
		case 3: while (fread(&serv, sizeof(service), 1, dataFile)) entryNumb++; break;
		default: return;
	}
	fclose(dataFile);

	do {
		fopen_s(&dataFile, fileName.c_str(), "a+b");

		size_t itemsLenght = 0;

		cout << "\n\tВведите данные " << entryNumb 
			<< (entryNumb % 10 == 3 && entryNumb % 100 != 13 ? "-ей" : "-ой") 
			<< " записи:\n";

		switch (dataType) {
		case 1:
			ord = orderInputLayout(entryNumb++);
			fwrite(&ord, sizeof(order), 1, dataFile);
			break;
		case 2:
			empl = emplInputLayout(entryNumb++);
			fwrite(&empl, sizeof(employee), 1, dataFile);
			break;
		case 3:
			serv = servInputLayout(entryNumb++);
			fwrite(&serv, sizeof(service), 1, dataFile);
			break;
		default: return;
		}

		fclose(dataFile);


		string titleVal = "\n\tДостаточно? (Д/Н), ваш вариант: ";
		string checkVal = "Д|д|Н|н";
		char inputRepeat = inputCurrentVal(&checkVal, &titleVal)[0];
		if (inputRepeat == 'Д' || inputRepeat == 'д') break;

	} while (true);
}

void output(size_t dataType, size_t outputType) {
	order ord;
	employee empl;
	service serv;

	size_t lenght = 0;
	string fileName;

	switch (dataType) {
	case 1: fileName = ORDERS; break;
	case 2: fileName = STAFF; break;
	case 3: fileName = SERVICES; break;
	}

	fopen_s(&dataFile, fileName.c_str(), "rb");
	switch (dataType) {
	case 1: while (fread(&ord, sizeof(order), 1, dataFile)) lenght++; break;
	case 2: while (fread(&empl, sizeof(employee), 1, dataFile)) lenght++; break;
	case 3: while (fread(&serv, sizeof(service), 1, dataFile)) lenght++; break;
	default: return;
	}
	fclose(dataFile);

	if (lenght) {
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
				{14, "Услуги"},
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

				if (tabItem[4].layoutLenght < tabItem[0].layoutLenght + tabItem[1].layoutLenght + minServWeight)
					tabItem[4].layoutLenght = tabItem[0].layoutLenght + tabItem[1].layoutLenght + minServWeight;
				if (tabItem[4].layoutLenght % 2) tabItem[4].layoutLenght++;
			}
			fclose(ordFile);

			size_t positionsSize = positionsKol * 3 + 1;
			for (size_t i = 0; i < positionsKol; i++)
				positionsSize += tabItem[i].layoutLenght;

			title->layoutLenght = positionsSize - 4;

			outputTableLine(positionsSize, tabItem, 5);

			outputTableHeaderRow(title, 1);

			outputTableLine(positionsSize, tabItem, 3);

			outputTableHeaderRow(tabItem, positionsKol);

			fopen_s(&ordFile, ORDERS, "rb");
			while (fread(&ord, sizeof(order), 1, ordFile)) {
				bool flag = false;
				if (outputType)
					for (size_t j = 0; j < ORDER_SERV_LENGHT; j++) {
						if (outputType == ord.servNumbers[j]) {
							flag = true;
							break;
						}
					}

				if (!outputType || flag) {
					outputTableLine(positionsSize, tabItem, 1);

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
						outputVerticalLine(0);
						//	ID
						str = to_string(ord.ID);
						cout << " " << right << setw(tabItem[0].layoutLenght) << (i + 1 == servLenght ? str : " ") << " ";
						outputVerticalLine(0);

						//	Full name
						str = ord.fullName;
						cout << " " << left << setw(tabItem[1].layoutLenght) << (i + 1 == servLenght ? str : " ") << " ";
						outputVerticalLine(0);

						//	Phone number
						str = "380";
						for (size_t j = 0; j < PHONE_NUMB_LENGHT; j++) {
							str.append(to_string(ord.phoneNumb[j]));
						}
						cout << " " << left << setw(tabItem[2].layoutLenght) << (i + 1 == servLenght ? str : " ") << " ";
						outputVerticalLine(0);

						//	Date
						str.clear();
						for (size_t j = 0; j < DATE_LENGHT; j++) {
							to_string(ord.dateOfOrder[j]).size() % 10 == 2 || to_string(ord.dateOfOrder[j]).size() % 10 == 4
								? str.append(to_string(ord.dateOfOrder[j]) + "/")
								: str.append("0" + to_string(ord.dateOfOrder[j]) + "/");
						}
						str.erase(str.end() - 1);
						cout << " " << left << setw(tabItem[3].layoutLenght) << (i + 1 == servLenght ? str : " ") << " ";
						outputVerticalLine(0);

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
						outputVerticalLine(0);

						//	Price
						str.clear();
						str.append(to_string(orderPrice));
						str.append("$");

						cout << " " << right << setw(tabItem[5].layoutLenght) << (i + 1 == servLenght ? str : " ") << " ";
						outputVerticalLine(0);

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
						outputVerticalLine(0);
					}
				}
			}
			fclose(ordFile);

			outputTableLine(positionsSize, tabItem, 2);
			cout << "\n\t";
		}
			  break;
		case 2: {
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

			FILE* staffFile;

			fopen_s(&staffFile, STAFF, "rb");
			while (fread(&empl, sizeof(employee), 1, staffFile)) {
				str = to_string(empl.ID);
				if (str.size() > tabItem[0].layoutLenght) tabItem[0].layoutLenght = str.size();

				str = empl.fullName;
				if (str.size() > tabItem[1].layoutLenght) tabItem[1].layoutLenght = str.size();
				if (tabItem[1].layoutLenght % 2) tabItem[1].layoutLenght++;

				for (size_t i = 0; i < POS_KOL; i++) {
					for (size_t j = 0; j < POS_KOL; j++) {
						if (empl.position[j] == pos[i].ID) {
							str = pos[i].occupation;
							if (str.size() > tabItem[3].layoutLenght)
								tabItem[3].layoutLenght = str.size();
						}
					}
				}
				if (tabItem[3].layoutLenght % 2) tabItem[3].layoutLenght++;
			}
			fclose(staffFile);

			size_t positionsSize = positionsKol * 3 + 1;
			for (size_t i = 0; i < positionsKol; i++)
				positionsSize += tabItem[i].layoutLenght;

			title->layoutLenght = positionsSize - 4;

			outputTableLine(positionsSize, tabItem, 5);

			outputTableHeaderRow(title, 1);

			outputTableLine(positionsSize, tabItem, 3);

			outputTableHeaderRow(tabItem, positionsKol);

			fopen_s(&staffFile, STAFF, "rb");
			while (fread(&empl, sizeof(employee), 1, staffFile)) {
				outputTableLine(positionsSize, tabItem, 1);

				size_t posLenght = 0;
				for (size_t i = 0; i < POS_KOL; i++) {
					if (empl.position[i]) {
						posLenght++;
					}
				}

				for (size_t i = 0; i < posLenght; i++) {
					cout << "\n\t";
					outputVerticalLine(0);
					//	ID
					str = to_string(empl.ID);
					cout << " " << right << setw(tabItem[0].layoutLenght) << (i + 1 == posLenght ? str : " ") << " ";
					outputVerticalLine(0);

					//	Full name
					str = empl.fullName;
					cout << " " << left << setw(tabItem[1].layoutLenght) << (i + 1 == posLenght ? str : " ") << " ";
					outputVerticalLine(0);

					//	Phone number
					str = "380";
					for (size_t j = 0; j < PHONE_NUMB_LENGHT; j++) {
						str.append(to_string(empl.phoneNumb[j]));
					}
					cout << " " << left << setw(tabItem[2].layoutLenght) << (i + 1 == posLenght ? str : " ") << " ";
					outputVerticalLine(0);

					//	Positions
					for (size_t j = 0; j < POS_KOL; j++) {
						if (pos[j].ID == empl.position[i]) {
							str = pos[j].occupation;
							break;
						}
					}
					cout << " " << left << setw(tabItem[3].layoutLenght) << str << " ";
					outputVerticalLine(0);
				}
			}
			fclose(staffFile);

			outputTableLine(positionsSize, tabItem, 2);
			cout << "\n\t";
		}
			  break;
		case 3: {
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

				FILE* staffFile;
				fopen_s(&staffFile, STAFF, "rb");
				while (fread(&empl, sizeof(employee), 1, staffFile)) {
					str = empl.fullName;
					if (str.size() > tabItem[1].layoutLenght) tabItem[1].layoutLenght = str.size();
					if (tabItem[1].layoutLenght % 2) tabItem[1].layoutLenght++;
				}
				fclose(staffFile);

				for (size_t i = 0; i < POS_KOL; i++) {
					if (serv.category == pos[i].ID) {
						str = pos[i].occupation;
						if (str.size() > tabItem[2].layoutLenght)
							tabItem[2].layoutLenght = str.size();
					}
				}
				if (tabItem[2].layoutLenght % 2) tabItem[2].layoutLenght++;

				str = serv.title;
				if (str.size() > tabItem[3].layoutLenght) tabItem[3].layoutLenght = str.size();
				if (tabItem[3].layoutLenght % 2) tabItem[3].layoutLenght++;
			}
			fclose(servFile);

			size_t positionsSize = positionsKol * 3 + 1;
			for (size_t i = 0; i < positionsKol; i++)
				positionsSize += tabItem[i].layoutLenght;

			title->layoutLenght = positionsSize - 4;

			outputTableLine(positionsSize, tabItem, 5);

			outputTableHeaderRow(title, 1);

			outputTableLine(positionsSize, tabItem, 3);

			outputTableHeaderRow(tabItem, positionsKol);

			fopen_s(&servFile, SERVICES, "rb");
			while (fread(&serv, sizeof(service), 1, servFile)) {
				if (!outputType || serv.employee == outputType) {
					outputTableLine(positionsSize, tabItem, 1);

					cout << "\n\t";
					outputVerticalLine(0);
					//	ID
					str = to_string(serv.ID);
					cout << " " << right << setw(tabItem[0].layoutLenght) << str << " ";
					outputVerticalLine(0);

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
					outputVerticalLine(0);

					//	Category
					for (size_t i = 0; i < POS_KOL; i++) {
						if (serv.category == pos[i].ID) {
							str = pos[i].occupation;
							break;
						}
					}

					cout << " " << left << setw(tabItem[2].layoutLenght) << str << " ";
					outputVerticalLine(0);

					//	Title
					str = serv.title;
					cout << " " << left << setw(tabItem[3].layoutLenght) << str << " ";
					outputVerticalLine(0);

					//	Price
					str = to_string(serv.price);
					str.append("$");
					cout << " " << right << setw(tabItem[4].layoutLenght) << str << " ";
					outputVerticalLine(0);

					//	Term
					str = to_string(serv.term);
					cout << " " << right << setw(tabItem[5].layoutLenght) << str << " ";
					outputVerticalLine(0);
				}
			}

			fclose(servFile);

			outputTableLine(positionsSize, tabItem, 2);
			cout << "\n\t";
		}
			  break;
		}
	}
	else cout << "\n\tФайл с данными пуст. Попробуйте заполнить его в пункте меню: \"1 - Новая запись.\"\n";
}

void outputTableLine(size_t lenght, table *positions, size_t linePos) {
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
					outputVerticalLine(3); break;
				case 1: 
				case 3: 
				case 4: 
					outputVerticalLine(7); break;
				case 2:
				case 6:
					outputVerticalLine(4); break;
			}
		}
		else if (i == lenght - 1) {
			switch (linePos) {
				case 0:
				case 5:
					outputVerticalLine(5); break;
				case 1: 
				case 3: 
				case 4: 
					outputVerticalLine(8); break;
				case 2:
				case 6:
					outputVerticalLine(6); break;
			}
		}
		else if (pos + 3 == i) {
			switch (linePos) {
				case 0: 
				case 3:	
					outputVerticalLine(9); break;
				case 1: 
					outputVerticalLine(2); break;
				case 2:
				case 4:
					outputVerticalLine(10); break;
				case 5:
				case 6:
					outputVerticalLine(1); break;
			}
			pos += positions[++j].layoutLenght + 3;
		}
		else outputVerticalLine(1);
}

void outputTableHeaderRow(table* item, size_t kol) {
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
		outputVerticalLine(0);

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

			outputVerticalLine(0);
		}
	}

	delete[] rowsKol;
}

void outputVerticalLine(size_t symbNumb) {
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

void editAndRemoveVal(size_t dataType, size_t navPos) {
	order ord;
	employee empl;
	service serv;

	size_t lenght = 0;
	size_t entryNumb;
	string fileName;

	switch (dataType) {
	case 1: fileName = ORDERS; break;
	case 2: fileName = STAFF; break;
	case 3: fileName = SERVICES; break;
	}

	string titleVal;
	string checkVal;

	fopen_s(&dataFile, fileName.c_str(), "rb");
	switch (dataType) {
	case 1: while (fread(&ord, sizeof(order), 1, dataFile)) lenght++; break;
	case 2: while (fread(&empl, sizeof(employee), 1, dataFile)) lenght++; break;
	case 3: while (fread(&serv, sizeof(service), 1, dataFile)) lenght++; break;
	default: return;
	}
	fclose(dataFile);

	if (lenght) {
		do {
			output(dataType, 0);

			switch (navPos) {
			case 3: titleVal = "\n\tВведите номер записи, которую вы собираетесь отредактировать (0 для отмены): "; break;
			case 4: titleVal = "\n\tВведите номер записи, которую вы собираетесь удалить (0 для отмены): "; break;
			default: titleVal = "\n\tВведите номер записи (0 для отмены): ";
			}
			checkVal = createPatternForMultipleNumbers(1, 0, lenght, true);
			entryNumb = stoi(inputCurrentVal(&checkVal, &titleVal));

			if (!entryNumb) break;

			bool flag = false;
			if (navPos == 4) {
				fopen_s(&dataFile, fileName.c_str(), "rb");
				switch (dataType) {
				case 2:
					fopen_s(&servFile, SERVICES, "rb");
					while (fread(&serv, sizeof(service), 1, servFile)) {
						if (entryNumb == serv.employee) {
							flag = true;
							break;
						}
					}
					fclose(servFile);
					break;
				case 3:
					fopen_s(&ordFile, ORDERS, "rb");
					while (fread(&ord, sizeof(order), 1, ordFile)) {
						for (size_t i = 0; i < ORDER_SERV_LENGHT; i++)
							if (entryNumb == ord.servNumbers[i]) {
								flag = true;
								break;
							}
						if (flag) break;
					}
					fclose(ordFile);
					break;
				}
				fclose(dataFile);
			}

			if (!flag) {
				fopen_s(&dataFile, fileName.c_str(), "a+b");

				FILE* pfiletemp;
				fopen_s(&pfiletemp, "temp.dat", "w+b");

				switch (dataType) {
				case 1:
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
					break;
				case 2:
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
								fwrite(&empl, sizeof(employee), 1, pfiletemp);
							}
							break;
						}
					}
					break;
				case 3:
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
								fwrite(&serv, sizeof(service), 1, pfiletemp);
							}
							break;
						}
					}
					break;
				}

				fclose(pfiletemp);

				fclose(dataFile);

				remove(fileName.c_str());
				rename("temp.dat", fileName.c_str());
			}
			else {
				cout << "\n\tУдалить данную запись невозможно";
				switch (dataType) {
				case 2:
					cout << ", т.к. она используется в следующих услугах:";
					output(3, entryNumb);
					break;
				case 3:
					cout << ", т.к. она используется в следующих заказах:";
					output(1, entryNumb);
					break;
				default:
					cout << ".";
				}
				cout << "(чтобы её удалить, избавитесь от всех её упоминаний)\n";
			}

			titleVal = "\n\tДостаточно? (Д/Н), ваш вариант: ";
			checkVal = "Д|д|Н|н";
			char inputRepeat = inputCurrentVal(&checkVal, &titleVal)[0];
			if (inputRepeat == 'Д' || inputRepeat == 'д') break;
		} while (true);
	}
}