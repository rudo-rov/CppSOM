#pragma once
#include <vector>
#include <stack>
#include <string>

#include "../ast/ASTNodes.h"

namespace som {

    class CMethodCompilationCtx {
        public:
            CMethodCompilationCtx() {}

            void setLocals(nodeVector* localDefs);
            void setArgs(nodeVector* args);
            void setArgs(nodePtr arg);

            void addLocal(const std::string& identifier);
            void addArgument(const std::string& identifier);

            int32_t localIdx(const std::string& identifier) const;
            int32_t argIdx(const std::string& identifier) const;

        private:
            std::vector<std::string> m_locals;
            std::vector<std::string> m_args;
        };
    
    class CScopeStack {
    public:
        CScopeStack() {}

        void newScope();
        void popScope();
        void registerLocals(nodeVector* locals);
        void addLocal(const std::string& identifier);
        void addArgument(const std::string& identifier);
        int32_t localIdx(const std::string& identifier) const;
        int32_t argIdx(const std::string& identifier) const;

    private:
        std::stack<CMethodCompilationCtx> m_scopes;
    };

}