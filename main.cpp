/*
 * Created by sh_z on 10/2/17
 * This is very bad implementation of OOP concept
 */

#include <QApplication>
#include <QMainWindow>
#include "qcustomplot.h"
#include <iostream>
#include <vector>
#include <cmath>

using std::cin;     using std::cout;
using std::endl;	using std::cerr;

constexpr long double randomval = 0;
constexpr int DIV_BY_ZERO = -1;

//inline int find(const std::string&);
int find(const std::string&, int = 0, int = 0);

class Graph {
    std::vector<long double> numbers;
    std::string operators;
    std::string localOperators;
    std::vector<long double> localNumbers;
    std::vector<long double> tempNumbers;
    std::string tempOperators;
    std::vector<int> indexes;
    std::vector<unsigned int> popen_indexes;
    std::vector<unsigned int> pclose_indexes;
    std::vector<unsigned int> tmp_open_indexes;
    std::vector<unsigned int> tmp_close_indexes;
    long double answer;
    inline void badoption() const;
    long double parenthesis(int, long double);

public:
    void getEquation();
    long double operate(long double);
    long double operate(int, int, long double);
    long double getAnswer() const { return answer; }
};


inline void Graph::badoption() const {
    cout << "Bad number/operator!" << endl;
    cin.ignore(4096, '\n');
    cin.clear();
    // print the before error equation
    for(int i = 0; i < numbers.size(); ++i) {
        cout << numbers[i];
        if(operators.size() > 0)
            cout << operators[i];
    }
}


void Graph::getEquation() {
    char tmp;
    char op;
    long double num;
    if(cin >> num) {
        numbers.push_back(num);
    }
    else {
        cin.clear();
        cin >> tmp;
        if(tmp == 'q') {
            return;
        }
        if(tmp == '(' || tmp == ')') {
            if(tmp == '(') {
                popen_indexes.push_back(operators.size());
                operators.push_back(tmp);
            }
            else {
                pclose_indexes.push_back(operators.size());
                operators.push_back(tmp);
            }
            if(cin >> num)
                numbers.push_back(num);
            else {
                cin.clear();
                cin >> tmp;
                if(tmp == 'x') {
                    indexes.push_back(numbers.size());
                    numbers.push_back(randomval);
                }
            }
        }
        else if(tmp == 'x') {
            indexes.push_back(numbers.size());
            numbers.push_back(randomval);
        }
        else {
            badoption();
        }
    }

    while(true) {
        cin >> op;
        if(op == 'q')
            break;
        if( op == '(' || op == ')') {
            if(op == '(') {
                popen_indexes.push_back(operators.size());
                operators.push_back(op);
            }
            else {
                pclose_indexes.push_back(operators.size());
                operators.push_back(op);
            }
        }
        else if(op == '+' || op == '*' || op == '-' || op == '/' || op == '^') {
            operators.push_back(op);
            if(cin >> num){
                numbers.push_back(num);
            }
            else {
                cin.clear();
                cin >> tmp;
                if(tmp == 'q')
                    return;
                if(tmp == 'x') {
                    indexes.push_back(numbers.size());
                    numbers.push_back(randomval);
                }
                else if(tmp == '(' || tmp == ')') {
                    if(tmp == '(') {
                        popen_indexes.push_back(operators.size());
                        operators.push_back(tmp);
                    }
                    else {
                        pclose_indexes.push_back(operators.size());
                        operators.push_back(tmp);
                    }
                    if(cin >> num)
                        numbers.push_back(num);
                    else {
                        cin.clear();
                        cin >> tmp;
                        if(tmp == 'x') {
                            indexes.push_back(numbers.size());
                            numbers.push_back(randomval);
                        }
                    }
                }
                else {
                    badoption();
                }
            }
        }
        else {
            badoption();
        }
    }
}

long double Graph::parenthesis(int index, long double val) {
    for(int i = tmp_open_indexes[index]; i < tmp_close_indexes[index]; ++i) {  // this is very well thought
        tempNumbers.push_back(localNumbers[i]);
    }

    for(int i = tmp_open_indexes[index]+1; i < tmp_close_indexes[index]; ++i) {
        tempOperators.push_back(localOperators[i]);
    }
    return operate(tmp_open_indexes[index], tmp_close_indexes[index], val);
}

long double Graph::operate(long double val) {
    for(int i = 0; i < indexes.size(); ++i) {
        numbers[indexes[i]] = val;
    }
    long double parvalue;
    tmp_open_indexes = popen_indexes;
    tmp_close_indexes = pclose_indexes;
    localNumbers = numbers;
    localOperators = operators;
    // Resolve parenthesis
    for(int i = 0, number = 0; i < operators.size(); ++i) {
        if(operators[i] == '(') {
            parvalue = parenthesis(number, val);

            localNumbers.erase(localNumbers.begin()+tmp_open_indexes[number]+1, localNumbers.begin()+tmp_close_indexes[number]);
            localNumbers[tmp_open_indexes[number]] = parvalue;
            unsigned int length = tmp_close_indexes[number] - tmp_open_indexes[number] + 1;
            localOperators.erase(tmp_open_indexes[number], length);
            number++;
            for(int i = number; i < tmp_close_indexes.size(); i++) {
                tmp_open_indexes[i]  -= length;
                tmp_close_indexes[i] -= length;
            }
            // TOUGH PART!
        }
    }
    // solve the equation out of parenthesis
    while(!localOperators.empty()) {
        int i = find(localOperators);    // give the operator's index in mathematical order
        char op = localOperators[i];
        if (op == '^')
            localNumbers[i] = std::pow(localNumbers[i], localNumbers[i + 1]);
        else if (op == '/') {
            try {
                if (localNumbers[i + 1] == 0)
                    throw DIV_BY_ZERO;
                localNumbers[i] /= localNumbers[i + 1];
            }
            catch (int) {
                cerr << "Divide by zero" << endl;
                return 0;
            }
        } else if (op == '*')
            localNumbers[i] *= localNumbers[i + 1];
        else if (op == '+')
            localNumbers[i] += localNumbers[i + 1];
        else if (op == '-')
            localNumbers[i] -= localNumbers[i + 1];

        localNumbers.erase(localNumbers.begin() + i + 1);
        localOperators.erase(localOperators.begin() + i);
    }
    answer = localNumbers[0];
    localNumbers.erase(localNumbers.begin());
    return answer;
}


// To operate on parentheses
long double Graph::operate(int start, int end, long double val) {
    for(int i = start; i < indexes.size() && i < end; i++)
        numbers[indexes[i]] = val;

    while(!tempOperators.empty()) {
        int i = find(tempOperators);
        if(tempOperators.size() == 0) {
            answer = tempNumbers[0];
            tempNumbers.pop_back();
            return answer;
        }
        char op = tempOperators[i];
        if(op == '^')
            tempNumbers[i] = std::pow(tempNumbers[i], tempNumbers[i+1]);
        else if (op == '/') {
            try {
                if(tempNumbers[i+1] == 0)
                    throw DIV_BY_ZERO;
                tempNumbers[i] /= tempNumbers[i + 1];
            }
            catch(int) {
                cerr << "Divide by zero" << endl;
                return 0;
            }
        }
        else if (op == '*')
            tempNumbers[i] *= tempNumbers[i + 1];
        else if (op == '+')
            tempNumbers[i] += tempNumbers[i + 1];
        else if (op == '-')
            tempNumbers[i] -= tempNumbers[i + 1];

        tempNumbers.erase(tempNumbers.begin() + i + 1);
        tempOperators.erase(tempOperators.begin() + i);
    }
    answer = tempNumbers[0];
    tempNumbers.pop_back();
    return answer;
}


int find(const std::string &operators, int start, int end) {
    // FIXME is there an better, efficient way?
    if(!end)	// if end is zero
        end = operators.size()-1;

    for(int i = start; i <= end; ++i)
        if(operators[i] == '(')
            return i;
    for(int i = end; i >= start; i--)
        if(operators[i] == '^')
            return i;
    for(int i = start; i <= end; i++)
        if(operators[i] == '/' || operators[i] == '*')
            return i;
    for(int i = start; i <= end; i++)
        if(operators[i] == '+' || operators[i] == '-')
            return i;
}


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QMainWindow window;

  // setup customPlot as central widget of window:
  QCustomPlot customPlot;
  window.setCentralWidget(&customPlot);

  // create plot (from quadratic plot example):
  QVector<double> x, y;
  cout << "Enter the equation(q to exit): ";
  Graph function;
  function.getEquation();

  for (double i=-100; i <= 100; i+= 0.01) {
    x.push_back(i);
    y.push_back(function.operate(i));
  }

  customPlot.addGraph();
  customPlot.graph(0)->setData(x, y);
  customPlot.xAxis->setLabel("x");
  customPlot.yAxis->setLabel("f(x)");
  customPlot.xAxis->setTickLength(0, 5);
  customPlot.xAxis->setSubTickLength(0, 3);
  customPlot.rescaleAxes();

  window.setGeometry(100, 100, 900, 700);
  window.show();
  return a.exec();
}
