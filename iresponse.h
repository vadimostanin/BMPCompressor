#ifndef IRESPONSE_H
#define IRESPONSE_H

#include <memory>

class IResponse
{
public:
    virtual ~IResponse(){}
};

typedef std::shared_ptr<IResponse> IShResponse;

#endif // IRESPONSE_H
