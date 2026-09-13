#include <iostream> // for cin, cout
#include <sstream> // for stringstream
#include <chrono> // for *fancy* timers
#include <iomanip> // for manipulators
#include <fstream> // for file streams
#include <vector> // for vector
#include <string> // for getline
// #include <stream>

/* Lecture 1 Code, by Avery Wang, Jan. 9, 2020 */

using namespace std;

int stringToInteger(const string& s);
void stringToIntegerTest();
int mindlessWork();
void bufferedExperiment(ostream& oss);
int endlEachTime();
int endlAtEnd();
void badWelcomeProgram();
void printStateBits(const istream& iss);
std::vector<string> stringSplit(const string& str);
void manipulatorFun();
void printEndTime(const string& input);
int getInteger(const std::string& prompt);


// During lecture, fill in this main function
// Follow along, type in some code, and
// try experimenting with anything you can think of
// did something unexpected happen? Ask!
int main() {

    /**
     * Output stringstream:
     * When you create one stream, its begin pointer points the its begin element.
     * So the oss is replaced some context.
     */
    ostringstream oss("Ito-En Green Tea");      // Create a output stringstream.
    cout << oss.str() << endl; // Ito-En Green Tea.

    oss << "16.9 Ounces";
    cout << oss.str() << endl; // 16.9 Ouncesn Tea

    /**
     * If you need to add some context at the end.
     * you can use the constant: ostringstream::ate
     */
    ostringstream expc_oss("Ito-En Green Tea", ostringstream::ate);  // Create a output stringstream.
    cout << expc_oss.str() << endl;  // The same effect to front code.

    expc_oss << "16.9 Ounces";
    cout << expc_oss.str() << endl;  // The correct result: Ito-En Green Tea16.9 Ounces

    /**
     * Input stringstream:
     * It's cool, because input stream can do type-conversion itself.
     */
    istringstream iss("16.9 Ounces");

    double amount;
    string unit;

    // Input stream will not stop greping string until meet the whitespace
    iss >> amount;  // amount is assigned to 16.9
    iss >> unit;    // unit is assigned to "Ounces"

    cout << amount / 2 << endl;

    /**
     * Notice: when truncation is happened. input stream begin its next work
     *         from the truncation point.
     */
    istringstream new_iss("16.9 Ounces");

    int new_amount; // Get the integer result.
    string new_unit; // Get the string.

    new_iss >> new_amount;  // Grep the 16. Then stop. Truncated!
    new_iss >> new_unit;    // Get the ".9", because the truncation point points '.'

    cout << new_amount / 2 << ' ' << new_unit << endl;  // 8 .9

    /**
     * Notice:
     * Type is important to the input stream.
     * It determines the location of the truncation point.
     * 
     * The stream extraction assigns the a value to the 
     * corresponding vatiable based on its type.
     */
    
    /* Uncomment these to play with them */
    // stringToIntegerTest();
    // bufferedExperiment(std::cout);

    // int a = endlEachTime();
    // int b = endlAtEnd();
    // cout << "endl each time: " << a << endl;
    // cout << "endl at end: " << b << endl;

    badWelcomeProgram();

    return 0;
}

// you haven't learned about const or the ampersand
// but that's fine, just pretend it's just a string
// converts the string to an integer
// if the string is not an integer, then throws an exception
int stringToInteger(const string& s) {
    /**
     * Take the advantage of input stringstream's intelligent
     * type conversion to convert string to integer.
     */
    istringstream iss(s);
    printStateBits(iss);    // Check the input is vaild

    int result;
    iss >> result;
    if (iss.fail()) throw std::domain_error("no value int at beginning!");

    char remain;
    iss >> remain;
    if (!iss.fail()) throw std::domain_error("more than a single vaild int.");

    /**
     * Because the implicit conversion of streams,
     * there's a functionally equivalent version:
     * ```cpp
     * if (!(iss >> result))
     *      throw std::domain_error("no valur int at beginning!")
     * if (iss >> remain)
     *      throw std::domain_error("more than a single vaild int.")
     * ```
     */

    printStateBits(iss);    // Check the buffer's status.

    return result;
}

// Call this function to test your stringToInteger function
// ignore how it is implemented - you'll learn about this next week.
void stringToIntegerTest() {
    while (true) {
        cout << "Type in an integer: ";
        string s;
        if (!getline(cin, s)) throw std::domain_error("getline failed");
        cout << "You typed in: " << s << "\n";
        int result = stringToInteger(s);
        cout << "As an integer, this is: " << result << "\n";
        cout << "Half of that is: " << result/2 << "\n";
    }

}

// the purpose of this function is to
// experiment with cout vs. cerr
// which are both types of ostreams
// we covered this optional topic at the end of lecture
void bufferedExperiment(ostream& oss) {
    oss << "CS";
    mindlessWork();

    oss << "106";
    mindlessWork();

    // oss << flush;
    // mindlessWork();

    oss << "L";
    mindlessWork();

    oss << endl;
    // mindlessWork();
}

// This is how I feel about my classes this quarter
// This function acts as a pause so you can more clearly see the interesting
// parts around this call, such as the buffering above.
// This is probably a life lesson about mindless work.
int mindlessWork() {
    double sum = 0;
    for (int i = 0; i < 400000000; ++i) {
        sum += i/999959;
        // large prime number so compiler can't optimize :)
    }
    return sum;
}

// Prints the first 100 integers including an endl at the end of each line
// Returns the duration it took.
// Optional topic covered at end of lecture.
int endlEachTime() {
    auto startMove = std::chrono::high_resolution_clock::now(); // records start time

    for (int i = 0; i < 10000; ++i) {
        cout << i << endl;
    }

    auto stopMove = std::chrono::high_resolution_clock::now(); // records end time
    // don't worry too much about this syntax, you can google it easily
    // in fact, that's exactly what I did
    auto moveDuration = std::chrono::duration_cast<std::chrono::microseconds>(stopMove - startMove);
    return moveDuration.count();
}

// Prints the first 100 integers including only a newline character after each line
// Returns the duration it took.
// Optional topic covered at end of lecture
int endlAtEnd() {
    auto startMove = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 10000; ++i) {
        cout << i << '\n';
    }
    cout << flush;

    auto stopMove = std::chrono::high_resolution_clock::now();
    auto moveDuration = std::chrono::duration_cast<std::chrono::microseconds>(stopMove - startMove);
    return moveDuration.count();
}

// This is a bad program, and is how most C++ beginners get themselves into
// trouble (so don't use it in CS 106B!)
// WE DID NOT GET TO THIS IN LECTURE, WILL BEGIN LECTURE 3 WITH THIS
void badWelcomeProgram() {
    /**
     * Notes:
     * When the fail bit is set, the cin's buffer will block.
     * In other words: cin will stop and subsequent work will also
     * be suspended.
     * 
     * eg:
     * If I type "Doro Knight", the cin will go wrong.
     * the result will be like this:
     * What is your name? Doro Knight
     * What is your age? Hello Doro (age 0)
     * Do you want to try again? You said: [nothing here]
     */
    string name, response;
    int age;

    cout << "What is your name? "; // sidenote: no flush needed! why?
    cin >> name;

    cout << "What is your age? ";
    cin >> age;

    cout << "Hello " << name << " (age " << age << ")" << '\n';

    cout << "Do you want to try again? ";
    cin >> response;

    cout << "You said: " << response << endl;
}

/**
 * For self-learner, I didn't downlowd the Stand0rd's Standand C++ Library
 * Then I implement the function 'getInteger' as follows:
 */
int getInteger(const std::string& prompt = "") {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line))
            throw domain_error("No integer at the beginning found.");
        istringstream iss(line);

        int result; char remain;
        if (iss >> result && !(iss >> remain)) 
            return result;
    }
}

// Takes in any istream and prints the state bits

void printStateBits(const istream& iss) {
    cout << "State bits: ";

    /*
     *
     * Quick syntax lesson (Keith will teach this too!)
     * TERNARY OPERATOR
     * The following are equivalent:
     * [boolean] ? [statement 1] : [statement 2]
     *
     * if (boolean) {
     *     statement1
     * } else {
     *     statement2
     * }
     *
     * Many people (understandably) hate it, but it makes some expressions super short.
     *
     * int i = 0;
     * if (x > 100) i = 3;
     * else i = 5
     *
     * can replaced by...
     *
     * int i = (x > 100) ? 3 : 5;
     *
     */

    cout << (iss.good() ? "G" : "-");
    cout << (iss.fail() ? "F" : "-");
    cout << (iss.eof() ? "E" : "-");
    cout << (iss.bad() ? "B" : "-");
    cout << '\n';
    cout << "-----------" << '\n';
    // automatically flushed! why?
}

// splits a string with whitespace separated tokens
// into a vector of tokens
std::vector<string> stringSplit(const string& str) {
    std::vector<string> tokens;
    // to add a token to tokens, use tokens.push_back(token);
    return tokens;
}

// some fun with manipulators
// we're not gonna spend too much time on these
// just google them and find the one you wnat to use
void manipulatorFun() {
    cout << "[" << std::setw(10) << "Ito" << "]";
    cout << "[" << std::left << std::setw(10) << "Ito" << "]";
    cout << "[" << std::left << std::setfill('-')
         << std::setw(10) << "Ito" << "]";

}

// Given a start time and a duration,
// calculate the end time.
// Assume correct formatting of string.

// input string: “1:30 PM \n 1 hour 20 minute”
// console string: “2:50 PM”

// Common bug: "1:30 PM \n 0 hour 30 minute"
// should print "2:00 PM", not "2:0 PM"
// Hint: use manipulators
void printEndTime(const string& input) {
    // you fill this out!
}

/**
 * Write the following function which prompts 
 * the user for a filename, opens the ifstream to
 * the file, reprompt if the filename is not vaild 
 * and then return the filename
 */
string promptUserForFile(ifstream& stream,
                         string prompt = "",
                         string reprompt = "") {
    cout << prompt;
    string filename;
    while (true) {
        getline(cin, filename);
        stream.open(filename);
        
        if (!stream.is_open()) {
            cout << reprompt;
        } else {
            return filename;
        }
    }
}




