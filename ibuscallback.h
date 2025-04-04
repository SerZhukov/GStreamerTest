#ifndef IBUSCALLBACK_H
#define IBUSCALLBACK_H

class IBusCallback
{
public:
    IBusCallback();
    virtual ~IBusCallback() = default;
    virtual void stopLoop() = 0;
    virtual void test() = 0;
};

#endif // IBUSCALLBACK_H
