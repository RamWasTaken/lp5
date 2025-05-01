#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
using namespace std;

void merge(int a[], int i1, int j1, int i2, int j2) {
    int* temp = new int[j2 - i1 + 1];  // Dynamic allocation
    int i = i1, j = i2, k = 0;
    while (i <= j1 && j <= j2) {
        if (a[i] < a[j]) temp[k++] = a[i++];
        else temp[k++] = a[j++];
    }
    while (i <= j1) temp[k++] = a[i++];
    while (j <= j2) temp[k++] = a[j++];
    for (i = i1, j = 0; i <= j2; i++, j++) a[i] = temp[j];
    delete[] temp;  // Free memory
}

// Sequential Merge Sort (for comparison)
void sequentialMergesort(int a[], int i, int j) {
    if (i < j) {
        int mid = (i + j) / 2;
        sequentialMergesort(a, i, mid);
        sequentialMergesort(a, mid + 1, j);
        merge(a, i, mid, mid + 1, j);
    }
}

// Parallel Merge Sort
void parallelMergesort(int a[], int i, int j) {
    if (i < j) {
        int mid = (i + j) / 2;
        // Only parallelize if the subarray is large enough
        if (j - i > 1000) {  // Threshold to avoid overhead
            #pragma omp parallel sections
            {
                #pragma omp section
                { parallelMergesort(a, i, mid); }
                #pragma omp section
                { parallelMergesort(a, mid + 1, j); }
            }
        } else {
            parallelMergesort(a, i, mid);
            parallelMergesort(a, mid + 1, j);
        }
        merge(a, i, mid, mid + 1, j);
    }
}

// Merge function (unchanged but with dynamic temp array)

int main() {
    int n;
    cout << "Enter total number of elements: ";
    cin >> n;
    int* a = new int[n];
    int* a_seq = new int[n];
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        a_seq[i] = a[i];  // Copy for sequential sort
    }

    // --- Sequential Merge Sort ---
    auto start_seq = chrono::high_resolution_clock::now();
    sequentialMergesort(a_seq, 0, n - 1);
    auto end_seq = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_seq = end_seq - start_seq;
    cout << "Sequential Time: " << elapsed_seq.count() << " sec\n";

    // --- Parallel Merge Sort ---
    auto start_par = chrono::high_resolution_clock::now();
    parallelMergesort(a, 0, n - 1);
    auto end_par = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_par = end_par - start_par;
    cout << "Parallel Time: " << elapsed_par.count() << " sec\n";

    // Verify correctness
    bool correct = true;
    for (int i = 0; i < n - 1; i++) {
        if (a[i] > a[i + 1]) {
            correct = false;
            break;
        }
    }
    cout << "Sorting " << (correct ? "SUCCESS" : "FAILED") << endl;

    // Print sorted array (optional)
    cout << "Sorted array (Parallel): ";
    for (int i = 0; i < n; i++) cout << a[i] << " ";
    cout << endl;

    delete[] a;
    delete[] a_seq;
    return 0;
}
