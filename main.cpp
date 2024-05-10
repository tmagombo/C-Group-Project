#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Constants
#define TAX_RATE 0.0875
#define OVERTIME 1.5
#define MAX_HOURS 60.0
#define MIN_HOURS 0.0
#define MIN_PAY 15.50
#define OVT_START 40.00

// Function declarations
void read_data(istream &stream, string &first_name, string &last_name,
               double &hours, double &rate);

void overtime_pay(double reg_hours, double &overtime_hours);
char get_yesno(string question);
string getFirstName();
string getLastName();
double getPay();
double get_hours();
void view_data_header();

// Employee struct
struct Employee {
  string first_name, last_name;
  double hours_reg, hours_OT, hourly_rate, gross_pay, net_pay, tax;
};
void processing_subsystem(Employee &a);
void get_gross_pay(Employee &a);
void get_tax(Employee &a);
void get_net_pay(Employee &a);
void bubble_sort_optimized(vector<Employee> &payroll_list);
void view_data(vector<Employee> payroll_list);
void welcome_menu(vector<Employee> payroll_list);
void input_main(vector<Employee> payroll_list);
void print_to_file(vector<Employee> payroll_list);
void view_stats(vector<Employee> payroll_list);
void print_stats(double total_gross, double total_tax, double total_OT,
                 double ave_hours);

// Statistics to add...
// Total Gross Pay, Tax Total, Average Hours Worked, Total OT Hours,

int main() {
  vector<Employee> payroll_list;
  Employee a;
  char yesno;
  ifstream in("newdata.txt");
  int size = payroll_list.size();

  /*
  int total_gross = 1;
  int total_tax = 2;
  int ave_hours = 3;
  int total_OT = 4;
  */

  // Check if file can be opened
  if (!in) {
    cout << "Cannot open input file" << endl;
    exit(EXIT_FAILURE);
  }
  // Read data from file
  while (!in.eof()) {
    read_data(in, a.first_name, a.last_name, a.hours_reg, a.hourly_rate);
    processing_subsystem(a);
    overtime_pay(a.hours_reg, a.hours_OT);
    payroll_list.push_back(a);
  }

  if (size != payroll_list.size()) {
    payroll_list.pop_back();
  }

  // Display welcome menu
  do {
    welcome_menu(payroll_list);
    yesno = get_yesno("Return to main menu (Y / N) ?");
  } while (yesno == 'Y');
}

// Function to display welcome menu
void welcome_menu(vector<Employee> payroll_list) {
  cout << "Welcome to the payroll program!" << endl;
  cout << "Please enter an option from the menu below: " << endl;
  cout << "1. View payroll data" << endl;
  cout << "2. Add a new employee" << endl;
  cout << "3. View Statistics" << endl;
  cout << "4. Exit" << endl;

  int choice;
  do {
    cin >> choice;
    if (choice != 1 && choice != 2 && choice != 3 && choice != 4) {
      cout << "Invalid choice, please select from the numbers above" << endl;
    };
    switch (choice) {
    case 1:
      view_data(payroll_list);
      break;
    case 2:
      input_main(payroll_list);
      break;
    case 3:
      view_stats(payroll_list);
      break;
    case 4:
      abort();
      break;
    }
  } while (choice != 1 && choice != 2 && choice != 3);
}

void bubble_sort_optimized(std::vector<Employee> &payroll_list) {
  unsigned int i = 1;
  bool sorted = false;
  while (i < payroll_list.size() && !sorted) {
    sorted = true;
    for (unsigned int k = 0; k < payroll_list.size() - i; ++k) {

      if (payroll_list[k].gross_pay > payroll_list[k + 1].gross_pay) {
        int grosspay = payroll_list[k].gross_pay;
        string first = payroll_list[k].first_name;
        string last = payroll_list[k].last_name;
        int rate = payroll_list[k].hourly_rate;

        payroll_list[k].gross_pay = payroll_list[k + 1].gross_pay;
        payroll_list[k + 1].gross_pay = grosspay;

        payroll_list[k].first_name = payroll_list[k + 1].first_name;
        payroll_list[k + 1].first_name = first;

        payroll_list[k].last_name = payroll_list[k + 1].last_name;
        payroll_list[k + 1].last_name = last;

        payroll_list[k].hourly_rate = payroll_list[k + 1].hourly_rate;
        payroll_list[k + 1].hourly_rate = rate;
        sorted = false;
      }
    }
  }
}

// Function to view payroll data
void view_data(vector<Employee> payroll_list) {
  bubble_sort_optimized(payroll_list);
  view_data_header();
  for (int i = 0; i < payroll_list.size(); i++) {
    string name;
    name = payroll_list[i].last_name + ", " + payroll_list[i].first_name;

    cout << std::fixed << std::setprecision(2);
    cout << std::setw(22) << std::left << name;
    cout << std::setw(5) << std::right << payroll_list[i].hours_reg;
    cout << std::setw(8) << payroll_list[i].hourly_rate;
    cout << std::setw(9) << payroll_list[i].gross_pay << endl;
  }
}

/*
//Employee Statistics
void view_stats(vector<Employee> payroll_list) {
    double total_gross = 0;
    double total_tax = 0;
    double total_OT = 0;
    double ave_hours = 0;
    double* ptr_gross = &payroll_list[0].gross_pay;

    for (int i = 0; i < payroll_list.size(); i++) {
        total_gross += *(ptr_gross + i * sizeof(struct Employee));
        total_tax += payroll_list[i].tax;
        total_OT += payroll_list[i].hours_OT;
        ave_hours += payroll_list[i].hours_reg;
    }
    ave_hours /= payroll_list.size();

    print_stats(total_gross, total_tax, total_OT, ave_hours);
    cout << sizeof(struct Employee) << endl;
}
*/

void view_stats(vector<Employee> payroll_list) {
  double total_gross = 0;
  double total_tax = 0;
  double total_OT = 0;
  double ave_hours = 0;

  for (int i = 0; i < payroll_list.size(); i++) {
    total_gross += payroll_list[i].gross_pay;
    total_tax += payroll_list[i].tax;
    total_OT += payroll_list[i].hours_OT;
    ave_hours += payroll_list[i].hours_reg;
  }
  ave_hours /= payroll_list.size();

  print_stats(total_gross, total_tax, total_OT, ave_hours);
}


void print_stats(double total_gross, double total_tax, double total_OT,
                 double ave_hours) {
  cout << std::fixed << std::setprecision(2);
  cout << "Total Gross Pay: " << total_gross << endl;
  cout << "Total Tax:       " << total_tax << endl;
  cout << "Total OT:        " << total_OT << endl;
  cout << "Average Hours:   " << ave_hours << endl;
}
// Function to print header for data
void view_data_header() {
  cout << "==============================" << std::endl;
  cout << "   Employee Payroll Data" << std::endl;
  cout << "==============================" << std::endl;

  cout << "Name" << setw(23) << "Hours" << setw(7) << "Rate" << setw(11)
       << "Gross Pay" << std::endl;
}

// Function to get yes/no answer

char get_yesno(string question) {
  string answer;
  do {
    cout << question;
    cin >> answer;
    if (answer == "y") {
      answer = "Y";
    } else if (answer == "n") {
      answer = "N";
    }
    if (answer != "Y" && answer != "N") {
      cout << "Please type 'Y' for yes or 'N' for no" << endl << endl;
    }
    if (answer == "Y") {
      cout << endl;
    }
  } while (answer != "Y" && answer != "N");
  return answer[0];
}

// Function to read employee data from file

void read_data(istream &stream, string &first_name, string &last_name,
               double &hours, double &rate) {
  stream >> first_name >> last_name;
  stream >> hours >> rate;
}

// Function to calculate overtime pay
void overtime_pay(double reg_hours, double &overtime_hours) {
  if (reg_hours > 40) {
    overtime_hours = reg_hours - 40;
  } else {
    overtime_hours = 0;
  }
}

// Function to add a new employee
void input_main(vector<Employee> payroll_list) {
  Employee a;
  a.first_name = getFirstName();
  a.last_name = getLastName();
  a.hours_reg = get_hours();
  a.hourly_rate = getPay();
  overtime_pay(a.hours_reg, a.hours_OT);
  get_gross_pay(a);
  payroll_list.push_back(a);
  print_to_file(payroll_list);
  string name;
  name = a.last_name + ", " + a.first_name;

  cout << std::fixed << std::setprecision(2);
  cout << std::setw(22) << std::left << name;
  cout << std::setw(5) << std::right << a.hours_reg;
  cout << std::setw(8) << a.hourly_rate;
  cout << std::setw(9) << a.gross_pay << endl;
};

// Function to print payroll data to file
void print_to_file(vector<Employee> payroll_list) {
  ofstream out;
  out.open("newdata.txt");
  for (int i = 0; i < payroll_list.size(); i++) {
    out << payroll_list[i].first_name << " " << payroll_list[i].last_name << " "
        << payroll_list[i].hours_reg << " " << payroll_list[i].hourly_rate
        << endl;
  }
  out.close();
}

// Function to get hourly pay rate
double getPay() {
  double hourly_rate;
  do {
    cout << "Enter hourly pay rate: ";
    cin >> hourly_rate;
    if (hourly_rate < MIN_PAY) {
      cout << "The hourly pay rate must be greater than " << MIN_PAY << endl;
    }
  } while (hourly_rate < MIN_PAY);
  return hourly_rate;
}

// Function to get hours worked
double get_hours() {

  double hours;

  do {
    cout << "Enter number of hours worked:";
    cin >> hours;
    if (hours > MAX_HOURS) {
      cout << "The number of hours worked must be between " << MIN_HOURS
           << " and " << MAX_HOURS << endl;
      cout << endl;
    } else if (hours < MIN_HOURS) {
      cout << "The number of hours worked must be between " << MIN_HOURS
           << " and " << MAX_HOURS << endl;
      cout << endl;
    }
  } while ((hours > MAX_HOURS || hours < MIN_HOURS));

  return hours;
}

// Function to get employee's first name
string getFirstName() {
  string first_name;
  cout << "Enter a employee's first name: ";
  cin >> first_name;

  return first_name;
}

// Function to get employee's last name
string getLastName() {
  string last_name;
  cout << "Enter a employee's last name: ";
  cin >> last_name;

  return last_name;
}

// PROCESSING STUFF FROM NOAH
// processing submodule
void processing_subsystem(Employee &a) {
  get_gross_pay(a);
  get_tax(a);
  get_net_pay(a);
}

// calculate gross pay from hours worked and hourly rate
void get_gross_pay(Employee &a) {
  a.gross_pay =
      (a.hours_reg * a.hourly_rate) + (a.hours_OT * a.hourly_rate * 1.5);
}

// calculate tax from gross pay and tax rate
void get_tax(Employee &a) { a.tax = a.gross_pay * TAX_RATE; }

// calculate net pay from gross pay, tax, and parking deduction
void get_net_pay(Employee &a) { a.net_pay = a.gross_pay - a.tax; }
// END PROCESSING STUFF FROM NOAH

/*
Horace Horsecollar 38.0 25.50
Rachel Reins       46.5 32.24
Samuel Saddle      51.0 43.15
*/