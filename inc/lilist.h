/******************************************************************************
 *  _      _ _      _     _
 * | |    (_) |    (_)   | |
 * | |     _| |     _ ___| |_
 * | |    | | |    | / __| __|
 * | |____| | |____| \__ \ |_
 * |______|_|______|_|___/\__| <einKnie@gmx.at>
 *****************************************************************************/
#ifndef _LILIST_H_
#define _LILIST_H_

#include <stdlib.h>

// IDEA: make these mode identifiers concatable:
// e.g. mode = EModeReference | EModeSorted -> would result in a sorted ref list!
typedef enum mode {
  EModeReference = (1 << 0),
  EModeCopy      = (1 << 1),
  EModeSorted    = (1 << 2)
} listMode_e;

template <class T> class LiList {
public:

  LiList();
  LiList(listMode_e mode);
  LiList(const LiList<T> &other);
  ~LiList();

  T *Head();
  T *Tail();
  T *Next();
  T *Prev();
  int add(T *t);
  int addSorted(T *t);
  int remove(T *t);
  T *find(T *t);
  T *get_random();
  unsigned int Size();
  void free();


private:

  typedef struct list {
    T *obj;

    list *next;
    list *prev;

    list() : list(NULL) {}
    list(T *o) {
      obj = o;

      next = NULL;
      prev = NULL;
    }

    ~list() {
      next = NULL;
      prev = NULL;
    }
  } list_t;

  list_t *m_head;
  list_t *m_tail;
  list_t *m_curr;
  unsigned int m_len;
  listMode_e m_mode;

  list_t* find_internal(T *t) {
    list_t *fnd = NULL;
    list_t *tmp = m_head;

    while (tmp != NULL) {
      if (*tmp->obj == *t) {
        fnd = tmp;
        break;
      }
      tmp = tmp->next;
    }
    return fnd;
  }

};

template <class T> LiList<T>::LiList() {
  m_head = NULL;
  m_tail = m_head;
  m_curr = m_head;
  m_len = 0;
  m_mode = EModeReference;
}

template <class T> LiList<T>::LiList(listMode_e mode) {
  m_head = NULL;
  m_tail = m_head;
  m_curr = m_head;
  m_len = 0;
  m_mode = mode;
}

template <class T> LiList<T>::LiList(const LiList &other) {
  m_head = NULL;
  m_tail = m_head;
  m_curr = m_head;
  m_len = 0;
  m_mode = other.m_mode;

  list_t *tmp = other.m_head;
  while (tmp != NULL) {
    add(tmp->obj);
    tmp = tmp->next;
  }
}

// template <class T> LiList<T>::operator+=(const LiList<T> &other) {
//   m_tail->next = other.m_head;
//   if (m_tail->next != NULL) m_tail->next->prev = m_tail;
// }

template <class T> LiList<T>::~LiList() {
  free();
}

template <class T> int LiList<T>::add(T *t) {
  if (t == NULL) {
    return 0;
  }

  T* elem = NULL;
  if (m_mode == EModeCopy) {
    elem = new T(*t);
  } else {
    elem = t;
  }

  list_t *tmp = new list_t(elem);
  if (!m_head) {
    m_head = tmp;
    m_tail = tmp;
  } else {
    m_tail->next = tmp;
    tmp->prev = m_tail;
    m_tail = tmp;
  }
  m_len++;
  return 1;
}

template <class T> int LiList<T>::addSorted(T *t) {
  if (t == NULL) {
    return 0;
  }

  T* elem = NULL;
  if (m_mode == EModeCopy) {
    elem = new T(*t);
  } else {
    elem = t;
  }

  list_t *tmp = new list_t(elem);
  list_t *curr = m_head;

  if (!m_head) {
    m_head = tmp;
    m_tail = tmp;
  } else {
    do {
      if (*tmp->obj < *curr->obj) {
        tmp->prev = curr->prev;
        tmp->next = curr;
        if (curr->prev) curr->prev->next = tmp;
        curr->prev = tmp;
        if (curr == m_head) {
          m_head = tmp;
        }
        break;
      }
      curr = curr->next;
    } while (curr);

    if (!curr) {
      // append new entry at end
      m_tail->next = tmp;
      tmp->prev = m_tail;
      m_tail = tmp;
    }
  }
  m_len++;
  return 1;
}

template <class T> int LiList<T>::remove(T *t) {
  list_t *rem = NULL;
  if ((rem = find_internal(t)) == NULL) {
    return 0;
  }

  if (rem->next) {
    rem->next->prev = rem->prev;
    if (m_curr == rem) m_curr = rem->next;
  } else {
    m_tail = rem->prev;
    if (m_curr == rem) m_curr = m_tail;
  }

  if (rem->prev) {
    rem->prev->next = rem->next;
    if (m_curr == rem) m_curr = rem->prev;
  } else {
    m_head = rem->next;
    if (m_curr == rem) m_curr = m_head;
  }

  if (m_mode == EModeCopy) {
    delete rem->obj;
  }
  delete rem;
  m_len --;
  return 1;
}

template <class T> T* LiList<T>::find(T *t) {
  list_t *fnd = NULL;
  list_t *tmp = m_head;

  while (tmp != NULL) {
    if (*tmp->obj == *t) {
      fnd = tmp;
      break;
    }
    tmp = tmp->next;
  }
  return fnd ? fnd->obj : NULL;
}

template <class T> T* LiList<T>::get_random() {
  if (m_len > 0) {
    int idx = rand() % m_len;
    list_t *tmp = m_head;

    while (tmp != NULL && idx) {
      tmp = tmp->next;
      idx--;
    }
    return tmp->obj;
  } else {
    return NULL;
  }
}

template <class T> T* LiList<T>::Head() {
  m_curr = m_head;
  return m_head ? m_head->obj : NULL;
}

template <class T> T* LiList<T>::Tail() {
  m_curr = m_tail;
  return m_tail ? m_tail->obj : NULL;
}

template <class T> T* LiList<T>::Next() {
  m_curr = m_curr ? m_curr->next : m_curr;
  return m_curr ? m_curr->obj : NULL;
}

template <class T> T* LiList<T>::Prev() {
  m_curr = m_curr ? m_curr->prev : m_curr;
  return m_curr ? m_curr->obj : NULL;
}

template <class T> void LiList<T>::free() {
  if (m_head != NULL) {
    list_t *tmp = m_tail;
    do {
      tmp = tmp->prev;
      if (m_mode == EModeCopy) {
        delete m_tail->obj;
        m_tail->obj = NULL;
      }
      delete m_tail;
      m_tail = tmp;
      m_len--;
    } while (tmp != NULL);
  }
  m_head = NULL;
}

template <class T> unsigned int LiList<T>::Size() {
  return m_len;
}

#endif // _LILIST_H_
