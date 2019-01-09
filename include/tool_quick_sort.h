////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Функция сортировки.
         \en A sorting function. \~

*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// \ru Стандартная функция void qsort( void *base, size_t num, size_t width, int ( *compare )(const void *elem1, const void *elem2 ) ) 
//     определенная в <stdlib.h> and <search.h> может работать неверно на массивах длиной <= 8. Пожалуйста, избегайте ее применения. 
// \en Standard function   void qsort( void *base, size_t num, size_t width, int ( *compare )(const void *elem1, const void *elem2 ) ) 
//     defined in <stdlib.h> and <search.h> may work uncorrectly in arrays with length <= 8. Please, avoid using of this. 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TOOL_QUICK_SORT_H
#define __TOOL_QUICK_SORT_H


#include <io_define.h>
#include <math_define.h>


//------------------------------------------------------------------------------
// \ru Функция сравнения \en A comparison function 
//---
typedef int (CALL_DECLARATION *KsQSortCompFunc)( const void*, const void* );


//------------------------------------------------------------------------------
// swap two elements
//
// Purpose:
//       swaps the two array elements of size width
//
// Entry:
//       char *a, *b = pointer to two elements to swap
//       unsigned width = width in bytes of each array element
//---
static void CALL_DECLARATION swap( char *a, char *b, size_t width ) 
{
  if ( a != b ) {
    char tmp;
    /* Do the swap one character at a time to avoid potential alignment
       problems. */
    while ( width-- ) {
        tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    }
  }
}


//------------------------------------------------------------------------------
// \ru Cортировка массива \en An array sorting 
//
// Purpose:
//       quicksort the array of elements
//       side effects:  sorts in place
//
// Entry:
//       char *base = pointer to base of array
//       unsigned num  = number of elements in the array
//       unsigned width = width in bytes of each array element
//       int (*compareFunc)() = pointer to function returning analog of strcmp for
//               strings, but supplied by user for comparing the array elements.
//               it accepts 2 pointers to elements and returns neg if 1<2, 0 if
//               1=2, pos if 1>2.
//
// sort the array between lo and hi (inclusive)
//---
inline void KsQSort( void          * base, 
                     size_t          num, 
                     size_t          width, 
                     KsQSortCompFunc compareFunc )
{
    char *lo, *hi;               /* ends of sub-array currently sorting */
    char *mid;                   /* points to middle of subarray */
    char *loguy, *higuy;         /* traveling pointers for partition step */
    size_t size;                 /* size of the sub-array */
    char *lostk[30], *histk[30];
    int stkptr;                  /* stack for saving sub-array to be processed */

    /* Note: the number of stack entries required is no more than
       1 + log2(size), so 30 is sufficient for any array */

    if (num < 2 || width == 0)
        return;                 /* nothing to do */

    stkptr = 0;                 /* initialize stack */

    lo = (char *)base;
    hi = (char *)base + width * (num-1); /* initialize limits */

    /* this entry point is for pseudo-recursion calling: setting
       lo and hi and jumping to here is like recursion, but stkptr is
       prserved, locals aren't, so we preserve stuff on the stack */
recurse:

    size = (hi - lo) / width + 1;  /* number of el's to sort */

    /* First we pick a partititioning element.  The efficiency of the
       algorithm demands that we find one that is approximately the
       median of the values, but also that we select one fast.  Using
       the first one produces bad performace if the array is already
       sorted, so we use the middle one, which would require a very
       wierdly arranged array for worst case performance.  Testing shows
       that a median-of-three algorithm does not, in general, increase
       performance. */

    mid = lo + (size / 2) * width; /* find middle element */
    swap(mid, lo, width);          /* swap it to beginning of array */

    /* We now wish to partition the array into three pieces, one
       consisiting of elements <= partition element, one of elements
       equal to the parition element, and one of element >= to it.  This
       is done below; comments indicate conditions established at every
       step. */

    loguy = lo;
    higuy = hi + width;

    /* Note that higuy decreases and loguy increases on every iteration,
       so loop must terminate. */
    for (;;) {
        /* lo <= loguy < hi, lo < higuy <= hi + 1,
           A[i] <= A[lo] for lo <= i <= loguy,
           A[i] >= A[lo] for higuy <= i <= hi */

        do  {
            loguy += width;
        } while (loguy <= hi && compareFunc(loguy, lo) <= 0);

        /* lo < loguy <= hi+1, A[i] <= A[lo] for lo <= i < loguy,
           either loguy > hi or A[loguy] > A[lo] */

        do  {
            higuy -= width;
        } while (higuy > lo && compareFunc(higuy, lo) >= 0);

        /* lo-1 <= higuy <= hi, A[i] >= A[lo] for higuy < i <= hi,
           either higuy <= lo or A[higuy] < A[lo] */

        if (higuy < loguy)
            break;

        /* if loguy > hi or higuy <= lo, then we would have exited, so
           A[loguy] > A[lo], A[higuy] < A[lo],
           loguy < hi, highy > lo */

        swap(loguy, higuy, width);

        /* A[loguy] < A[lo], A[higuy] > A[lo]; so condition at top
           of loop is re-established */
    }

    /*     A[i] >= A[lo] for higuy < i <= hi,
           A[i] <= A[lo] for lo <= i < loguy,
           higuy < loguy, lo <= higuy <= hi
       implying:
           A[i] >= A[lo] for loguy <= i <= hi,
           A[i] <= A[lo] for lo <= i <= higuy,
           A[i] = A[lo] for higuy < i < loguy */

    swap(lo, higuy, width);     /* put partition element in place */

    /* OK, now we have the following:
          A[i] >= A[higuy] for loguy <= i <= hi,
          A[i] <= A[higuy] for lo <= i < higuy
          A[i] = A[lo] for higuy <= i < loguy    */

    /* We've finished the partition, now we want to sort the subarrays
       [lo, higuy-1] and [loguy, hi].
       We do the smaller one first to minimize stack usage.
       We only sort arrays of length 2 or more.*/

    if ( higuy - 1 - lo >= hi - loguy ) {
        if (lo + width < higuy) {
            lostk[stkptr] = lo;
            histk[stkptr] = higuy - width;
            ++stkptr;
        }                           /* save big recursion for later */

        if (loguy < hi) {
            lo = loguy;
            goto recurse;           /* do small recursion */
        }
    }
    else {
        if (loguy < hi) {
            lostk[stkptr] = loguy;
            histk[stkptr] = hi;
            ++stkptr;               /* save big recursion for later */
        }

        if (lo + width < higuy) {
            hi = higuy - width;
            goto recurse;           /* do small recursion */
        }
    }

    /* We have sorted the array, except for any pending sorts on the stack.
       Check if there are any, and do them. */

    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;           /* pop subarray from stack */
    }
    else
        return;                 /* all subarrays done */
}


//------------------------------------------------------------------------------
/// \ru Функция автоматического наращивания памяти \en A function of automatical allocating of the memory 
/**\ru Функция применяется в библиотеке шаблонов Sys для автоматического наращивания памяти SArray, RPArray, SQueue ...
   \en This function is used for the template library Sys for the automatical allocation of the memory in  SArray, RPArray, SQueue ... \~
*/
//---
inline size_t KsAutoDelta( size_t count )
{ 
  return std_min( (size_t)1024, std_max( (size_t)4, count / 8) ); //-V112
} 


//------------------------------------------------------------------------------
// \ru Вычисление автоматического выделения памяти массива \en Calculating of the automatical memory allocation of an array. 
//  (this avoids heap fragmentation in many situations)
//---
inline size_t KsAutoDelta( size_t count, uint16 delta )
{ 
  return ( delta > 2 ) ? delta : KsAutoDelta( count );
} 


//------------------------------------------------------------------------------
// \ru тест на запрос распределения памяти в пределах половины адресного пространства для 32- и 64-разрядного приложения \en a test for the request of memory allocation inside a half of address space for 32- and 64-bit applications. 
// ---
inline bool TestNewSize( size_t sizeOfType, size_t count )
{
  return ( double(count) * double(sizeOfType) < double(SYS_MAX_ST) );
}


#endif // __TOOL_QUICK_SORT_H
