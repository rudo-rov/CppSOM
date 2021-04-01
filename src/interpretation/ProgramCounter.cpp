#include <vector>
#include <memory>

#include "ProgramCounter.h"

namespace som {

   bool CProgramCounter::shouldExit() const
   {
       return m_pc == m_programEnd;
   }

}