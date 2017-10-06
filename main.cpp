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
using std::endl;

constexpr int randomval = 0;

inline int find(const std::string&);

class Graph {
    std::vector<long double> numbers;
    std::string operators;
    std::vector<int> indexes;
    long double answer;
    inline void badoption() const;
public:
    void getEquation();
    long double operate(long double);
    long double getAnswer() const { return answer; }
};

inline void Graph::badoption() const {
    cout << "Bad number/operator!" << endl;
    cin.ignore(4096, '\n');
    cin.clear();
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
        if(tmp == 'x') {
            indexes.push_back(0);
            numbers.push_back(randomval);
        }
        else
            badoption();
    }

    while(true) {
        cin >> op;
        if(op == 'q')
            break;
        if(op == '+' || op == '*' || op == '-' || op == '/' || op == '^') {
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


long double Graph::operate(long double val) {
    for(int i = 0; i < indexes.size(); ++i) {
        numbers[indexes[i]] = val;
    }
    std::vector<long double> localNumbers = numbers;
    std::string localOperators = operators;
    while(!localOperators.empty()) {
        int i = find(localOperators);	// give the operator's index in mathematical order
        char op = localOperators[i];
        if (op == '^')
            localNumbers[i] = std::pow(localNumbers[i], localNumbers[i + 1]);
        else if (op == '/')
            localNumbers[i] /= localNumbers[i + 1];
        else if (op == '*')
            localNumbers[i] *= localNumbers[i + 1];
        else if (op == '+')
            localNumbers[i] += localNumbers[i + 1];
        else if (op == '-')
            localNumbers[i] -= localNumbers[i + 1];

        localNumbers.erase(localNumbers.begin() + i + 1);
        localOperators.erase(localOperators.begin() + i);
    }
    answer = localNumbers[0];
    return answer;
}

inline int find(const std::string &operators) {
    // FIXME is there an better, efficient way?
    for(int i = operators.size()-1; i >= 0; i--)
        if(operators[i] == '^')
            return i;
    for(int i = 0; i < operators.size(); i++)
        if(operators[i] == '/' || operators[i] == '*')
            return i;
    for(int i = 0; i < operators.size(); i++)
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
