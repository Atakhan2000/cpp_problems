#ifndef TEMPLATES_LIST_2021_02_11
#define TEMPLATES_LIST_2021_02_11
#include <exception>
#include <stdexcept>

namespace lab618
{
    template<class T>
    class CSingleLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext;
            leaf(T& _data, leaf * _pnext): data(_data), pnext(_pnext)
            {
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator(): m_pCurrent(0), m_pBegin(0)
            {
            }

            CIterator(leaf *p)
            {
              m_pBegin = 0;
              m_pCurrent = p;
            }

            CIterator(const CIterator &src): m_pBegin(src.m_pBegin), m_pCurrent(src.m_pCurrent)
            {
            }

            ~CIterator()
            {
            }

            CIterator& operator = (const CIterator&  src)
            {
              m_pBegin = src.m_pBegin;
              m_pCurrent = src.m_pCurrent;
              return *this;
            }

            bool operator != (const CIterator&  it) const
            {
              return (m_pCurrent != it.m_pCurrent) || (m_pBegin != it.m_pBegin);
            }

            void operator++()
            {
              if (m_pCurrent == 0)
                {
                m_pCurrent = m_pBegin;
                m_pBegin = 0;
                    return;
                }
              m_pCurrent = m_pCurrent->pnext;
            }

            T& getData()
            {
              if (m_pCurrent == 0)
              {
                std::invalid_argument("m_pCurrent == 0");
              }
                return m_pCurrent->data;
            }

            T& operator* ()
            {
                return getData();
            }

            leaf* getLeaf()
            {
              if (m_pCurrent == 0)
              {
                std::invalid_argument("m_pCurrent == 0");
              }
              return m_pCurrent;
            }

            void setLeaf(leaf* p)
            {
                m_pBegin = 0;
                m_pCurrent = p;
            }

            void setLeafPreBegin(leaf* p)
            {
                m_pBegin = p;
                m_pCurrent = 0;
            }

            bool isValid()
            {
                return m_pCurrent != 0;
            }

        private:
            //храним голову перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
        };

    public:
        CSingleLinkedList(): m_pBegin(0), m_pEnd(0)
        {
        }

        virtual ~CSingleLinkedList()
        {
          clear();
        }

        void pushBack(T& data)
        {
          leaf* pNewEnd = new leaf(data, 0);
          if (m_pEnd == 0)
          {
            m_pEnd = pNewEnd;
            m_pBegin = m_pEnd;
            return;
          }
          m_pEnd->pnext = pNewEnd;
          m_pEnd = pNewEnd;
        }

        void pushFront(T& data)
        {
          leaf* pNewBegin = new leaf(data, m_pBegin);
          pNewBegin->pnext = m_pBegin;
          m_pBegin = pNewBegin;
          if (m_pEnd == 0)
          {
            m_pEnd = m_pBegin;
          }
        }

        T popFront()
        {
            if (m_pBegin == 0)
            {
                throw std::invalid_argument("Empty list");
            }
            leaf* pTmp = m_pBegin;

            m_pBegin = m_pBegin->pnext;
            if (m_pBegin == 0)
            {
              m_pEnd = 0;
            }

            T tmp = std::move(pTmp->data);
            delete pTmp;

            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
          if (!it.isValid())
            return;
          leaf* p = it.getLeaf();

          leaf *pprev = 0;
          if (p == m_pBegin)
          {
            m_pBegin = p->pnext;
            it.setLeafPreBegin(m_pBegin);
          }
          else
          {
            //находим предыдущий элемент
            pprev = m_pBegin;
            while (pprev != 0)
            {
              if (pprev->pnext == p)
                break;
              pprev = pprev->pnext;
            }
            it.setLeaf(pprev);
          }
          if (pprev != 0)
          {
            pprev->pnext = p->pnext;
          }
          if (!p->pnext) // находимся в конце
          {
            m_pEnd = pprev;
          }
          p->pnext = 0;
          delete p;
        }

        int getSize()
        {
          size_t size = 0;
          for (leaf* pCurrent = m_pBegin; pCurrent != 0;
            pCurrent = pCurrent->pnext)
          {
            ++size;
          }
          return size;
        }

        void clear()
        {
          leaf* pCurrent = m_pBegin;

          m_pBegin = 0;
          m_pEnd = 0;

          while (pCurrent != 0)
          {
            leaf* pNext = pCurrent->pnext;
            delete pCurrent;
            pCurrent = pNext;
          }
        }

        CIterator begin()
        {
            return CIterator(m_pBegin);
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };

//DUAL LINKED list
    template<class T>
    class CDualLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf *pnext, *pprev;
            leaf(T& _data, leaf * _pprev, leaf * _pnext):
              pnext(_pnext), pprev(_pprev), data(_data)
            {
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator() : m_pBegin(0), m_pEnd(0), m_pCurrent(0)
            {
            }

            CIterator(leaf *p): m_pCurrent(p), m_pBegin(0), m_pEnd(0)
            {
            }

            CIterator(const CIterator &src):
              m_pCurrent(src.m_pCurrent),
              m_pBegin(src.m_pBegin),
              m_pEnd(src.m_pEnd)
            {
            }

            ~CIterator()
            {
            }

            CIterator& operator = (const CIterator&  src)
            {
              m_pCurrent = src.m_pCurrent;
              m_pBegin = src.m_pBegin;
              m_pEnd = src.m_pEnd;
              return *this;
            }

            bool operator != (const CIterator&  it) const
            {
              return (this->m_pCurrent != it.m_pCurrent) ||(this->m_pBegin != it.m_pBegin)|| (this->m_pEnd != it.m_pEnd);
            }

            void operator++()
            {
              if (m_pCurrent == m_pEnd)
              {
                throw std::invalid_argument("can't move right");
              }

              if (m_pCurrent == m_pBegin)
              {
                  m_pBegin = 0;
                  return;
              }

              m_pCurrent = m_pCurrent->pnext;
            }

            void operator--()
            {
              if (m_pCurrent == m_pBegin)
              {
                throw std::invalid_argument("can't move left");
              }

              if (m_pCurrent == m_pEnd )
              {
                m_pEnd = 0;
                return;
              }

              m_pCurrent = m_pCurrent->pprev;
            }

            T& getData()
            {
              if (m_pCurrent == 0)
              {
                std::invalid_argument("can't get data from preBegin and postEnd");
              }
                return m_pCurrent->data;
            }

            T& operator* ()
            {
              return getData();
            }

            leaf* getLeaf()
            {
              return m_pCurrent;
            }

            // применяется в erase и eraseAndNext
            void setLeaf(leaf* p)
            {
                m_pBegin = 0;
                m_pCurrent = p;
                m_pEnd = 0;
            }

            // применяется в erase и eraseAndNext
            void setLeafPreBegin(leaf* p)
            {
                m_pBegin = p;
                m_pCurrent = p;
                m_pEnd = 0;
            }

            // применяется в erase и eraseAndNext
            void setLeafPostEnd(leaf* p)
            {
                m_pBegin = 0;
                m_pCurrent = p;
                m_pEnd = p;
            }

            bool isValid()
            {
              return m_pCurrent != 0;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
            //храним конец списка, если мы находимся после конца
            leaf* m_pEnd;
        };

    public:

        CDualLinkedList(): m_pBegin(0), m_pEnd(0)
        {
        };

        virtual ~CDualLinkedList()
        {
          clear();
        };

        void pushBack(T& data)
        {
          leaf* pNewEnd = new leaf(data, 0, 0);

          if (!m_pBegin)
          {
            m_pBegin = m_pEnd = pNewEnd;
            return;
          }

          m_pEnd->pnext = pNewEnd;
          pNewEnd->pprev = m_pEnd;
          m_pEnd = pNewEnd;
        }

        T popBack()
        {
            if (!m_pEnd)
            {
                throw std::invalid_argument("Empty list");
            }

            T data = m_pEnd->data;

            if (m_pBegin == m_pEnd)
            {
                delete m_pBegin;
                m_pBegin = m_pEnd = 0;
                return data;
            }

            leaf* pOldEnd = m_pEnd;
            m_pEnd = m_pEnd->pprev;
            m_pEnd->pnext = 0;
            delete pOldEnd;
            return data;
        }

        void pushFront(T& data)
        {
            leaf* pNewBegin = new leaf(data, 0, 0);
            if (!m_pBegin)
            {
                m_pBegin = m_pEnd = pNewBegin;
                return;
            }

            pNewBegin->pnext = m_pBegin;
            m_pBegin->pprev = pNewBegin;
            m_pBegin = pNewBegin;
        }

        T popFront()
        {
            if (!m_pBegin)
            {
                throw std::invalid_argument("Empty list");
            }

            T data = m_pBegin->data;

            if (m_pBegin == m_pEnd)
            {
                delete m_pBegin;
                m_pBegin = m_pEnd = 0;
                return data;
            }

            leaf* pOldBegin = m_pBegin;
            m_pBegin = m_pBegin->pnext;
            m_pBegin->pprev = 0;
            delete pOldBegin;
            return data;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            if (!it.isValid())
            {
                throw std::invalid_argument("Can't move left");
            }

            if (it.getLeaf() == m_pBegin)
            {
                if (m_pBegin == m_pEnd)
                {
                    m_pBegin = m_pEnd = 0;
                } else {
                    m_pBegin = m_pBegin->pnext;
                    m_pBegin->pprev = 0;
                }
                it.setLeafPreBegin(m_pBegin);
                return;
            }

            leaf* pOldLeaf = it.getLeaf();
            it.setLeaf(pOldLeaf->pprev);
            it.getLeaf()->pnext = pOldLeaf->pnext;

            if (pOldLeaf != m_pEnd)
            {
              pOldLeaf->pnext->pprev = it.getLeaf();
            }
            else if (pOldLeaf == m_pEnd)
            {
              m_pEnd = pOldLeaf->pprev;
              m_pEnd->pnext = 0;
            }
            delete pOldLeaf;
            pOldLeaf = 0;
        }

        // изменяет состояние итератора. выставляет следующую позицию.
        void eraseAndNext(CIterator& it)
        {
            if (!it.isValid())
            {
                throw std::invalid_argument("Can't move right");
            }

            if (it.getLeaf() == m_pEnd)
            {
                leaf* pEnd = m_pEnd;
                if (m_pBegin == m_pEnd)
                {
                    m_pBegin = m_pEnd = 0;
                } else
                {
                    m_pEnd = m_pEnd->pprev;
                    m_pEnd->pnext = 0;
                }
                it.setLeafPostEnd(m_pEnd);

                delete pEnd;
                pEnd = 0;
                return;
            }

            leaf* pOldLeaf = it.getLeaf();
            it.setLeaf(pOldLeaf->pnext);
            it.getLeaf()->pprev = pOldLeaf->pprev;
            if (pOldLeaf != m_pBegin) {
              pOldLeaf->pprev->pnext = it.getLeaf();
            }
            else if (pOldLeaf == m_pBegin)
            {
              m_pBegin = pOldLeaf->pnext;
              m_pBegin->pprev = 0;
            }
            delete pOldLeaf;
            pOldLeaf = 0;
        }

        int getSize()
        {
            int size = 0;
            if (m_pBegin == 0)
            {
              return 0;
            }
            for (CIterator it = this->begin(); it.isValid(); ++it)
            {
                ++size;
            }
            return size;
        }

        void clear()
        {
            if (m_pBegin == 0) {
              return;
            }

            while (m_pBegin)
            {
              leaf* pOldLeaf = m_pBegin;
              m_pBegin = m_pBegin->pnext;
              delete pOldLeaf;
              pOldLeaf = 0;
            }
            m_pBegin = m_pEnd = 0;
        }

        CIterator begin()
        {
            return CIterator(m_pBegin);
        }

        CIterator end()
        {
            return CIterator(m_pEnd);
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };

};
#endif //#ifndef TEMPLATES_LIST_2021_02_11
