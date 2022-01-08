#include "TorchPrecompiled.h"
#include <libethcore/ABI.h>
#include <torch/script.h>
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
    function loadmodel(string path) public view returns(string){}
}
*/

// [NOTE!!!] if you test the precompiled contract using fisco-bcos console, 
// please remember to delete "console/contract/.compiled/" manually when you update the contract
// because the console cannot automatically update the abi code

// Also, if you compile failed for undefined reference or something like that,
// you can try to comment the line "set(TORCH_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=1")" in file TorchConfig.cmake
// it works for me


// tensor interface
const char* const TORCH_METHOD_TENSOR = "tensor()";
// inference
const char* const TORCH_METHOD_LOAD_MODEL = "loadmodel(string)";

TorchPrecompiled::TorchPrecompiled()
{
    name2Selector[TORCH_METHOD_TENSOR] = getFuncSelector(TORCH_METHOD_TENSOR);
    name2Selector[TORCH_METHOD_LOAD_MODEL] = getFuncSelector(TORCH_METHOD_LOAD_MODEL);
}

PrecompiledExecResult::Ptr TorchPrecompiled::call(
    std::shared_ptr<ExecutiveContext>, bytesConstRef _param, Address const&, Address const&)
{
    auto funcSelector = getParamFunc(_param);
    auto paramData = getParamData(_param);
    ContractABI abi;
    auto callResult = m_precompiledExecResultFactory->createPrecompiledResult();
    callResult->gasPricer()->setMemUsed(_param.size());
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
    } else if (funcSelector == name2Selector[TORCH_METHOD_LOAD_MODEL])
    {
        torch::jit::script::Module module;
        std::string retValue;
        std::string inputData;
        abi.abiOut(paramData, inputData);
        try {
            // Deserialize the ScriptModule from a file using torch::jit::load().
            module = torch::jit::load("/home/junqin/FISCO-BCOS/build/traced_resnet_model.pt");
            retValue = "load model success;" + inputData;
        }
        catch (const c10::Error& e) {
            retValue = "error loading the model;" + inputData;
            // return -1;
        }
        callResult->setExecResult(abi.abiIn("", retValue));
    } else
    {
        // no defined function
        PRECOMPILED_LOG(ERROR) << LOG_BADGE("TorchPrecompiled")
                               << LOG_DESC("call undefined function") << LOG_KV("func", funcSelector);
        callResult->setExecResult(abi.abiIn("", u256((uint32_t)CODE_UNKNOW_FUNCTION_CALL)));
    }
    return callResult;
}
