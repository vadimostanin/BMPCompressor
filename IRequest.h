#ifndef _I_REQUEST_H_
#define _I_REQUEST_H_

#include <memory>

class IRequest
{
public:
	virtual ~IRequest(){}
};

typedef std::shared_ptr<IRequest> IShRequest;

#endif
