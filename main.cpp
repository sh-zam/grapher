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

constexpr int randomval = 0;
constexpr int DIV_BY_ZERO = -1;

//inline int find(const std::string&);
inline int find(const std::string&, int, int);

class Graph {
    std::vector<long double> numbers;
    std::string operators;
    std::vector<int> indexes;
    long double answer;
    inline void badoption() const;
    long double parenthesis(int, long double);
    std::string localOperators;
    std::vector<long double> localNumbers;
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
    if(cin >> num){
        numbers.push_back(num);
    }
    else {
        cin.clear();
        cin >> tmp;
        if(tmp == 'q')
            return;
        if(tmp == '(' || tmp == ')') {
            operators.push_back(tmp);
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
        else
            badoption();
    }

    while(true) {
        cin >> op;
        if(op == 'q')
            break;
        if( op == '(' || op == ')') {
            operators.push_back(op);
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
                else
                    badoption();
            }
        }
        else {
                badoption();
        }
    }
}

long double Graph::parenthesis(int index, long double val) {
    for(int i = index; i < operators.size(); ++i) {
        if(operators[i] == ')'){
            return operate(index, i, val);
        }
    }
}

long double Graph::operate(long double val) {
    for(int i = 0; i < indexes.size(); ++i) {
        numbers[indexes[i]] = val;
    }
    localNumbers = numbers;
    localOperators = operators;
    while(!localOperators.empty()) {
        int i = find(localOperators, 0, 0);	// give the operator's index in mathematical order
        char op = localOperators[i];
        if(op == '(') {
            parenthesis(i, val);
            // this code is making me fucking numb-nuts!!
            continue;
        }
        if (op == '^')
            localNumbers[i] = std::pow(localNumbers[i], localNumbers[i + 1]);
        else if (op == '/') {
            try {
                if(localNumbers[i+1] == 0)
                    throw DIV_BY_ZERO;
                localNumbers[i] /= localNumbers[i + 1];
            }
            catch(int) {
                cerr << "Divide by zero" << endl;
                return 0;
            }
        }
        else if (op == '*')
            localNumbers[i] *= localNumbers[i + 1];
        else if (op == '+')
            localNumbers[i] += localNumbers[i + 1];
        else if (op == '-')
            localNumbers[i] -= localNumbers[i + 1];

        localNumbers.erase(localNumbers.begin() + i + 1);
        localOperators.erase(localOperators.begin() + i);
    }
    return answer = localNumbers[0];
}


// To operate on parentheses
long double Graph::operate(int start, int end, long double val) {
    for(int i = start; i < indexes.size() && i < end; i++)
        numbers[indexes[i]] = val;

    localNumbers = numbers;
    localOperators = operators;
    while(!localOperators.empty()) {
        int i = find(localOperators, start, end);
        // experimental
        localOperators.erase(localOperators.begin()+end);
        localOperators.erase(localOperators.begin()+start);
        if(localOperators.size() == 0)
            return answer = localNumbers[0];
        char op = localOperators[i];
        if(op == '^')
            localNumbers[i] = std::pow(localNumbers[i], localNumbers[i+1]);
        else if (op == '/') {
            try {
                if(localNumbers[i+1] == 0)
                    throw DIV_BY_ZERO;
                localNumbers[i] /= localNumbers[i + 1];
            }
            catch(int) {
                cerr << "Divide by zero" << endl;
                return 0;
            }
        }
        else if (op == '*')
            localNumbers[i] *= localNumbers[i + 1];
        else if (op == '+')
            localNumbers[i] += localNumbers[i + 1];
        else if (op == '-')
            localNumbers[i] -= localNumbers[i + 1];

        localNumbers.erase(localNumbers.begin() + i + 1);
        localOperators.erase(localOperators.begin() + i);
    }
    return answer = localOperators[0];
}


inline int find(const std::string &operators, int start = 0, int end = 0) {
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
    return -1;
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
