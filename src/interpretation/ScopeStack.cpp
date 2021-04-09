#include <cassert>
#include "ScopeStack.h"

namespace som {

    void CScopeStack::newScope()
    {
        m_scopes.push(CMethodCompilationCtx());
    }

    void CScopeStack::addLocal(const std::string& identifier)
    {
        assert(!m_scopes.empty());
        m_scopes.top().addLocal(identifier);
    }

    int32_t CScopeStack::localIdx(const std::string& identifier) const
    {
        assert(!m_scopes.empty());
        return m_scopes.top(). localIdx(identifier);
    }

    int32_t CScopeStack::argIdx(const std::string& identifier) const
    {

        return 0;
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

    int32_t CMethodCompilationCtx::localIdx(const std::string& identifier) const
    {
        for (auto i = 0; i < m_locals.size(); ++i) {
            if (m_locals.at(i) == identifier) {
                return i;
            }
        }
        return -1;
    }

    int32_t CMethodCompilationCtx::argIdx(const std::string& identifier) const
    {
        for (auto i = 0; i < m_args.size(); ++i) {
            if (m_args.at(i) == identifier) {
                return i;
            }
        }
        return -1;
    }

}