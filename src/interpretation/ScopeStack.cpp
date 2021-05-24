#include <cassert>
#include "ScopeStack.h"

namespace som {

    void CScopeStack::newScope()
    {
        m_scopes.push_back(CMethodCompilationCtx());
    }

    void CScopeStack::popScope()
    {
        m_scopes.erase(--m_scopes.end());
    }

    void CScopeStack::registerLocals(nodeVector* locals)
    {
        for (const auto& var : *locals) {
            Variable* varPtr = dynamic_cast<Variable*>(var.get());
            if (varPtr) {
                addLocal(varPtr->m_identifier);
            }
        }
    }

    void CScopeStack::addLocal(const std::string& identifier)
    {
        assert(!m_scopes.empty());
        m_scopes.back().addLocal(identifier);
    }

    void CScopeStack::addArgument(const std::string& identifier)
    {
        assert(!m_scopes.empty());
        m_scopes.back().addArgument(identifier);
    }

    int32_t CScopeStack::localIdx(const std::string& identifier)
    {
        assert(!m_scopes.empty());
        int32_t currentOffset = 0;
        for (auto& ctx : m_scopes) {
            if (ctx.localIdx(identifier) < 0) {
                currentOffset += ctx.sizeLocals();
            } else {
                return currentOffset + ctx.localIdx(identifier);
            }
        }

        return -1;
    }

    int32_t CScopeStack::argIdx(const std::string& identifier)
    {
        assert(!m_scopes.empty());
        int32_t currentOffset = 0;
        for (auto& ctx : m_scopes) {
            if (ctx.argIdx(identifier) < 0) {
                currentOffset += ctx.sizeArgs();
            } else {
                return currentOffset + ctx.argIdx(identifier);
            }
        }

        return -1;
    }
    
    void CMethodCompilationCtx::setLocals(nodeVector* localDefs)
    {
        for (const auto& local : *localDefs) {
            Variable* localPtr = dynamic_cast<Variable*>(local.get());
            if (localPtr)
                m_locals.push_back(localPtr->m_identifier);
        }
    }

    void CMethodCompilationCtx::addLocal(const std::string& identifier)
    {
        m_locals.push_back(identifier);
    }

    void CMethodCompilationCtx::addArgument(const std::string& identifier)
    {
        m_args.push_back(identifier);
    }

    void CMethodCompilationCtx::setArgs(nodeVector* args)
    {
        for (const auto& arg : *args) {
            Variable* argPtr = dynamic_cast<Variable*>(arg.get());
            if (argPtr)
                m_args.push_back(argPtr->m_identifier);
        }
    }

    void CMethodCompilationCtx::setArgs(nodePtr arg)
    {
        Variable* argPtr = dynamic_cast<Variable*>(arg.get());
        if (argPtr) {
            m_args.push_back(argPtr->m_identifier);
        }
    }

    int32_t CMethodCompilationCtx::localIdx(const std::string& identifier)
    {
        for (auto i = 0; i < m_locals.size(); ++i) {
            if (m_locals.at(i) == identifier) {
                return i;
            }
        }
        return -1;
    }

    int32_t CMethodCompilationCtx::argIdx(const std::string& identifier)
    {
        for (auto i = 0; i < m_args.size(); ++i) {
            if (m_args.at(i) == identifier) {
                return i;
            }
        }
        return -1;
    }

}