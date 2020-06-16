//import libraries
#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<vector>
#include<cassert>
using namespace std;

//linear regression  class
class linerRegression
{
    private:
    double m;
    double c;
    
    public:
    //to fit the regressor
    void fit(const vector<double> &x_train, const vector<double> &y_train)
    {
        int x_len = x_train.size();
        int y_len = y_train.size();
        assert((x_len == y_len) || cout<<"ERROR: model not fit-- x_train, y_train length not match\n");
        
        long double xy_sum =0;
        long double x_sum =0;
        long double y_sum =0;
        long double x_sq_sum =0;

        for(int i(0); i<x_len; i++)
        {
            x_sum += x_train[i];
            y_sum += y_train[i];
            xy_sum += x_train[i] * y_train[i];
            x_sq_sum += x_train[i] * x_train[i];
        }

        m = (x_len*xy_sum - x_sum*y_sum) / (x_len*x_sq_sum - x_sum*x_sum);
        c = (x_sq_sum*y_sum - x_sum*xy_sum) / (x_len*x_sq_sum - x_sum*x_sum);
    }

    //predict for input array
    void predict(const vector<double> &x_test, vector<double> &y_pred)
    {
        int x_len = x_test.size(); 
        y_pred.resize(x_len);
    
        for(int i(0); i<x_len; ++i)
        y_pred[i] = m*x_test[i] + c;
    }

    //to calculate mean squred error
    long double meanSquredError(const vector<double> &y_actual, const vector<double> &y_predict)
    {
        long double error = 0;
        int y_actual_len = y_actual.size();
        int y_predict_len = y_predict.size();

        assert((y_actual_len == y_predict_len) || cout<<"ERROR: MSR not calculated-- y_actual, y_predict length not match\n");
        
        for (int i = 0; i < y_actual_len; i++)
        {
            error += pow((y_actual[i] - y_predict[i]), 2.0);
        }

        error = error/y_actual_len;
        error = sqrt(error);
        return error;
    }

    //get slope for trained model
    double get_slope()
    {
        return m;
    }

    //get intercept for trained model
    double get_intersect()
    {
        return c;
    }
};

//to read file and prprocess the data
class preprocessing
{
    public:
    int count;
    vector<double> x;
    vector<double> y;
    vector<double> x_train;
    vector<double> y_train;
    vector<double> x_test;
    vector<double> y_test;
    
    public:
    //read the file
    void readFile(ifstream &file)
    {
        assert(file.is_open() || "ERROR: not able to read file");
        double temp_x;
        double temp_y;
        char temp_sep;
        count = 0;
        while(file>>temp_x>>temp_sep>>temp_y)
        {
            x.push_back(temp_x);
            y.push_back(temp_y);
            count++;
        }
    }

    //split the data in train and test set in givn train perentage
    void splitTrainTest(double percent)
    {
        int x_train_count = (percent*count)/100;

        x_train.resize(x_train_count);
        x_test.resize(count - x_train_count);
        y_train.resize(x_train_count);
        y_test.resize(count - x_train_count);

        int i =0;
        for(; i<x_train_count; ++i)
        {
            x_train[i] = x[i];
            y_train[i] = y[i];
        }
        int j=0;
        for(; i<count; ++i)
        {
            x_test[j] = x[i];
            y_test[j] = y[i];
            ++j;
        }
    }

    //apply feature scaling 
    void featureScaling(vector<double> &i)
    {
        cout<<"feature scaling is under processing\n";
    }
};


//tester function
int main()
{
    ifstream input_file("Cricket_chirps(linear_reg).csv");
    
    //data preprocessig
    preprocessing pp;
    pp.readFile(input_file);
    pp.splitTrainTest(80);

    //linear regression model
    linerRegression regressor;
    regressor.fit(pp.x_train, pp.y_train);
    cout<<"slope: "<<regressor.get_slope()<<" intercept: "<<regressor.get_intersect()<<endl;
    vector<double> y_pred;

    //predict resul
    regressor.predict(pp.x_test, y_pred);
    
    //print actual and predict valye for test data
    for(int i(0); i<y_pred.size(); i++)
    cout<<pp.y_test[i]<<" "<<y_pred[i]<<endl;
    
    //clculate mean squre error for test data
    cout<<"Mean Square Error: "<<regressor.meanSquredError(pp.y_test, y_pred);
    return 0;
}