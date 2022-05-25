#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <istream>
 
using namespace std;
string data_path="/home/pjh/catkin_ws/src/slam_tutorial/data/rgbd_dataset_freiburg2_desk/";
string data_fill=data_path+"fill.csv";

//csv 파일안에 있는 데이터를 읽어서 vector에 저장한다.
vector<string> csv_read_row(istream &file, char delimiter);
 
int main(int argc, char *argv[])
{
    //ifstream은 파일을 읽게 해주는 함수로써 ifstream (파일명 or 경로)
    ifstream file(data_fill); 
 
    if (file.fail())  //만약 bad() 함수가 실패 하면..
    {
        return (cout << "file not found" << endl) && 0;
    }
        
    vector<string> b_row=csv_read_row(file, ',');
    b_row=csv_read_row(file, ',');
    double gap;
    while(file.good()) //eof, bad, fail 함수가 거짓의 참을 반환할 때까지..
    {
        vector<string> row = csv_read_row(file, ',');


        cout << "[" << row[2] << "]" << endl;

        // gap=stod(row[1])-stod(b_row[1]);   
        // cout<<gap<<endl;
        // b_row=row;
    }
 
    file.close(); //파일 입출력 완료 후 닫아준다.
 
    return 0;
}
 
vector<string> csv_read_row(istream &file, char delimiter)
{
    stringstream ss;
    bool inquotes = false;
    vector<string> row;//relying on RVO
 
    while(file.good())
    {
        char c = file.get();
        if (!inquotes && c=='"') 
        {
            inquotes=true;
        }
        else if (inquotes && c=='"') 
        {
            if ( file.peek() == '"')
            {
                ss << (char)file.get();
            }
            else 
            {
                inquotes=false;
            }
        }
        else if (!inquotes && c==delimiter) 
        {
            row.push_back( ss.str() );
            ss.str("");
        }
        else if (!inquotes && (c=='\r' || c=='\n') )
        {
            if(file.peek()=='\n') { file.get(); }
            row.push_back( ss.str() );
            return row;
        }
        else
        {
            ss << c;
        }
    }
}
