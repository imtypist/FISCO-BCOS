#pragma once
#include "libprecompiled/Common.h"
#include "libprecompiled/Precompiled.h"

namespace dev
{
namespace precompiled
{
class TorchPrecompiled : public dev::precompiled::Precompiled
{
public:
    typedef std::shared_ptr<TorchPrecompiled> Ptr;
    TorchPrecompiled();
    virtual ~TorchPrecompiled(){};

    PrecompiledExecResult::Ptr call(std::shared_ptr<dev::blockverifier::ExecutiveContext> _context,
        bytesConstRef _param, Address const& _origin = Address(),
        Address const& _sender = Address()) override;

};

}  // namespace precompiled

}  // namespace dev