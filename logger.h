#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <fstream>
#include<iostream>
using namespace std;
class Logger{

public :
    Logger(string logFileName):file(logFileName,ios::app){

}

   void log(string when,string who,string where,float similarity){
       file<<when<<" : "<<who<<" : "<<where<<" : "<<(isnan(similarity)?0.000:similarity)<<endl;
}
private :
    fstream file;
};

#endif // LOGGER_H
