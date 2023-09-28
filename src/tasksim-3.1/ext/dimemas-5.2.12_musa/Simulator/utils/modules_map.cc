extern "C" {
#include "define.h"
#include "types.h"
#include "extern.h"
#include "list.h"
#include "subr.h"
}

#include <map>
using std::map;
using std::pair;

#include<vector>
using std::vector;

using std::make_pair;

#include "modules_map.h"

class ModuleMap
{
  private:
    map<pair<unsigned long int , unsigned long int >, void*> Map;

    map<pair<unsigned long int , unsigned long int >, void*>::iterator curr;
    map<pair<unsigned long int , unsigned long int >, void*>::iterator curr_end; // invalidated if "head" changes

    map<pair<unsigned long int , unsigned long int >, void*>::iterator it;

    t_count _count;

  public:

    ModuleMap(void)
    {
      Map.clear();
      _count = 0;
    }

    inline void* FindModule(unsigned long int type, unsigned long int value)
    {
      if (Map.size() == 0)
      {
        return NULL;
      }

      if (Map.count(make_pair(type, value)) == 1)
      {
        return Map[make_pair(type, value)];
      }
      else
      {
        return NULL;
      }
    }

    inline void InsertModule(unsigned long int type,
                             unsigned long int value,
                             void             *module)
    {
      Map[make_pair(type, value)] = module;
      _count++;
    }


    inline void* head(void)
    {

      void *Result = NULL;
      this->curr = Map.begin();

      if (this->curr != (this->curr_end = Map.end()))
      {
        Result = (*this->curr).second;
      }
      return Result;
    }

    inline void *next(void)
    {
      void *Result = NULL;
      ++this->curr;

      if (this->curr != this->curr_end)
      {
        Result = (*this->curr).second;
      }
      return Result;
    }

    inline t_count count(void)
    {
      return _count;
    }
};

static     vector<ModuleMap> ModuleMaps;
static int LastModuleMap = 0;

void create_modules_map(modules_map *mm)
{
  *mm = LastModuleMap;
  LastModuleMap++;

  if (ModuleMaps.size() <= (LastModuleMap - 1))
  {
    ModuleMaps.resize(*mm+10);
  }
  //printf("Created queue %d\n", *q);
  return ;
}

void* find_module(modules_map      *mm,
                  unsigned long int type,
                  unsigned long int value)
{
  return ModuleMaps[*mm].FindModule(type, value);
}

void insert_module(modules_map      *mm,
                   unsigned long int type,
                   unsigned long int value,
                   void             *new_module)
{
  ModuleMaps[*mm].InsertModule(type, value, new_module);
}

void *head(modules_map* mm)
{
  return ModuleMaps[*mm].head();
}

void *next(modules_map* mm)
{
  return ModuleMaps[*mm].next();
}

t_count count_map(modules_map* mm)
{
  return ModuleMaps[*mm].count();
}
