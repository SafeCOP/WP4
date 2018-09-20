#ifndef MONITOR_STRESSDATA_H
#define MONITOR_STRESSDATA_H

#include <stddef.h>
#include <string.h>
#include <iostream>

/**
 * StressData for multi thread tests.
 *
 * This class is used for multi thread testing since it facilitates data integrity verification.
 *
 * The class contain an array of unsigned long long numbers which are sequential. If a number is not sequential then the data integrity has been compromised.
 */
template<size_t T>
class StressData {
public:
    /**
     * Instantiates a new StressData object.
     */
    StressData();

    /**
     * The data array.
     */
    unsigned long long data[T];

    /**
     * Increments the data sequentially, the first index will be the older last index+1, the subsequent indexes will be the previous index+1.
     */
    void increment();


    /**
     * Checks that the data is sequentially correct. Prints
     * error messages to stdin if it is not.
     *
     * @param counter the current data counter.
     * @return the number of errors that occurred.
     */
    unsigned int check(unsigned long long &counter);

    /**
     * Returns the element at index n.
     *
     * @warning n must be a valid index otherwise an overflow will occur.
     *
     * @param n the index to read.
     *
     * @return the value at the index
     */
    unsigned long long &operator[](const size_t &n);
};

template<size_t T>
StressData<T>::StressData() {
    for (unsigned long long i = 0; i < T; i++)
        data[i] = i;
}

template<size_t T>
void StressData<T>::increment() {
    unsigned long long inc = data[T - 1];
    for (size_t i = 0; i < T; i++)
        data[i] = ++inc;
}

template<size_t T>
unsigned int StressData<T>::check(unsigned long long &counter) {
    unsigned int errors = 0;
    for (size_t i = 0; i < T; i++) {
        if (data[i] != counter) {
            std::cout << "Pop value 1 differs expected: " << counter << " actual " << data[i] << std::endl;
            errors++;
            counter = data[i] + 1;
        } else
            counter++;
    }
    return errors;
}


template<size_t T>
unsigned long long &StressData<T>::operator[](const size_t &n) {
    return data[n];
}


#endif //MONITOR_STRESSDATA_H
