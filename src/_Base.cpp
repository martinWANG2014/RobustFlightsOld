#include "_Base.h"

_Base::_Base(void)
{
}

_Base::~_Base(void)
{
}
int _Base::getOrder() const
{ 
	return m_order;
}
void _Base::setOrder(int order) 
{ 
	m_order = order;
}
int _Base::getOrder2() const
{ 
	return m_order2;
}
void _Base::setOrder2(int order) 
{ 
	m_order2 = order;
}