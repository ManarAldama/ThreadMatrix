# ThreadMatrix

#### Given two input matrices M1 and M2, you will traverse M1 in iterations. At each iteration, you will multiply corresponding elements of M2 and the part of M1 which overlaps with M2. You will sum the result of multiplications and write the result to the corresponding element of the output matrix.

#### You will implement a multithreaded version of your work in phase 1 using POSIX Threads. 
•	You will compute each number in the output matrix with a separate thread. 
•	You will display the contents of the output matrix whenever a thread sets a value in the output matrix.
•	Initially, all the values in the output matrix will be equal to -1.

