#include "sorting.h"

Sorter::Sorter(char *file_name) {
    check_eof(file_name);
    file_p = static_cast<char*>( mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0) );
    assert(file_p!=MAP_FAILED);
    map_strings();
}

Sorter::~Sorter() {
    close(fd);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * Checking file for '\n' in the end and add it
 */
void Sorter::check_eof(char *file_name) {
    char last_letter;
    fd = open(file_name, O_RDONLY);
    assert(fd>=0);
    fstat(fd, &statbuf);
    if (lseek(fd,-1,SEEK_END)==-1L)
        printf("Seek Error\n");
    read(fd, &last_letter, 1);
    if (last_letter!='\n') { 
        close(fd);
        add_eof(file_name);
        fd = open(file_name, O_RDONLY);
        assert(fd>=0);
        fstat(fd, &statbuf);
    }
}

/**
 * Add '\n' to end of file
 */
void Sorter::add_eof(char *file_name) {
    fd = open(file_name, O_WRONLY);
    assert(fd>=0);
    char last_letter = '\n';
    lseek(fd, 0, SEEK_END);
    write(fd, &last_letter, sizeof(last_letter));
    close(fd);
}

/**
 * Map strings from file and delete empty strings
 */
void Sorter::map_strings() {
    int counter = 0;
    bool empty_string = true;
    strings_ptrs.push_back(file_p);
    for (;counter<statbuf.st_size-1; ++counter) {
        if (file_p[counter]=='\n') {
            if (empty_string)
                strings_ptrs.pop_back();  
            strings_ptrs.push_back(file_p + counter + 1);
            empty_string = true;
        }
        else
            empty_string=false;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * Return str_1>=str_2
 */
bool Sorter::comparator(char *str_1, char *str_2) {
    int counter_1 = 0, counter_2 = 0;
    char let_1, let_2;
    while (str_1[counter_1]!='\n' && str_2[counter_2]!='\n') {
        if (!isalpha(str_1[counter_1])) {
            ++counter_1;
            continue;
        }
        if (!isalpha(str_2[counter_2])) {
            ++counter_2;
            continue;  
        }
        // checking for big letters
        let_1 = str_1[counter_1];
        let_2 = str_2[counter_2];
        if (isupper(str_1[counter_1])) {
            let_1 += 32;
        }
        if (isupper(str_2[counter_2])) {
            let_2 += 32;
        }
        // compare letters
        if (let_1==let_2) {
            ++counter_1;
            ++counter_2;
            continue;
        }
        else 
            return (let_1>let_2);
    } 
    // if meet '\n'
    return (str_2[counter_2]=='\n');
}   

/**
 * Return reverse str_1>=str_2
 */
bool Sorter::reverse_comparator(char *str_1, char *str_2) {
    int counter_1 = 0, counter_2 = 0;
    int len_1 = 0, len_2 = 0;
    char let_1, let_2;
    while (str_1[counter_1]!='\n')
        ++counter_1;
    while (str_2[counter_2]!='\n')
        ++counter_2; 
    --counter_1; --counter_2;
    while (counter_1>=0 && counter_2>=0) {
        // checking for not letters
        if (!isalpha(str_1[counter_1])) {
            --counter_1;
            continue;
        }
        if (!isalpha(str_2[counter_2])) {
            --counter_2;
            continue;  
        }
        let_1 = str_1[counter_1];
        let_2 = str_2[counter_2];
        // checking for big letters
        if (isupper(str_1[counter_1])) {
            let_1 += 32;
        }
        if (isupper(str_2[counter_2])) {
            let_2 += 32;
        }
        // compare letters
        if (let_1==let_2) {
            --counter_1;
            --counter_2;
            continue;
        }
        else 
            return (let_1>let_2);
    } 
    // if meet '\n'
    return (counter_2==0);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * Sorts elements relative to the middle element and returns it
 */
int Sorter::partition(vector<char*>& array, int left_elem, int right_elem, bool reverse) {
    int pivot = left_elem++;
    while (left_elem < right_elem) {
        while (( reverse ? // array[left_elem] <= array[pivot]
            reverse_comparator(array[pivot], array[left_elem]) : 
            comparator(array[pivot], array[left_elem])) 
            && (left_elem < right_elem))
            left_elem++; // --->
        while (( reverse ? // array[right_elem] >= array[pivot]
            reverse_comparator(array[right_elem], array[pivot]) : 
            comparator(array[right_elem], array[pivot])) 
            && (left_elem < right_elem))
            right_elem--; // <---
        if (left_elem < right_elem)
            swap(array, left_elem++, right_elem--);
    } 
    swap(array, pivot, right_elem);
    return right_elem;
}  

/**
 * Swap 2 elements
 */
void Sorter::swap(vector<char*>& array, int left_index, int right_index) {
    char *buf = array[left_index];
    array[left_index] = array[right_index];
    array[right_index] = buf;
}


/**
 * My quick sort
 */
void Sorter::quick_sort(vector<char*>& array, int left_elem, int right_elem, bool reverse) {
    if (right_elem>left_elem) {
        switch (right_elem - left_elem) {
            case 1: // 2 elems
                if (reverse ? 
                    reverse_comparator(array[left_elem], array[right_elem]) :
                    comparator(array[left_elem], array[right_elem]) ) 
                    swap(array, left_elem, right_elem);
                break;
            case 2: // 3 elems
                if (reverse ? 
                    reverse_comparator(array[left_elem], array[right_elem]) :
                    comparator(array[left_elem], array[right_elem]) ) 
                    swap(array, left_elem, right_elem);
                if (reverse ? 
                    reverse_comparator(array[right_elem-1], array[right_elem]) :
                    comparator(array[right_elem-1], array[right_elem]) ) 
                    swap(array, right_elem-1, right_elem);
                else {
                    if (reverse ? 
                        reverse_comparator(array[left_elem], array[right_elem-1]) :
                        comparator(array[left_elem], array[right_elem-1]) ) 
                        swap(array, left_elem, right_elem-1);  
                }
                break;   
            default: // >3 elems
                int mid_elem = partition(array, left_elem, right_elem, reverse);
                quick_sort(array, left_elem, mid_elem-1, reverse);
                quick_sort(array, mid_elem+1, right_elem, reverse); 
                break;  
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * Print string with '\n' to stdout
 */ 
void Sorter::print_string(char* str) {
    int i = 0;
    do {
        cout << str[i];
    } while (str[i++]!='\n');
}

/**
 * Print first PRINT_SIZE strings of array
 */
void Sorter::print_head(vector<char*> array) {
    cout << "\n***HEAD***\n" << endl;
    for (int i=0; i<PRINT_SIZE && i<array.size(); i++) 
        print_string(array[i]);
}

/**
 * Print last PRINT_SIZE strings of array
 */
void Sorter::print_tail(vector<char*> array) {
    cout << "\n***TAIL***\n" << endl;
    for (int i=array.size()-1; i>array.size()-1-PRINT_SIZE && i>=0; i--) 
        print_string(array[i]);
}

/**
 * Show result of base sort
 */
void Sorter::sort() {
    base_sort = strings_ptrs;
    quick_sort(base_sort, 0, base_sort.size()-1);
    print_head(base_sort);
    print_tail(base_sort);
}

/**
 * Show result of reverse sort
 */
void Sorter::reverse_sort() {
    rev_sort = strings_ptrs;
    quick_sort(rev_sort, 0, rev_sort.size()-1, true);
    print_head(rev_sort);
    print_tail(rev_sort);
}