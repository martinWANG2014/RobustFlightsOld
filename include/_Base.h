#ifndef BASE_H
#define BASE_H

class _Base {
public:
    int m_order;    // or
    int m_order2;

    _Base(void);

    virtual ~_Base(void);

    int getOrder() const;

    int getOrder2() const;

    void setOrder(int order);

    void setOrder2(int order);
};

#endif
