/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                                  trf2trf                                  *
 *       Dimemas TRF to Dimemas DIM trace translator (old to new format)     *
 *****************************************************************************
 *     ___        This tool is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.12.1   *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This tool is distributed in hope that it will be         *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\

  $URL:: https://svn.bsc.es/repos/ptools/trf2dim/trunk/src/ApplicationDesc#$:

  $Rev:: 483                        $:  Revision of last commit
  $Author:: jgonzale                $:  Author of last commit
  $Date:: 2010-10-29 11:54:45 +0200#$:  Date of last commit

\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "ApplicationDescription.hpp"
#include <cstdlib>
#include <cstdio>
#include <cstring>

/*****************************************************************************
 * class TaskDescription
 ****************************************************************************/

void TaskDescription::Write ( ostream & os ) const
{
  os << "TASK: " << TaskId << "(" << ThreadCount << ":" << Node << ")";
};

ostream& operator<< (ostream& os, const TaskDescription& Task)
{
  Task.Write(os);
  return os;
}

/*****************************************************************************
 * class ApplicationDescription
 ****************************************************************************/

ApplicationDescription::ApplicationDescription(INT32 ApplicationId,
                                               INT32 TaskCount,
                                               INT32 CommunicatorCount)
{
  this->ApplicationId     = ApplicationId;
  this->TaskCount         = TaskCount;
  this->CommunicatorCount = CommunicatorCount;
}

ApplicationDescription::ApplicationDescription(INT32 ApplicationId,
                                               INT32 TaskCount,
                                               INT32 CommunicatorCount,
                                               char* ASCIIAppDescription)
{
  INT32 CurrentTaskId, ThreadCount, Node;
  char *InternalAppDescription = (char*) calloc(strlen(ASCIIAppDescription)+1,
                                                sizeof(char));
  char *CurrentTaskInfo;
  
  strcpy(InternalAppDescription, ASCIIAppDescription);
  
  this->ApplicationId     = ApplicationId;
  this->TaskCount         = TaskCount;
  this->CommunicatorCount = CommunicatorCount;
  
  CurrentTaskId = 1;
  CurrentTaskInfo = strtok(InternalAppDescription, ",");
  while (CurrentTaskInfo != NULL)
  {
    if (sscanf (CurrentTaskInfo, "%d:%d", &ThreadCount, &Node) == 2)
    {
      AddTaskDescription(CurrentTaskId, ThreadCount, Node);
      CurrentTaskId++;
    }
    CurrentTaskInfo = strtok(NULL, ",");
  }
  
  if (TaskInfo.size() != TaskCount)
  {
    SetError(true);
    LastError = "Number of tasks in task info list less than task count";
  }
}

void ApplicationDescription::Write ( ostream & os ) const
{
  os << "** APPLICATION DESCRIPTION **" << endl;
  os << "Task #: " << TaskCount << " Communicators: ";
  os << CommunicatorCount << endl;
  os << "Tasks information" << endl;
  for (unsigned int i = 0; i < TaskInfo.size(); i++)
  {
    os << *TaskInfo[i];
    if ((i+1)%4 == 0)
      os << endl;
    else
      os << ", ";
  }
  os << endl;
};

ostream& operator<< (ostream& os, const ApplicationDescription& App)
{
  App.Write(os);
  return os;
}
