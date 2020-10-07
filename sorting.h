#ifndef SORTING_H
#define SORTING_H

#include <iostream>
#include <sys/mman.h> // for mmap
#include <fcntl.h> // for open
#include <sys/stat.h> // for fstat
#include <unistd.h>
#include <assert.h>
#include <vector>
#include <string>

#define PRINT_SIZE 100

using namespace std;

class Sorter {
    private:
        // file info
        int fd; 
        char *file_p;
        struct stat statbuf;
        // strings info
        vector<char*> strings_ptrs;
        vector<char*> base_sort;
        vector<char*> rev_sort;

        void quick_sort(vector<char*>& array, int left_elem, int right_elem, bool reverse=false);
        bool comparator(char *str_1, char *str_2);
        bool reverse_comparator(char *str_1, char *str_2);
        void check_eof(char *file_name);
        void add_eof(char *file_name);
        void map_strings();

        int partition(vector<char*>& array, int left_elem, int right_elem, bool reverse);
        void swap(vector<char*>& array, int left_index, int right_index);

        void print_head(vector<char*> array);
        void print_tail(vector<char*> array);
        void print_string(char* str);
    public:
        Sorter(char *file_name);
        ~Sorter();

        void sort();
        void reverse_sort();
};

#endif