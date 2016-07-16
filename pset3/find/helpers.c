/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 * 
 * Binary Search implementation:
 * 
*/
bool search(int searchValue, int values[], int n)
{
    // with respect to the indices of the array
    int first = 0,
        last = n - 1;
    int mid = (first + last)/2;
    
    while (first <= last)
    {
        if(values[mid] < searchValue)
        {
            first = mid + 1;
        }
        else if (values[mid] == searchValue)
        {
            return true;
        }
        else
        {
            last = mid - 1;
        }
        mid = (first+last)/2;
    }
    
    return false;
}

int find_largest_number(int* array, int arraySize)
{
    int largestNumber = 0;
    for(int i = 0; i < arraySize; ++i)
    {
        if(array[i] > largestNumber)
            largestNumber = array[i];
    }
    return largestNumber;
}

/*
// for testing 
void print_sorted_array(int * array, int size)
{
    for(int i = 0; i < size; ++i)
        printf("array[%d]= %d\n", i, array[i]);
}
*/

/**
 * Sorts array of n values.
 * 
 * using Radix Sort
 * time Complexity: O(n) 
*/
void sort(int values[], int arraySize)
{
    int largest_value = find_largest_number(values, arraySize);
    int semi_sorted_array[arraySize];
    int significant_digit = 1; // the right most digit of a value
    
    while(largest_value / significant_digit > 0)
    {
        // initialize a bucket that will store 
        int bucket[10] = {0};
        int i = 0;
        // Number of values that will go into each bucket
        for (i = 0; i < arraySize; ++i)
        {
            int location = (values[i] / significant_digit) % 10;
            bucket[location]++;
        }
        /*
            Add the count of the previous buckets. This acquires the indexes 
            after the end of each bucket's location in the (bucket) array.
        */
        for (i = 1; i < 10; ++i)
            bucket[i] += bucket[i - 1];
      
        // Use the bucket to fill a "semiSorted" array
        for (i = arraySize - 1; i > -1; i--)
        {
            int location = (values[i] / significant_digit) % 10;
            semi_sorted_array[ --bucket[location] ] = values[i];
        }
        for (i = 0; i < arraySize; ++i)
            values[i] = semi_sorted_array[i];
        
        // Next significant digit
        significant_digit *= 10;
    }
    return;
}