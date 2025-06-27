//inputs: month, date, year. Follow the Gregorian calendar. Works from the year 0 and onwards.

//structure: main writes a shell to gather info and validate, then pass it to the calendar function.

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

bool is_leap_year(const unsigned int);
unsigned int calendar(const unsigned int, const unsigned int, const unsigned int);
unsigned int num_leap_years(const unsigned int end, const unsigned int start = 0);

const unsigned int jan_1_0000 = 6; //idk, i'm putting something random here for now

const vector<string> day_map = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const vector<string> month_map = {"January", "February", "March", "April", "May", "June", "July", "August", 
    "September", "October", "November", "December"};

const unordered_map<string, int> month_day_map = {{"January", 0}, {"February", 1}, {"March", 2}, {"April", 3}, {"May", 4},
    {"June", 5}, {"July", 6}, {"August", 7}, {"September", 8}, {"October", 9}, {"November", 10}, {"December", 11}};

const vector<int> month_days = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int main() {
    unsigned int month = 6;
    unsigned int date = 1;
    unsigned int year = 2025;


    string month_str;
    while (true) {
        //get year
        cout << "Year: ";
        cin >> year;

        //get month
        cout << "Month: ";
        cin >> month_str;
        //validate month
        if (month_day_map.find(month_str) == month_day_map.end()) {
            cout << "Unrecognized month. Start over." << endl;
            continue;
        }
        //convert month
        month = month_day_map.at(month_str);

        //get date
        cout << "Date: ";
        cin >> date;
        //How do we check if the reading is a valid unsigned int?
        //validate date
        if (date > month_days[month]) {
            if (month != 1) {
                cout << "Date too high. Starting over." << endl;
                continue;
            } else if (date == 29) {
                if (!is_leap_year(year)) {
                    cout << "It's not a leap year! Starting over." << endl;
                    continue;
                }
            } else {
                cout << "Date too high. Starting over." << endl;
                continue;
            }
        }

        //calculate day of week
        string day_of_week = day_map[calendar(month, date, year)];
        cout << "Day of week is: " << day_of_week << endl << endl;
    }

}

unsigned int calendar(const unsigned int month, const unsigned int date, const unsigned int year) {

//get jan 1 in the desired year
int num_days_added = 365 * year; //does this fit inside int?
// the number of days we have to add is limited by the size of the year datatype
// so we might have to go step by step. check if we can fit 365 * year inside the largest datatype we have,
// and if not, we go step by step. In this case, pre-calculating values lets us more quickly
// calculate way later years.

int num_leap_year_days = num_leap_years(year); //num_leap_years goes from 0 inclusive to year exclusive.
// we can start from some other year if we want, if we're using some pre-calculated value. check the signature if
// you forget what I mean. if you include start, the function will calculate including start and excluding end.
// num_leap_years counts number of leap days from jan1 of start to jan1 of end. if start == end, then
// num leap days is 0.
cout << "leap year days " << num_leap_year_days << endl; 

num_days_added += num_leap_year_days;

int jan_1_year = (jan_1_0000 + num_days_added) % 7;

//now for months.
num_days_added = 0;
for (int i = 0; i < month; i++) {
    num_days_added += month_days[i];
    //cout << "adding stuff: " << month_days[i] << endl;
}

num_days_added += (month > 1 && is_leap_year(year)); // if it's past february and we're in a leap year, add a day.

//cout << "month num days added: " << num_days_added << endl;

int month_1_year = (jan_1_year + num_days_added) % 7;

//now for days.

num_days_added = date - 1;

int month_date_year = (month_1_year + num_days_added) % 7;

return month_date_year;

}


bool is_leap_year(unsigned int year) {

//if not div by 4, definitely not a leap year

//if div by 4 but not by 100, definitely a leap year

//if div by 100 but not 400, def not a leap year

//if div by 400, def a leap year

    if (year % 4) return false;
    if (year % 100) return true;
    if (year % 400) return false;
    return true;
}

unsigned int num_leap_years(const unsigned int end, const unsigned int start) {
    //start inclusive to end exclusive
    if (start > end) return 0;
    
    int num_leap_years = 0;
    for (int y = start; y < end; ++y) {
        num_leap_years += is_leap_year(y);
    }
    //one improvement is to find the first leap year >= start, then loop by y += 4 instead of ++y
    //another improvement is to sum how many div4 there are, then subtract how many div100,
    // then add how many div400

    return num_leap_years;
}