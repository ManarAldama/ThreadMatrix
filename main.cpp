#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

const int M1r = 7;
const int M1c = 7;
const int M2r = 4;
const int M2c = 4;
int bigger[10][10];
int M1[M1r][M1c];
int M2[M2r][M2c];
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // Mutex for synchronization
#define NTHREADS (M1r * M1c)

void file(int arr1[M1r][M1c], int arr2[M2r][M2c]) {
    fstream f;
    f.open("m1.txt");

    if (f.is_open()) {
        for (int i = 0; i < M1r; i++) {
            for (int j = 0; j < M1c; j++) {
                f >> arr1[i][j];
                if (arr1[i][j] == 0) {
                	cout<<"file is empty for Matrix 1!!"<<endl;
                	exit(0);
				}
            }
        }
        f.close();
        } else {
            cout << "Error opening file for Matrix 1." << endl;
            exit(1);
    }

    f.open("m2.txt");
    if (f.is_open()) {
        for (int i = 0; i < M2r; i++) {
            for (int j = 0; j < M2c; j++) {
                f >> arr2[i][j];
                if (arr2[i][j] == 0) {
                    cout << "File is empty for Matrix 2!!" << endl;
                    exit(0);
                }
            }
        }

        f.close();
    } else {
        cout << "Error opening file for Matrix 2." << endl;
        exit(1);
    }

}


void random(int arr1[M1r][M1c], int arr2[M2r][M2c]) {
    char choice;
    int c = 0;

agin:
    cout << "Take matrix data from txt file (press y) or (press n)to generate a random matrix: ";
    cin >> choice;

    switch (choice) {
    case 'y':
    case 'Y':
        file(arr1, arr2);

        cout<<"\t\t first matrix\t\t"<<endl;
        for(int i=0;i<M1r;i++){
        	for(int j=0;j<M1c;j++) {
        	cout << "\t" << "[" << arr1[i][j] << "]";
			}
			cout<<endl;
		}

		cout << "\t\tSecond matrix\t\t" << endl;
        for (int i = 0; i < M2r; i++) {
            for (int j = 0; j < M2c; j++) {
                cout << "\t" << "[" << arr2[i][j] << "]";
            }
            cout << endl;
        }

        break;
    case 'n':
    case 'N':
        cout << "\t\tRandom matrix\t\t" << endl;

        cout << "\t\tFirst matrix\t\t" << endl;

        for (int i = 0; i < M1r; ++i) {
            for (int j = 0; j < M1c; ++j) {
                arr1[i][j] = rand() % 10;
                cout << "\t" << "[" << arr1[i][j] << "]";
            }
            cout << "\n";
        }
        cout << "\t\tSecond matrix\t\t" << endl;
        for (int i = 0; i < M2r; ++i) {
            for (int j = 0; j < M2c; ++j) {
                arr2[i][j] = rand() % 10;
                cout << "\t" << "[" << arr2[i][j] << "]";
            }
            cout << "\n";
        }
        break;
    default:
        c++;
        cout << "Wrong option!!" << endl;
        if (c == 3) {
            cout << "Overflow!!";
            exit(0);
        } else {
            goto agin;
        }
    }
}

void* multiplyAndSum(void *ptr) {
    int *params = (int *)ptr;
    int i = params[0];
    int j = params[1];

    int sum = 0;

    for (int k = 0; k < M2r; k++) {
        for (int l = 0; l < M2c; l++) {
            sum += M1[i + k][j + l] * M2[k][l];
        }
    }

    pthread_mutex_lock(&mtx);  // Lock the mutex for synchronization
    bigger[i][j] = sum;
    cout << " The Matrix With "<< "Thread (" << pthread_self() <<")"<< endl;
    for (int row = 0; row < M1r; ++row) {
        for (int col = 0; col < M1c; ++col) {
            cout << "\t" << "[" << bigger[row][col] << "]";
        }
        cout << "\n";
    }
    pthread_mutex_unlock(&mtx);

    return NULL;
}


void initialMatrix() {
    for (int i = 0; i < M1r; i++) {
        for (int j = 0; j < M1c; j++) {
            bigger[i][j] = -1;
        }
    }
}

void iterations() {
    initialMatrix();
    random(M1,M2);

    pthread_t threads[NTHREADS];
    int thread_index = 0;
    int params[2];
    for (int i = 0; i < M1r; i++) {
        for (int j = 0; j < M1c; j++) {
            int* params = new int[2];  // Create a new array for each thread
            params[0] = i;
            params[1] = j;
            pthread_create(&threads[thread_index], NULL, &multiplyAndSum, (void *)params);
            thread_index++;
        }
        cout<<endl;
    }

    // Join all threads
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
srand(time(0));

iterations();
cout<<"The output matrix after iterations\t"<<endl;

// Display the resulting matrix
    for (int i = 0; i < M1r; ++i) {
        for (int j = 0; j < M1c; ++j) {
            cout << "\t" << "[" << bigger[i][j] << "]";
        }
        cout << "\n";
}


return 0;
}
