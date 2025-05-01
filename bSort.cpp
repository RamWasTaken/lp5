#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <omp.h>

using namespace std;

// Sequential Bubble Sort
void sequentialBubbleSort(vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort (Odd-Even Transposition)
void parallelBubbleSort(vector<int> &arr) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n; ++i) {
        swapped = false;
        #pragma omp parallel for shared(arr, swapped)
        for (int j = i % 2; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // Early exit if no swaps
    }
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> arr(n), arr_seq(n), arr_par(n);

    // Input numbers from user
    cout << "Enter " << n << " numbers (separated by spaces or newlines):\n";
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    // Copy for sequential and parallel
    arr_seq = arr;
    arr_par = arr;

    // --- Sequential Version ---
    auto start_seq = chrono::high_resolution_clock::now();
    sequentialBubbleSort(arr_seq);
    auto end_seq = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_seq = end_seq - start_seq;
    cout << "\nSequential Time: " << elapsed_seq.count() << " sec\n";

    // --- Parallel Version ---
    auto start_par = chrono::high_resolution_clock::now();
    parallelBubbleSort(arr_par);
    auto end_par = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_par = end_par - start_par;
    cout << "Parallel Time: " << elapsed_par.count() << " sec\n";

    // Verify correctness
    bool correct_seq = is_sorted(arr_seq.begin(), arr_seq.end());
    bool correct_par = is_sorted(arr_par.begin(), arr_par.end());
    cout << "\nSequential Sorting: " << (correct_seq ? "SUCCESS" : "FAILED") << endl;
    cout << "Parallel Sorting: " << (correct_par ? "SUCCESS" : "FAILED") << endl;

    // Print sorted array (optional)
    cout << "\nSorted Array (Parallel): ";
    for (int num : arr_par) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}
