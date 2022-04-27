#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <exception>
#include <unistd.h>

#define MAXSIZE 100

using namespace std ;

fstream file ;

string GetcurrentTime()
{
    time_t now = time(0) ;
    struct tm *ltm = localtime( &now ) ;
    // char* dt = ctime(&now) ;
    // cout << dt ;
    char date[60] = {0} ;
    sprintf( date, "%d-%02d-%02d %02d:%02d:%02d",
             ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday,
             ltm->tm_hour, ltm->tm_min, ltm->tm_sec ) ;
             
    return date ;
}

class DataManager{

private:

    vector<int> dataSet ;
    int buffer ;
    string inputFilePath = "input/" ;
    string outputFilePath = "output/" ;

    string outputName = "_output" ;
    string fileType = ".txt" ;
    string fileName ;

    bool OpenFile()
    {
        return file.is_open() ;
    }

public:

    bool LoadFile( string fname )
    {
        fileName = fname ;
        file.open( inputFilePath + fileName + fileType, ios::in ) ;
        return OpenFile() ;
    } // LoadFile

    bool CreateDataSet( )
    {

        if ( file.is_open() )
            while ( file >> buffer )
            {
                dataSet.push_back( buffer ) ;
            }

    } // CreateDataSet

    void CloseFile() 
    {
        if ( file.is_open() ) file.close() ;
    }

    void PrintData()
    {
        if ( !dataSet.empty() )
        {
            int i = 0 ;
            do {
                cout << dataSet[i] << endl ;
            } while ( i == dataSet.size() ) ;
        }
    }

    vector<int> GetData()
    {
        return dataSet ;
    }

    void Write( vector<int> sorted , string filenum, double CPU_Time )
    {
        file.open( fileName + outputName + filenum + fileType, ios::out ) ;

        if ( file.is_open() )
            {
                file << "Sort : " << endl ;
                for ( int i=0 ; i<sorted.size() ; i++ )
                {
                    file << sorted[i] << endl ;
                }
                file << "CPU Time : " << CPU_Time << "s" << endl ;
                file << "Output Time : " << GetcurrentTime() ;
            }

        file.close() ;
    }

    void ClearDataSet()
    {
        dataSet.clear() ;
    }

} ; // DataManeger

class Sort{

private:

    vector<int> unsortedDatas ;
    vector<int> sortedDatas ;

    vector<int> Merge( vector<int>::iterator left, vector<int>::iterator leftEnd, vector<int>::iterator right, vector<int>::iterator rightEnd )
    {
        vector<int> A ;
        // cout << "left:" << *left << endl ;
        // cout << "leftEnd:" << *leftEnd << endl ;
        // cout << "right:" << *right << endl ;
        // cout << "rightEnd:" << *rightEnd << endl << endl ;

        while ( left != leftEnd || right != rightEnd )
        {
            if ( left == leftEnd )
            {
                A.push_back( *right ) ;
                right++ ;
            }
            else if ( right == rightEnd )
            {
                A.push_back( *left ) ;
                left++ ;
            }
            else if ( *left <= *right )
            {
                A.push_back( *left ) ;
                left++ ;
            }
            else
            {
                A.push_back( *right ) ;
                right++ ;
            }
        }
        return A ;
    } // Merge()

public:

    void ImportData( vector<int> dataSet )
    {
        sortedDatas = dataSet ;
        unsortedDatas = dataSet ;
    }

    /*   Bubble Sort Default   */
    vector<int> BubbleSort()
    {
        bool flag = true ;
        for ( int i=0 ; i<sortedDatas.size() && flag ; i++ )
        {
            flag = false ;
            for ( int j=0 ; j<sortedDatas.size()-i-1 ; j++ )
            {
                if ( sortedDatas[j] > sortedDatas[j+1] )
                {
                    int temp = sortedDatas[j] ;
                    sortedDatas[j] = sortedDatas[j+1] ;
                    sortedDatas[j+1] = temp ;
                    flag = true ;
                }
            }
        }
        return sortedDatas ;
    } // BubbleSort

    /*   Bubble Sort Overloading   */
    vector<int> BubbleSort( vector<int> &dataSet )
    {
        bool flag = true ;
        for ( int i=0 ; i<dataSet.size() && flag ; i++ )
        {
            flag = false ;
            for ( int j=0 ; j<dataSet.size()-i-1 ; j++ )
            {
                if ( dataSet[j] > dataSet[j+1] )
                {
                    /*  swap  */
                    int temp = dataSet[j] ;
                    dataSet[j] = dataSet[j+1] ;
                    dataSet[j+1] = temp ;
                    flag = true ;
                }
            }
        }
        return dataSet ;
    } // BubbleSort

    /*   Merge Sort   */
    void MergeSort( int dividCount )
    {

        int capacity = sortedDatas.size() / dividCount ;
        int lastCapacity = capacity + sortedDatas.size() % dividCount ;
        vector<vector<int>> arrays ;
        vector<int> subarray ;

        /*   Divid the subarray from array   */
        for ( int i=0 ; i<sortedDatas.size() ; i++ )
        {
            subarray.push_back(sortedDatas[i]) ;

            if ( i == sortedDatas.size()-1 )
            {
                arrays.push_back( subarray ) ;
                subarray.clear() ;
            }

            if ( i % capacity == 0 && i+lastCapacity <= sortedDatas.size()-1 )
            {
                arrays.push_back( subarray ) ;
                subarray.clear() ;
            }
        }
        // cout << "Divid complete ! " << arrays.size() << endl ;


        /*   Sort subarray   */
        for ( int i=0 ; i<arrays.size() ; i++ )
        {
            BubbleSort( arrays[i] ) ;
        }

        /*   Merge subarray   */
        while ( arrays.size() != 1 )
        {
            arrays.push_back( Merge( arrays[0].begin(), arrays[0].end(), arrays[1].begin(), arrays[1].end() ) ) ;
            arrays.erase( arrays.begin() ) ;
            arrays.erase( arrays.begin() ) ;
        }

        sortedDatas = arrays[0] ;

    } // End MergeSort

    void MultiProcess_MergeSort( int dividCount )
    {

        int capacity = sortedDatas.size() / dividCount ;
        int lastCapacity = capacity + sortedDatas.size() % capacity ;
        vector<vector<int>> arrays ;
        vector<int> subarray ;

        /*   Divid the subarray from array   */
        for ( int i=0 ; i<sortedDatas.size() ; i++ )
        {
            subarray.push_back(sortedDatas[i]) ;

            if ( i == sortedDatas.size()-1 )
            {
                arrays.push_back( subarray ) ;
                subarray.clear() ;
            }

            if ( i % capacity == 0 && i+lastCapacity <= sortedDatas.size()-1 )
            {
                arrays.push_back( subarray ) ;
                subarray.clear() ;
            }
        }
        // cout << "Divid complete ! " << arrays.size() << endl ;


        /*   Sort subarray   */
        for ( int i=0 ; i<arrays.size() ; i++ )
        {
            BubbleSort( arrays[i] ) ;
        }

        /*   Merge subarray   */
        while ( arrays.size() != 1 )
        {
            arrays.push_back( Merge( arrays[0].begin(), arrays[0].end(), arrays[1].begin(), arrays[1].end() ) ) ;
            arrays.erase( arrays.begin() ) ;
            arrays.erase( arrays.begin() ) ;
        }

        sortedDatas = arrays[0] ;

    } // End MergeSort

    void PrintData()
    {
        if ( !sortedDatas.empty() )
        {
            int i = 0 ;
            do {
                cout << sortedDatas[i] << endl ;
            } while ( i == sortedDatas.size() ) ;
        }
    }

    vector<int> GetSortedData()
    {
        return sortedDatas ;
    }

    void ResetData()
    {
        sortedDatas.clear() ;
        unsortedDatas.clear() ; 
    }

}; // Sort

int main ()
{
    DataManager DM ;
    Sort sort ;
    string fileName ;
    double TimeStart=0, TimeEnd=0, CPU_Time=0 ;
    int selection ;
    
    do {

        // Selection
        cout << "Select a optin to continue...( 1:Sort / 0:Exit ) : " ;
        cin >> selection ;

        if ( selection == 1 )
        {

            cout << "Enter File Name: " ;
            cin >> fileName ;

            if ( DM.LoadFile( fileName ) )
            {
                DM.CreateDataSet() ;
                sort.ImportData( DM.GetData() ) ;
                cout << "Data Loading Completely !" << endl << endl ;
                
                DM.CloseFile() ;

                int method = 0 ;
                int dividCount = 0 ;
                cout << "Please Enter Method ( 1 / 2 / 3 / 4 ) : " ;
                cin >> method ;

                switch( method )
                {
                    case 1:

                        /**
                         * Method 1
                         * @name BUBBLE SORT
                         * In this method, only use bubble sort without merge and multi-processes / threads
                         */
                        cout << "Bubble Sorting..." << endl ;

                        /*  Record Time & executing bubble sort  */
                        TimeStart = clock();
                        sort.BubbleSort() ;
                        TimeEnd = clock();
                        CPU_Time = (TimeEnd - TimeStart)/CLOCKS_PER_SEC ; 

                        cout << "Bubble Sort Complete !" << endl ;

                        DM.Write( sort.GetSortedData(), "1_CPP", CPU_Time ) ;
                        break ;

                    case 2:

                        /**
                         * Method 2
                         * @name MERGE SORT
                         * In this method, only use merge sort without and multi-processes / threads
                         */
                        cout << "Please Enter a number you want to divid : " ;
                        cin >> dividCount ;
                        if ( dividCount<0 ) 
                        {
                            cout << "Error ! Please Enter a number bigger than zero ! " << endl << endl ;
                            break ;
                        }

                        cout << "Merge Sorting..." << endl ;

                        /*  Record Time & executing bubble sort  */
                        TimeStart = clock();
                        sort.MergeSort( dividCount ) ;
                        TimeEnd = clock();
                        CPU_Time = (TimeEnd - TimeStart)/CLOCKS_PER_SEC ; 

                        cout << "Merge Sort Complete !" << endl ;

                        DM.Write( sort.GetSortedData(), "2_CPP", CPU_Time ) ;
                        break ;

                    case 3:
                        cout << "TODO" ;
                        /**
                         * Method 3
                         * @name MERGE SORT WITH MULTI-PROCESSES
                         * In this method, use merge sort with multi-processes
                         */
                        break ;

                    case 4:
                        cout << "TODO" ;
                        /**
                         * Method 4
                         * @name MERGE SORT WITH THREADS
                         * In this method, use merge sort with threads
                         */
                        break ;

                    default:
                    cout << "Error ! Please Select Method again ! " << endl ;
                } // switch case

                DM.ClearDataSet() ;
                sort.ResetData() ;
            } // if open file successfully

        } // if selection == 1
        else if ( selection == 0 ) cout << "Program Exit... " << endl ;
        else cout << "Error ! Please Select again ! " << endl ;
        cout << endl ;
    } while ( selection != 0 ) ;

        

    return 0 ;
}
