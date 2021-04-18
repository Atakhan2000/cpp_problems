#ifndef SORT_HEAD_H_2021_02_25
#define SORT_HEAD_H_2021_02_25
#include <algorithm>

namespace templates
{
	/** Определение типа функции сравнения элементов
	\return
	\li > 0  - pElem1 меньше pElem2
	\li 0   - элементы pElem1 и pElem2 равны
	\li < 0 - pElem1 больше pElem2
	 */
  typedef  int (CompareSortType)(const void *pElem1, const void *pElem2);

  void merge(void **ppLhsArray, void **ppRhsArray,
             int lhsLen, int rhsLen,
             CompareSortType pCompareFunc,
             void **Tmp)
  {
    int length = lhsLen + rhsLen;
    int lhsIter = 0;
    int rhsIter = 0;

    for (int i = 0; i < length; ++i)
    {
      if (lhsIter == lhsLen)
      {
        for (int j = 0; i + j < length; ++j)
        {
          Tmp[i + j] = ppRhsArray[j + rhsIter];
        }
        break;
      }

      if (rhsIter == rhsLen)
      {
        for (int j = 0; i + j < length; ++j)
        {
          Tmp[i + j] = ppLhsArray[j + lhsIter];
        }
        break;
      }



      if (pCompareFunc(ppLhsArray[lhsIter], ppRhsArray[rhsIter]) == -1)
      {
        Tmp[i] = ppRhsArray[rhsIter];
        ++rhsIter;
      }
      else if (pCompareFunc(ppLhsArray[lhsIter], ppRhsArray[rhsIter]) == 1
            || pCompareFunc(ppLhsArray[lhsIter], ppRhsArray[rhsIter]) == 0)
      {
        Tmp[i] = ppLhsArray[lhsIter];
        ++lhsIter;
      }
    }

    for (int i = 0; i < lhsLen; ++i)
    {
      ppLhsArray[i] = Tmp[i];
    }
    for (int i = 0; i < rhsLen; ++i)
    {
      ppRhsArray[i] = Tmp[i + lhsLen];
    }
  }

  void merge_(void **ppArray, int length, CompareSortType pCompareFunc, void **Tmp)
  {
    if (length < 2)
    {
      return;
    }

    int lhsLen = length/2;
    int rhsLen = length - lhsLen;
    merge_(ppArray, lhsLen, pCompareFunc, Tmp);
    merge_(ppArray + lhsLen, rhsLen, pCompareFunc, Tmp);

    merge(ppArray, ppArray + lhsLen, lhsLen, rhsLen, pCompareFunc, Tmp);
  }

  void mergeSort(void **ppArray, int length, CompareSortType pCompareFunc)
  {
    void **Tmp = new void *[length];
    merge_(ppArray, length, pCompareFunc, Tmp);
    delete[] Tmp;
  }

  template <class T>
  void mergeSort(T **ppArray, int length, int (pCompareFunc)(const T *pElem1, const T *pElem2))
  {
    mergeSort(reinterpret_cast<void**>(ppArray), length, (CompareSortType*)pCompareFunc);
  }
}; // namespace templates

#endif // #define SORT_HEAD_H_2021_02_25
