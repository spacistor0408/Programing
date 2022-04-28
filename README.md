1. 使用開發環境

- 作業系統：Windows 10
- 使用軟體：Visual Studio Code
- 使用語言：Python 3.8.8

2. 流程

- 使用者選擇是否繼續程式或是離開（ 1:Sort / 0:Exit ）
- 使用者輸入欲載入之檔名
- 使用者選擇要執行的方法（ 1 / 2 / 3 / 4 ）
- 方法 1：直接將資料進行Bubble Sort
- 方法 2：使用者輸入欲分割大小，將資料依照輸入進行分割，每一分割部分分別進行Bubble Sort後，再以Merge Sort的方式合併
- 方法 3：使用者輸入欲分割大小，將資料依照輸入進行分割，每一分割部分透過1個process進行Bubble Sort後，再以Merge Sort的方式合併
- 方法 4：使用者輸入欲分割大小，將資料依照輸入進行分割，每一分割部分透過1個thread進行Bubble Sort後，再以Merge Sort的方式合併
- 紀錄時間執行該方法之時間
- 將結果與方法執行時間、輸出日期時間寫入新的檔案

3. 使用的資料結構

- list
- queue.Queue
- multiprocessing.queue
- multiprocessing.Process
- threading.Thread

4. 完成的功能

- 全數完成

5. 分析

 | method/time | 1w      | 10w     | 50w       | 100w
 | method1	   | 6.906   | 717.449 | 18933.509 | 75734.036
 | method2	   | 0.049   | 0.927   | 17.391    | 70.031
 | method3	   | 8.396   | 9.106   | 11.697    | 54.371
 | method4     | 0.14    | 1.061   | 17.482    | 69.473

![image](https://github.com/spacistor0408/Threads_OSProject01/blob/master/chart.png)
