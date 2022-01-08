#include "TorchPrecompiled.h"
#include <libethcore/ABI.h>
#include <torch/torch.h>
#include <iostream>

using namespace dev;
using namespace dev::eth;
using namespace dev::blockverifier;
using namespace dev::storage;
using namespace dev::precompiled;

/*
contract TorchPrecompiled {
    function tensor() public view returns(string){}
}
*/


// tensor interface
const char* const TORCH_METHOD_TENSOR = "tensor()";


TorchPrecompiled::TorchPrecompiled()
{
    name2Selector[TORCH_METHOD_TENSOR] = getFuncSelector(TORCH_METHOD_TENSOR);
}

PrecompiledExecResult::Ptr TorchPrecompiled::call(
    std::shared_ptr<ExecutiveContext>, bytesConstRef _param, Address const&, Address const&)
{
    auto funcSelector = getParamFunc(_param);
    // auto paramData = getParamData(_param);
    ContractABI abi;
    auto callResult = m_precompiledExecResultFactory->createPrecompiledResult();
    callResult->gasPricer()->setMemUsed(_param.size());
    do
    {
        if (funcSelector == name2Selector[TORCH_METHOD_TENSOR])
        {
            // bytes inputData;
            // abi.abiOut(paramData, inputData);
            // auto sm3Hash = sm3(inputData);
            std::ostringstream stream;
            torch::Tensor tensor = torch::rand({2, 3});
            stream << tensor;
            // PRECOMPILED_LOG(TRACE)
            //     << LOG_DESC("TorchPrecompiled: tensor")
            //     << LOG_KV("result", tensor_str);
            std::string retValue = stream.str();
            callResult->setExecResult(abi.abiIn("", retValue));
            break;
        }
        // no defined function
        callResult->setExecResult(abi.abiIn("", u256((uint32_t)CODE_UNKNOW_FUNCTION_CALL)));
    } while (0);
    return callResult;
}
