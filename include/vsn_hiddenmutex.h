#ifndef __VSN_HIDDENMUTEX_H
#define __VSN_HIDDENMUTEX_H

//------------------------------------------------------------------------------
/** \brief   \ru Скрытый mutex
\en Hidden mutex. \~
\details \ru Класс нужен что бы убрать из заголовков <mutex>
\en NOT TRANSLATED \~
*/
//---
class HiddenMutex
{
public:
    HiddenMutex();
    ~HiddenMutex();

    void lock();
    bool try_lock();
    void unlock();

private:
    HiddenMutex(const HiddenMutex&);
    void operator=(const HiddenMutex&);

private:
    class Private;
    Private* pimpl;
};

#endif
