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
#include <stdexcept>
#include <assert.h>

using std::cin;     using std::cout;
using std::endl;

constexpr int randomval = 0;

inline int find(const std::string&);

class Graph {
    std::vector<double> numbers;
    std::string operators;
    std::vector<int> indexes;
    double answer;
    inline void badoption() const;
public:
    void getEquation();
    int operate(int);
    double getAnswer() const { return answer; }
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
    int num;
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

    /*
    cin >> tmp;
    if(tmp == 'q')
        return;
    if(tmp == 'x') {     // if variable
        indexes.push_back(0);
        numbers.push_back(randomval);
    }
    else
        numbers.push_back(tmp - 48);
    */
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
                else {
                    badoption();
                }

            }
            /*cin >> tmp;
            if(tmp == 'x') {
                indexes.push_back(numbers.size());
                numbers.push_back(randomval);
            }
            else
                numbers.push_back(tmp - 48);
                */
        }
        else {
                badoption();
        }
    }
}


int Graph::operate(int val) {
    for(int i = 0; i < indexes.size(); ++i) {
        numbers[indexes[i]] = val;
    }
    std::vector<double> localNumbers = numbers;
    std::string localOperators = operators;
    while(!localOperators.empty()) {
        int i = find(localOperators);
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
  Graph g;
  g.getEquation();

  for (double i=-10; i <= 10; i+= 1) {
    x.push_back(i);
    y.push_back(g.operate(i));
  }
  customPlot.addGraph();
  customPlot.graph(0)->setData(x, y);
  customPlot.xAxis->setLabel("x");
  customPlot.yAxis->setLabel("f(x)");
  customPlot.xAxis->setTickLength(0, 5);
  customPlot.xAxis->setSubTickLength(0, 3);
//  customPlot.xAxis->setRange(-10, 100);
  customPlot.rescaleAxes();
//  customPlot.replot();

  window.setGeometry(100, 100, 500, 400);
  window.show();
  return a.exec();
}
