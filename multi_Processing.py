from decimal import DivisionByZero
import multiprocessing as mp
import threading as td
import time
import queue

Path = "input/"
FileType = ".txt"

def ReadFile(filename):
    
    Dataset = []
    with open( Path + filename + FileType, mode="r" ) as File:
        for line in File.readlines():
            Dataset.append(int(line))
    return Dataset
    

def WriteFile(filename, CPU_TIME, Dataset):
    with open( filename + FileType, mode="w" ) as File:
        for i in range(len(Dataset)):
            File.write( str(Dataset[i]) + '\n' )
        File.write("CPU Time : " + str(CPU_TIME) + 'sec\n')
        File.write("Output Time : " + time.strftime(('%Y-%m-%d %H:%M:%S'), time.localtime()) )

def PrintData(A):
    for line in A:
        print( line )

def BubbleSort(A, sortedQueue=None):
    flag = True
    for i in range(len(A)-1):
        if not flag : break 
        flag = False
        for j in range(len(A)-i-1):
            if A[j] > A[j+1]:
                A[j], A[j+1] = A[j+1], A[j]
                flag = True
    if sortedQueue: sortedQueue.put(A)
    return A

def MergeSort(A, k):
    
    arrays = CutArray(A, k)
    sortedQueue = queue.Queue()

    for i in range(len(arrays)):
        BubbleSort(arrays[i], sortedQueue)
    
    while sortedQueue.qsize() != 1:
        Merge(sortedQueue.get(), sortedQueue.get(), sortedQueue)

    return sortedQueue.get()

def MergeSort_MultiProcess(A, k):
    
    arrays = CutArray(A, k)
    multi_sortedQueue = mp.Queue()
    processes = []

    for i in range(len(arrays)):
        process = mp.Process( target=BubbleSort, args=(arrays[i], multi_sortedQueue) )
        process.start()
        processes.append(process)

    while multi_sortedQueue.qsize() != 1:
        Merge(multi_sortedQueue.get(), multi_sortedQueue.get(), multi_sortedQueue)

    return multi_sortedQueue.get()

def MergeSort_Thread(A, k):
    
    arrays = CutArray(A, k)
    sortedQueue = queue.Queue()
    threads = []

    for i in range(len(arrays)):
        thread = td.Thread( target=BubbleSort, args=(arrays[i], sortedQueue) )
        thread.start()
        threads.append(thread)

    while sortedQueue.qsize() != 1:
        Merge(sortedQueue.get(), sortedQueue.get(), sortedQueue)

    return sortedQueue.get()

### Merge Function is to combine left Array and right Array ###
def Merge(leftArray, rightArray, sortedQueue):
    
    leftIndex = 0
    rightIndex = 0
    mergeArray = []
    
    while leftIndex != len(leftArray) or rightIndex != len(rightArray):

        if leftIndex == len(leftArray):
            mergeArray.append( int(rightArray[rightIndex]) )
            rightIndex += 1

        elif rightIndex == len(rightArray):
            mergeArray.append( int(leftArray[leftIndex]) )
            leftIndex += 1

        elif leftArray[leftIndex] < rightArray[rightIndex]:
            mergeArray.append( int(leftArray[leftIndex]) )
            leftIndex += 1

        else:
            mergeArray.append( int(rightArray[rightIndex]) )
            rightIndex += 1
    
    sortedQueue.put(mergeArray)

    return mergeArray

def CutArray( A, k ):
    partition = int(len(A)/k)
    arrays = []
    index = 0

    while index < len(A):
        subArray = []

        if index+partition >= len(A)-1:
            partition += len(A)%partition

        for i in range(partition):
            subArray.append(A[index+i])

        index += partition
        arrays.append(subArray)

    return arrays




def main():

    while int(input( "Select a optin to continue...( 1:Sort / 0:Exit ) : " )):
        filename = input("Enter File Name: ")
        try:
            Dataset = ReadFile(filename)
        except IOError:
            print( 'ERROR: can not found ' + filename + '\n' )
        else:
            size = len(Dataset)
            selection = int(input( "Please Enter Method ( 1 / 2 / 3 / 4 ): " ))

            if selection == 1:
                print( "Running Bubble Sort...\n" )

                start = time.time()
                BubbleSort(Dataset)
                end = time.time()

                WriteFile(filename + "_output1", round(end-start, 14), Dataset )
                print( "Bubble Sort Completed !\n\n" )

            elif selection == 2:
                dividCount = int(input( "Please Enter a number you want to divid: " ))
                try:
                    if ( dividCount > size or dividCount <= 0  ): raise ValueError
                except ValueError:
                    print( "Out of Dataset size or equal to zero !\n" )
                else:
                    print( "Running Merge Sort...\n" )

                    start = time.time()
                    A = MergeSort(Dataset, dividCount)
                    end = time.time()

                    WriteFile(filename + "_output2", round(end-start, 14), A )
                    print( "Merge Sort Completed !\n\n" )
                
            elif selection == 3:
                dividCount = int(input( "Please Enter a number you want to divid: " ))
                try:
                    if ( dividCount > size or dividCount <= 0 ): raise ValueError
                except ValueError:
                    print( "Out of Dataset size or equal to zero !\n" )
                else:
                    print( "Running Merge Sort with multi-processing...\n" )

                    start = time.time()
                    A = MergeSort_MultiProcess(Dataset, dividCount)
                    end = time.time()
                    WriteFile(filename + "_output3", round(end-start, 14), A )
                    print( "Merge Sort Completed !\n\n" )

            elif selection == 4:
                dividCount = int(input( "Please Enter a number you want to divid: " ))
                try:
                    if ( dividCount > size or dividCount <= 0 ): raise ValueError
                except ValueError:
                    print( "Out of Dataset size or equal to zero !\n" )
                else:
                    print( "Running Merge Sort with threads...\n" )

                    start = time.time()
                    A = MergeSort_Thread(Dataset, dividCount)
                    end = time.time()
                    WriteFile(filename + "_output4", round(end-start, 14), A )
                    print( "Merge Sort Completed !\n\n" )
            
            else:
                print( "ERROR: the method doesn't exist\n" )
    print( "Program Exist...\n" )

if __name__ == '__main__':
    main()